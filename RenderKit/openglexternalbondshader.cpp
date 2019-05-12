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

#include "openglexternalbondshader.h"
#include "rkrenderuniforms.h"
#include "glgeterror.h"
#include "cylindergeometry.h"
#include "cappedcylindergeometry.h"
#include "cubegeometry.h"

OpenGLExternalBondShader::OpenGLExternalBondShader()
{

}

void OpenGLExternalBondShader::setRenderStructures(std::vector<std::vector<std::shared_ptr<RKRenderStructure>>> structures)
{
  deleteBuffers();
  _renderStructures = structures;
  generateBuffers();
}


void OpenGLExternalBondShader::paintGL(GLuint structureUniformBuffer)
{
  glEnable(GL_CLIP_DISTANCE0);
  glEnable(GL_CLIP_DISTANCE1);
  glEnable(GL_CLIP_DISTANCE2);
  glEnable(GL_CLIP_DISTANCE3);
  glEnable(GL_CLIP_DISTANCE4);
  glEnable(GL_CLIP_DISTANCE5);

  glUseProgram(_program);
  check_gl_error();

  int index = 0;
  for(int i=0;i<_renderStructures.size();i++)
  {
    for(int j=0;j<_renderStructures[i].size();j++)
    {
      if(_renderStructures[i][j]->drawBonds() && _renderStructures[i][j]->isVisible())
      {
        glBindBufferRange(GL_UNIFORM_BUFFER, 1, structureUniformBuffer, GLintptr(index * sizeof(RKStructureUniforms)), GLsizeiptr(sizeof(RKStructureUniforms)));


        glBindVertexArray(_vertexArrayObject[i][j]);
        check_gl_error();

        glDrawElementsInstanced(GL_TRIANGLES, _externalBondNumberOfIndices[i][j], GL_UNSIGNED_SHORT, nullptr, _numberOfExternalBonds[i][j]);

        check_gl_error();
        glBindVertexArray(0);
      }
      index++;
    }
  }
  glUseProgram(0);

  glDisable(GL_CLIP_DISTANCE0);
  glDisable(GL_CLIP_DISTANCE1);
  glDisable(GL_CLIP_DISTANCE2);
  glDisable(GL_CLIP_DISTANCE3);
  glDisable(GL_CLIP_DISTANCE4);
  glDisable(GL_CLIP_DISTANCE5);


  // draw caps on bonds

  glEnable(GL_STENCIL_TEST);

  index = 0;
  for(int i=0;i<_renderStructures.size();i++)
  {
    for(int j=0;j<_renderStructures[i].size();j++)
    {
      if(_renderStructures[i][j]->drawBonds() && _renderStructures[i][j]->isVisible())
      {
        // Disable depth and color writes
        glDepthMask(GLboolean(GL_FALSE));
        glColorMask(GLboolean(GL_FALSE), GLboolean(GL_FALSE), GLboolean(GL_FALSE), GLboolean(GL_FALSE));


        // Invert the stencil everytimes a front or back is encountered (using a mask of 0x1; and independent of
        // whether the depth-test fails or succeeds). Insides will have count 1, outsides have count 0.
        // For this to work no two cut-bonds can be 'behind' eachother. In the shader only the front-facing
        // planes of the (unit-)cell are clipped.
        glStencilFunc(GLenum(GL_ALWAYS),1,0x1);
        glStencilOp(GLenum(GL_KEEP), GLenum(GL_INVERT), GLenum(GL_INVERT));


        // Enable the clipping planes

        glEnable(GLenum(GL_CLIP_DISTANCE0));
        glEnable(GLenum(GL_CLIP_DISTANCE1));
        glEnable(GLenum(GL_CLIP_DISTANCE2));
        glEnable(GLenum(GL_CLIP_DISTANCE3));
        glEnable(GLenum(GL_CLIP_DISTANCE4));
        glEnable(GLenum(GL_CLIP_DISTANCE5));

        glUseProgram(_programStencil);

        if (_numberOfExternalBonds[i][j] > 0 )
        {
          // Write both the front and the back planes (we use 'GL_INVERT')
          glDisable(GLenum(GL_CULL_FACE));

          glBindVertexArray(_vertexStencilArrayObject[i][j]);
          glBindBufferRange(GL_UNIFORM_BUFFER, 1, structureUniformBuffer, GLintptr(index * sizeof(RKStructureUniforms)), GLsizeiptr(sizeof(RKStructureUniforms)));

          glDrawElementsInstanced(GL_TRIANGLES, _externalBondNumberOfIndices[i][j], GL_UNSIGNED_SHORT, nullptr, _numberOfExternalBonds[i][j]);

          glBindVertexArray(0);
        }

        glUseProgram(0);

        // Disables the clipping planes
        glDisable(GLenum(GL_CLIP_DISTANCE0));
        glDisable(GLenum(GL_CLIP_DISTANCE1));
        glDisable(GLenum(GL_CLIP_DISTANCE2));
        glDisable(GLenum(GL_CLIP_DISTANCE3));
        glDisable(GLenum(GL_CLIP_DISTANCE4));
        glDisable(GLenum(GL_CLIP_DISTANCE5));


        // Back to 'normal' operation: enable depth-and color writes and culling
        glEnable(GLenum(GL_CULL_FACE));
        glCullFace(GLenum(GL_BACK));
        glDepthMask(GLboolean(GL_TRUE));
        glColorMask(GLboolean(GL_TRUE), GLboolean(GL_TRUE), GLboolean(GL_TRUE), GLboolean(GL_TRUE));

        // Set the stencil-buffer back to zero
        glStencilOp(GLenum(GL_KEEP), GLenum(GL_ZERO), GLenum(GL_ZERO));

        // Draw plane into colour buffer where the stencil_buffer is set
        glStencilFunc(GLenum(GL_EQUAL),1,0x1);


        // Draw the 6 planes of the (unit-)cell using the stencil-buffer
        glUseProgram(_programBox);
        glBindVertexArray(_boxVertexArray);

        glBindBufferRange(GL_UNIFORM_BUFFER, 1, structureUniformBuffer, GLintptr(index * sizeof(RKStructureUniforms)), GLsizeiptr(sizeof(RKStructureUniforms)));

        glDrawElements(GLenum(GL_TRIANGLE_STRIP), 34, GLenum(GL_UNSIGNED_SHORT), nullptr);
        glBindVertexArray(0);
        glUseProgram(0);

        index += 1;
      }
    }
  }

  glDisable(GLenum(GL_STENCIL_TEST));
}

