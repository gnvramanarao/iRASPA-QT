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

#include "openglbackgroundshader.h"
#include <iostream>
#include "glgeterror.h"
#include "rkrenderuniforms.h"
#include "quadgeometry.h"
#include "glgeterror.h"

OpenGLBackgroundShader::OpenGLBackgroundShader()
{

}


void OpenGLBackgroundShader::paintGL()
{
  glDepthMask(false);

  glUseProgram(_program);
  glBindVertexArray(_vertexArray);
  check_gl_error();

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _backgroundTexture);
  glUniform1i(_textureUniformLocation, 0);
  check_gl_error();

  glDrawElements(GL_TRIANGLE_STRIP,4,GL_UNSIGNED_SHORT, nullptr);
  check_gl_error();

  glBindVertexArray(0);
  glUseProgram(0);

  glDepthMask(true);
}

void OpenGLBackgroundShader::reload(std::shared_ptr<RKRenderDataSource> source)
{
  std::cout << "setRenderDataSource: " << source << std::endl;
  QImage image;
  if(source)
  {
    image = QGLWidget::convertToGLFormat(source->renderBackgroundCachedImage());
  }
  else
  {
    QImage whiteImage = QImage(QSize(64,64),QImage::Format_ARGB32);
    whiteImage.fill(QColor(255,255,255,255));
    image = QGLWidget::convertToGLFormat(whiteImage);
  }
  glBindTexture(GL_TEXTURE_2D, _backgroundTexture);
  check_gl_error();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  check_gl_error();
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
  check_gl_error();
  glBindTexture(GL_TEXTURE_2D, 0);
}



void OpenGLBackgroundShader::initializeVertexArrayObject()
{
  glGenVertexArrays(1, &_vertexArray);
  glGenBuffers(1, &_vertexBuffer);
  glGenBuffers(1, &_indexBuffer);

  glGenTextures(1, &_backgroundTexture);
check_gl_error();
  QuadGeometry quad = QuadGeometry();

  glBindVertexArray(_vertexArray);
   check_gl_error();
  glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
  if(quad.vertices().size()>0)
  {
    glBufferData(GL_ARRAY_BUFFER, quad.vertices().size()*sizeof(RKVertex), quad.vertices().data(), GL_STATIC_DRAW);
    check_gl_error();
  }

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
  if(quad.indices().size()>0)
  {
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, quad.indices().size() * sizeof(GLushort), quad.indices().data(), GL_STATIC_DRAW);
    check_gl_error();
  }

  glVertexAttribPointer(_vertexPositionAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKVertex), reinterpret_cast<GLvoid*>(offsetof(RKVertex, position)));
  glEnableVertexAttribArray(_vertexPositionAttributeLocation);
  check_gl_error();
  glBindVertexArray(0);

  QImage whiteImage = QImage(QSize(64,64),QImage::Format_ARGB32);
  whiteImage.fill(QColor(255,255,255,255));
  QImage image = QGLWidget::convertToGLFormat(whiteImage);

  glBindTexture(GL_TEXTURE_2D, _backgroundTexture);
  check_gl_error();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  check_gl_error();
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
  check_gl_error();
  glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGLBackgroundShader::loadShader(void)
{
  GLuint vertexShader;
  GLuint fragmentShader;

  vertexShader=compileShaderOfType(GL_VERTEX_SHADER,OpenGLBackgroundShader::_vertexShaderSource.c_str());
  fragmentShader=compileShaderOfType(GL_FRAGMENT_SHADER,OpenGLBackgroundShader::_fragmentShaderSource.c_str());

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


    _textureUniformLocation = glGetUniformLocation(_program, "backgroundTexture");
    _vertexPositionAttributeLocation = glGetAttribLocation(_program, "vertexPosition");

    glDeleteShader(vertexShader);
    check_gl_error();
    glDeleteShader(fragmentShader);
    check_gl_error();
  }
}

const std::string OpenGLBackgroundShader::_vertexShaderSource  =
OpenGLVersionStringLiteral +
std::string(R"foo(
in vec4 vertexPosition;
out vec2 texcoord;

void main()
{
  gl_Position = vertexPosition;
  texcoord = vertexPosition.xy * vec2(0.5) + vec2(0.5);
}
)foo");

const std::string OpenGLBackgroundShader::_fragmentShaderSource  =
OpenGLVersionStringLiteral +
std::string(R"foo(
uniform sampler2D backgroundTexture;

in vec2 texcoord;
out vec4 vFragColor;

void main()
{
  vFragColor =  texture(backgroundTexture,texcoord);
}
)foo");
