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

#include "glwidget.h"
#include <QCoreApplication>
#include <QKeyEvent>
#include <QPaintDevice>
#include <foundationkit.h>
#include "quadgeometry.h"
#include <QtOpenGL/QtOpenGL>
#include <iostream>
#include <array>
#include <algorithm>
#include "glgeterror.h"
#include <QDebug>

#if defined(Q_OS_WIN)
  #include "wingdi.h"
#endif

#if defined(Q_OS_LINUX)
  #include <QtPlatformHeaders/QGLXNativeContext>
#endif


GLWidget::GLWidget(QWidget* parent ): QOpenGLWidget(parent),
    _isOpenGLInitialized(false),
    _isOpenCLInitialized(false),
    _backgroundShader(),
    _atomShader(),
    _atomOrthographicImposterShader(_atomShader),
    _atomPerspectiveImposterShader(_atomShader),
    _internalBondShader(),
    _externalBondShader(),
    _unitCellSphereShader(),
    _unitCellCylinderShader(),
    _pickingShader(_atomShader,_atomOrthographicImposterShader),
    _atomSelectionWorleyNoise3DShader(),
    _atomSelectionWorleyNoise3DOrthographicImposterShader(_atomSelectionWorleyNoise3DShader),
    _atomSelectionWorleyNoise3DPerspectiveImposterShader(_atomSelectionWorleyNoise3DShader),
    _atomSelectionStripesShader(),
    _atomSelectionStripesOrthographicImposterShader(_atomSelectionWorleyNoise3DShader),
    _atomSelectionStripesPerspectiveImposterShader(_atomSelectionWorleyNoise3DShader),
    _atomSelectionGlowShader(_atomSelectionWorleyNoise3DShader),
    _blurShader(),
    _atomAmbientOcclusionShader(_atomShader,_atomOrthographicImposterShader)
{
  setAttribute( Qt::WA_OpaquePaintEvent );
  setFocusPolicy(Qt::StrongFocus);

}

void GLWidget::setLogReportingWidget(LogReporting *logReporting)
{
  _logReporter = logReporting;

  _energySurfaceShader.setLogReportingWidget(logReporting);
}

void GLWidget::setRenderDataSource(std::shared_ptr<RKRenderDataSource> source)
{
  makeCurrent();

  _dataSource = source;
  //_atomAmbientOcclusionShader.setRenderDataSource(source);
  if(std::shared_ptr<RKRenderDataSource> dataSource = source)
  {
    _camera = dataSource->camera();
    if (std::shared_ptr<RKCamera> camera = _camera.lock())
    {
      camera->updateCameraForWindowResize(this->size().width(),this->size().height());
      camera->resetForNewBoundingBox(dataSource->renderBoundingBox());
    }

    _renderStructures = dataSource->renderStructures();

    _backgroundShader.reload(dataSource);

    _blurShader.resizeGL(_width, _height);
  }


  reloadData();
  update();

  if (source)
  {
    renderSceneToImage(width(),height());
  }
}

void GLWidget::redraw()
{
  //_quality = RKRenderQuality::high;
  update();
}

void GLWidget::redrawWithLowQuality()
{
  _quality = RKRenderQuality::low;
  update();
}

void GLWidget::redrawWithMediumQuality()
{
  _quality = RKRenderQuality::medium;
  update();
}

void GLWidget::redrawWithHighQuality()
{
  _quality = RKRenderQuality::high;
  update();
}

void GLWidget::redrawWithPictureQuality()
{
  _quality = RKRenderQuality::picture;
  update();
}

void GLWidget::invalidateCachedAmbientOcclusionTexture(std::vector<std::shared_ptr<RKRenderStructure>> structures)
{
  makeCurrent();
  _atomAmbientOcclusionShader.invalidateCachedAmbientOcclusionTexture(structures);
}

void GLWidget::invalidateIsosurface(std::vector<std::shared_ptr<RKRenderStructure>> structures)
{
  makeCurrent();
  _energySurfaceShader.invalidateIsosurface(structures);
}

std::array<int,4> GLWidget::pickTexture(int x, int y, int width, int height)
{
  makeCurrent();
  return _pickingShader.pickTexture(x,y,width,height);
}

