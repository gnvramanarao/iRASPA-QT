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

#include <QObject>
#include <QtGlobal>
#define GL_GLEXT_PROTOTYPES
#include <QtOpenGL>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QPoint>
#include <QCache>
#include <QOpenGLTexture>
#include <foundationkit.h>
#include <optional>
#include "rkrenderkitprotocols.h"
#include "rkrenderuniforms.h"
#include "openglatomshader.h"
#include "openglatomorthographicimpostershader.h"
#include "openglatomperspectiveimpostershader.h"
#include "openglinternalbondshader.h"
#include "openglexternalbondshader.h"
#include "openglunitcellsphereshader.h"
#include "openglunitcellcylindershader.h"
#include "openglatompickingshader.h"
#include "openglbackgroundshader.h"
#include "openglatomselectionworleynoise3dshader.h"
#include "openglatomselectionworleynoise3dorthographicimpostershader.h"
#include "openglatomselectionworleynoise3dperspectiveimpostershader.h"
#include "openglatomselectionstripesshader.h"
#include "openglatomselectionstripesorthographicimpostershader.h"
#include "openglatomselectionstripesperspectiveimpostershader.h"
#include "openglatomselectionglowshader.h"
#include "openglblurshader.h"
#include "openglambientocclusionshadowmapshader.h"
#include "openglenergysurface.h"
#include "rkcamera.h"
#include "trackball.h"
#include "skopenclenergygridunitcell.h"
#include "openglinternalbondselectionstripesshader.h"
#include "openglexternalbondselectionstripesshader.h"
#include "openglinternalbondselectionworleynoise3dshader.h"
#include "openglexternalbondselectionworleynoise3dshader.h"
#include "openglinternalbondselectionglowshader.h"
#include "openglexternalbondselectionglowshader.h"
#include "openglinternalbondpickingshader.h"
#include "openglexternalbondpickingshader.h"
#include "openglboundingboxcylindershader.h"
#include "openglboundingboxsphereshader.h"
#include "openglcrystalellipseobjectshader.h"
#include "openglcrystalcylinderobjectshader.h"
#include "openglcrystalpolygonalprismobjectshader.h"
#include "openglellipseobjectshader.h"
#include "openglcylinderobjectshader.h"
#include "openglpolygonalprismobjectshader.h"

#ifdef Q_OS_MACOS
  #include <OpenCL/opencl.h>
  #include <OpenGL/OpenGL.h>
#else
  #include <CL/opencl.h>
#endif

enum class Tracking
{
  none, panning, addToSelection, newSelection, draggedAddToSelection, draggedNewSelection, backgroundClick, measurement, translateSelection, other
};

class GLWidget : public QOpenGLWidget, public RKRenderViewController, public OpenGLShader, public LogReportingConsumer
{
  Q_OBJECT

public:
  GLWidget(QWidget* parent = nullptr );
  ~GLWidget();
  void redraw() override final;
  void redrawWithLowQuality() override final;
  void redrawWithMediumQuality() override final;
  void redrawWithHighQuality() override final;
  void redrawWithPictureQuality() override final;

  void setRenderStructures(std::vector<std::vector<std::shared_ptr<RKRenderStructure>>> structures) override final;
  void setRenderDataSource(std::shared_ptr<RKRenderDataSource> source) override final;
  void reloadData() override final;
  void reloadData(RKRenderQuality ambientOcclusionQuality) override final;
  void reloadAmbientOcclusionData() override final;
  void reloadRenderData() override final;
  void reloadSelectionData() override final;
  void reloadRenderMeasurePointsData() override final;
  void reloadBoundingBoxData() override final;
  void reloadBackgroundImage() override final;

  void setLogReportingWidget(LogReporting *logReporting)  override final;

  void invalidateCachedAmbientOcclusionTexture(std::vector<std::shared_ptr<RKRenderStructure>> structures) override;
  void invalidateIsosurface(std::vector<std::shared_ptr<RKRenderStructure>> structures) override;

  void updateTransformUniforms() override final;
  void updateStructureUniforms() override final;
  void updateIsosurfaceUniforms() override final;
  void updateLightUniforms() override final;

  void updateVertexArrays() override final;

  void loadShader(void) override final;

  QImage renderSceneToImage(int width, int height) override final;
  std::array<int,4> pickTexture(int x, int y, int width, int height) override final;

  void computeHeliumVoidFraction(std::vector<std::shared_ptr<RKRenderStructure>> structures) override final;
  void computeNitrogenSurfaceArea(std::vector<std::shared_ptr<RKRenderStructure>> structures) override final;
private:
  bool _isOpenGLInitialized;
  bool _isOpenCLInitialized;
  GLuint _program;
  RKRenderQuality _quality = RKRenderQuality::high;
  std::shared_ptr<RKRenderDataSource> _dataSource;
  LogReporting* _logReporter = nullptr;

