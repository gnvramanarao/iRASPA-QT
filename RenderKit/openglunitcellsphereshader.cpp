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

#include "openglunitcellsphereshader.h"
#include <QDebug>
#include <iostream>
#include "glgeterror.h"
#include "spheregeometry.h"
#include "cylindergeometry.h"

OpenGLUnitCellSphereShader::OpenGLUnitCellSphereShader()
{

}

void OpenGLUnitCellSphereShader::setRenderStructures(std::vector<std::vector<std::shared_ptr<RKRenderStructure>>> structures)
{
  deleteBuffers();
  _renderStructures = structures;
  generateBuffers();
}

void OpenGLUnitCellSphereShader::paintGL(GLuint structureUniformBuffer)
{
  glUseProgram(_program);
  check_gl_error();

  int index = 0;
  for(int i=0;i<_renderStructures.size();i++)
  {
    for(int j=0;j<_renderStructures[i].size();j++)
    {
      if(_renderStructures[i][j]->drawUnitCell() && _renderStructures[i][j]->isVisible())
      {
        glBindBufferRange(GL_UNIFORM_BUFFER, 1, structureUniformBuffer, GLintptr(index * sizeof(RKStructureUniforms)), GLsizeiptr(sizeof(RKStructureUniforms)));

        glBindVertexArray(_vertexArrayObject[i][j]);
        check_gl_error();

        glDrawElementsInstanced(GL_TRIANGLE_STRIP, _numberOfIndices[i][j], GL_UNSIGNED_SHORT, nullptr,(GLsizei)_numberOfUnitCellSpheres[i][j]);
        check_gl_error();
        glBindVertexArray(0);
      }
      index++;
    }
  }
  glUseProgram(0);
}

void OpenGLUnitCellSphereShader::reloadData()
{
  initializeVertexArrayObject();
}

void OpenGLUnitCellSphereShader::initializeVertexArrayObject()
{
  SphereGeometry sphere = SphereGeometry(1.0,41);

  for(int i=0;i<_renderStructures.size();i++)
  {
    for(int j=0;j<_renderStructures[i].size();j++)
    {
      glBindVertexArray(_vertexArrayObject[i][j]);
check_gl_error();
      glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer[i][j]);
      glBufferData(GL_ARRAY_BUFFER, sphere.vertices().size() * sizeof(RKVertex), sphere.vertices().data(), GL_DYNAMIC_DRAW);
check_gl_error();
      _numberOfIndices[i][j] = sphere.indices().size();

      glVertexAttribPointer(_vertexPositionAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKVertex), (GLvoid *)offsetof(RKVertex,position));
      glVertexAttribPointer(_vertexNormalAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKVertex), (GLvoid *)offsetof(RKVertex,position));

check_gl_error();
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer[i][j]);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere.indices().size() * sizeof(GLshort), sphere.indices().data(), GL_DYNAMIC_DRAW);
check_gl_error();

      std::vector<RKInPerInstanceAttributesAtoms> unitCellSphereInstanceData = _renderStructures[i][j]->renderUnitCellSpheres();
      _numberOfUnitCellSpheres[i][j] = unitCellSphereInstanceData.size();


      glBindBuffer(GL_ARRAY_BUFFER, _vertexInstanceBuffer[i][j]);
      glBufferData(GL_ARRAY_BUFFER, unitCellSphereInstanceData.size()*sizeof(RKInPerInstanceAttributesAtoms), unitCellSphereInstanceData.data(), GL_DYNAMIC_DRAW);
      glVertexAttribPointer(_instancePositionAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKInPerInstanceAttributesAtoms), (GLvoid *)offsetof(RKInPerInstanceAttributesAtoms,position));
      glVertexAttribDivisor(_instancePositionAttributeLocation,1);
check_gl_error();


      glVertexAttribPointer(_scaleAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKInPerInstanceAttributesAtoms), (GLvoid *)offsetof(RKInPerInstanceAttributesAtoms,scale));
      glVertexAttribDivisor(_scaleAttributeLocation, 1);
check_gl_error();

      glEnableVertexAttribArray(_vertexPositionAttributeLocation);
      glEnableVertexAttribArray(_vertexNormalAttributeLocation);
      glEnableVertexAttribArray(_instancePositionAttributeLocation);
      glEnableVertexAttribArray(_scaleAttributeLocation);
check_gl_error();
      glBindVertexArray(0);
    }
  }
}

void OpenGLUnitCellSphereShader::deleteBuffers()
{
  for(int i=0;i<_renderStructures.size();i++)
  {
    glDeleteBuffers(_renderStructures[i].size(), _vertexBuffer[i].data());
    glDeleteBuffers(_renderStructures[i].size(), _indexBuffer[i].data());
    glDeleteBuffers(_renderStructures[i].size(), _vertexInstanceBuffer[i].data());

    glDeleteVertexArrays(_renderStructures[i].size(), _vertexArrayObject[i].data());
  }
}


