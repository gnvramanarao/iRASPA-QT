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

#include "openglatomselectionworleynoise3dorthographicimpostershader.h"
#include <QDebug>
#include <iostream>
#include "quadgeometry.h"
#include "glgeterror.h"
#include "openglatomselectionworleynoise3dshader.h"

OpenGLAtomSelectionWorleyNoise3DOrthographicImposterShader::OpenGLAtomSelectionWorleyNoise3DOrthographicImposterShader(OpenGLAtomSelectionWorleyNoise3DShader &atomSelectionShader): _atomSelectionShader(atomSelectionShader)
{

}

void OpenGLAtomSelectionWorleyNoise3DOrthographicImposterShader::setRenderStructures(std::vector<std::vector<std::shared_ptr<RKRenderStructure>>> structures)
{
  deleteBuffers();
  _renderStructures = structures;
  generateBuffers();
}

void OpenGLAtomSelectionWorleyNoise3DOrthographicImposterShader::deleteBuffers()
{
  for(size_t i=0;i<_renderStructures.size();i++)
  {
    glDeleteBuffers(static_cast<GLsizei>(_renderStructures[i].size()), _vertexBuffer[i].data());
    glDeleteBuffers(static_cast<GLsizei>(_renderStructures[i].size()), _indexBuffer[i].data());
    glDeleteVertexArrays(static_cast<GLsizei>(_renderStructures[i].size()), _vertexArrayObject[i].data());
  }
}

void OpenGLAtomSelectionWorleyNoise3DOrthographicImposterShader::generateBuffers()
{
  _numberOfIndices.resize(_renderStructures.size());

  _vertexBuffer.resize(_renderStructures.size());
  _indexBuffer.resize(_renderStructures.size());
  _vertexArrayObject.resize(_renderStructures.size());


  for(size_t i=0l;i<_renderStructures.size();i++)
  {
    _numberOfIndices[i].resize(_renderStructures[i].size());

    _vertexBuffer[i].resize(_renderStructures[i].size());
    _indexBuffer[i].resize(_renderStructures[i].size());
    _vertexArrayObject[i].resize(_renderStructures[i].size());
  }

  for(size_t i=0;i<_renderStructures.size();i++)
  {
    glGenBuffers(static_cast<GLsizei>(_renderStructures[i].size()), _vertexBuffer[i].data());
    glGenBuffers(static_cast<GLsizei>(_renderStructures[i].size()), _indexBuffer[i].data());
    glGenVertexArrays(static_cast<GLsizei>(_renderStructures[i].size()), _vertexArrayObject[i].data());
  }
}