void OpenGLExternalBondShader::reloadData()
{
  initializeVertexArrayObject();
}

void OpenGLExternalBondShader::initializeVertexArrayObject()
{
  CappedCylinderGeometry cylinder =  CappedCylinderGeometry(1.0,41);

  for(int i=0;i<_renderStructures.size();i++)
  {
    for(int j=0;j<_renderStructures[i].size();j++)
    {
      glBindVertexArray(_vertexArrayObject[i][j]);
      check_gl_error();

      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer[i][j]);
      check_gl_error();

      if(cylinder.vertices().size()>0)
      {
        glBufferData(GL_ARRAY_BUFFER, cylinder.vertices().size()*sizeof(RKVertex), cylinder.vertices().data(), GL_DYNAMIC_DRAW);
        check_gl_error();
      }

      glVertexAttribPointer(_vertexPositionAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKVertex), (GLvoid *)offsetof(RKVertex,position));
      glVertexAttribPointer(_vertexNormalAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKVertex), (GLvoid *)offsetof(RKVertex,normal));
      check_gl_error();

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer[i][j]);
      if(cylinder.indices().size() >0)
      {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, cylinder.indices().size() * sizeof(GLshort), cylinder.indices().data(), GL_DYNAMIC_DRAW);
        check_gl_error();
      }

      std::vector<RKInPerInstanceAttributesBonds> bondInstanceData= _renderStructures[i][j]->renderExternalBonds();
      _numberOfExternalBonds[i][j] = bondInstanceData.size();
      _externalBondNumberOfIndices[i][j] = cylinder.indices().size();

      glBindBuffer(GL_ARRAY_BUFFER, _vertexInstanceBuffer[i][j]);

      if(_numberOfExternalBonds[i][j]>0)
      {
        glBufferData(GL_ARRAY_BUFFER, _numberOfExternalBonds[i][j]*sizeof(RKInPerInstanceAttributesBonds), bondInstanceData.data(), GL_DYNAMIC_DRAW);
        check_gl_error();
      }
      glVertexAttribPointer(_instancePositionFirstAtomAttributeLocation, 4, GL_FLOAT,GL_FALSE, sizeof(RKInPerInstanceAttributesBonds), (void*)offsetof(RKInPerInstanceAttributesBonds, position1));
      glVertexAttribPointer(_instancePositionSecondAtomAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKInPerInstanceAttributesBonds), (void*)offsetof(RKInPerInstanceAttributesBonds, position2));
      glVertexAttribDivisor(_instancePositionFirstAtomAttributeLocation,1);
      glVertexAttribDivisor(_instancePositionSecondAtomAttributeLocation,1);
      check_gl_error();

      glVertexAttribPointer(_instanceColorFirstAtomAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKInPerInstanceAttributesBonds), (void*)offsetof(RKInPerInstanceAttributesBonds, color1));
      glVertexAttribPointer(_instanceColorSecondAtomAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKInPerInstanceAttributesBonds), (void*)offsetof(RKInPerInstanceAttributesBonds, color2));
      glVertexAttribDivisor(_instanceColorFirstAtomAttributeLocation,1);
      glVertexAttribDivisor(_instanceColorSecondAtomAttributeLocation,1);
      check_gl_error();

      glVertexAttribPointer(_instanceScaleAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKInPerInstanceAttributesBonds), (void*)offsetof(RKInPerInstanceAttributesBonds, scale));
      glVertexAttribDivisor(_instanceScaleAttributeLocation,1);
      check_gl_error();

      glEnableVertexAttribArray(_vertexPositionAttributeLocation);
      glEnableVertexAttribArray(_vertexNormalAttributeLocation);
      glEnableVertexAttribArray(_instancePositionFirstAtomAttributeLocation);
      glEnableVertexAttribArray(_instancePositionSecondAtomAttributeLocation);
      glEnableVertexAttribArray(_instanceColorFirstAtomAttributeLocation);
      glEnableVertexAttribArray(_instanceColorSecondAtomAttributeLocation);
      glEnableVertexAttribArray(_instanceScaleAttributeLocation);
      check_gl_error();
      glBindVertexArray(0);


      glBindVertexArray(_vertexStencilArrayObject[i][j]);
      check_gl_error();

      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer[i][j]);
      check_gl_error();

      glVertexAttribPointer(_vertexStencilPositionAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKVertex), (GLvoid *)offsetof(RKVertex,position));
      glVertexAttribPointer(_vertexStencilNormalAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKVertex), (GLvoid *)offsetof(RKVertex,normal));
      check_gl_error();

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer[i][j]);


      glBindBuffer(GL_ARRAY_BUFFER, _vertexInstanceBuffer[i][j]);


      glVertexAttribPointer(_instanceStencilPositionFirstAtomAttributeLocation, 4, GL_FLOAT,GL_FALSE, sizeof(RKInPerInstanceAttributesBonds), (void*)offsetof(RKInPerInstanceAttributesBonds, position1));
      glVertexAttribPointer(_instanceStencilPositionSecondAtomAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKInPerInstanceAttributesBonds), (void*)offsetof(RKInPerInstanceAttributesBonds, position2));
      glVertexAttribDivisor(_instanceStencilPositionFirstAtomAttributeLocation,1);
      glVertexAttribDivisor(_instanceStencilPositionSecondAtomAttributeLocation,1);
      check_gl_error();

      glVertexAttribPointer(_instanceStencilColorFirstAtomAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKInPerInstanceAttributesBonds), (void*)offsetof(RKInPerInstanceAttributesBonds, color1));
      glVertexAttribPointer(_instanceStencilColorSecondAtomAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKInPerInstanceAttributesBonds), (void*)offsetof(RKInPerInstanceAttributesBonds, color2));
      glVertexAttribDivisor(_instanceStencilColorFirstAtomAttributeLocation,1);
      glVertexAttribDivisor(_instanceStencilColorSecondAtomAttributeLocation,1);
      check_gl_error();

      glVertexAttribPointer(_instanceStencilScaleAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKInPerInstanceAttributesBonds), (void*)offsetof(RKInPerInstanceAttributesBonds, scale));
      glVertexAttribDivisor(_instanceStencilScaleAttributeLocation,1);
      check_gl_error();

      glEnableVertexAttribArray(_vertexStencilPositionAttributeLocation);
      glEnableVertexAttribArray(_vertexStencilNormalAttributeLocation);
      glEnableVertexAttribArray(_instanceStencilPositionFirstAtomAttributeLocation);
      glEnableVertexAttribArray(_instanceStencilPositionSecondAtomAttributeLocation);
      glEnableVertexAttribArray(_instanceStencilColorFirstAtomAttributeLocation);
      glEnableVertexAttribArray(_instanceStencilColorSecondAtomAttributeLocation);
      glEnableVertexAttribArray(_instanceStencilScaleAttributeLocation);
      check_gl_error();
      glBindVertexArray(0);
    }
  }

  if(_renderStructures.size() > 0)
  {
    CubeGeometry cube = CubeGeometry();

    glBindVertexArray(_boxVertexArray);

    glBindBuffer(GLenum(GL_ARRAY_BUFFER), _boxVertexBuffer);
    if(cube.vertices().size()>0)
    {
      glBufferData(GLenum(GL_ARRAY_BUFFER), cube.vertices().size()*sizeof(RKVertex), cube.vertices().data(), GLenum(GL_STATIC_DRAW));
      check_gl_error();
    }

    glVertexAttribPointer(_boxVertexPositionAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKVertex), (void*)offsetof(RKVertex, position));
    glVertexAttribPointer(_boxVertexNormalAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKVertex), (void*)offsetof(RKVertex, normal));

    glBindBuffer(GLenum(GL_ELEMENT_ARRAY_BUFFER), _boxIndexBuffer);
    if(cube.indices().size()>0)
    {
      glBufferData(GLenum(GL_ELEMENT_ARRAY_BUFFER), cube.indices().size() * sizeof(GLushort), cube.indices().data(), GL_DYNAMIC_DRAW);
      check_gl_error();
    }

    glEnableVertexAttribArray(_boxVertexPositionAttributeLocation);
    glEnableVertexAttribArray(_boxVertexNormalAttributeLocation);

   glBindVertexArray(0);
  }
}

