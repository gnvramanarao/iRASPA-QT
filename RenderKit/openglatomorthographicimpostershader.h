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
#include "glgeterror.h"

class OpenGLAtomShader;
class OpenGLAmbientOcclusionShadowMapShader;
class OpenGLAtomPickingShader;

class OpenGLAtomOrthographicImposterShader: public OpenGLShader
{
public:
  OpenGLAtomOrthographicImposterShader(OpenGLAtomShader &atomShader);
  void loadShader(void) override final;
  void deleteBuffers();
  void generateBuffers();

  void paintGL(std::vector<std::vector<GLuint>>& atomTextures, GLuint structureUniformBuffer);

  void reloadData();
  void initializeVertexArrayObject();
  void setRenderStructures(std::vector<std::vector<std::shared_ptr<RKRenderStructure>>> structures);
  GLuint program() {return _program;}
private:
  GLuint _program;
  std::vector<std::vector<std::shared_ptr<RKRenderStructure>>> _renderStructures;
  OpenGLAtomShader& _atomShader;

  std::vector<std::vector<size_t>> _numberOfIndices;

  std::vector<std::vector<GLuint>> _vertexBuffer;
  std::vector<std::vector<GLuint>> _indexBuffer;

  std::vector<std::vector<GLuint>> _vertexArrayObject;

  GLint _ambientOcclusionTextureUniformLocation;
  GLint _vertexPositionAttributeLocation;
  GLint _instancePositionAttributeLocation;
  GLint _scaleAttributeLocation;
  GLint _ambientColorAttributeLocation;
  GLint _diffuseColorAttributeLocation;
  GLint _specularColorAttributeLocation;

  static const std::string _vertexShaderSource;
  static const std::string _fragmentShaderSource;

  friend OpenGLAmbientOcclusionShadowMapShader;
  friend OpenGLAtomPickingShader;
};
