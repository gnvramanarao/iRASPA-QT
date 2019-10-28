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

#include "glgeterror.h"
#include <QOpenGLFunctions>

void _check_gl_error(const char *file, int line)
{
  QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

  if(f)
  {
    GLenum err (f->glGetError());

    while(err!=GL_NO_ERROR)
    {
      std::string error;

      switch(err) {
         case GL_INVALID_OPERATION:      error="INVALID_OPERATION";      break;
         case GL_INVALID_ENUM:           error="INVALID_ENUM";           break;
         case GL_INVALID_VALUE:          error="INVALID_VALUE";          break;
         case GL_OUT_OF_MEMORY:          error="OUT_OF_MEMORY";          break;
         case GL_INVALID_FRAMEBUFFER_OPERATION:  error="INVALID_FRAMEBUFFER_OPERATION";  break;
      }

      qDebug() << "GL_" << error.c_str() << " - " << file <<":" << line;
      std::cout << "GL_" << error.c_str() << " - " << file << ":" << line << std::endl;
      err=f->glGetError();
    }
  }
}