void OpenGLExternalBondShader::deleteBuffers()
{
  for(int i=0;i<_renderStructures.size();i++)
  {
    glDeleteVertexArrays(_renderStructures[i].size(), _vertexArrayObject[i].data());
    glDeleteVertexArrays(_renderStructures[i].size(), _vertexStencilArrayObject[i].data());
    glDeleteBuffers(_renderStructures[i].size(), _vertexInstanceBuffer[i].data());
    glDeleteBuffers(_renderStructures[i].size(), _vertexBuffer[i].data());
    glDeleteBuffers(_renderStructures[i].size(), _indexBuffer[i].data());
  }

  glDeleteVertexArrays(1, &_boxVertexArray);
  glDeleteBuffers(1, &_boxVertexBuffer);
  glDeleteBuffers(1, &_boxIndexBuffer);
}

void OpenGLExternalBondShader::generateBuffers()
{
  _numberOfExternalBonds.resize(_renderStructures.size());
  _externalBondNumberOfIndices.resize(_renderStructures.size());

  _vertexArrayObject.resize(_renderStructures.size());
  _vertexStencilArrayObject.resize(_renderStructures.size());

  _vertexInstanceBuffer.resize(_renderStructures.size());
  _vertexBuffer.resize(_renderStructures.size());
  _indexBuffer.resize(_renderStructures.size());

  for(int i=0;i<_renderStructures.size();i++)
  {
    _numberOfExternalBonds[i].resize(_renderStructures[i].size());
    _externalBondNumberOfIndices[i].resize(_renderStructures[i].size());

    _vertexArrayObject[i].resize(_renderStructures[i].size());
    _vertexStencilArrayObject[i].resize(_renderStructures[i].size());
    _vertexInstanceBuffer[i].resize(_renderStructures[i].size());
    _vertexBuffer[i].resize(_renderStructures[i].size());
    _indexBuffer[i].resize(_renderStructures[i].size());
  }

  for(int i=0;i<_renderStructures.size();i++)
  {
    glGenVertexArrays(_renderStructures[i].size(), _vertexArrayObject[i].data());
    glGenVertexArrays(_renderStructures[i].size(), _vertexStencilArrayObject[i].data());
    glGenBuffers(_renderStructures[i].size(), _vertexInstanceBuffer[i].data());
    glGenBuffers(_renderStructures[i].size(), _vertexBuffer[i].data());
    glGenBuffers(_renderStructures[i].size(), _indexBuffer[i].data());
  }

  glGenVertexArrays(1, &_boxVertexArray);
  glGenBuffers(1, &_boxVertexBuffer);
  glGenBuffers(1, &_boxIndexBuffer);
}


