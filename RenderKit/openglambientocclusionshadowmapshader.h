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
#define GL_GLEXT_PROTOTYPES
#include <QtOpenGL>
#include <QGLFunctions>
#include "openglshader.h"
#include "rkrenderkitprotocols.h"
#include "openglatomorthographicimpostershader.h"

class OpenGLAtomOrthographicImposterShader;

class OpenGLAmbientOcclusionShadowMapShader: public OpenGLShader
{
public:
  OpenGLAmbientOcclusionShadowMapShader(OpenGLAtomShader &atomShader,OpenGLAtomOrthographicImposterShader &atomOrthographicImposterShader);
  ~OpenGLAmbientOcclusionShadowMapShader() override final;
  void setRenderStructures(std::vector<std::vector<std::shared_ptr<RKRenderStructure>>> structures);
  void reloadData(std::shared_ptr<RKRenderDataSource> dataSource);
  void deleteBuffers();
  void generateBuffers();
  void initializeVertexArrayObject();
  void loadShader(void) override final;

  void adjustAmbientOcclusionTextureSize();
  void  updateAmbientOcclusionTextures(std::shared_ptr<RKRenderDataSource> dataSource, RKRenderQuality quality);
  GLuint ambientOcclusionProgram() {return _ambientOcclusionProgram;}
  GLuint shadowMapProgram() {return _shadowMapProgram;}

  std::vector<std::vector<GLuint>> &generatedAmbientOcclusionTextures() {return _generatedAmbientOcclusionTexture;}
  void invalidateCachedAmbientOcclusionTexture(std::vector<std::shared_ptr<RKRenderStructure>> structure);

private:
  GLuint _ambientOcclusionProgram;
  GLuint _shadowMapProgram;

  OpenGLAtomShader& _atomShader;
  OpenGLAtomOrthographicImposterShader& _atomOrthographicImposterShader;

  std::vector<std::vector<std::shared_ptr<RKRenderStructure>>> _renderStructures;

  GLint _atomAmbientOcclusionMapUniformLocation;
  GLint _atomAmbientOcclusionMapWeightUniformLocation;

  GLint _atomAmbientOcclusionMapPositionAttributeLocation;
  GLint _atomAmbientOcclusionMapInstancePositionAttributeLocation;
  GLint _atomAmbientOcclusionMapScaleAttributeLocation;

  GLint _atomShadowMapVertexPositionAttributeLocation;
  GLint _atomShadowMapInstancePositionAttributeLocation;
  GLint _atomShadowMapScaleAttributeLocation;

  std::vector<std::vector<GLuint>> _atomShadowMapVertexArrayObject;
  std::vector<std::vector<GLuint>> _atomAmbientOcclusionMapVertexArrayObject;
  std::vector<std::vector<GLuint>> _ambientOcclusionFrameBufferObject;
  std::vector<std::vector<GLuint>> _generatedAmbientOcclusionTexture;

  QCache<RKRenderStructure*, std::vector<uint16_t>> _cache;

  static const std::string _vertexAmbientOcclusionShaderSource;
  static const std::string _fragmentAmbientOcclusionShaderSource;

  static const std::string _vertexShadowMapShaderSource;
  static const std::string _fragmentShadowMapShaderSource;

};

