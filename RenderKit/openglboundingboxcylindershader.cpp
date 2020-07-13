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

#include "openglboundingboxcylindershader.h"
#include <QDebug>
#include <iostream>
#include "glgeterror.h"
#include "spheregeometry.h"
#include "cylindergeometry.h"

OpenGLBoundingBoxCylinderShader::OpenGLBoundingBoxCylinderShader()
{

}


void OpenGLBoundingBoxCylinderShader::setRenderDataSource(std::shared_ptr<RKRenderDataSource> source)
{
  _dataSource = source;
}

void OpenGLBoundingBoxCylinderShader::paintGL()
{
  if(std::shared_ptr<RKRenderDataSource> dataSource = _dataSource)
  {
    if(dataSource->showBoundingBox())
    {
      glUseProgram(_program);
      check_gl_error();

      glBindVertexArray(_vertexArrayObject);
      check_gl_error();

      glDrawElementsInstanced(GL_TRIANGLE_STRIP, static_cast<GLsizei>(_numberOfIndices), GL_UNSIGNED_SHORT, nullptr, static_cast<GLsizei>(_numberOfUnitCellCylinders));
      check_gl_error();
      glBindVertexArray(0);
      glUseProgram(0);
    }
  }

}

void OpenGLBoundingBoxCylinderShader::generateBuffers()
{
  glGenVertexArrays(1, &_vertexArrayObject);
  glGenBuffers(1, &_vertexInstanceBuffer);
  glGenBuffers(1, &_vertexBuffer);
  glGenBuffers(1, &_indexBuffer);
}

void OpenGLBoundingBoxCylinderShader::reloadData()
{
  initializeVertexArrayObject();
}

void OpenGLBoundingBoxCylinderShader::initializeVertexArrayObject()
{
  CylinderGeometry cylinder = CylinderGeometry(1.0,41);


  if(std::shared_ptr<RKRenderDataSource> dataSource = _dataSource)
    {
      glBindVertexArray(_vertexArrayObject);

      _numberOfIndices = cylinder.indices().size();

      glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
      if(cylinder.vertices().size()>0)
      {
        glBufferData(GL_ARRAY_BUFFER, cylinder.vertices().size()*sizeof(RKVertex), cylinder.vertices().data(), GL_DYNAMIC_DRAW);
        check_gl_error();
      }

      glVertexAttribPointer(_vertexPositionAttributeLocation, 4, GL_FLOAT, GL_FALSE,sizeof(RKVertex), (GLvoid *)offsetof(RKVertex,position));
      glVertexAttribPointer(_vertexNormalAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKVertex), (GLvoid *)offsetof(RKVertex,normal));

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
      if(cylinder.indices().size() >0)
      {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, cylinder.indices().size() * sizeof(GLshort), cylinder.indices().data(), GL_DYNAMIC_DRAW);
        check_gl_error();
      }

      std::vector<RKInPerInstanceAttributesBonds> unitCellCylinderInstanceData = dataSource->renderBoundingBoxCylinders();
      _numberOfUnitCellCylinders = unitCellCylinderInstanceData.size();

      glBindBuffer(GL_ARRAY_BUFFER, _vertexInstanceBuffer);
      if(unitCellCylinderInstanceData.size()>0)
      {
        glBufferData(GL_ARRAY_BUFFER, unitCellCylinderInstanceData.size() * sizeof(RKInPerInstanceAttributesBonds), unitCellCylinderInstanceData.data(), GL_DYNAMIC_DRAW);
        check_gl_error();
      }
      glVertexAttribPointer(_instancePositionFirstAtomAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKInPerInstanceAttributesBonds), (void*)offsetof(RKInPerInstanceAttributesBonds, position1));
      glVertexAttribPointer(_instancePositionSecondAtomAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKInPerInstanceAttributesBonds), (void*)offsetof(RKInPerInstanceAttributesBonds, position2));
      glVertexAttribDivisor(_instancePositionFirstAtomAttributeLocation,1);
      glVertexAttribDivisor(_instancePositionSecondAtomAttributeLocation,1);
      glVertexAttribPointer(_instanceScaleAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKInPerInstanceAttributesBonds), (void*)offsetof(RKInPerInstanceAttributesBonds, scale));
      glVertexAttribDivisor(_instanceScaleAttributeLocation,1);

      glEnableVertexAttribArray(_vertexPositionAttributeLocation);
      glEnableVertexAttribArray(_vertexNormalAttributeLocation);
      glEnableVertexAttribArray(_instancePositionFirstAtomAttributeLocation);
      glEnableVertexAttribArray(_instancePositionSecondAtomAttributeLocation);
      glEnableVertexAttribArray(_instanceScaleAttributeLocation);

      glBindVertexArray(0);
  }
}

