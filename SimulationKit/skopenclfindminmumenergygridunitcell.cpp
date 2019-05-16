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

#include "skopenclfindminmumenergygridunitcell.h"
#include <iostream>
#include <algorithm>
#include <QDebug>

SKOpenCLFindMinmumEnergyGridUnitCell::SKOpenCLFindMinmumEnergyGridUnitCell(): _isOpenCLInitialized(false), _isOpenCLReady(false)
{

}


void SKOpenCLFindMinmumEnergyGridUnitCell::initialize(bool isOpenCLInitialized, cl_context context, cl_device_id device_id, cl_command_queue queue)
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
  _clDeviceId = device_id;

  const char* shaderSourceCode = SKOpenCLFindMinmumEnergyGridUnitCell::_findMinimumEnergyKernel;
  _program = clCreateProgramWithSource(context, 1, &shaderSourceCode, nullptr, &err);
  if (err != CL_SUCCESS)
  {
    QString message = QString("OpenCL minimum energy: Failed to create program (error: %1)").arg(QString::number(err));
    _logReporter->logMessage(LogReporting::ErrorLevel::error, message);
    return;
  }

  // Build the program executable
  err = clBuildProgram(_program, 0, nullptr, nullptr, &SKOpenCLFindMinmumEnergyGridUnitCell::callBack, this);
  if (err != CL_SUCCESS)
  {
    size_t len;
    char buffer[2048];
    clGetProgramBuildInfo(_program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
    QString message = QString("OpenCL minimum energy: Failed to build program (error: %1)").arg(QString::fromUtf8(buffer));
    _logReporter->logMessage(LogReporting::ErrorLevel::error, message);
  }
}