void OpenGLExternalBondShader::loadShader(void)
{
  GLuint vertexShader;
  GLuint fragmentShader;

  vertexShader=compileShaderOfType(GL_VERTEX_SHADER,OpenGLExternalBondShader::_vertexShaderSource.c_str());
  fragmentShader=compileShaderOfType(GL_FRAGMENT_SHADER,OpenGLExternalBondShader::_fragmentShaderSource.c_str());

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

    _vertexNormalAttributeLocation   = glGetAttribLocation(_program, "vertexNormal");
    _vertexPositionAttributeLocation = glGetAttribLocation(_program, "vertexPosition");
    _instancePositionFirstAtomAttributeLocation = glGetAttribLocation(_program, "instancePosition1");
    _instancePositionSecondAtomAttributeLocation = glGetAttribLocation(_program, "instancePosition2");
    _instanceColorFirstAtomAttributeLocation = glGetAttribLocation(_program, "instanceColor1");
    _instanceColorSecondAtomAttributeLocation = glGetAttribLocation(_program, "instanceColor2");
    _instanceScaleAttributeLocation = glGetAttribLocation(_program, "instanceScale");

    if (_vertexNormalAttributeLocation < 0) qDebug() << "Shader did not contain the 'vertexNormal' attribute.";
    if (_vertexPositionAttributeLocation < 0) qDebug() << "Shader did not contain the 'vertexPosition' attribute.";
    if (_instancePositionFirstAtomAttributeLocation < 0) qDebug() << "Shader did not contain the 'instancePosition1' attribute.";
    if (_instancePositionSecondAtomAttributeLocation < 0) qDebug() << "Shader did not contain the 'instancePosition2' attribute.";
    if (_instanceColorFirstAtomAttributeLocation < 0) qDebug() << "Shader did not contain the 'instanceColor1' attribute.";
    if (_instanceColorSecondAtomAttributeLocation < 0) qDebug() << "Shader did not contain the 'instanceColor2' attribute.";
    if (_instanceScaleAttributeLocation < 0) qDebug() << "Shader did not contain the 'instanceScale' attribute.";

    glDeleteShader(vertexShader);
    check_gl_error();
    glDeleteShader(fragmentShader);
    check_gl_error();
  }

  vertexShader=compileShaderOfType(GL_VERTEX_SHADER,OpenGLExternalBondShader::_vertexShaderSourceStencil.c_str());
  fragmentShader=compileShaderOfType(GL_FRAGMENT_SHADER,OpenGLExternalBondShader::_fragmentShaderSourceStencil.c_str());

  if (0 != vertexShader && 0 != fragmentShader)
  {
    _programStencil = glCreateProgram();
    check_gl_error();

    glAttachShader(_programStencil, vertexShader);
    check_gl_error();
    glAttachShader(_programStencil, fragmentShader);
    check_gl_error();

    glBindFragDataLocation(_programStencil, 0, "vFragColor");

    linkProgram(_programStencil);

    _vertexStencilNormalAttributeLocation   = glGetAttribLocation(_programStencil, "vertexNormal");
    _vertexStencilPositionAttributeLocation = glGetAttribLocation(_programStencil, "vertexPosition");
    _instanceStencilPositionFirstAtomAttributeLocation = glGetAttribLocation(_programStencil, "instancePosition1");
    _instanceStencilPositionSecondAtomAttributeLocation = glGetAttribLocation(_programStencil, "instancePosition2");
    _instanceStencilColorFirstAtomAttributeLocation = glGetAttribLocation(_programStencil, "instanceColor1");
    _instanceStencilColorSecondAtomAttributeLocation = glGetAttribLocation(_programStencil, "instanceColor2");
    _instanceStencilScaleAttributeLocation = glGetAttribLocation(_programStencil, "instanceScale");

    if (_vertexStencilNormalAttributeLocation < 0) qDebug() << "Shader did not contain the 'vertexNormal' attribute.";
    if (_vertexStencilPositionAttributeLocation < 0) qDebug() << "Shader did not contain the 'vertexPosition' attribute.";
    if (_instanceStencilPositionFirstAtomAttributeLocation < 0) qDebug() << "Shader did not contain the 'instancePosition1' attribute.";
    if (_instanceStencilPositionSecondAtomAttributeLocation < 0) qDebug() << "Shader did not contain the 'instancePosition2' attribute.";
    if (_instanceStencilColorFirstAtomAttributeLocation < 0) qDebug() << "Shader did not contain the 'instanceColor1' attribute.";
    if (_instanceStencilColorSecondAtomAttributeLocation < 0) qDebug() << "Shader did not contain the 'instanceColor2' attribute.";
    if (_instanceStencilScaleAttributeLocation < 0) qDebug() << "Shader did not contain the 'instanceScale' attribute.";

    glDeleteShader(vertexShader);
    check_gl_error();
    glDeleteShader(fragmentShader);
    check_gl_error();
  }

  vertexShader=compileShaderOfType(GL_VERTEX_SHADER,OpenGLExternalBondShader::_vertexShaderSourceBox.c_str());
  fragmentShader=compileShaderOfType(GL_FRAGMENT_SHADER,OpenGLExternalBondShader::_fragmentShaderSourceBox.c_str());

  if (0 != vertexShader && 0 != fragmentShader)
  {
    _programBox = glCreateProgram();
    check_gl_error();

    glAttachShader(_programBox, vertexShader);
    check_gl_error();
    glAttachShader(_programBox, fragmentShader);
    check_gl_error();

    glBindFragDataLocation(_programBox, 0, "vFragColor");

    linkProgram(_programBox);

    _boxVertexPositionAttributeLocation = glGetAttribLocation(_programBox, "vertexPosition");
    _boxVertexNormalAttributeLocation   = glGetAttribLocation(_programBox, "vertexNormal");

    glDeleteShader(vertexShader);
    check_gl_error();
    glDeleteShader(fragmentShader);
    check_gl_error();
  }
}