void GLWidget::initializeGL()
{
  cl_int err;
  cl_context _clContext = 0;
  cl_device_id _clDeviceId = 0;
  cl_command_queue _clCommandQueue = 0;

  if(!context())
  {
	QMessageBox messageBox;
	messageBox.setFixedSize(650, 200);
	messageBox.critical(nullptr, tr("Critical error"), "No OpenGL context present, install OpenGL drivers (>=3.3)");
	QApplication::quit();
  }

  initializeOpenGLFunctions();
  _backgroundShader.initializeOpenGLFunctions();
  _atomShader.initializeOpenGLFunctions();
  _atomOrthographicImposterShader.initializeOpenGLFunctions();
  _atomPerspectiveImposterShader.initializeOpenGLFunctions();
  _internalBondShader.initializeOpenGLFunctions();
  _externalBondShader.initializeOpenGLFunctions();
  _unitCellSphereShader.initializeOpenGLFunctions();
  _unitCellCylinderShader.initializeOpenGLFunctions();
  _pickingShader.initializeOpenGLFunctions();
  _atomSelectionWorleyNoise3DShader.initializeOpenGLFunctions();
  _atomSelectionWorleyNoise3DOrthographicImposterShader.initializeOpenGLFunctions();
  _atomSelectionWorleyNoise3DPerspectiveImposterShader.initializeOpenGLFunctions();
  _atomSelectionStripesShader.initializeOpenGLFunctions();
   _atomSelectionStripesOrthographicImposterShader.initializeOpenGLFunctions();
  _atomSelectionStripesPerspectiveImposterShader.initializeOpenGLFunctions();
  _atomSelectionGlowShader.initializeOpenGLFunctions();
  _blurShader.initializeOpenGLFunctions();
  _atomAmbientOcclusionShader.initializeOpenGLFunctions();
  _energySurfaceShader.initializeOpenGLFunctions();


  _logReporter->logMessage(LogReporting::ErrorLevel::verbose, "OpenGL initialized");

  // get OpenGL capabilities
  glGetIntegerv(GL_MAX_CLIP_DISTANCES, &_maxNumberOfClipPlanes);
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &_maxTextureSize);

  _logReporter->logMessage(LogReporting::ErrorLevel::verbose, "max texture size: " + QString::number(_maxTextureSize));

  glGetIntegerv(GLenum(GL_MAX_SAMPLES), &_maxSampleCount);
  glGetIntegerv(GL_MAX_COLOR_TEXTURE_SAMPLES, &_maxSampleColorCount);
  glGetIntegerv(GL_MAX_DEPTH_TEXTURE_SAMPLES, &_maxSampleDepthCount);
  _maxMultiSampling = std::min(_maxSampleCount, std::min(_maxSampleColorCount, _maxSampleDepthCount));
  _multiSampling = std::max(1,std::min(8, std::min(_maxSampleCount, std::min(_maxSampleColorCount, _maxSampleDepthCount))));

  _logReporter->logMessage(LogReporting::ErrorLevel::verbose, "multisampling: " + QString::number(_multiSampling));

  _devicePixelRatio = QPaintDevice:: devicePixelRatio();
  _width = std::max(512, this->width());
  _height = std::max(512, this->height());

  _logReporter->logMessage(LogReporting::ErrorLevel::verbose, "pixel device ratio: " + QString::number(_devicePixelRatio));

  glGenFramebuffers(1, &_sceneFrameBuffer);
  glGenTextures(1, &_sceneDepthTexture);
  glGenTextures(1, &_sceneTexture);
  glGenTextures(1, &_glowSelectionTexture);
  check_gl_error();

  glBindFramebuffer(GL_FRAMEBUFFER, _sceneFrameBuffer);
  check_gl_error();

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, _sceneTexture);
  glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, _multiSampling, GL_RGBA16F, _width * _devicePixelRatio, _height * _devicePixelRatio, GL_TRUE);
  check_gl_error();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
  check_gl_error();

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, _glowSelectionTexture);
  glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, _multiSampling, GL_RGBA16F, _width * _devicePixelRatio, _height * _devicePixelRatio, GL_TRUE);
  check_gl_error();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  check_gl_error();
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, _sceneDepthTexture);
  check_gl_error();
  glTexParameteri(GL_TEXTURE_2D, GLenum(GL_TEXTURE_WRAP_S), GLint(GL_CLAMP_TO_EDGE));
  check_gl_error();
  glTexParameteri(GL_TEXTURE_2D, GLenum(GL_TEXTURE_WRAP_T), GLint(GL_CLAMP_TO_EDGE));
  check_gl_error();
  glTexParameteri(GL_TEXTURE_2D, GLenum(GL_TEXTURE_MIN_FILTER), GLint(GL_NEAREST));
  check_gl_error();
  glTexParameteri(GL_TEXTURE_2D, GLenum(GL_TEXTURE_MAG_FILTER), GLint(GL_NEAREST));
  check_gl_error();
  glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, _multiSampling, GL_DEPTH32F_STENCIL8, _width * _devicePixelRatio, _height * _devicePixelRatio, GL_TRUE);
  check_gl_error();


  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, _sceneTexture, 0);
  check_gl_error();
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D_MULTISAMPLE, _glowSelectionTexture, 0);
  check_gl_error();
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, _sceneDepthTexture, 0);
  check_gl_error();

  // check framebuffer completeness
  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE)
  {
    qWarning("initializeSceneFrameBuffer fatal error: framebuffer incomplete");
  }

  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  std::cout << "OpenGLContext: " << context() << std::endl;
  _blurShader.initializeFramebuffers();

  _pickingShader.generateFrameBuffers();


  // Set up the context with OpenCL/OpenGL interop.
  #if defined (Q_OS_OSX)
    CGLContextObj cgl_current_context = CGLGetCurrentContext();
    CGLShareGroupObj cgl_share_group = CGLGetShareGroup(cgl_current_context);
    cl_context_properties properties[] = { CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
                                             cl_context_properties(cgl_share_group),
                                             0 };

    _clContext = clCreateContext(properties, 0, nullptr, nullptr, nullptr, &err);
    if (err == CL_SUCCESS)
    {
      _logReporter->logMessage(LogReporting::ErrorLevel::verbose, "Create OpenCL context");

      if (clGetGLContextInfoAPPLE(_clContext, static_cast<void *>(CGLGetCurrentContext()),
                                   CL_CGL_DEVICE_FOR_CURRENT_VIRTUAL_SCREEN_APPLE,
                                   sizeof(cl_device_id), &_clDeviceId, nullptr) == CL_SUCCESS)
      {
        _clCommandQueue = clCreateCommandQueue(_clContext, _clDeviceId, 0, &err);
        if(err==CL_SUCCESS)
        {
          _logReporter->logMessage(LogReporting::ErrorLevel::verbose, "OpenCL initialized");
          _isOpenCLInitialized = true;
        }
        else
        {
          _logReporter->logMessage(LogReporting::ErrorLevel::error, "OpenCL command-queue creation failed");
        }
      }
      else
      {
        _logReporter->logMessage(LogReporting::ErrorLevel::error, "Failed to get OpenCL device for current screen, error code: " + QString::number(err));
      }
    }
    else
    {
      _logReporter->logMessage(LogReporting::ErrorLevel::error, "Failed to create OpenCL context, error code: " + QString::number(err));
    }
  #elif defined(Q_OS_WIN)


	cl_uint n;
	err = clGetPlatformIDs(0, nullptr, &n);
	if (n > 0)
	{
		QVector<cl_platform_id> platformIds;
		platformIds.resize(n);
		if (clGetPlatformIDs(n, platformIds.data(), 0) == CL_SUCCESS)
		{
			cl_platform_id platform = platformIds[0];
			QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
			const char *vendor = (const char *)f->glGetString(GL_VENDOR);
			_logReporter->logMessage(LogReporting::ErrorLevel::verbose, "OpenGL vender: " + QString::fromUtf8(vendor));
			const bool isNV = vendor && strstr(vendor, "NVIDIA");
			const bool isIntel = vendor && strstr(vendor, "Intel");
			const bool isAMD = vendor && strstr(vendor, "ATI");

			_logReporter->logMessage(LogReporting::ErrorLevel::verbose, "Found " + QString::number(n) + " OpenCL platforms.");

			for (cl_uint i = 0; i < n; ++i)
			{
				QByteArray name;
				name.resize(1024);
				clGetPlatformInfo(platformIds[i], CL_PLATFORM_NAME, name.size(), name.data(), 0);
				qDebug("Platform %p: %s", platformIds[i], name.constData());
				// Running with an OpenCL platform without GPU support is not going
				// to cut it. In practice we want the platform for the GPU which we
				// are using with OpenGL.
				if (isNV && name.contains(QByteArrayLiteral("NVIDIA")))
				{
					platform = platformIds[i];
					_logReporter->logMessage(LogReporting::ErrorLevel::verbose, "Using NVIDIA platform.");
					break;
				}
				else if (isIntel && name.contains(QByteArrayLiteral("Intel")))
				{
					platform = platformIds[i];
					_logReporter->logMessage(LogReporting::ErrorLevel::verbose, "Using Intel platform.");
					break;
				}
				else if (isAMD && name.contains(QByteArrayLiteral("AMD")))
				{
					platform = platformIds[i];
					_logReporter->logMessage(LogReporting::ErrorLevel::verbose, "Using AMD platform.");
					break;
				}
			}


			// Count the number of devices in the platform
			cl_uint num_devices;

			// Get the device IDs
			err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &_clDeviceId, nullptr);

			_clContext = clCreateContext(nullptr, 1, &_clDeviceId, nullptr, nullptr, &err);

			if (err == CL_SUCCESS)
			{
				_logReporter->logMessage(LogReporting::ErrorLevel::verbose, "OpenCL initialized");
			}
			else
			{
				_logReporter->logMessage(LogReporting::ErrorLevel::error, "OpenCL initialization failed");
			}
			_energySurfaceShader.setGLInteroperability(false);

			_clCommandQueue = clCreateCommandQueue(_clContext, _clDeviceId, 0, &err);

			if (err == CL_SUCCESS)
			{
				_logReporter->logMessage(LogReporting::ErrorLevel::verbose, "OpenCL command-queue created");
				_isOpenCLInitialized = true;
			}
			else
			{
				_logReporter->logMessage(LogReporting::ErrorLevel::error, "OpenCL command-queue creation failed");
			}
		}
		else
		{
			_logReporter->logMessage(LogReporting::ErrorLevel::verbose, "Failed to get platform IDs.");
		}
	}
	else
	{
		_logReporter->logMessage(LogReporting::ErrorLevel::verbose, "OpenCL initialization failed: no platforms found.");
	}

  #elif defined(Q_OS_LINUX)

  cl_uint n;
  err = clGetPlatformIDs(0, nullptr, &n);
  if (n > 0)
  {
    QVector<cl_platform_id> platformIds;
    platformIds.resize(n);
    if (clGetPlatformIDs(n, platformIds.data(), 0) == CL_SUCCESS)
    {
      cl_platform_id platform = platformIds[0];
      QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
      const char *vendor = (const char *) f->glGetString(GL_VENDOR);
      _logReporter->logMessage(LogReporting::ErrorLevel::verbose, "OpenGL vender: " + QString::fromUtf8(vendor));
      const bool isNV = vendor && strstr(vendor, "NVIDIA");
      const bool isIntel = vendor && strstr(vendor, "Intel");
      const bool isAMD = vendor && strstr(vendor, "ATI");

      _logReporter->logMessage(LogReporting::ErrorLevel::verbose, "Found " + QString::number(n) + " OpenCL platforms.");

      for (cl_uint i = 0; i < n; ++i)
      {
        QByteArray name;
        name.resize(1024);
        clGetPlatformInfo(platformIds[i], CL_PLATFORM_NAME, name.size(), name.data(), 0);
        qDebug("Platform %p: %s", platformIds[i], name.constData());
        // Running with an OpenCL platform without GPU support is not going
        // to cut it. In practice we want the platform for the GPU which we
        // are using with OpenGL.
        if (isNV && name.contains(QByteArrayLiteral("NVIDIA")))
        {
          platform = platformIds[i];
          _logReporter->logMessage(LogReporting::ErrorLevel::verbose, "Using NVIDIA platform.");
          break;
        }
        else if (isIntel && name.contains(QByteArrayLiteral("Intel")))
        {
          platform = platformIds[i];
          _logReporter->logMessage(LogReporting::ErrorLevel::verbose, "Using Intel platform.");
          break;
        }
        else if (isAMD && name.contains(QByteArrayLiteral("AMD")))
        {
          platform = platformIds[i];
          _logReporter->logMessage(LogReporting::ErrorLevel::verbose, "Using AMD platform.");
          break;
        }
      }


      // Count the number of devices in the platform
      cl_uint num_devices;

      // Get the device IDs
      err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &_clDeviceId, nullptr);

      _clContext = clCreateContext(nullptr, 1, &_clDeviceId, nullptr, nullptr, &err);

      if(err==CL_SUCCESS)
      {
        _logReporter->logMessage(LogReporting::ErrorLevel::verbose, "OpenCL initialized");
      }
      else
      {
        _logReporter->logMessage(LogReporting::ErrorLevel::error, "OpenCL initialization failed");
      }
      _energySurfaceShader.setGLInteroperability(false);

      _clCommandQueue = clCreateCommandQueue(_clContext, _clDeviceId, 0, &err);

      if(err == CL_SUCCESS)
      {
        _logReporter->logMessage(LogReporting::ErrorLevel::verbose, "OpenCL command-queue created");
        _isOpenCLInitialized = true;
      }
      else
      {
        _logReporter->logMessage(LogReporting::ErrorLevel::error, "OpenCL command-queue creation failed");
      }
    }
    else
    {
      _logReporter->logMessage(LogReporting::ErrorLevel::verbose, "Failed to get platform IDs.");
    }
  }
  else
  {
    _logReporter->logMessage(LogReporting::ErrorLevel::verbose, "OpenCL initialization failed: no platforms found.");
  }
  #endif

  _energySurfaceShader.initializeOpenCL(_isOpenCLInitialized, _clContext, _clDeviceId, _clCommandQueue);

  // Set the clear color to white
  glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );

  this->loadShader();
  _backgroundShader.loadShader();
  _atomShader.loadShader();
  _atomOrthographicImposterShader.loadShader();
  _atomPerspectiveImposterShader.loadShader();
  _internalBondShader.loadShader();
  _externalBondShader.loadShader();
  _unitCellSphereShader.loadShader();
  _unitCellCylinderShader.loadShader();
  _pickingShader.loadShader();
  _atomSelectionWorleyNoise3DShader.loadShader();
  _atomSelectionWorleyNoise3DOrthographicImposterShader.loadShader();
  _atomSelectionWorleyNoise3DPerspectiveImposterShader.loadShader();
  _atomSelectionStripesShader.loadShader();
  _atomSelectionStripesOrthographicImposterShader.loadShader();
  _atomSelectionStripesPerspectiveImposterShader.loadShader();
  _atomSelectionGlowShader.loadShader();
  _blurShader.loadShader();
  _atomAmbientOcclusionShader.loadShader();
  _energySurfaceShader.loadShader();

  _atomShader.generateBuffers();
  _atomOrthographicImposterShader.generateBuffers();
  _atomPerspectiveImposterShader.generateBuffers();
  _internalBondShader.generateBuffers();
  _externalBondShader.generateBuffers();
  _unitCellSphereShader.generateBuffers();
  _unitCellCylinderShader.generateBuffers();
  _pickingShader.generateBuffers();
  _atomSelectionWorleyNoise3DShader.generateBuffers();
  _atomSelectionWorleyNoise3DOrthographicImposterShader.generateBuffers();
  _atomSelectionWorleyNoise3DPerspectiveImposterShader.generateBuffers();
  _atomSelectionStripesShader.generateBuffers();
  _atomSelectionStripesOrthographicImposterShader.generateBuffers();
  _atomSelectionStripesPerspectiveImposterShader.generateBuffers();
  _atomSelectionGlowShader.generateBuffers();
  _atomAmbientOcclusionShader.generateBuffers();
  _energySurfaceShader.generateBuffers();

  _backgroundShader.initializeVertexArrayObject();
  _atomShader.initializeVertexArrayObject();
  _atomOrthographicImposterShader.initializeVertexArrayObject();
  _atomPerspectiveImposterShader.initializeVertexArrayObject();
  _internalBondShader.initializeVertexArrayObject();
  _externalBondShader.initializeVertexArrayObject();
  _unitCellSphereShader.initializeVertexArrayObject();
  _unitCellSphereShader.initializeVertexArrayObject();
  _pickingShader.initializeVertexArrayObject();
  _atomSelectionWorleyNoise3DShader.initializeVertexArrayObject();
  _atomSelectionWorleyNoise3DOrthographicImposterShader.initializeVertexArrayObject();
  _atomSelectionWorleyNoise3DPerspectiveImposterShader.initializeVertexArrayObject();
  _atomSelectionStripesShader.initializeVertexArrayObject();
  _atomSelectionStripesOrthographicImposterShader.initializeVertexArrayObject();
  _atomSelectionStripesPerspectiveImposterShader.initializeVertexArrayObject();
  _atomSelectionGlowShader.initializeVertexArrayObject();
  _atomAmbientOcclusionShader.initializeVertexArrayObject();
  _energySurfaceShader.initializeVertexArrayObject();
  _blurShader.initializeVertexArrayObject();

  initializeTransformUniforms();
  check_gl_error();
  initializeStructureUniforms();
  check_gl_error();
  initializeIsosurfaceUniforms();
  check_gl_error();
  initializeLightUniforms();
  check_gl_error();



  glGenVertexArrays(1,&_downSamplerVertexArray);
  glGenBuffers(1, &_quad_vertexbuffer);
  glGenBuffers(1, &_quad_indexbuffer);

  QuadGeometry quad = QuadGeometry();

  glBindVertexArray(_downSamplerVertexArray);
  check_gl_error();

  glBindBuffer(GL_ARRAY_BUFFER, _quad_vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, quad.vertices().size() * sizeof(RKVertex), quad.vertices().data(), GL_STATIC_DRAW);
  check_gl_error();

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _quad_indexbuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, quad.indices().size()*sizeof(GLushort), quad.indices().data(), GL_STATIC_DRAW);
  check_gl_error();

  glVertexAttribPointer(_downSamplePositionAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKVertex), reinterpret_cast<GLvoid*>(offsetof(RKVertex,position)));
  glEnableVertexAttribArray(_downSamplePositionAttributeLocation);
  check_gl_error();

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, _sceneTexture);
  check_gl_error();

  glBindVertexArray(0);
  check_gl_error();



  _isOpenGLInitialized = true;
}