  std::vector<std::vector<std::shared_ptr<RKRenderStructure>>> _renderStructures = std::vector<std::vector<std::shared_ptr<RKRenderStructure>>>{};
  GLuint _sceneFrameBuffer;
  GLuint _sceneDepthTexture;
  GLuint _sceneTexture;
  GLuint _glowSelectionTexture;
  GLuint _downSamplerVertexArray;
  GLuint _quad_vertexbuffer;
  GLuint _quad_indexbuffer;
  int _devicePixelRatio;
  int _maxTextureSize;
  GLint _maxNumberOfClipPlanes;
  GLint _maxSampleCount;
  GLint _maxSampleColorCount;
  GLint _maxSampleDepthCount;
  GLint _maxMultiSampling;
  GLint _multiSampling;
  GLint _width;
  GLint _height;

  GLuint _frameUniformBuffer;
  GLuint _structureUniformBuffer;
  GLuint _isosurfaceUniformBuffer;
  GLuint _lightsUniformBuffer;
  GLsync _fence;

  std::weak_ptr<RKCamera> _camera;
  OpenGLBackgroundShader _backgroundShader;
  OpenGLAtomShader _atomShader;
  OpenGLAtomOrthographicImposterShader _atomOrthographicImposterShader;
  OpenGLAtomPerspectiveImposterShader _atomPerspectiveImposterShader;
  OpenGLInternalBondShader _internalBondShader;
  OpenGLExternalBondShader _externalBondShader;
  OpenGLUnitCellSphereShader _unitCellSphereShader;
  OpenGLUnitCellCylinderShader _unitCellCylinderShader;
  OpenGLAtomPickingShader _atomPickingShader;
  OpenGLAtomSelectionWorleyNoise3DShader _atomSelectionWorleyNoise3DShader;
  OpenGLAtomSelectionWorleyNoise3DOrthographicImposterShader _atomSelectionWorleyNoise3DOrthographicImposterShader;
  OpenGLAtomSelectionWorleyNoise3DPerspectiveImposterShader _atomSelectionWorleyNoise3DPerspectiveImposterShader;
  OpenGLAtomSelectionStripesShader _atomSelectionStripesShader;
  OpenGLAtomSelectionStripesOrthographicImposterShader _atomSelectionStripesOrthographicImposterShader;
  OpenGLAtomSelectionStripesPerspectiveImposterShader _atomSelectionStripesPerspectiveImposterShader;
  OpenGLAtomSelectionGlowShader _atomSelectionGlowShader;
  OpenGLInternalBondSelectionStripesShader _internalBondSelectionStripesShader;
  OpenGLExternalBondSelectionStripesShader _externalBondSelectionStripesShader;
  OpenGLInternalBondSelectionWorleyNoise3DShader _internalBondSelectionWorleyNoise3DShader;
  OpenGLExternalBondSelectionWorleyNoise3DShader _externalBondSelectionWorleyNoise3DShader;
  OpenGLInternalBondSelectionGlowShader _internalBondSelectionGlowShader;
  OpenGLExternalBondSelectionGlowShader _externalBondSelectionGlowShader;
  OpenGLBlurShader _blurShader;
  OpenGLAmbientOcclusionShadowMapShader _atomAmbientOcclusionShader;
  OpenGLEnergySurface _energySurfaceShader;
  OpenGLInternalBondPickingShader _internalBondPickingShader;
  OpenGLExternalBondPickingShader _externalBondPickingShader;
  OpenGLBoundingBoxCylinderShader _boundingBoxCylinderShader;
  OpenGLBoundingBoxSphereShader _boundingBoxSphereShader;
  OpenGLCrystalEllipseObjectShader _crystalEllipseObjectShader;
  OpenGLCrystalCylinderObjectShader _crystalCylinderObjectShader;
  OpenGLCrystalPolygonalPrismObjectShader _crystalPolygonalPrismObjectShader;
  OpenGLEllipseObjectShader _ellipseObjectShader;
  OpenGLCylinderObjectShader _cylinderObjectShader;
  OpenGLPolygonalPrismObjectShader _polygonalPrismObjectShader;

  void drawSceneToFramebuffer(GLuint framebuffer);

  void adjustAmbientOcclusionTextureSize();

  void initializeTransformUniforms();
  void initializeStructureUniforms();
  void initializeIsosurfaceUniforms();
  void initializeLightUniforms();

  TrackBall _trackBall = TrackBall();

  std::optional<QPoint> _startPoint = std::nullopt;
  std::optional<QPoint> _panStartPoint = std::nullopt;

  GLint _downSampleInputTextureUniformLocation;
  GLint _blurredInputTextureUniformLocation;
  GLint _downSamplePositionAttributeLocation;
  GLint _numberOfMultiSamplePointsUniformLocation;

  static const std::string _vertexShaderSource;
  static const std::string _fragmentShaderSource;

  Tracking _tracking;
  QPoint _origin;

protected:
   void initializeGL() final override;
   void resizeGL( int w, int h ) final override;
   void paintGL() final override;
   void keyPressEvent( QKeyEvent* e ) final override;
   void mousePressEvent(QMouseEvent *event) final override;
   void mouseMoveEvent(QMouseEvent *event) final override;
   void mouseReleaseEvent(QMouseEvent *event) final override;
   void wheelEvent(QWheelEvent *event ) final override;
};