void OpenGLAtomSelectionWorleyNoise3DOrthographicImposterShader::paintGL(GLuint structureUniformBuffer)
{
  glDepthMask(GL_FALSE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glUseProgram(_program);
  check_gl_error();

  int index = 0;
  for(size_t i=0;i<_renderStructures.size();i++)
  {
    for(size_t j=0;j<_renderStructures[i].size();j++)
    {
      if(_renderStructures[i][j]->atomSelectionStyle() == RKSelectionStyle::WorleyNoise3D && _renderStructures[i][j]->drawAtoms() && _renderStructures[i][j]->isVisible() && _numberOfIndices[i][j]>0 && _atomSelectionShader._numberOfDrawnAtoms[i][j]>0)
      {
        glBindBufferRange(GL_UNIFORM_BUFFER, 1, structureUniformBuffer, GLintptr(index * sizeof(RKStructureUniforms)), GLsizeiptr(sizeof(RKStructureUniforms)));
        glBindVertexArray(_vertexArrayObject[i][j]);
        check_gl_error();

        glDrawElementsInstanced(GL_TRIANGLE_STRIP, static_cast<GLsizei>(_numberOfIndices[i][j]), GL_UNSIGNED_SHORT, nullptr, static_cast<GLsizei>(_atomSelectionShader._numberOfDrawnAtoms[i][j]));
        check_gl_error();
        glBindVertexArray(0);
      }
      index++;
    }
  }
  glUseProgram(0);

  glDisable(GL_BLEND);
  glDepthMask(GL_TRUE);
}

void OpenGLAtomSelectionWorleyNoise3DOrthographicImposterShader::reloadData()
{
  initializeVertexArrayObject();
}

void OpenGLAtomSelectionWorleyNoise3DOrthographicImposterShader::initializeVertexArrayObject()
{
  QuadGeometry quad = QuadGeometry();

  for(size_t i=0;i<_renderStructures.size();i++)
  {
    for(size_t j=0;j<_renderStructures[i].size();j++)
    {
      glBindVertexArray(_vertexArrayObject[i][j]);
      check_gl_error();

      _numberOfIndices[i][j] = quad.indices().size();

      // each crystal can have a different number of quad-slices
      glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer[i][j]);
      check_gl_error();

      if(quad.vertices().size()>0)
      {
        glBufferData(GL_ARRAY_BUFFER, quad.vertices().size() * sizeof(RKVertex), quad.vertices().data(), GL_DYNAMIC_DRAW);
        check_gl_error();
      }

      glVertexAttribPointer(_vertexPositionAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKVertex), reinterpret_cast<GLvoid*>(offsetof(RKVertex,position)));
      check_gl_error();

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer[i][j]);
      check_gl_error();

      if(quad.indices().size()>0)
      {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, quad.indices().size() * sizeof(GLshort), quad.indices().data(), GL_DYNAMIC_DRAW);
        check_gl_error();
      }

      glBindBuffer(GL_ARRAY_BUFFER, _atomSelectionShader._instancePositionBuffer[i][j]);
      check_gl_error();

      glVertexAttribPointer(_instancePositionAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKInPerInstanceAttributesAtoms), reinterpret_cast<GLvoid*>(offsetof(RKInPerInstanceAttributesAtoms,position)));
      check_gl_error();
      glVertexAttribDivisor(_instancePositionAttributeLocation, 1);
      check_gl_error();

      glVertexAttribPointer(_ambientColorAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKInPerInstanceAttributesAtoms), reinterpret_cast<GLvoid*>(offsetof(RKInPerInstanceAttributesAtoms,ambient)));
      check_gl_error();
      glVertexAttribDivisor(_ambientColorAttributeLocation, 1);
      check_gl_error();

      glVertexAttribPointer(_diffuseColorAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKInPerInstanceAttributesAtoms), reinterpret_cast<GLvoid*>(offsetof(RKInPerInstanceAttributesAtoms,diffuse)));
      check_gl_error();
      glVertexAttribDivisor(_diffuseColorAttributeLocation, 1);
      check_gl_error();

      glVertexAttribPointer(_specularColorAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKInPerInstanceAttributesAtoms), reinterpret_cast<GLvoid*>(offsetof(RKInPerInstanceAttributesAtoms,specular)));
      check_gl_error();
      glVertexAttribDivisor(_specularColorAttributeLocation, 1);
      check_gl_error();

      glVertexAttribPointer(_scaleAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKInPerInstanceAttributesAtoms), reinterpret_cast<GLvoid*>(offsetof(RKInPerInstanceAttributesAtoms,scale)));
      check_gl_error();
      glVertexAttribDivisor(_scaleAttributeLocation, 1);
      check_gl_error();

      glEnableVertexAttribArray(_vertexPositionAttributeLocation);
      glEnableVertexAttribArray(_instancePositionAttributeLocation);
      glEnableVertexAttribArray(_scaleAttributeLocation);
      glEnableVertexAttribArray(_ambientColorAttributeLocation);
      glEnableVertexAttribArray(_diffuseColorAttributeLocation);
      glEnableVertexAttribArray(_specularColorAttributeLocation);
      check_gl_error();

      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindVertexArray(0);
    }
  }
}