void GLWidget::resizeGL( int w, int h )
{
  if (std::shared_ptr<RKCamera> camera = _camera.lock())
  {
    camera->updateCameraForWindowResize(w, h);
  }

  _pickingShader.resizeGL(w,h);


  _width = std::max(16, w);
  _height = std::max(16, h);

  glBindFramebuffer(GL_FRAMEBUFFER, _sceneFrameBuffer);

  // on nvidia, the multisampled (depth-)texture needs to be recreated to avoid memory corruption
  if(_sceneTexture)
    glDeleteTextures(1, &_sceneTexture);
  glGenTextures(1, &_sceneTexture);

  if(_sceneDepthTexture)
    glDeleteTextures(1, &_sceneDepthTexture);
  glGenTextures(1, &_sceneDepthTexture);

  if(_glowSelectionTexture)
    glDeleteTextures(1, &_glowSelectionTexture);
  glGenTextures(1, &_glowSelectionTexture);


  glBindFramebuffer(GL_FRAMEBUFFER, _sceneFrameBuffer);
  glGetError();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, _sceneTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, _multiSampling, GL_RGBA16F, _width * _devicePixelRatio, _height * _devicePixelRatio, GL_TRUE);
  glGetError();
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, _sceneTexture, 0);
  glGetError();
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, _glowSelectionTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, _multiSampling, GL_RGBA16F, _width * _devicePixelRatio, _height * _devicePixelRatio, GL_TRUE);
  glGetError();
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D_MULTISAMPLE, _glowSelectionTexture, 0);
  glGetError();
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, _sceneDepthTexture);
  glTexParameteri(GL_TEXTURE_2D, GLenum(GL_TEXTURE_WRAP_S), GLint(GL_CLAMP_TO_EDGE));
  glTexParameteri(GL_TEXTURE_2D, GLenum(GL_TEXTURE_WRAP_T), GLint(GL_CLAMP_TO_EDGE));
  glTexParameteri(GL_TEXTURE_2D, GLenum(GL_TEXTURE_MIN_FILTER), GLint(GL_NEAREST));
  glTexParameteri(GL_TEXTURE_2D, GLenum(GL_TEXTURE_MAG_FILTER), GLint(GL_NEAREST));
  glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, _multiSampling, GL_DEPTH32F_STENCIL8, _width * _devicePixelRatio, _height * _devicePixelRatio, GL_TRUE);
  glGetError();
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, _sceneDepthTexture, 0);
  glGetError();

  // check framebuffer completeness
  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE)
  {
    qWarning("initializeSceneFrameBuffer fatal error: framebuffer incomplete: %0x",status);
    glGetError();
  }

  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  if(std::shared_ptr<RKRenderDataSource> dataSource = _dataSource)
  {
    _blurShader.resizeGL(_width,_height);
  }
  std::cout << "end resize" << std::endl;
}