const std::string  OpenGLExternalBondShader::_vertexShaderSource =
OpenGLVersionStringLiteral +
OpenGLFrameUniformBlockStringLiteral +
OpenGLStructureUniformBlockStringLiteral +
OpenGLLightUniformBlockStringLiteral +
R"foo(
in vec4 vertexPosition;
in vec4 vertexNormal;

in vec4 instancePosition1;
in vec4 instancePosition2;
in vec4 instanceColor1;
in vec4 instanceColor2;
in vec4 instanceScale;


// Inputs from vertex shader
out VS_OUT
{
  flat vec4 color1;
  flat vec4 color2;
  smooth vec4 mix;

  smooth vec3 N;
  smooth vec3 L;
  smooth vec3 V;
} vs_out;


void main(void)
{
  vec3 v1,v2;

  vec4 scale = instanceScale;
  vec4 pos =  vertexPosition;

  vec4 pos1 = instancePosition1;
  vec4 pos2 = instancePosition2;

  vec3 dr = (pos1 - pos2).xyz;
  float bondLength = length(dr);

  vs_out.mix.x = clamp(structureUniforms.atomScaleFactor, 0.0, 0.7) * scale.x;
  vs_out.mix.y = vertexPosition.y;  // range 0.0..1.0
  vs_out.mix.z = 1.0 - clamp(structureUniforms.atomScaleFactor, 0.0, 0.7) * scale.z;
  vs_out.mix.w = scale.x/scale.z;


  scale.x = structureUniforms.bondScaling;
  scale.y = bondLength;
  scale.z = structureUniforms.bondScaling;
  scale.w = 1.0;

  dr = normalize(dr);
  v1 = normalize(abs(dr.x) > abs(dr.z) ? vec3(-dr.y, dr.x, 0.0) : vec3(0.0, -dr.z, dr.y));
  v2=normalize(cross(dr,v1));
  mat4 orientationMatrix=mat4x4(vec4(-v1.x,-v1.y,-v1.z,0),
                                vec4(-dr.x,-dr.y,-dr.z,0),
                                vec4(-v2.x,-v2.y,-v2.z,0),
                                vec4(0,0,0,1));

  vs_out.color1 = instanceColor1;
  vs_out.color2 = instanceColor2;

  vs_out.N = (frameUniforms.normalMatrix * structureUniforms.modelMatrix * orientationMatrix * vertexNormal).xyz;

  vec4 P =  frameUniforms.viewMatrix *  structureUniforms.modelMatrix * vec4((orientationMatrix * (scale * pos) + pos1).xyz,1.0);

  // Calculate light vector
  vs_out.L = (lightUniforms.lights[0].position - P*lightUniforms.lights[0].position.w).xyz;

  // Calculate view vector
  vs_out.V = -P.xyz;

  vec4 vertexPos = (orientationMatrix * (scale * pos) + pos1);
  gl_Position = frameUniforms.mvpMatrix * structureUniforms.modelMatrix * vertexPos;

  gl_ClipDistance[0] = dot(structureUniforms.clipPlaneBack, vertexPos);
  gl_ClipDistance[1] = dot(structureUniforms.clipPlaneBottom, vertexPos);
  gl_ClipDistance[2] = dot(structureUniforms.clipPlaneLeft, vertexPos);

  gl_ClipDistance[3] = dot(structureUniforms.clipPlaneFront, vertexPos);
  gl_ClipDistance[4] = dot(structureUniforms.clipPlaneTop, vertexPos);
  gl_ClipDistance[5] = dot(structureUniforms.clipPlaneRight, vertexPos);
}
)foo";

