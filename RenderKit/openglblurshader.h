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
#include <array>
#define GL_GLEXT_PROTOTYPES
#include <QtOpenGL>
#include <QGLFunctions>
#include "rkrenderkitprotocols.h"
#include "rkrenderuniforms.h"
#include "openglshader.h"

class OpenGLBlurShader: public OpenGLShader
{
public:
  OpenGLBlurShader();
  void loadShader(void) override final;
  GLuint blurredTexture() {return _blurVerticalTexture;}
  GLuint downSampledTexture() {return _blurDownSamplerTexture;}
  void paintGL(GLuint texture, int width, int height);
  void resizeGL(int width, int height);
  void initializeFramebuffers();
  void initializeVertexArrayObject();
  GLuint blurHorizontalTexture() {return _blurHorizontalTexture;}
private:
  GLuint _blurDownSamplerProgram = 0;
  GLuint _blurHorizontalProgram = 0;
  GLuint _blurVerticalProgram = 0;

  GLuint _blurDownSamplerFrameBufferObject = 0;
  GLuint _blurHorizontalFrameBufferObject = 0;
  GLuint _blurVerticalFrameBufferObject = 0;

  GLuint _blurDownSamplerTexture = 0;
  GLuint _blurHorizontalTexture = 0;
  GLuint _blurVerticalTexture = 0;

  GLint _blurDownSamplerInputTextureUniformLocation = 0;
  GLint _blurDownSamplerVertexPositionAttributeLocation = 0;

  GLint _blurHorizontalInputTextureUniformLocation = 0;
  GLint _blurHorizontalVertexPositionAttributeLocation = 0;

  GLint _blurVerticalInputTextureUniformLocation = 0;
  GLint _blurVerticalVertexPositionAttributeLocation = 0;

  GLuint _blurDownSamplerVertexArray = 0;
  GLuint _blurDownSamplerVertexBuffer = 0;
  GLuint _blurDownSamplerIndexBuffer = 0;

  GLuint _blurHorizontalVertexArray = 0;
  GLuint _blurHorizontalVertexBuffer = 0;
  GLuint _blurHorizontalIndexBuffer = 0;

  GLuint _blurVerticalVertexArray = 0;
  GLuint _blurVerticalVertexBuffer = 0;
  GLuint _blurVerticalIndexBuffer = 0;

  static const std::string _downSamplerVertexShaderSource;
  static const std::string _downSamplerFragmentShaderSource;
  static const std::string _blurHorizontalVertexShaderSource;
  static const std::string _blurVerticalVertexShaderSource;
  static const std::string _fragmentShaderSource;
};
