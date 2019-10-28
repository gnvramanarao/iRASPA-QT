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

#pragma once

#include <QtGlobal>
#include <array>
#include <vector>
#include <mathkit.h>
#include <QOpenGLFunctions_3_3_Core>
#include <foundationkit.h>

#ifdef Q_OS_MACOS
  #include <OpenCL/opencl.h>
  #include <OpenGL/OpenGL.h>
#elif _WIN32
 #include <CL/opencl.h>
#else
  #include <CL/opencl.h>
#endif

class SKOpenCLVoidFractionUnitCell: public QOpenGLFunctions_3_3_Core, public LogReportingConsumer
{
public:
  SKOpenCLVoidFractionUnitCell();
  void initialize(bool isOpenCLInitialized, cl_context context, cl_device_id device_id, cl_command_queue queue);
  double computeVoidFraction(std::vector<cl_float> *voxels);
  void setLogReportingWidget(LogReporting *logReporting)  override final {_logReporter = logReporting;}
private:
  LogReporting* _logReporter = nullptr;
  bool _isOpenCLInitialized;
  bool _isOpenCLReady;
  cl_program _program;
  cl_context _clContext;
  cl_command_queue _clCommandQueue;
  cl_device_id _clDeviceId;
  cl_command_queue _queue;
  cl_kernel _kernel;
  size_t _workGroupSize;
  static const char* _voidFractionKernelSource;
  static void __stdcall callBack(cl_program progran, void *user_data);
};
