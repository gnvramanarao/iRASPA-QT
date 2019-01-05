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

#include "openglshader.h"
#include "glgeterror.h"
#include <QDebug>
#include <iostream>

OpenGLShader::OpenGLShader()
{

}

OpenGLShader::~OpenGLShader()
{
}


GLuint OpenGLShader::compileShaderOfType(GLenum shaderType,const GLchar * shaderSourceCode)
{
  GLint status;
  GLuint shader;
  GLint length;

  shader = glCreateShader(shaderType);
  check_gl_error();
  length=strlen(shaderSourceCode);
  glShaderSource(shader, 1, &shaderSourceCode, &length);
  check_gl_error();
  glCompileShader(shader);
  check_gl_error();
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  check_gl_error();
  if (status == GL_FALSE)
  {
    GLint maxLength = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
    std::vector<char> errorLog(maxLength);
    glGetShaderInfoLog(shader, maxLength, &maxLength, errorLog.data());
    std::string log;
    for (int i = 0; i < errorLog.size(); i++)
    {
      log += errorLog[i];
    }

    std::cout << "ERROR IN SHADER: " << log << std::endl;

    glDeleteShader(shader);
    check_gl_error();
  }
  return shader;
}

void OpenGLShader::linkProgram(GLuint program)
{
  GLint status;

  glLinkProgram(program);
  check_gl_error();

  glGetProgramiv(program, GL_LINK_STATUS, &status);
  check_gl_error();
  if (0 == status)
  {
    qDebug() <<"Failed to link shader program";
  }
}