const std::string  OpenGLExternalBondShader::_fragmentShaderSource =
OpenGLVersionStringLiteral +
OpenGLFrameUniformBlockStringLiteral +
OpenGLStructureUniformBlockStringLiteral +
OpenGLLightUniformBlockStringLiteral +
R"foo(
vec3 rgb2hsv(vec3 c)
{
  vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
  vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
  vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

  float d = q.x - min(q.w, q.y);
  float e = 1.0e-10;
  return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c)
{
  vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
  vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
  return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

out vec4 vFragColor;

// Input from vertex shader
in VS_OUT
{
  flat vec4 color1;
  flat vec4 color2;
  smooth vec4 mix;

  smooth vec3 N;
  smooth vec3 L;
  smooth vec3 V;
} fs_in;

void main(void)
{

  // Normalize the incoming N and L vectors
  vec3 N = normalize(fs_in.N);
  vec3 L = normalize(fs_in.L);
  vec3 V = normalize(fs_in.V);

   // Calculate R locally
  vec3 R = reflect(-L, N);

  vec4 ambient = lightUniforms.lights[0].ambient * structureUniforms.bondAmbientColor;
  vec4 specular = pow(max(dot(R, V), 0.0),  lightUniforms.lights[0].shininess + structureUniforms.bondShininess) * lightUniforms.lights[0].specular * structureUniforms.bondSpecularColor;
  vec4 diffuse = vec4(max(dot(N, L), 0.0));
  float t = clamp((fs_in.mix.y - fs_in.mix.x)/(fs_in.mix.z - fs_in.mix.x),0.0,1.0);

  switch(structureUniforms.bondColorMode)
  {
    case 0:
      diffuse *= structureUniforms.bondDiffuseColor;
      break;
    case 1:
      diffuse *= (t < 0.5 ? fs_in.color1 : fs_in.color2);
      break;
    case 2:
      diffuse *= mix(fs_in.color1,fs_in.color2,smoothstep(0.0,1.0,t));
      break;
  }

  vec4 color= vec4(ambient.xyz + diffuse.xyz + specular.xyz, 1.0);

  if (structureUniforms.bondHDR)
  {
    vec4 vLdrColor = 1.0 - exp2(-color * structureUniforms.bondHDRExposure);
    vLdrColor.a = 1.0;
    color= vLdrColor;
  }

  vec3 hsv = rgb2hsv(color.xyz);
  hsv.x = hsv.x * structureUniforms.bondHue;
  hsv.y = hsv.y * structureUniforms.bondSaturation;
  hsv.z = hsv.z * structureUniforms.bondValue;
  vFragColor = vec4(hsv2rgb(hsv),1.0);
}
)foo";


const std::string  OpenGLExternalBondShader::_vertexShaderSourceStencil =
OpenGLVersionStringLiteral +
OpenGLFrameUniformBlockStringLiteral +
OpenGLStructureUniformBlockStringLiteral +
OpenGLLightUniformBlockStringLiteral +
R"foo(
in vec4 vertexPosition;
in vec4 vertexNormal;

in vec4 instancePosition1;
in vec4 instancePosition2;
in vec4 instanceColor1;
in vec4 instanceColor2;
in vec4 instanceScale;


// Inputs from vertex shader
out VS_OUT
{
  flat vec4 color1;
  flat vec4 color2;
  smooth vec4 mix;

  smooth vec3 N;
  smooth vec3 L;
  smooth vec3 V;
} vs_out;

// works for both orthogonal and perspective
float frontFacing(vec4 pos0, vec4 pos1, vec4 pos2)
{
  return pos0.x*pos1.y - pos1.x*pos0.y + pos1.x*pos2.y - pos2.x*pos1.y + pos2.x*pos0.y - pos0.x*pos2.y;
}