void GLWidget::paintGL()
{
  updateStructureUniforms();
  updateTransformUniforms();
  updateIsosurfaceUniforms();
  updateLightUniforms();

  GLint prev_fbo_id;
  glGetIntegerv( GL_FRAMEBUFFER_BINDING, &prev_fbo_id );

  // pass mouse coordinates (real coordinates twice larger for retina-displays)
  _pickingShader.paintGL(_width, _height, _structureUniformBuffer);

  glViewport(0,0,_width * _devicePixelRatio,_height * _devicePixelRatio);

  drawSceneToFramebuffer(_sceneFrameBuffer);

  _blurShader.paintGL(_glowSelectionTexture, _width, _height);

  glBindFramebuffer(GL_DRAW_FRAMEBUFFER,prev_fbo_id);
  glViewport(0,0,_width * _devicePixelRatio, _height * _devicePixelRatio);
  glDrawBuffer(GL_COLOR_ATTACHMENT0);

  glUseProgram(_program);
  glBindVertexArray(_downSamplerVertexArray);

  // input scene texture
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, _sceneTexture);
  glUniform1i(_downSampleInputTextureUniformLocation, 0);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, _blurShader.blurredTexture());
  glUniform1i(_blurredInputTextureUniformLocation, 1);
  glDrawElements(GL_TRIANGLE_STRIP,4,GL_UNSIGNED_SHORT, nullptr);

  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(0);
  glUseProgram(0);
}

QImage GLWidget::renderSceneToImage(int width, int height)
{
  GLuint sceneFrameBuffer;
  GLuint sceneDepthTexture;
  GLuint sceneTexture;
  GLuint glowSelectionTexture;

  makeCurrent();

  //int multiSampling = 8;

  updateStructureUniforms();
  updateTransformUniforms();
  updateIsosurfaceUniforms();
  updateLightUniforms();

  glGenFramebuffers(1, &sceneFrameBuffer);
  glGenTextures(1, &sceneDepthTexture);
  glGenTextures(1, &sceneTexture);
  glGenTextures(1, &glowSelectionTexture);
  check_gl_error();

  glBindFramebuffer(GL_FRAMEBUFFER, sceneFrameBuffer);
  check_gl_error();

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, sceneTexture);
  glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, _multiSampling, GL_RGBA16F, width, height, GL_TRUE);
  check_gl_error();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
  check_gl_error();

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, glowSelectionTexture);
  glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, _multiSampling, GL_RGBA16F, width, height, GL_TRUE);
  check_gl_error();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  check_gl_error();
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, sceneDepthTexture);
  check_gl_error();
  glTexParameteri(GL_TEXTURE_2D, GLenum(GL_TEXTURE_WRAP_S), GLint(GL_CLAMP_TO_EDGE));
  check_gl_error();
  glTexParameteri(GL_TEXTURE_2D, GLenum(GL_TEXTURE_WRAP_T), GLint(GL_CLAMP_TO_EDGE));
  check_gl_error();
  glTexParameteri(GL_TEXTURE_2D, GLenum(GL_TEXTURE_MIN_FILTER), GLint(GL_NEAREST));
  check_gl_error();
  glTexParameteri(GL_TEXTURE_2D, GLenum(GL_TEXTURE_MAG_FILTER), GLint(GL_NEAREST));
  check_gl_error();
  glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, _multiSampling, GL_DEPTH32F_STENCIL8, width, height, GL_TRUE);
  check_gl_error();


  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, sceneTexture, 0);
  check_gl_error();
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D_MULTISAMPLE, glowSelectionTexture, 0);
  check_gl_error();
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, sceneDepthTexture, 0);
  check_gl_error();

  // check framebuffer completeness
  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE)
  {
    qWarning("initializeSceneFrameBuffer fatal error: framebuffer incomplete");
  }

  glBindTexture(GL_TEXTURE_2D, 0);
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

  glViewport(0,0,width,height);

  drawSceneToFramebuffer(sceneFrameBuffer);

  //s_blurShader.paintGL(glowSelectionTexture, width, height);


  GLuint downSamplerFrameBufferObject;
  GLuint downSamplerTexture;

  glGenFramebuffers(1, &downSamplerFrameBufferObject);
  glGenTextures(1, &downSamplerTexture);


  glBindFramebuffer(GL_FRAMEBUFFER, downSamplerFrameBufferObject);
  check_gl_error();

  glBindTexture(GL_TEXTURE_2D, downSamplerTexture);
  check_gl_error();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
  check_gl_error();
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, downSamplerTexture, 0);
  check_gl_error();
  glBindTexture(GL_TEXTURE_2D, 0);

  // check framebuffer completeness
  status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE)
  {
    qWarning("initializeHorizontalBlurFrameBuffer fatal error: framebuffer incomplete");
  }

  glViewport(0,0,width, height);
  check_gl_error();
  glDrawBuffer(GL_COLOR_ATTACHMENT0);
  check_gl_error();

  glUseProgram(_program);
  check_gl_error();
  glBindVertexArray(_downSamplerVertexArray);
  check_gl_error();

  // input scene texture
  glActiveTexture(GL_TEXTURE0);
  check_gl_error();
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, sceneTexture);
  check_gl_error();
  glUniform1i(_downSampleInputTextureUniformLocation, 0);
  check_gl_error();

 // glActiveTexture(GL_TEXTURE1);