void OpenGLAtomSelectionWorleyNoise3DOrthographicImposterShader::loadShader(void)
{
  GLuint vertexShader;
  GLuint fragmentShader;

  vertexShader=compileShaderOfType(GL_VERTEX_SHADER,OpenGLAtomSelectionWorleyNoise3DOrthographicImposterShader::_vertexShaderSource.c_str());
  fragmentShader=compileShaderOfType(GL_FRAGMENT_SHADER,OpenGLAtomSelectionWorleyNoise3DOrthographicImposterShader::_fragmentShaderSource.c_str());

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

    _vertexPositionAttributeLocation = glGetAttribLocation(_program, "vertexPosition");
    _instancePositionAttributeLocation = glGetAttribLocation(_program, "instancePosition");
    _ambientColorAttributeLocation = glGetAttribLocation(_program, "instanceAmbientColor");
    _diffuseColorAttributeLocation = glGetAttribLocation(_program, "instanceDiffuseColor");
    _specularColorAttributeLocation = glGetAttribLocation(_program, "instanceSpecularColor");
    _scaleAttributeLocation = glGetAttribLocation(_program, "instanceScale");

    if (_vertexPositionAttributeLocation < 0) qDebug() << "Shader did not contain the 'vertexPosition' attribute.";
    if (_instancePositionAttributeLocation < 0) qDebug() << "Shader did not contain the 'instancePosition' attribute.";
    if (_ambientColorAttributeLocation < 0) qDebug() << "Shader did not contain the 'instanceAmbientColor' attribute.";
    if (_diffuseColorAttributeLocation  < 0) qDebug() << "Shader did not contain the 'instanceDiffuseColor' attribute.";
    if (_specularColorAttributeLocation  < 0) qDebug() << "Shader did not contain the 'instanceSpecularColor' attribute.";
    if (_scaleAttributeLocation  < 0) qDebug() << "Shader did not contain the 'instanceScale' attribute.";


    glDeleteShader(vertexShader);
    check_gl_error();
    glDeleteShader(fragmentShader);
    check_gl_error();
  }
}


const std::string  OpenGLAtomSelectionWorleyNoise3DOrthographicImposterShader::_vertexShaderSource =
OpenGLVersionStringLiteral +
OpenGLFrameUniformBlockStringLiteral +
OpenGLStructureUniformBlockStringLiteral +
OpenGLLightUniformBlockStringLiteral +
R"foo(
// Inputs from vertex shader
out VS_OUT
{
  smooth vec4 eye_position;
  smooth vec2 texcoords;
  flat vec4 ambient;
  flat vec4 diffuse;
  flat vec4 specular;
  smooth vec3 frag_pos;
  flat vec3 frag_center;
  smooth vec3 N;
  smooth vec3 L;
  smooth vec3 V;
  flat vec4 sphere_radius;
  flat float k1;
  flat float k2;
  flat mat4x4 ambientOcclusionTransformMatrix;
} vs_out;

in vec4 vertexPosition;

in vec4 instancePosition;
in vec4 instanceScale;
in vec4 instanceAmbientColor;
in vec4 instanceDiffuseColor;
in vec4 instanceSpecularColor;

void main(void)
{
  vec4 scale = structureUniforms.atomSelectionScaling * structureUniforms.atomScaleFactor * instanceScale;
  vs_out.ambient=lightUniforms.lights[0].ambient * structureUniforms.atomAmbientColor * instanceAmbientColor;
  vs_out.diffuse=lightUniforms.lights[0].diffuse * structureUniforms.atomDiffuseColor * instanceDiffuseColor;
  vs_out.specular=lightUniforms.lights[0].specular * structureUniforms.atomSpecularColor * instanceSpecularColor;

  vs_out.N = vec3(0,0,1);

  vs_out.eye_position = frameUniforms.viewMatrix * structureUniforms.modelMatrix * instancePosition;
  vs_out.ambientOcclusionTransformMatrix = transpose(frameUniforms.normalMatrix * structureUniforms.modelMatrix);

  // Calculate light vector
  vs_out.L = (lightUniforms.lights[0].position - vs_out.eye_position*lightUniforms.lights[0].position.w).xyz;

  // Calculate view vector
  vs_out.V = -vs_out.eye_position.xyz;

  vs_out.texcoords = vertexPosition.xy;
  vs_out.sphere_radius = scale;
  vec4 pos2 = frameUniforms.viewMatrix * structureUniforms.modelMatrix * instancePosition;
  pos2.xy += scale.xy * vec2(vertexPosition.x,vertexPosition.y);

  vs_out.frag_pos = pos2.xyz;

  int iid = gl_InstanceID;
  int patchNumber=structureUniforms.ambientOcclusionPatchNumber;
  vs_out.k1=iid%patchNumber;
  vs_out.k2=iid/patchNumber;

  gl_Position =  frameUniforms.projectionMatrix * pos2;

}
)foo";