void SKOpenCLFindMinmumEnergyGridUnitCell::callBack(cl_program program, void *user_data)
{
  cl_int err;

  SKOpenCLFindMinmumEnergyGridUnitCell *ptr = reinterpret_cast<SKOpenCLFindMinmumEnergyGridUnitCell*>(user_data);

  size_t len;
  char buffer[2048];
  cl_build_status bldstatus;
  clGetProgramBuildInfo(program, ptr->_clDeviceId, CL_PROGRAM_BUILD_STATUS, sizeof(bldstatus), (void *)&bldstatus, &len);

  switch(bldstatus)
  {
    case CL_BUILD_NONE:
      ptr->_logReporter->logMessage(LogReporting::ErrorLevel::error, "OpenCL minimum energy: No build was performed");
      break;
    case CL_BUILD_ERROR:
      {
        clGetProgramBuildInfo(program, ptr->_clDeviceId, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
        QString message = QString("OpenCL minimum energy: Failed to build program (error: %1)").arg(QString::fromUtf8(buffer));
        ptr->_logReporter->logMessage(LogReporting::ErrorLevel::error, message);
      }
      break;
    case CL_BUILD_SUCCESS:
      ptr->_logReporter->logMessage(LogReporting::ErrorLevel::info, "OpenCL minimum energy: Build success");
      break;
    case CL_BUILD_IN_PROGRESS:
      ptr->_logReporter->logMessage(LogReporting::ErrorLevel::warning, "OpenCL minimum energy: Build still in progress");
      break;
  }


  ptr->_kernel = clCreateKernel(program, "findMinimum", &err);
  if (err != CL_SUCCESS)
  {
    QString message = QString("OpenCL minimum energy: Failed to create OpenCL findMinimum kernel (error: %1)").arg(QString::number(err));
    ptr->_logReporter->logMessage(LogReporting::ErrorLevel::error, message);
    return;
  }

  err = clGetKernelWorkGroupInfo(ptr->_kernel, ptr->_clDeviceId, CL_KERNEL_WORK_GROUP_SIZE, sizeof(size_t), &ptr->_workGroupSize, nullptr);
  if (err != CL_SUCCESS)
  {
    QString message = QString("OpenCL minimum energy: Failed in clGetKernelWorkGroupInfo (error: %1)").arg(QString::number(err));
    ptr->_logReporter->logMessage(LogReporting::ErrorLevel::error, message);
    return;
  }

  QString message = QString("OpenCL minimum energy: work-group size set to %1").arg(QString::number(ptr->_workGroupSize));
  ptr->_logReporter->logMessage(LogReporting::ErrorLevel::verbose, message);

  ptr->_isOpenCLReady = true;
}

double SKOpenCLFindMinmumEnergyGridUnitCell::findMinimumEnergy(std::vector<cl_float>* voxels)
{
  cl_int err;

  if(!_isOpenCLInitialized)
  {
    _logReporter->logMessage(LogReporting::ErrorLevel::error, "OpenCL minimum energy: OpenCL not available");
    return 0.0;
  }

  if(!_isOpenCLReady)
  {
    _logReporter->logMessage(LogReporting::ErrorLevel::error, "OpenCL minimum energy: OpenCL not yet ready");
    return 0.0;
  }

  // make sure the the global work size is an multiple of the work group size
  int temp = voxels->size();
  size_t numberOfGridPoints = (temp  + _workGroupSize-1) & ~(_workGroupSize-1);
  size_t global_work_size = numberOfGridPoints;

  cl_mem rawData = clCreateBuffer(_clContext,  CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,  sizeof(cl_float) * voxels->size(), voxels->data(), &err);
  if (err != CL_SUCCESS)
  {
    QString message = QString("OpenCL minimum energy: error in clCreateBuffer (error: %1)").arg(QString::number(err));
    _logReporter->logMessage(LogReporting::ErrorLevel::error, message);
    return 0;
  }

  int nWorkGroups = numberOfGridPoints/_workGroupSize;

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
    QString message = QString("OpenCL minimum energy: error in clEnqueueNDRangeKernel (error: %1)").arg(QString::number(err));
    _logReporter->logMessage(LogReporting::ErrorLevel::error, message);
    return 0.0;
  }

     // Read the buffer back to the array
  err = clEnqueueReadBuffer(_clCommandQueue, reductionBuffer, CL_TRUE, 0, nWorkGroups*sizeof(float), sumReduction, 0, nullptr, nullptr);
  if (err != CL_SUCCESS)
  {
    QString message = QString("OpenCL minimum energy: error in clEnqueueReadBuffer (error: %1)").arg(QString::number(err));
    _logReporter->logMessage(LogReporting::ErrorLevel::error, message);
    return 0.0;
  }

  clFinish(_clCommandQueue);

  // Final summation with CPU
  double minimumEnergy = 0.0;
  for (int i=0; i<nWorkGroups; i++)
    minimumEnergy = std::min(minimumEnergy, double(sumReduction[i]));

  clReleaseMemObject(rawData);
  clReleaseMemObject(reductionBuffer);

  return minimumEnergy;
}

const char* SKOpenCLFindMinmumEnergyGridUnitCell::_findMinimumEnergyKernel = R"foo(
__kernel void findMinimum (__global const float *input,
                           __global float *partialSums,
                           __local float *localSums)
{
   uint local_id = get_local_id(0);
   uint group_size = get_local_size(0);

   // Copy from global memory to local memory
   localSums[local_id] = input[get_global_id(0)];

   // Loop for computing localSums
   for (uint stride = group_size/2; stride>0; stride/=2) {
      // Waiting for each 2x2 addition into given workgroup
      barrier(CLK_LOCAL_MEM_FENCE);

      // Divide WorkGroup into 2 parts and add elements 2 by 2
      // between local_id and local_id + stride
      if (local_id < stride)
        localSums[local_id] = fmin(localSums[local_id], localSums[local_id + stride]);
   }

   // Write result into partialSums[nWorkGroups]
   if (local_id == 0)
     partialSums[get_group_id(0)] = localSums[0];
}
)foo";