//  glBindTexture(GL_TEXTURE_2D, _blurShader.blurredTexture());
//  glUniform1i(_blurredInputTextureUniformLocation, 1);
  glDrawElements(GL_TRIANGLE_STRIP,4,GL_UNSIGNED_SHORT, nullptr);
  check_gl_error();

  glBindVertexArray(0);
  check_gl_error();
  glUseProgram(0);
  check_gl_error();

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  check_gl_error();

  QImage img(width, height, QImage::Format_ARGB32);
  glBindTexture(GL_TEXTURE_2D, downSamplerTexture);
  glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, img.bits());
  glBindTexture(GL_TEXTURE_2D, 0);

  glDeleteTextures(1, &downSamplerTexture);
  glDeleteFramebuffers(1, &downSamplerFrameBufferObject);

  glDeleteTextures(1, &glowSelectionTexture);
  glDeleteTextures(1, &sceneTexture);
  glDeleteTextures(1, &sceneDepthTexture);
  glDeleteFramebuffers(1, &sceneFrameBuffer);

  return img.mirrored();
}

void GLWidget::drawSceneToFramebuffer(GLuint framebuffer)
{
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER,framebuffer);
  glDrawBuffer(GL_COLOR_ATTACHMENT0);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  check_gl_error();

  // clear the color, depth, and stencil buffer
  // the background default color is white
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  _backgroundShader.paintGL();

  glEnable(GL_DEPTH_TEST);


  //_atomShader.paintGL(_atomAmbientOcclusionShader.generatedAmbientOcclusionTextures());
  if (std::shared_ptr<RKCamera> camera = _camera.lock())
  {

    if(camera->isOrthographic())
    {
      if(_quality == RKRenderQuality:: high || _quality == RKRenderQuality:: picture)
      {
        _atomShader.paintGL(_atomAmbientOcclusionShader.generatedAmbientOcclusionTextures(), _structureUniformBuffer);
      }
      else
      {
        _atomOrthographicImposterShader.paintGL(_atomAmbientOcclusionShader.generatedAmbientOcclusionTextures(), _structureUniformBuffer);
      }

      _internalBondShader.paintGL(_structureUniformBuffer);
      _externalBondShader.paintGL(_structureUniformBuffer);
      _unitCellSphereShader.paintGL(_structureUniformBuffer);
      _unitCellCylinderShader.paintGL(_structureUniformBuffer);

      _energySurfaceShader.paintGLOpaque(_structureUniformBuffer,_isosurfaceUniformBuffer);

      if(_quality == RKRenderQuality:: high || _quality == RKRenderQuality:: picture)
      {
        _atomSelectionWorleyNoise3DShader.paintGL(_structureUniformBuffer);
        _atomSelectionStripesShader.paintGL(_structureUniformBuffer);
      }
      else
      {
        _atomSelectionWorleyNoise3DOrthographicImposterShader.paintGL(_structureUniformBuffer);
        _atomSelectionStripesOrthographicImposterShader.paintGL(_structureUniformBuffer);
      }

      _energySurfaceShader.paintGLTransparent(_structureUniformBuffer,_isosurfaceUniformBuffer);
    }
    else
    {
      if(_quality == RKRenderQuality:: high || _quality == RKRenderQuality:: picture)
      {
        _atomShader.paintGL(_atomAmbientOcclusionShader.generatedAmbientOcclusionTextures(), _structureUniformBuffer);
      }
      else
      {
        _atomPerspectiveImposterShader.paintGL(_atomAmbientOcclusionShader.generatedAmbientOcclusionTextures(), _structureUniformBuffer);
      }

      _internalBondShader.paintGL(_structureUniformBuffer);
      _externalBondShader.paintGL(_structureUniformBuffer);
      _unitCellSphereShader.paintGL(_structureUniformBuffer);
      _unitCellCylinderShader.paintGL(_structureUniformBuffer);

      _energySurfaceShader.paintGLOpaque(_structureUniformBuffer,_isosurfaceUniformBuffer);

      if(_quality == RKRenderQuality:: high || _quality == RKRenderQuality:: picture)
      {
        _atomSelectionWorleyNoise3DShader.paintGL(_structureUniformBuffer);
        _atomSelectionStripesShader.paintGL(_structureUniformBuffer);
      }
      else
      {
        _atomSelectionWorleyNoise3DPerspectiveImposterShader.paintGL(_structureUniformBuffer);
        _atomSelectionStripesPerspectiveImposterShader.paintGL(_structureUniformBuffer);
      }

      _energySurfaceShader.paintGLTransparent(_structureUniformBuffer,_isosurfaceUniformBuffer);
    }

  }

  glDrawBuffer(GL_COLOR_ATTACHMENT1);
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);
  _atomSelectionGlowShader.paintGL(_structureUniformBuffer);

  glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
}

void GLWidget::keyPressEvent( QKeyEvent* e )
{
  makeCurrent();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
  makeCurrent();
  _startPoint = event->pos();
  _origin = event->pos();

  _quality = RKRenderQuality::medium;

  if (std::shared_ptr<RKCamera> camera = _camera.lock())
  {
    camera->setTrackBallRotation(simd_quatd(1.0, double3(0.0, 0.0, 0.0)));
  }


  if(event->modifiers() & Qt::ShiftModifier)   // contains shift
  {
    // Using the shift key means a new selection is chosen. If later a drag occurs it is modified to 'draggedNewSelection'
    _tracking = Tracking::newSelection;
  }
  else if(event->modifiers()  == Qt::ControlModifier)  // command, not option
  {
    // Using the command key means the selection is extended. If later a drag occurs it is modified to 'draggedAddToSelection'
    _tracking = Tracking::addToSelection;

  }
  else if(event->modifiers()  & Qt::AltModifier & Qt::ControlModifier)  // option and command
  {
    _tracking = Tracking::translateSelection;
  }
  else if(event->modifiers()  == Qt::AltModifier) // option, not command
  {
    _tracking = Tracking::measurement;
    _trackBall.start(event->x(),event->y(), 0, 0, this->width(), this->height());
  }
  else
  {
    _tracking = Tracking::backgroundClick;
    _trackBall.start(event->x(),event->y(), 0, 0, this->width(), this->height());
  }


  update();
}


void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
  makeCurrent();
  switch(_tracking)
  {
    case Tracking::newSelection:
      // convert to dragged version
      _tracking = Tracking::draggedNewSelection;
      break;
    case Tracking::addToSelection:
       // convert to dragged version
       _tracking = Tracking::draggedAddToSelection;
       break;
    case Tracking::draggedNewSelection:
       break;
    case Tracking::draggedAddToSelection:
      break;
    case Tracking::translateSelection:
       break;
    case Tracking::measurement:
       break;
    default:
       _tracking =Tracking::other;
       if(_startPoint)
       {
         simd_quatd trackBallRotation = _trackBall.rollToTrackball(event->x(), event->y());

         if (std::shared_ptr<RKCamera> camera = _camera.lock())
         {
           camera->setTrackBallRotation(trackBallRotation);
         }
       }
       break;
  }

  update();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
  makeCurrent();

  switch(_tracking)
  {
    case Tracking::newSelection:
      break;
    case Tracking::addToSelection:
      break;
    case Tracking::draggedNewSelection:
      break;
    case Tracking::draggedAddToSelection:
      break;
    case Tracking::translateSelection:
      break;
    case Tracking::measurement:
      break;
    case Tracking::backgroundClick:
      break;
    default:
      simd_quatd trackBallRotation = _trackBall.rollToTrackball(event->x(), event->y() );

      if (std::shared_ptr<RKCamera> camera = _camera.lock())
      {
        simd_quatd worldRotation = camera->worldRotation();
        camera->setWorldRotation(trackBallRotation * worldRotation);
        camera->setTrackBallRotation(simd_quatd(1.0, double3(0.0, 0.0, 0.0)));
      }
      break;
  }

  _quality = RKRenderQuality::high;

  update();
}


void GLWidget::wheelEvent(QWheelEvent *event)
{
  makeCurrent();
  if (std::shared_ptr<RKCamera> camera = _camera.lock())
  {
    camera->increaseDistance(event->delta()/40.0);
  }
  update();
}

