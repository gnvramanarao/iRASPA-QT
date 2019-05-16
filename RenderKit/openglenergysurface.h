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

#include <vector>
#include <QCache>
#define GL_GLEXT_PROTOTYPES
#include <QtOpenGL>
#include <QGLFunctions>
#include "openglshader.h"
#include "rkrenderkitprotocols.h"
#include <foundationkit.h>
#include <simulationkit.h>

#ifdef Q_OS_MACOS
  #include <OpenCL/opencl.h>
  #include <OpenGL/OpenGL.h>
#else
  #include <CL/opencl.h>
#endif

class OpenGLEnergySurface: public OpenGLShader, public LogReportingConsumer
{
public:
  OpenGLEnergySurface();
  ~OpenGLEnergySurface() override final;
  void loadShader(void) override final;
  void initializeOpenCL(bool isOpenCLInitialized, cl_context _clContext, cl_device_id _clDeviceId, cl_command_queue _clCommandQueue);
  void deleteBuffers();
  void generateBuffers();
  void paintGLOpaque(GLuint structureUniformBuffer, GLuint isosurfaceUniformBuffer);
  void paintGLTransparent(GLuint structureUniformBuffer, GLuint isosurfaceUniformBuffer);

  void setLogReportingWidget(LogReporting *logReporting)  override final;

  void reloadData();
  void initializeVertexArrayObject();
  void setRenderStructures(std::vector<std::vector<std::shared_ptr<RKRenderStructure>>> structures);
  GLuint program() {return _program;}
  void invalidateIsosurface(std::vector<std::shared_ptr<RKRenderStructure>> structures);
  void computeHeliumVoidFraction(std::vector<std::shared_ptr<RKRenderStructure>> structures);
  void computeNitrogenSurfaceArea(std::vector<std::shared_ptr<RKRenderStructure>> structures);
  void setGLInteroperability(bool value) {_energyGridMarchingCubes.setGLInteroperability(value);}
private:
  bool _isOpenCLInitialized;
  GLuint _program;
  std::vector<std::vector<std::shared_ptr<RKRenderStructure>>> _renderStructures;

  LogReporting* _logReporter = nullptr;

  GLuint _atomSurfaceVertexNormalAttributeLocation;
  GLuint _atomSurfaceVertexPositionAttributeLocation;
  GLuint _atomSurfaceInstancePositionAttributeLocation;

  std::vector<std::vector<int>> _surfaceNumberOfIndices;
  std::vector<std::vector<int>> _surfaceNumberOfInstances;
  std::vector<std::vector<GLuint>> _surfaceVertexArrayObject;
  std::vector<std::vector<GLuint>> _surfaceVertexBuffer;
  std::vector<std::vector<GLuint>> _surfaceIndexBuffer;
  std::vector<std::vector<GLuint>> _surfaceInstancePositionBuffer;

  cl_context _clContext;
  cl_device_id _clDeviceId;
  cl_command_queue _clCommandQueue;
  SKOpenCLEnergyGridUnitCell _energyGridUnitCell;
  SKOpenCLMarchingCubes _energyGridMarchingCubes;
  SKOpenCLFindMinmumEnergyGridUnitCell _findMinimumEnergyGridUnitCell;
  SKOpenCLVoidFractionUnitCell _voidFractionUnitCell;

  QCache<RKRenderStructure*, std::vector<cl_float>> _cache;

  static const std::string _vertexShaderSource;
  static const std::string _fragmentShaderSource;
};