void main(void)
{
  vec3 v1,v2;

  vec4 scale = instanceScale;
  vec4 pos =  vertexPosition;

  vec4 pos1 = instancePosition1;
  vec4 pos2 = instancePosition2;

  vec3 dr = (pos1 - pos2).xyz;
  float bondLength = length(dr);

  vs_out.mix.x = clamp(structureUniforms.atomScaleFactor, 0.0, 0.7) * scale.x;
  vs_out.mix.y = vertexPosition.y;  // range 0.0..1.0
  vs_out.mix.z = 1.0 - clamp(structureUniforms.atomScaleFactor, 0.0, 0.7) * scale.z;
  vs_out.mix.w = scale.x/scale.z;


  scale.x = structureUniforms.bondScaling;
  scale.y = bondLength;
  scale.z = structureUniforms.bondScaling;
  scale.w = 1.0;

  dr = normalize(dr);
  v1 = normalize(abs(dr.x) > abs(dr.z) ? vec3(-dr.y, dr.x, 0.0) : vec3(0.0, -dr.z, dr.y));
  v2=normalize(cross(dr,v1));
  mat4 orientationMatrix=mat4x4(vec4(-v1.x,-v1.y,-v1.z,0),
                                vec4(-dr.x,-dr.y,-dr.z,0),
                                vec4(-v2.x,-v2.y,-v2.z,0),
                                vec4(0,0,0,1));

  vs_out.color1 = instanceColor1;
  vs_out.color2 = instanceColor2;

  vs_out.N = (frameUniforms.normalMatrix * structureUniforms.modelMatrix * orientationMatrix * vertexNormal).xyz;

  vec4 P =  frameUniforms.viewMatrix *  structureUniforms.modelMatrix * vec4((orientationMatrix * (scale * pos) + pos1).xyz,1.0);

  // Calculate light vector
  vs_out.L = (lightUniforms.lights[0].position - P*lightUniforms.lights[0].position.w).xyz;

  // Calculate view vector
  vs_out.V = -P.xyz;

  vec4 vertexPos = (orientationMatrix * (scale * pos) + pos1);
  gl_Position = frameUniforms.mvpMatrix * structureUniforms.modelMatrix * vertexPos;

  float v_clipDistLeft = dot(structureUniforms.clipPlaneLeft,vertexPos);
  float v_clipDistRight = dot(structureUniforms.clipPlaneRight,vertexPos);

  float v_clipDistTop = dot(structureUniforms.clipPlaneTop,vertexPos);
  float v_clipDistBottom = dot(structureUniforms.clipPlaneBottom,vertexPos);

  float v_clipDistFront = dot(structureUniforms.clipPlaneFront,vertexPos);
  float v_clipDistBack = dot(structureUniforms.clipPlaneBack,vertexPos);

  // compute 3 reference points to determine front- or backfacing
  mat4 mvpMatrix = frameUniforms.mvpMatrix *  structureUniforms.modelMatrix;
  vec4 boxPosition0 = mvpMatrix * (structureUniforms.boxMatrix*vec4(0.0, 0.0, 0.0, 1.0));
  vec4 boxPosition1 = mvpMatrix * (structureUniforms.boxMatrix*vec4(1.0, 0.0, 0.0, 1.0));
  vec4 boxPosition2 = mvpMatrix * (structureUniforms.boxMatrix*vec4(1.0, 1.0, 0.0, 1.0));
  vec4 boxPosition3 = mvpMatrix * (structureUniforms.boxMatrix*vec4(0.0, 1.0, 0.0, 1.0));
  vec4 boxPosition4 = mvpMatrix * (structureUniforms.boxMatrix*vec4(0.0, 0.0, 1.0, 1.0));
  vec4 boxPosition5 = mvpMatrix * (structureUniforms.boxMatrix*vec4(1.0, 0.0, 1.0, 1.0));
  vec4 boxPosition6 = mvpMatrix * (structureUniforms.boxMatrix*vec4(1.0, 1.0, 1.0, 1.0));
  vec4 boxPosition7 = mvpMatrix * (structureUniforms.boxMatrix*vec4(0.0, 1.0, 1.0, 1.0));


  // perspective division
  boxPosition0 = boxPosition0/boxPosition0.w;
  boxPosition1 = boxPosition1/boxPosition1.w;
  boxPosition2 = boxPosition2/boxPosition2.w;
  boxPosition3 = boxPosition3/boxPosition3.w;
  boxPosition4 = boxPosition4/boxPosition4.w;
  boxPosition5 = boxPosition5/boxPosition5.w;
  boxPosition6 = boxPosition6/boxPosition6.w;
  boxPosition7 = boxPosition7/boxPosition7.w;

  float leftFrontfacing = frontFacing(boxPosition0, boxPosition3, boxPosition7);
  float rightFrontfacing = frontFacing(boxPosition1, boxPosition5, boxPosition2);

  float topFrontFacing = frontFacing(boxPosition3, boxPosition2, boxPosition7);
  float bottomFrontFacing = frontFacing(boxPosition0, boxPosition4, boxPosition1);

  float frontFrontFacing = frontFacing(boxPosition4, boxPosition6, boxPosition5);
  float backFrontFacing = frontFacing(boxPosition0, boxPosition1, boxPosition2);


  gl_ClipDistance[0] = (leftFrontfacing<0.0) ? v_clipDistLeft : 0.0;
  gl_ClipDistance[1] = (rightFrontfacing<0.0) ? v_clipDistRight : 0.0;

  gl_ClipDistance[2] = (topFrontFacing<0.0) ? v_clipDistTop : 0.0;
  gl_ClipDistance[3] = (bottomFrontFacing<0.0) ? v_clipDistBottom : 0.0;

  gl_ClipDistance[4] = (frontFrontFacing<0.0) ? v_clipDistFront : 0.0;
  gl_ClipDistance[5] = (backFrontFacing<0.0) ? v_clipDistBack : 0.0;
}
)foo";