void OpenGLUnitCellSphereShader::generateBuffers()
{
  _numberOfUnitCellSpheres.resize(_renderStructures.size());
  _numberOfIndices.resize(_renderStructures.size());

  _vertexBuffer.resize(_renderStructures.size());
  _indexBuffer.resize(_renderStructures.size());
  _vertexInstanceBuffer.resize(_renderStructures.size());

  _vertexArrayObject.resize(_renderStructures.size());

  for(int i=0;i<_renderStructures.size();i++)
  {
    _numberOfUnitCellSpheres[i].resize(_renderStructures[i].size());
    _numberOfIndices[i].resize(_renderStructures[i].size());

    _vertexBuffer[i].resize(_renderStructures[i].size());
    _indexBuffer[i].resize(_renderStructures[i].size());
    _vertexInstanceBuffer[i].resize(_renderStructures[i].size());
    _vertexArrayObject[i].resize(_renderStructures[i].size());
  }

  for(int i=0;i<_renderStructures.size();i++)
  {
    glGenBuffers(_renderStructures[i].size(), _vertexBuffer[i].data());
    glGenBuffers(_renderStructures[i].size(), _indexBuffer[i].data());
    glGenBuffers(_renderStructures[i].size(), _vertexInstanceBuffer[i].data());
    glGenVertexArrays(_renderStructures[i].size(), _vertexArrayObject[i].data());
  }
}

void OpenGLUnitCellSphereShader::loadShader(void)
{
  GLuint vertexShader;
  GLuint fragmentShader;

  vertexShader=compileShaderOfType(GL_VERTEX_SHADER,OpenGLUnitCellSphereShader::_vertexShaderSource.c_str());
  fragmentShader=compileShaderOfType(GL_FRAGMENT_SHADER,OpenGLUnitCellSphereShader::_fragmentShaderSource.c_str());

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

    _vertexNormalAttributeLocation = glGetAttribLocation(_program, "vertexNormal");
    _vertexPositionAttributeLocation = glGetAttribLocation(_program, "vertexPosition");
    _instancePositionAttributeLocation = glGetAttribLocation(_program, "instancePosition");
    _scaleAttributeLocation = glGetAttribLocation(_program, "instanceScale");

    if (_vertexNormalAttributeLocation < 0) qDebug() << "Shader did not contain the 'vertexNormal' attribute.";
    if (_vertexPositionAttributeLocation < 0) qDebug() << "Shader did not contain the 'vertexPosition' attribute.";
    if (_instancePositionAttributeLocation < 0) qDebug() << "Shader did not contain the 'instancePosition' attribute.";
    if (_scaleAttributeLocation < 0) qDebug() << "Shader did not contain the 'instanceScale' attribute.";

    glDeleteShader(vertexShader);
    check_gl_error();
    glDeleteShader(fragmentShader);
    check_gl_error();
  }
}


const std::string  OpenGLUnitCellSphereShader::_vertexShaderSource =
OpenGLVersionStringLiteral +
OpenGLFrameUniformBlockStringLiteral +
OpenGLStructureUniformBlockStringLiteral +
OpenGLLightUniformBlockStringLiteral +
R"foo(
in vec4 vertexPosition;
in vec4 vertexNormal;

in vec4 instancePosition;
in vec4 instanceScale;

// Inputs from vertex shader
out VS_OUT
{
  smooth vec3 N;
  smooth vec3 L;
  smooth vec3 V;
} vs_out;


void main(void)
{
  vec4 scale = structureUniforms.unitCellScaling * instanceScale;
  vec4 pos = instancePosition + scale*vertexPosition;

  // Calculate normal in view-space
  vs_out.N = (frameUniforms.normalMatrix * structureUniforms.modelMatrix * vertexNormal).xyz;

  vec4 P = frameUniforms.viewMatrix * structureUniforms.modelMatrix * pos;

  // Calculate light vector
  vs_out.L = (lightUniforms.lights[0].position - P*lightUniforms.lights[0].position.w).xyz;

  // Calculate view vector
  vs_out.V = -P.xyz;

  gl_Position = frameUniforms.mvpMatrix * structureUniforms.modelMatrix * pos;
}
)foo";

const std::string  OpenGLUnitCellSphereShader::_fragmentShaderSource =
OpenGLVersionStringLiteral +
OpenGLFrameUniformBlockStringLiteral +
OpenGLStructureUniformBlockStringLiteral +
R"foo(
// Input from vertex shader
in VS_OUT
{
  smooth vec3 N;
  smooth vec3 L;
  smooth vec3 V;
} fs_in;

uniform sampler2D ambientOcclusionTexture;
out  vec4 vFragColor;

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


void main(void)
{
  // Normalize the incoming N, L and V vectors
  vec3 N = normalize(fs_in.N);
  vec3 L = normalize(fs_in.L);
  vec3 V = normalize(fs_in.V);

  // Calculate R locally
  vec3 R = reflect(-L, N);

  vec4 color = max(dot(N, L), 0.0) * structureUniforms.unitCellColor;

  vec3 hsv = rgb2hsv(color.xyz);
  hsv.x = hsv.x * structureUniforms.changeHueSaturationValue.x;
  hsv.y = hsv.y * structureUniforms.changeHueSaturationValue.y;
  hsv.z = hsv.z * structureUniforms.changeHueSaturationValue.z;
  vFragColor = vec4(hsv2rgb(hsv),1.0);
}
)foo";
