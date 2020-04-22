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

#include "openglatompickingshader.h"
#include "openglatomshader.h"
#include "openglatomorthographicimpostershader.h"

OpenGLAtomPickingShader::OpenGLAtomPickingShader(OpenGLAtomShader &atomShader, OpenGLAtomOrthographicImposterShader &atomOrthographicImposterShader): _atomShader(atomShader), _atomOrthographicImposterShader(atomOrthographicImposterShader)
{
}

void OpenGLAtomPickingShader::generateFrameBuffers()
{
  GLenum status;

  glGenFramebuffers(1, &_frameBufferObject);
  glGenTextures(1, &_texture);
  glGenTextures(1, &_depthTexture);
  check_gl_error();

  glBindFramebuffer(GL_FRAMEBUFFER, _frameBufferObject);
  check_gl_error();
  glBindTexture(GL_TEXTURE_2D, _texture);
  check_gl_error();
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32UI, 512, 512, 0, GL_RGBA_INTEGER, GL_UNSIGNED_INT, nullptr);
  check_gl_error();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  check_gl_error();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  check_gl_error();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  check_gl_error();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  check_gl_error();
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture, 0);
  glBindTexture(GL_TEXTURE_2D, 0);
  check_gl_error();

  glBindTexture(GL_TEXTURE_2D, _depthTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 512, 512, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthTexture, 0);
  check_gl_error();

  // check framebuffer completeness
  status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE)
  {
    qWarning("initializePickingFrameBuffer fatal error: framebuffer incomplete");
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLAtomPickingShader::resizeGL(int w, int h)
{
  glBindTexture(GL_TEXTURE_2D, _texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32UI, w, h, 0, GL_RGBA_INTEGER, GL_UNSIGNED_INT, nullptr);
  check_gl_error();

  glBindTexture(GL_TEXTURE_2D, _depthTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
  check_gl_error();

  glBindTexture(GL_TEXTURE_2D, 0);
  check_gl_error();
}

void OpenGLAtomPickingShader::setRenderStructures(std::vector<std::vector<std::shared_ptr<RKRenderStructure>>> structures)
{
  deleteBuffers();
  _renderStructures = structures;
  generateBuffers();
}


void OpenGLAtomPickingShader::deleteBuffers()
{
  for(size_t i=0;i<_renderStructures.size();i++)
  {
    glDeleteVertexArrays(_renderStructures[i].size(), _atomPickingVertexArrayObject[i].data());
  }
}

void OpenGLAtomPickingShader::generateBuffers()
{
  _atomPickingVertexArrayObject.resize(_renderStructures.size());

  for(size_t i=0l;i<_renderStructures.size();i++)
  {
    _atomPickingVertexArrayObject[i].resize(_renderStructures[i].size());
  }

  for(size_t i=0;i<_renderStructures.size();i++)
  {
    glGenVertexArrays(_renderStructures[i].size(), _atomPickingVertexArrayObject[i].data());

  }
}

void OpenGLAtomPickingShader::paintGL(int width,int height,GLuint structureUniformBuffer)
{

  GLfloat black[4] = {0.0,0.0,0.0,0.0};

  glBindFramebuffer(GL_DRAW_FRAMEBUFFER,_frameBufferObject);

  glViewport(0,0,width,height);
  glDisable(GL_BLEND);
  glClearBufferfv(GL_COLOR, 0, black);
  glClear(GL_DEPTH_BUFFER_BIT);

  glUseProgram(_atomPickingProgram);
  check_gl_error();

  int index = 0;
  for(size_t i=0;i<_renderStructures.size();i++)
  {
    for(size_t j=0;j<_renderStructures[i].size();j++)
    {
      if(_renderStructures[i][j]->drawAtoms() && _renderStructures[i][j]->isVisible() && _atomOrthographicImposterShader._numberOfIndices[i][j]>0 && _atomShader._numberOfDrawnAtoms[i][j]>0)
      {
        glBindBufferRange(GL_UNIFORM_BUFFER, 1, structureUniformBuffer, index * sizeof(RKStructureUniforms), sizeof(RKStructureUniforms));
        glBindVertexArray(_atomPickingVertexArrayObject[i][j]);
        check_gl_error();

        glDrawElementsInstanced(GL_TRIANGLE_STRIP, _atomOrthographicImposterShader._numberOfIndices[i][j], GL_UNSIGNED_SHORT, nullptr,_atomShader._numberOfDrawnAtoms[i][j]);
        check_gl_error();
        glBindVertexArray(0);
      }
      index++;
    }
  }
  glUseProgram(0);

  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

std::array<int,4> OpenGLAtomPickingShader::pickTexture(int x, int y, int width, int height)
{
  std::array<int,4> pixel{0,0,0,0};

  glBindFramebuffer(GL_FRAMEBUFFER, _frameBufferObject);
  check_gl_error();

  glReadBuffer(GL_COLOR_ATTACHMENT0);
  check_gl_error();

  glReadPixels(x, height - y, 1, 1, GL_RGBA_INTEGER, GL_UNSIGNED_INT, pixel.data());
  check_gl_error();

  glReadBuffer(GL_NONE);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  return pixel;
}

void OpenGLAtomPickingShader::reloadData()
{
  initializeVertexArrayObject();
}

void OpenGLAtomPickingShader::initializeVertexArrayObject()
{

  for(size_t i=0;i<_renderStructures.size();i++)
  {
    for(size_t j=0;j<_renderStructures[i].size();j++)
    {
      glBindVertexArray(_atomPickingVertexArrayObject[i][j]);

      glBindBuffer(GL_ARRAY_BUFFER, _atomOrthographicImposterShader._vertexBuffer[i][j]);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _atomOrthographicImposterShader._indexBuffer[i][j]);

      glVertexAttribPointer(_atomPickingVertexPositionAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKVertex), (GLvoid *)offsetof(RKVertex,position));

      glBindBuffer(GL_ARRAY_BUFFER,_atomShader._instancePositionBuffer[i][j]);
      glVertexAttribPointer(_atomPickingInstancePositionAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKInPerInstanceAttributesAtoms), reinterpret_cast<GLvoid*>(offsetof(RKInPerInstanceAttributesAtoms,position)));
      glVertexAttribDivisor(_atomPickingInstancePositionAttributeLocation, 1);


      glVertexAttribPointer(_atomPickingScaleAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKInPerInstanceAttributesAtoms), reinterpret_cast<GLvoid*>(offsetof(RKInPerInstanceAttributesAtoms,scale)));
      glVertexAttribDivisor(_atomPickingScaleAttributeLocation, 1);

      glVertexAttribIPointer(_atomPickingTagAttributeLocation, 1, GL_INT, sizeof(RKInPerInstanceAttributesAtoms), reinterpret_cast<GLvoid*>(offsetof(RKInPerInstanceAttributesAtoms,tag)));
      glVertexAttribDivisor(_atomPickingTagAttributeLocation, 1);

      glEnableVertexAttribArray(_atomPickingVertexPositionAttributeLocation);
      glEnableVertexAttribArray(_atomPickingInstancePositionAttributeLocation);
      glEnableVertexAttribArray(_atomPickingScaleAttributeLocation);
      glEnableVertexAttribArray(_atomPickingTagAttributeLocation);


      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindVertexArray(0);
    }
  }
}