const std::string  OpenGLExternalBondShader::_fragmentShaderSourceStencil =
OpenGLVersionStringLiteral +
OpenGLFrameUniformBlockStringLiteral +
OpenGLStructureUniformBlockStringLiteral +
OpenGLLightUniformBlockStringLiteral +
R"foo(
vec3 rgb2hsv(vec3 c)
{
  vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
  vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
  vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

  float d = q.x - min(q.w, q.y);
  float e = 1.0e-10;
  return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c)
{
  vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
  vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
  return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

out vec4 vFragColor;

// Input from vertex shader
in VS_OUT
{
  flat vec4 color1;
  flat vec4 color2;
  smooth vec4 mix;

  smooth vec3 N;
  smooth vec3 L;
  smooth vec3 V;
} fs_in;

void main(void)
{

  // Normalize the incoming N and L vectors
  vec3 N = normalize(fs_in.N);
  vec3 L = normalize(fs_in.L);
  vec3 V = normalize(fs_in.V);

   // Calculate R locally
  vec3 R = reflect(-L, N);

  vec4 ambient = lightUniforms.lights[0].ambient * structureUniforms.bondAmbientColor;
  vec4 specular = pow(max(dot(R, V), 0.0),  lightUniforms.lights[0].shininess + structureUniforms.bondShininess) * lightUniforms.lights[0].specular * structureUniforms.bondSpecularColor;
  vec4 diffuse = vec4(max(dot(N, L), 0.0));
  float t = clamp((fs_in.mix.y - fs_in.mix.x)/(fs_in.mix.z - fs_in.mix.x),0.0,1.0);

  switch(structureUniforms.bondColorMode)
  {
    case 0:
      diffuse *= structureUniforms.bondDiffuseColor;
      break;
    case 1:
      diffuse *= (t < 0.5 ? fs_in.color1 : fs_in.color2);
      break;
    case 2:
      diffuse *= mix(fs_in.color1,fs_in.color2,smoothstep(0.0,1.0,t));
      break;
  }

  vec4 color= vec4(ambient.xyz + diffuse.xyz + specular.xyz, 1.0);

  if (structureUniforms.bondHDR)
  {
    vec4 vLdrColor = 1.0 - exp2(-color * structureUniforms.bondHDRExposure);
    vLdrColor.a = 1.0;
    color= vLdrColor;
  }

  vec3 hsv = rgb2hsv(color.xyz);
  hsv.x = hsv.x * structureUniforms.bondHue;
  hsv.y = hsv.y * structureUniforms.bondSaturation;
  hsv.z = hsv.z * structureUniforms.bondValue;
  vFragColor = vec4(hsv2rgb(hsv),1.0);
}
)foo";

const std::string  OpenGLExternalBondShader::_vertexShaderSourceBox =
OpenGLVersionStringLiteral +
OpenGLFrameUniformBlockStringLiteral +
OpenGLStructureUniformBlockStringLiteral +
OpenGLLightUniformBlockStringLiteral +
R"foo(
in vec4 vertexPosition;
in vec4 vertexNormal;

// Inputs from vertex shader
out VS_OUT
{
  flat vec4 ambient;
  flat vec4 diffuse;
  flat vec4 specular;
  smooth vec3 N;
  smooth vec3 L;
  smooth vec3 V;
} vs_out;

void main()
{
  vec4 pos;

  vs_out.ambient = lightUniforms.lights[0].ambient * structureUniforms.bondAmbientColor;
  vs_out.diffuse = lightUniforms.lights[0].diffuse * structureUniforms.bondDiffuseColor;
  vs_out.specular = lightUniforms.lights[0].specular * structureUniforms.bondSpecularColor;

  // Calculate normal in view-space
  vs_out.N = (frameUniforms.normalMatrix * structureUniforms.modelMatrix * vertexNormal).xyz;

  vec4 P = frameUniforms.viewMatrix * structureUniforms.modelMatrix * structureUniforms.boxMatrix * vertexPosition;

  // Calculate light vector
  vs_out.L = (lightUniforms.lights[0].position - P*lightUniforms.lights[0].position.w).xyz;

  // Calculate view vector
  vs_out.V = -P.xyz;

  gl_Position = frameUniforms.projectionMatrix * P;
}
)foo";

const std::string  OpenGLExternalBondShader::_fragmentShaderSourceBox =
OpenGLVersionStringLiteral +
OpenGLFrameUniformBlockStringLiteral +
OpenGLStructureUniformBlockStringLiteral +
OpenGLLightUniformBlockStringLiteral +
R"foo(
out vec4 vFragColor;

// Inputs from vertex shader
in VS_OUT
{
  flat vec4 ambient;
  flat vec4 diffuse;
  flat vec4 specular;
  smooth vec3 N;
  smooth vec3 L;
  smooth vec3 V;
} fs_in;

void main()
{
  // Normalize the incoming N, L and V vectors
  vec3 N = normalize(fs_in.N);
  vec3 L = normalize(fs_in.L);
  vec3 V = normalize(fs_in.V);

  // Calculate R locally
  vec3 R = reflect(-L, N);

  // Compute the diffuse and specular components for each fragment
  vec4 ambient = fs_in.ambient;
  vec4 diffuse = max(dot(N, L), 0.0) * fs_in.diffuse;
  vec4 color = vec4(ambient.xyz+diffuse.xyz,1.0);

  if (structureUniforms.atomHDR)
  {
    vec4 vLdrColor = 1.0 - exp2(-color * structureUniforms.atomHDRExposure);
    vLdrColor.a = 1.0;
    color= vLdrColor;
  }

  vFragColor = color;
}
)foo";