void GLWidget::initializeTransformUniforms()
{
  glGenBuffers(1, &_frameUniformBuffer);
  check_gl_error();

  glBindBufferBase(GL_UNIFORM_BUFFER, 0, _frameUniformBuffer);
  check_gl_error();
  glUniformBlockBinding(_program, glGetUniformBlockIndex(_program, "FrameUniformBlock"), 0);
  glUniformBlockBinding(_atomShader.program(), glGetUniformBlockIndex(_atomShader.program(), "FrameUniformBlock"), 0);
  glUniformBlockBinding(_atomOrthographicImposterShader.program(), glGetUniformBlockIndex(_atomOrthographicImposterShader.program(), "FrameUniformBlock"), 0);
  glUniformBlockBinding(_atomPerspectiveImposterShader.program(), glGetUniformBlockIndex(_atomPerspectiveImposterShader.program(), "FrameUniformBlock"), 0);
  glUniformBlockBinding(_internalBondShader.program(), glGetUniformBlockIndex(_internalBondShader.program(), "FrameUniformBlock"), 0);
  glUniformBlockBinding(_externalBondShader.program(), glGetUniformBlockIndex(_externalBondShader.program(), "FrameUniformBlock"), 0);
  glUniformBlockBinding(_externalBondShader.stencilProgram(), glGetUniformBlockIndex(_externalBondShader.stencilProgram(), "FrameUniformBlock"), 0);
  glUniformBlockBinding(_externalBondShader.boxProgram(), glGetUniformBlockIndex(_externalBondShader.boxProgram(), "FrameUniformBlock"), 0);
  glUniformBlockBinding(_unitCellSphereShader.program(), glGetUniformBlockIndex(_unitCellSphereShader.program(), "FrameUniformBlock"), 0);
  glUniformBlockBinding(_unitCellCylinderShader.program(), glGetUniformBlockIndex(_unitCellCylinderShader.program(), "FrameUniformBlock"), 0);
  glUniformBlockBinding(_pickingShader.atomPickingProgram(), glGetUniformBlockIndex(_pickingShader.atomPickingProgram(), "FrameUniformBlock"), 0);
  glUniformBlockBinding(_atomSelectionWorleyNoise3DShader.program(), glGetUniformBlockIndex(_atomSelectionWorleyNoise3DShader.program(), "FrameUniformBlock"), 0);
  glUniformBlockBinding(_atomSelectionWorleyNoise3DOrthographicImposterShader.program(), glGetUniformBlockIndex(_atomSelectionWorleyNoise3DOrthographicImposterShader.program(), "FrameUniformBlock"), 0);
  glUniformBlockBinding(_atomSelectionWorleyNoise3DPerspectiveImposterShader.program(), glGetUniformBlockIndex(_atomSelectionWorleyNoise3DPerspectiveImposterShader.program(), "FrameUniformBlock"), 0);
  glUniformBlockBinding(_atomSelectionStripesShader.program(), glGetUniformBlockIndex(_atomSelectionStripesShader.program(), "FrameUniformBlock"), 0);
  glUniformBlockBinding(_atomSelectionStripesOrthographicImposterShader.program(), glGetUniformBlockIndex(_atomSelectionStripesOrthographicImposterShader.program(), "FrameUniformBlock"), 0);
  glUniformBlockBinding(_atomSelectionStripesPerspectiveImposterShader.program(), glGetUniformBlockIndex(_atomSelectionStripesPerspectiveImposterShader.program(), "FrameUniformBlock"), 0);
  glUniformBlockBinding(_atomSelectionGlowShader.program(), glGetUniformBlockIndex(_atomSelectionGlowShader.program(), "FrameUniformBlock"), 0);
  glUniformBlockBinding(_energySurfaceShader.program(), glGetUniformBlockIndex(_energySurfaceShader.program(), "FrameUniformBlock"), 0);

  check_gl_error();
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
  check_gl_error();
}

void GLWidget::updateTransformUniforms()
{
  double4x4 projectionMatrix = double4x4();
  double4x4 viewMatrix = double4x4();
  double bloomLevel = 1.0;;
  double bloomPulse = 1.0;

  if (std::shared_ptr<RKCamera> camera = _camera.lock())
  {
    projectionMatrix = camera->projectionMatrix();
    viewMatrix = camera->modelViewMatrix();
    bloomLevel = camera->bloomLevel();
    bloomPulse = camera->bloomPulse();
  }

  glBindBuffer(GL_UNIFORM_BUFFER, _frameUniformBuffer);
  check_gl_error();
  RKTransformationUniforms transformationUniforms = RKTransformationUniforms(projectionMatrix, viewMatrix, bloomLevel, bloomPulse);
  glBufferData (GL_UNIFORM_BUFFER, sizeof(RKTransformationUniforms), &transformationUniforms, GL_DYNAMIC_DRAW);
  check_gl_error();
  glBindBuffer(GL_UNIFORM_BUFFER,0);
}