void OpenGLAtomPickingShader::loadShader(void)
{
  GLuint vertexShader;
  GLuint fragmentShader;
  vertexShader=compileShaderOfType(GL_VERTEX_SHADER,OpenGLAtomPickingShader::_atomVertexShaderSource.c_str());
  fragmentShader=compileShaderOfType(GL_FRAGMENT_SHADER,OpenGLAtomPickingShader::_atomFragmentShaderSource.c_str());

  if (0 != vertexShader && 0 != fragmentShader)
  {
    _atomPickingProgram = glCreateProgram();
    check_gl_error();

    glAttachShader(_atomPickingProgram, vertexShader);
    check_gl_error();
    glAttachShader(_atomPickingProgram, fragmentShader);
    check_gl_error();

    glBindFragDataLocation(_atomPickingProgram, 0, "vFragColor");

    linkProgram(_atomPickingProgram);

    _atomPickingVertexPositionAttributeLocation = glGetAttribLocation(_atomPickingProgram, "vertexPosition");
    if (_atomPickingVertexPositionAttributeLocation < 0) qDebug() << "Shader did not contain the 'vertexPosition' attribute.";

    _atomPickingInstancePositionAttributeLocation = glGetAttribLocation(_atomPickingProgram, "instancePosition");
    if (_atomPickingInstancePositionAttributeLocation < 0) qDebug() << "Shader did not contain the 'vertexPosition' attribute.";

    _atomPickingScaleAttributeLocation = glGetAttribLocation(_atomPickingProgram, "instanceScale");
    if (_atomPickingScaleAttributeLocation < 0) qDebug() << "Shader did not contain the 'vertexPosition' attribute.";

    _atomPickingTagAttributeLocation = glGetAttribLocation(_atomPickingProgram, "instanceTag");
    if (_atomPickingTagAttributeLocation < 0) qDebug() << "Shader did not contain the 'instanceTag' attribute.";

    glDeleteShader(vertexShader);
    check_gl_error();
    glDeleteShader(fragmentShader);
    check_gl_error();
  }
}