const std::string  OpenGLAtomSelectionWorleyNoise3DOrthographicImposterShader::_fragmentShaderSource =
OpenGLVersionStringLiteral +
OpenGLFrameUniformBlockStringLiteral +
OpenGLStructureUniformBlockStringLiteral +
OpenGLLightUniformBlockStringLiteral +
OpenGLRGBHSVStringLiteral+
OpenGLWorleyNoise3DStringLiteral +
R"foo(
// Inputs from vertex shader
in VS_OUT
{
  smooth vec4 eye_position;
  smooth vec2 texcoords;
  flat vec4 ambient;
  flat vec4 diffuse;
  flat vec4 specular;
  smooth vec3 frag_pos;
  flat vec3 frag_center;
  smooth vec3 N;
  smooth vec3 L;
  smooth vec3 V;
  flat vec4 sphere_radius;
  flat float k1;
  flat float k2;
  flat mat4x4 ambientOcclusionTransformMatrix;
} fs_in;


vec2 textureCoordinateForSphereSurfacePosition(vec3 sphereSurfacePosition)
{
  vec3 absoluteSphereSurfacePosition = abs(sphereSurfacePosition);
  float d = absoluteSphereSurfacePosition.x + absoluteSphereSurfacePosition.y + absoluteSphereSurfacePosition.z;

  return (sphereSurfacePosition.z > 0.0) ? sphereSurfacePosition.xy / d : sign(sphereSurfacePosition.xy) * ( 1.0 - absoluteSphereSurfacePosition.yx / d);
}

out vec4 vFragColor;

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
  pos = frameUniforms.projectionMatrix *   pos;
  gl_FragDepth = 0.5*(pos.z / pos.w)+0.5;

  // Normalize the incoming N, L and V vectors
  vec3 N = vec3(x,y,z);
  vec3 L = normalize(fs_in.L);
  vec3 V = normalize(fs_in.V);

  // Calculate R locally
  vec3 R = reflect(-L, N);

  // Compute the diffuse and specular components for each fragment
  vec4 ambient = fs_in.ambient;
  vec4 diffuse = max(dot(N, L), 0.0) * fs_in.diffuse;
  vec4 specular = pow(max(dot(R, V), 0.0), lightUniforms.lights[0].shininess + structureUniforms.atomShininess) * fs_in.specular;

  vec3 t1 = (fs_in.ambientOcclusionTransformMatrix * vec4(N,1.0)).xyz;

  float frequency = structureUniforms.atomSelectionWorleyNoise3DFrequency;
  float jitter = structureUniforms.atomSelectionWorleyNoise3DJitter;
  vec2 F = cellular3D(frequency*t1, jitter);
  float n = F.y-F.x;

  vec4 color= n * (ambient + diffuse + specular);


  if (structureUniforms.atomHDR)
  {
    vec4 vLdrColor = 1.0 - exp2(-color * structureUniforms.atomHDRExposure);
    vLdrColor.a = 1.0;
    color= vLdrColor;
  }

  vec3 hsv = rgb2hsv(color.xyz);
  hsv.x = hsv.x * structureUniforms.atomHue;
  hsv.y = hsv.y * structureUniforms.atomSaturation;
  hsv.z = hsv.z * structureUniforms.atomValue;
  float bloomLevel = frameUniforms.bloomLevel * structureUniforms.atomSelectionIntensity;
  vFragColor = vec4(hsv2rgb(hsv)*bloomLevel,bloomLevel);

}
)foo";

