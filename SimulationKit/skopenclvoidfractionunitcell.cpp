/*************************************************************************************************************
 Copyright (c) 2006-2019 David Dubbeldam, Sofia Calero, Thijs J.H. Vlugt.

    D.Dubbeldam@uva.nl            http://www.acmm.nl/molsim/users/dubbeldam/index.html
    scaldia@upo.es                http://www.upo.es/raspa/sofiacalero.php
    t.j.h.vlugt@tudelft.nl        http://homepage.tudelft.nl/v9k6y

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
 *************************************************************************************************************/

#include "skopenclvoidfractionunitcell.h"
#include <iostream>
#include <algorithm>
#include <QDebug>

SKOpenCLVoidFractionUnitCell::SKOpenCLVoidFractionUnitCell(): _isOpenCLInitialized(false)
{

}


void SKOpenCLVoidFractionUnitCell::initialize(bool isOpenCLInitialized, cl_context context, cl_device_id device_id, cl_command_queue queue)
{
  cl_int err;

  _isOpenCLInitialized = isOpenCLInitialized;
  if(!_isOpenCLInitialized)
  {
    return;
  }

  _queue = queue;
  _clContext = context;
  _clCommandQueue = queue;


  const char* shaderSourceCode = SKOpenCLVoidFractionUnitCell::_voidFractionKernelSource;
  //const char* shaderSourceCode = SKOpenCLVoidFractionUnitCell::_energyGridUnitCellKernel;
  _program = clCreateProgramWithSource(context, 1, &shaderSourceCode, nullptr, &err);
  if (err != CL_SUCCESS)
  {
    qWarning("Failed to create OpenCL program: %d", err);
    return;
  }

  // Build the program executable
  err = clBuildProgram(_program, 0, nullptr, nullptr, nullptr, nullptr);
  if (err != CL_SUCCESS)
  {
    size_t len;
    char buffer[2048];
    printf("Error: Failed to build program executable! SKOpenCLVoidFractionUnitCell\n");
    clGetProgramBuildInfo(_program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
    printf("%s\n", buffer);
    exit(1);
  }

  _kernel = clCreateKernel(_program, "ComputeVoidFraction", &err);
  if (err != CL_SUCCESS)
  {
    qWarning("Failed to create OpenCL kernel: %d", err);
    return;
  }

  err = clGetKernelWorkGroupInfo(_kernel, device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(size_t), &_workGroupSize, nullptr);
  if (err != CL_SUCCESS)
  {
    qWarning("Failed to clGetKernelWorkGroupInfo: %d", err);
    return;
  }

  qDebug() << "SKOpenCLVoidFractionUnitCell Work-group size is: " <<  _workGroupSize;
}

double SKOpenCLVoidFractionUnitCell::computeVoidFraction(std::vector<cl_float>* voxels)
{
  cl_int err;

  if(!_isOpenCLInitialized)
  {
    return 0.0;
  }

  // make sure the the global work size is an multiple of the work group size
  int temp = voxels->size();
  size_t numberOfGridPoints = (temp  + _workGroupSize-1) & ~(_workGroupSize-1);
  size_t global_work_size = numberOfGridPoints;


  std::cout << "global_work_size: " << global_work_size << std::endl;

  // Transfer dataset to device
  cl_mem rawData = clCreateBuffer(_clContext,  CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,  sizeof(cl_float) * voxels->size(), voxels->data(), &err);
  if (err != CL_SUCCESS)
  {
    qWarning("Error clCreateImage:  %d\n",err);
    return 0;
  }

  int nWorkGroups = numberOfGridPoints/_workGroupSize;


  std::cout << "nWorkGroups: " << nWorkGroups << std::endl;

  // Allocate cumulative error array
  float* sumReduction = new float[nWorkGroups];
  cl_mem reductionBuffer = clCreateBuffer(_clContext, CL_MEM_READ_ONLY, nWorkGroups*sizeof(float), nullptr, &err);

  err = clEnqueueWriteBuffer(_clCommandQueue, reductionBuffer, CL_TRUE, 0, nWorkGroups*sizeof(float), sumReduction, 0, nullptr, nullptr);


  // Set the arguments of the kernel
  clSetKernelArg(_kernel, 0, sizeof(cl_mem), &rawData);
  clSetKernelArg(_kernel, 1, sizeof(cl_mem), &reductionBuffer);
  clSetKernelArg(_kernel, 2, _workGroupSize*sizeof(cl_float),nullptr);

  // Execute kernel code
  err = clEnqueueNDRangeKernel(_clCommandQueue, _kernel, 1, nullptr, &global_work_size, &_workGroupSize, 0, nullptr, nullptr);
  if (err != CL_SUCCESS)
  {
   fprintf(stderr,"Error in clEnqueueNDRangeKernel\n");
  }

     // Read the buffer back to the array
  err = clEnqueueReadBuffer(_clCommandQueue, reductionBuffer, CL_TRUE, 0, nWorkGroups*sizeof(float), sumReduction, 0, nullptr, nullptr);
  if (err != CL_SUCCESS)
  {
    fprintf(stderr,"Error in clEnqueueReadBuffer with reductionBuffer: %d\n",err);
  }

  clFinish(_clCommandQueue);

  // Final summation with CPU
  double fraction = 0.0;
  for (int i=0; i<nWorkGroups; i++)
    fraction += double(sumReduction[i]);

  clReleaseMemObject(rawData);
  clReleaseMemObject(reductionBuffer);

   return fraction/(128.0*128.0*128.0);
}

const char* SKOpenCLVoidFractionUnitCell::_voidFractionKernelSource = R"foo(
__kernel void ComputeVoidFraction (__global const float *input,
                           __global float *partialSums,
                           __local float *localSums)
{
   size_t local_id = get_local_id(0);
   size_t global_id = get_global_id(0);
   size_t group_size = get_local_size(0);
   size_t group_id = get_group_id(0);

   // Copy from global memory to local memory
   localSums[local_id] = exp(-(1.0f/298.0f)*input[global_id]);

   // Loop for computing localSums
   for (uint stride = group_size/2; stride>0; stride/=2)
   {
      // Waiting for each 2x2 addition into given workgroup
      barrier(CLK_LOCAL_MEM_FENCE);

      // Divide WorkGroup into 2 parts and add elements 2 by 2
      // between local_id and local_id + stride
      if (local_id < stride)
        localSums[local_id] += localSums[local_id + stride];
   }

   // Write result into partialSums[nWorkGroups]
   if (local_id == 0)
     partialSums[group_id] = localSums[0];
}
)foo";