void GLWidget::initializeStructureUniforms()
{
  glGenBuffers(1, &_structureUniformBuffer);
  check_gl_error();

  glBindBufferBase(GL_UNIFORM_BUFFER, 1, _structureUniformBuffer);
  check_gl_error();
  glUniformBlockBinding(_atomShader.program(), glGetUniformBlockIndex(_atomShader.program(), "StructureUniformBlock"), 1);
  glUniformBlockBinding(_atomOrthographicImposterShader.program(), glGetUniformBlockIndex(_atomOrthographicImposterShader.program(), "StructureUniformBlock"), 1);
  glUniformBlockBinding(_atomPerspectiveImposterShader.program(), glGetUniformBlockIndex(_atomPerspectiveImposterShader.program(), "StructureUniformBlock"), 1);
  glUniformBlockBinding(_internalBondShader.program(), glGetUniformBlockIndex(_internalBondShader.program(), "StructureUniformBlock"), 1);
  glUniformBlockBinding(_externalBondShader.program(), glGetUniformBlockIndex(_externalBondShader.program(), "StructureUniformBlock"), 1);
  glUniformBlockBinding(_externalBondShader.stencilProgram(), glGetUniformBlockIndex(_externalBondShader.stencilProgram(), "StructureUniformBlock"), 1);
  glUniformBlockBinding(_externalBondShader.boxProgram(), glGetUniformBlockIndex(_externalBondShader.boxProgram(), "StructureUniformBlock"), 1);
  glUniformBlockBinding(_unitCellSphereShader.program(), glGetUniformBlockIndex(_unitCellSphereShader.program(), "StructureUniformBlock"), 1);
  glUniformBlockBinding(_unitCellCylinderShader.program(), glGetUniformBlockIndex(_unitCellCylinderShader.program(), "StructureUniformBlock"), 1);
  glUniformBlockBinding(_pickingShader.atomPickingProgram(), glGetUniformBlockIndex(_pickingShader.atomPickingProgram(), "StructureUniformBlock"), 1);
  glUniformBlockBinding(_atomSelectionWorleyNoise3DShader.program(), glGetUniformBlockIndex(_atomSelectionWorleyNoise3DShader.program(), "StructureUniformBlock"), 1);
  glUniformBlockBinding(_atomSelectionWorleyNoise3DOrthographicImposterShader.program(), glGetUniformBlockIndex(_atomSelectionWorleyNoise3DOrthographicImposterShader.program(), "StructureUniformBlock"), 1);
  glUniformBlockBinding(_atomSelectionWorleyNoise3DPerspectiveImposterShader.program(), glGetUniformBlockIndex(_atomSelectionWorleyNoise3DPerspectiveImposterShader.program(), "StructureUniformBlock"), 1);
  glUniformBlockBinding(_atomSelectionStripesShader.program(), glGetUniformBlockIndex(_atomSelectionStripesShader.program(), "StructureUniformBlock"), 1);
  glUniformBlockBinding(_atomSelectionStripesOrthographicImposterShader.program(), glGetUniformBlockIndex(_atomSelectionStripesOrthographicImposterShader.program(), "StructureUniformBlock"), 1);
  glUniformBlockBinding(_atomSelectionStripesPerspectiveImposterShader.program(), glGetUniformBlockIndex(_atomSelectionStripesPerspectiveImposterShader.program(), "StructureUniformBlock"), 1);
  glUniformBlockBinding(_atomSelectionGlowShader.program(), glGetUniformBlockIndex(_atomSelectionGlowShader.program(), "StructureUniformBlock"), 1);
  glUniformBlockBinding(_energySurfaceShader.program(), glGetUniformBlockIndex(_energySurfaceShader.program(), "StructureUniformBlock"), 1);

  check_gl_error();
  std::vector<RKStructureUniforms> structureUniforms{RKStructureUniforms()};
  glBufferData(GL_UNIFORM_BUFFER, sizeof(RKStructureUniforms) * structureUniforms.size(), structureUniforms.data(), GL_DYNAMIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
  check_gl_error();
}

void GLWidget::updateStructureUniforms()
{
  glBindBuffer(GL_UNIFORM_BUFFER, _structureUniformBuffer);

  std::vector<RKStructureUniforms> structureUniforms = std::vector<RKStructureUniforms>();

  for(uint i=0;i<_renderStructures.size();i++)
  {
    for(uint j=0;j<_renderStructures[i].size();j++)
    {
      RKStructureUniforms structureUniform = RKStructureUniforms(i, j, _renderStructures[i][j]);
      structureUniforms.push_back(structureUniform);
    }
  }

  if (structureUniforms.size() == 0)
  {
    structureUniforms.push_back(RKStructureUniforms());
  }


  glBufferData(GL_UNIFORM_BUFFER, sizeof(RKStructureUniforms) * structureUniforms.size(), structureUniforms.data(), GL_DYNAMIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GLWidget::initializeIsosurfaceUniforms()
{
  glGenBuffers(1, &_isosurfaceUniformBuffer);
  check_gl_error();

  glBindBufferBase(GL_UNIFORM_BUFFER, 2, _isosurfaceUniformBuffer);
  glUniformBlockBinding(_energySurfaceShader.program(), glGetUniformBlockIndex(_energySurfaceShader.program(), "IsosurfaceUniformBlock"), 2);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
  check_gl_error();
}

void GLWidget::updateIsosurfaceUniforms()
{
  std::vector<RKIsosurfaceUniforms> isosurfaceUniforms;

  if(_dataSource)
  {
    for(uint i=0;i<_renderStructures.size();i++)
    {
      for(uint j=0;j<_renderStructures[i].size();j++)
      {
        RKIsosurfaceUniforms isosurfaceUniform = RKIsosurfaceUniforms(_renderStructures[i][j]);
        isosurfaceUniforms.push_back(isosurfaceUniform);
      }
    }
  }
  else
  {
    isosurfaceUniforms = {RKIsosurfaceUniforms()};
  }


  glBindBuffer(GL_UNIFORM_BUFFER, _isosurfaceUniformBuffer);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(RKIsosurfaceUniforms) * isosurfaceUniforms.size(), isosurfaceUniforms.data(), GL_DYNAMIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}


void GLWidget::initializeLightUniforms()
{
  glGenBuffers(1, &_lightsUniformBuffer);
  check_gl_error();

  glBindBufferBase(GL_UNIFORM_BUFFER, 3, _lightsUniformBuffer);
  glUniformBlockBinding(_atomShader.program(), glGetUniformBlockIndex(_atomShader.program(), "LightsUniformBlock"), 3);
  glUniformBlockBinding(_atomOrthographicImposterShader.program(), glGetUniformBlockIndex(_atomOrthographicImposterShader.program(), "LightsUniformBlock"), 3);
  glUniformBlockBinding(_atomPerspectiveImposterShader.program(), glGetUniformBlockIndex(_atomPerspectiveImposterShader.program(), "LightsUniformBlock"), 3);
  glUniformBlockBinding(_internalBondShader.program(), glGetUniformBlockIndex(_internalBondShader.program(), "LightsUniformBlock"), 3);
  glUniformBlockBinding(_externalBondShader.program(), glGetUniformBlockIndex(_externalBondShader.program(), "LightsUniformBlock"), 3);
  glUniformBlockBinding(_externalBondShader.stencilProgram(), glGetUniformBlockIndex(_externalBondShader.stencilProgram(), "LightsUniformBlock"), 3);
  glUniformBlockBinding(_externalBondShader.boxProgram(), glGetUniformBlockIndex(_externalBondShader.boxProgram(), "LightsUniformBlock"), 3);
  glUniformBlockBinding(_unitCellSphereShader.program(), glGetUniformBlockIndex(_unitCellSphereShader.program(), "LightsUniformBlock"), 3);
  glUniformBlockBinding(_unitCellCylinderShader.program(), glGetUniformBlockIndex(_unitCellCylinderShader.program(), "LightsUniformBlock"), 3);
  glUniformBlockBinding(_atomSelectionWorleyNoise3DShader.program(), glGetUniformBlockIndex(_atomSelectionWorleyNoise3DShader.program(), "LightsUniformBlock"), 3);
  glUniformBlockBinding(_atomSelectionWorleyNoise3DOrthographicImposterShader.program(), glGetUniformBlockIndex(_atomSelectionWorleyNoise3DOrthographicImposterShader.program(), "LightsUniformBlock"), 3);
  glUniformBlockBinding(_atomSelectionWorleyNoise3DPerspectiveImposterShader.program(), glGetUniformBlockIndex(_atomSelectionWorleyNoise3DPerspectiveImposterShader.program(), "LightsUniformBlock"), 3);
  glUniformBlockBinding(_atomSelectionStripesShader.program(), glGetUniformBlockIndex(_atomSelectionStripesShader.program(), "LightsUniformBlock"), 3);
  glUniformBlockBinding(_atomSelectionStripesOrthographicImposterShader.program(), glGetUniformBlockIndex(_atomSelectionStripesOrthographicImposterShader.program(), "LightsUniformBlock"), 3);
  glUniformBlockBinding(_atomSelectionStripesPerspectiveImposterShader.program(), glGetUniformBlockIndex(_atomSelectionStripesPerspectiveImposterShader.program(), "LightsUniformBlock"), 3);
  glUniformBlockBinding(_atomSelectionGlowShader.program(), glGetUniformBlockIndex(_atomSelectionGlowShader.program(), "LightsUniformBlock"), 3);
  glUniformBlockBinding(_energySurfaceShader.program(), glGetUniformBlockIndex(_energySurfaceShader.program(), "LightsUniformBlock"), 3);
  std::vector<RKLightsUniforms> lightUniforms{RKLightsUniforms()};
  glBufferData(GL_UNIFORM_BUFFER, sizeof(RKLightsUniforms) * lightUniforms.size(), lightUniforms.data(), GL_DYNAMIC_DRAW);  // FIX
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
  check_gl_error();
}

void GLWidget::updateLightUniforms()
{
  RKLightsUniforms lightUniforms = RKLightsUniforms(_dataSource);
  glBindBuffer(GL_UNIFORM_BUFFER, _lightsUniformBuffer);
  glBufferData(GL_UNIFORM_BUFFER, lightUniforms.lights.size() * sizeof(RKLightUniform), lightUniforms.lights.data(), GL_DYNAMIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}


void GLWidget::reloadData()
{
  makeCurrent();

  if(_dataSource)
  {
    _atomShader.setRenderStructures(_dataSource->renderStructures());
    _atomOrthographicImposterShader.setRenderStructures(_dataSource->renderStructures());
    _atomPerspectiveImposterShader.setRenderStructures(_dataSource->renderStructures());
    _internalBondShader.setRenderStructures(_dataSource->renderStructures());
    _externalBondShader.setRenderStructures(_dataSource->renderStructures());
    _unitCellSphereShader.setRenderStructures(_dataSource->renderStructures());
    _unitCellCylinderShader.setRenderStructures(_dataSource->renderStructures());
    _atomSelectionWorleyNoise3DShader.setRenderStructures(_dataSource->renderStructures());
    _atomSelectionWorleyNoise3DOrthographicImposterShader.setRenderStructures(_dataSource->renderStructures());
    _atomSelectionWorleyNoise3DPerspectiveImposterShader.setRenderStructures(_dataSource->renderStructures());
    _atomSelectionStripesShader.setRenderStructures(_dataSource->renderStructures());
    _atomSelectionStripesOrthographicImposterShader.setRenderStructures(_dataSource->renderStructures());
    _atomSelectionStripesPerspectiveImposterShader.setRenderStructures(_dataSource->renderStructures());
    _atomSelectionGlowShader.setRenderStructures(_dataSource->renderStructures());

    _pickingShader.setRenderStructures(_dataSource->renderStructures());
    _atomAmbientOcclusionShader.setRenderStructures(_dataSource->renderStructures());
    _energySurfaceShader.setRenderStructures(_dataSource->renderStructures());
  }
check_gl_error();

  _atomShader.reloadData();
  _atomOrthographicImposterShader.reloadData();
  _atomPerspectiveImposterShader.reloadData();
  _pickingShader.reloadData();
  _atomAmbientOcclusionShader.reloadData(_dataSource);
  _energySurfaceShader.reloadData();
  check_gl_error();

  _internalBondShader.reloadData();
  check_gl_error();
  _externalBondShader.reloadData();
  check_gl_error();
  _unitCellSphereShader.reloadData();
  check_gl_error();
  _unitCellCylinderShader.reloadData();
  check_gl_error();

  _atomSelectionWorleyNoise3DShader.reloadData();
  _atomSelectionWorleyNoise3DOrthographicImposterShader.reloadData();
  _atomSelectionWorleyNoise3DPerspectiveImposterShader.reloadData();
  _atomSelectionStripesShader.reloadData();
  _atomSelectionStripesOrthographicImposterShader.reloadData();
  _atomSelectionStripesPerspectiveImposterShader.reloadData();
  _atomSelectionGlowShader.reloadData();

  update();

  check_gl_error();
}

void GLWidget::reloadData(RKRenderQuality ambientOcclusionQuality)
{

}

void GLWidget::reloadAmbientOcclusionData()
{
  makeCurrent();
  _atomAmbientOcclusionShader.reloadData(_dataSource);
}

void GLWidget::reloadRenderData()
{
  makeCurrent();

  check_gl_error();
  _atomShader.reloadData();
  _atomOrthographicImposterShader.reloadData();
  _atomPerspectiveImposterShader.reloadData();
  _pickingShader.reloadData();
  _atomAmbientOcclusionShader.reloadData(_dataSource);
  _energySurfaceShader.reloadData();

  check_gl_error();
  _internalBondShader.reloadData();
  _externalBondShader.reloadData();
  _unitCellSphereShader.reloadData();
  _unitCellCylinderShader.reloadData();
  check_gl_error();


  _atomSelectionWorleyNoise3DShader.reloadData();
  _atomSelectionWorleyNoise3DOrthographicImposterShader.reloadData();
  _atomSelectionWorleyNoise3DPerspectiveImposterShader.reloadData();
  _atomSelectionStripesShader.reloadData();
  _atomSelectionStripesOrthographicImposterShader.reloadData();
  _atomSelectionStripesPerspectiveImposterShader.reloadData();
  _atomSelectionGlowShader.reloadData();

  update();
}

void GLWidget::reloadRenderMeasurePointsData()
{
  makeCurrent();
}

void GLWidget::reloadBoundingBoxData()
{
  makeCurrent();
}

void GLWidget::reloadSelectionData()
{
  makeCurrent();

  _atomSelectionWorleyNoise3DShader.reloadData();
  _atomSelectionWorleyNoise3DOrthographicImposterShader.reloadData();
  _atomSelectionWorleyNoise3DPerspectiveImposterShader.reloadData();
  _atomSelectionStripesShader.reloadData();
  _atomSelectionStripesOrthographicImposterShader.reloadData();
  _atomSelectionStripesPerspectiveImposterShader.reloadData();
  _atomSelectionGlowShader.reloadData();
}

void GLWidget::reloadBackgroundImage()
{
  makeCurrent();
  if(_dataSource)
  {
    _backgroundShader.reload(_dataSource);
  }
}



void GLWidget::updateVertexArrays()
{

}

void GLWidget::computeHeliumVoidFraction(std::vector<std::shared_ptr<RKRenderStructure>> structures)
{
  if(_isOpenCLInitialized)
  {
    makeCurrent();
    _energySurfaceShader.computeHeliumVoidFraction(structures);
  }
  else
  {
    _logReporter->logMessage(LogReporting::ErrorLevel::verbose, "Unable to compute Helium void fraction (OpenCL not initialized)");
  }
}

void GLWidget::computeNitrogenSurfaceArea(std::vector<std::shared_ptr<RKRenderStructure>> structures)
{
  if(_isOpenCLInitialized)
  {
    makeCurrent();
    _energySurfaceShader.computeNitrogenSurfaceArea(structures);
  }
  else
  {
    _logReporter->logMessage(LogReporting::ErrorLevel::verbose, "Unable to compute Nitrogen surface area (OpenCL not initialized)");
  }
}

void GLWidget::loadShader(void)
{
  GLuint vertexShader;
  GLuint fragmentShader;

  vertexShader=compileShaderOfType(GL_VERTEX_SHADER,GLWidget::_vertexShaderSource.c_str());
  fragmentShader=compileShaderOfType(GL_FRAGMENT_SHADER,GLWidget::_fragmentShaderSource.c_str());

  if (0 != vertexShader && 0 != fragmentShader)
  {
    _program = glCreateProgram();
    check_gl_error();

    glAttachShader(_program, vertexShader);
    check_gl_error();
    glAttachShader(_program, fragmentShader);
    check_gl_error();

    glBindFragDataLocation(_program, 0, "vFragColor");

    linkProgram(_program);

    _downSampleInputTextureUniformLocation = glGetUniformLocation(_program, "originalTexture");
    _blurredInputTextureUniformLocation = glGetUniformLocation(_program, "blurredTexture");
    _downSamplePositionAttributeLocation = glGetAttribLocation(_program, "position");

    if (_downSampleInputTextureUniformLocation < 0) qDebug() << "Shader did not contain the 'originalTexture' uniform.";
    if (_downSamplePositionAttributeLocation < 0) qDebug() << "Shader did not contain the 'position' attribute.";

    glDeleteShader(vertexShader);
    check_gl_error();
    glDeleteShader(fragmentShader);
    check_gl_error();
  }
}

const std::string GLWidget::_vertexShaderSource =
OpenGLVersionStringLiteral +
std::string(R"foo(
in vec4 position;

out vec2 texcoord;

void main()
{
  gl_Position = position;
  texcoord = position.xy * vec2(0.5) + vec2(0.5);
}
)foo");

const std::string GLWidget:: _fragmentShaderSource =
OpenGLVersionStringLiteral +
OpenGLFrameUniformBlockStringLiteral +
std::string(R"foo(
in vec2 texcoord;
out vec4 vFragColor;

uniform sampler2DMS originalTexture;
uniform sampler2D blurredTexture;

void main()
{
  vec4 sampleValue;

  // Calculate un-normalized texture coordinates
  vec2 tmp = floor(textureSize(originalTexture) * texcoord);

  // Find both the weighted and unweighted colors
  vec4 vColor = vec4(0.0, 0.0, 0.0, 0.0);

  for (int i = 0; i < frameUniforms.numberOfMultiSamplePoints ; i++)
  {
    sampleValue = texelFetch(originalTexture, ivec2(tmp), i);
    vColor += sampleValue;
  }

  vFragColor = vColor/float(frameUniforms.numberOfMultiSamplePoints) + frameUniforms.bloomPulse * frameUniforms.bloomLevel * texture(blurredTexture,texcoord);
}
)foo");