void OpenGLBoundingBoxCylinderShader::loadShader(void)
{
  GLuint vertexShader;
  GLuint fragmentShader;

  vertexShader=compileShaderOfType(GL_VERTEX_SHADER,OpenGLBoundingBoxCylinderShader::_vertexShaderSource.c_str());
  fragmentShader=compileShaderOfType(GL_FRAGMENT_SHADER,OpenGLBoundingBoxCylinderShader::_fragmentShaderSource.c_str());

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
    _instancePositionFirstAtomAttributeLocation = glGetAttribLocation(_program, "instancePosition");
    _instancePositionSecondAtomAttributeLocation = glGetAttribLocation(_program, "instancePosition2");
    _instanceScaleAttributeLocation = glGetAttribLocation(_program, "instanceScale");

    if (_vertexNormalAttributeLocation < 0) qDebug() << "Shader did not contain the 'vertexNormal' attribute.";
    if (_vertexPositionAttributeLocation < 0) qDebug() << "Shader did not contain the 'vertexPosition' attribute.";
    if (_instancePositionFirstAtomAttributeLocation < 0) qDebug() << "Shader did not contain the 'instancePosition' attribute.";
    if (_instancePositionSecondAtomAttributeLocation < 0) qDebug() << "Shader did not contain the 'instancePosition2' attribute.";
    if (_instanceScaleAttributeLocation < 0) qDebug() << "Shader did not contain the 'instanceScale' attribute.";

    glDeleteShader(vertexShader);
    check_gl_error();
    glDeleteShader(fragmentShader);
    check_gl_error();
  }
}


const std::string  OpenGLBoundingBoxCylinderShader::_vertexShaderSource =
OpenGLVersionStringLiteral +
OpenGLFrameUniformBlockStringLiteral +
OpenGLLightUniformBlockStringLiteral +
R"foo(
// Inputs from vertex shader
out VS_OUT
{
  smooth vec3 N;
  smooth vec3 L;
  smooth vec4 diffuse;
} vs_out;

in vec4 vertexPosition;
in vec4 vertexNormal;
in vec4 instancePosition;
in vec4 instancePosition2;
in vec4 instanceScale;


void main(void)
{
  vec3 v1,v2;

  //vert.instanceId = iid;
  vec4 scale = instanceScale;

  vec4 pos =  scale * vec4((vertexPosition).xyz,1.0);

  vec4 pos1 = instancePosition;
  vec4 pos2 = instancePosition2;

  vec3 dr = (pos1 - pos2).xyz;
  float bondLength = length(dr);

  vs_out.diffuse = vec4(1.0,1.0,1.0,1.0);

  scale.x = 1.0;
  scale.y = bondLength;
  scale.z = 1.0;

  dr = normalize(dr);
  if ((dr.z !=0 ) && (-dr.x != dr.y ))
    v1=normalize(vec3(-dr.y-dr.z,dr.x,dr.x));
  else
    v1=normalize(vec3(dr.z,dr.z,-dr.x-dr.y));

  v2=normalize(cross(dr,v1));

  mat4 orientationMatrix=mat4(vec4(-v1.x,-v1.y,-v1.z,0),
                              vec4(-dr.x,-dr.y,-dr.z,0),
                              vec4(-v2.x,-v2.y,-v2.z,0),
                              vec4(0,0,0,1));

  vs_out.N = (frameUniforms.normalMatrix * orientationMatrix * vertexNormal).xyz;

  vec4 P =  frameUniforms.viewMatrix *  vec4((orientationMatrix * (scale * pos) + pos1).xyz,1.0);

  // Calculate light vector
  vs_out.L = (lightUniforms.lights[0].position - P*lightUniforms.lights[0].position.w).xyz;

  gl_Position = frameUniforms.mvpMatrix * vec4((orientationMatrix * (scale * pos) + pos1).xyz,1.0);
}
)foo";

const std::string  OpenGLBoundingBoxCylinderShader::_fragmentShaderSource =
OpenGLVersionStringLiteral +
OpenGLFrameUniformBlockStringLiteral +
OpenGLStructureUniformBlockStringLiteral +
R"foo(
// Input from vertex shader
in VS_OUT
{
  smooth vec3 N;
  smooth vec3 L;
  smooth vec4 diffuse;
} fs_in;

out  vec4 vFragColor;

void main(void)
{
  // Normalize the incoming N and L vectors
  vec3 N = normalize(fs_in.N);
  vec3 L = normalize(fs_in.L);

  vec4 color = max(dot(N, L), 0.0) * fs_in.diffuse;
  vFragColor = vec4(vec3(0.0,0.75,1.0) * color.xyz, 1.0);
}
)foo";