const std::string  OpenGLAtomPickingShader::_atomVertexShaderSource =
OpenGLVersionStringLiteral +
OpenGLFrameUniformBlockStringLiteral +
OpenGLStructureUniformBlockStringLiteral +
R"foo(
in vec4 vertexPosition;

in vec4 instancePosition;
in vec4 instanceScale;
in int instanceTag;

// Inputs from vertex shader
out VS_OUT
{
  smooth vec4 eye_position;
  smooth vec2 texcoords;
  smooth vec3 frag_pos;
  flat vec3 frag_center;
  flat vec4 sphere_radius;
  flat int instanceId;
} vs_out;


void main(void)
{
  vs_out.instanceId = instanceTag;
  vec4 scale = structureUniforms.atomScaleFactor * instanceScale;

  vs_out.eye_position = frameUniforms.viewMatrix * structureUniforms.modelMatrix * instancePosition;

  vs_out.texcoords = vertexPosition.xy;
  vs_out.sphere_radius = scale;
  vec4 pos2 = frameUniforms.viewMatrix * structureUniforms.modelMatrix * instancePosition;
  pos2.xy += scale.xy * vec2(vertexPosition.x,vertexPosition.y);

  vs_out.frag_pos = pos2.xyz;

  gl_Position =  frameUniforms.projectionMatrix * pos2;
}
)foo";

const std::string  OpenGLAtomPickingShader::_atomFragmentShaderSource =
OpenGLVersionStringLiteral +
OpenGLFrameUniformBlockStringLiteral +
OpenGLStructureUniformBlockStringLiteral +
R"foo(
// Inputs from vertex shader
in VS_OUT
{
  smooth vec4 eye_position;
  smooth vec2 texcoords;
  smooth vec3 frag_pos;
  flat vec3 frag_center;
  flat vec4 sphere_radius;
  flat int instanceId;
} fs_in;

out uvec4 vFragColor;

void main(void)
{
  float x = fs_in.texcoords.x;
  float y = fs_in.texcoords.y;
  float zz = 1.0 - x*x - y*y;

  if (zz <= 0.0)
    discard;

  float z = sqrt(zz);
  vec4 pos = fs_in.eye_position;
  pos.z += fs_in.sphere_radius.z*z;
  pos = frameUniforms.projectionMatrix * pos;
  gl_FragDepth = 0.5*(pos.z / pos.w)+0.5;

  vFragColor = uvec4(1,structureUniforms.sceneIdentifier,structureUniforms.MovieIdentifier,fs_in.instanceId);
}
)foo";
