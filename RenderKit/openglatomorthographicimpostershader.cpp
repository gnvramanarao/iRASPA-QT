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

#include "openglatomorthographicimpostershader.h"
#include <QDebug>
#include <iostream>
#include "glgeterror.h"
#include "quadgeometry.h"
#include "openglatomshader.h"


OpenGLAtomOrthographicImposterShader::OpenGLAtomOrthographicImposterShader(OpenGLAtomShader &atomShader): _atomShader(atomShader)
{

}



void OpenGLAtomOrthographicImposterShader::deleteBuffers()
{
  for(int i=0;i<_renderStructures.size();i++)
  {
    glDeleteBuffers(_renderStructures[i].size(), _vertexBuffer[i].data());
    glDeleteBuffers(_renderStructures[i].size(), _indexBuffer[i].data());
  //  glDeleteBuffers(_renderStructures[i].size(), _instancePositionBuffer[i].data());

    glDeleteVertexArrays(_renderStructures[i].size(), _vertexArrayObject[i].data());
  }
}

void OpenGLAtomOrthographicImposterShader::generateBuffers()
{
 // _numberOfDrawnAtoms.resize(_renderStructures.size());
  _numberOfIndices.resize(_renderStructures.size());

  _vertexBuffer.resize(_renderStructures.size());
  _indexBuffer.resize(_renderStructures.size());
//  _instancePositionBuffer.resize(_renderStructures.size());

  _vertexArrayObject.resize(_renderStructures.size());

  for(int i=0;i<_renderStructures.size();i++)
  {
 //   _numberOfDrawnAtoms[i].resize(_renderStructures[i].size());
    _numberOfIndices[i].resize(_renderStructures[i].size());

    _vertexBuffer[i].resize(_renderStructures[i].size());
    _indexBuffer[i].resize(_renderStructures[i].size());
  //  _instancePositionBuffer[i].resize(_renderStructures[i].size());

    _vertexArrayObject[i].resize(_renderStructures[i].size());
  }

  for(int i=0;i<_renderStructures.size();i++)
  {
    glGenBuffers(_renderStructures[i].size(), _vertexBuffer[i].data());
    glGenBuffers(_renderStructures[i].size(), _indexBuffer[i].data());
 //   glGenBuffers(_renderStructures[i].size(), _instancePositionBuffer[i].data());

    glGenVertexArrays(_renderStructures[i].size(), _vertexArrayObject[i].data());
  }
}

void OpenGLAtomOrthographicImposterShader::paintGL(std::vector<std::vector<GLuint>>& atomTextures, GLuint structureUniformBuffer)
{
  glUseProgram(_program);
  check_gl_error();

  int index = 0;
  for(int i=0;i<_renderStructures.size();i++)
  {
    for(int j=0;j<_renderStructures[i].size();j++)
    {
      if(_renderStructures[i][j]->drawAtoms() && _renderStructures[i][j]->isVisible())
      {
        if(_renderStructures[i][j]->clipAtomsAtUnitCell())
        {
          glEnable(GL_CLIP_DISTANCE0);
          glEnable(GL_CLIP_DISTANCE1);
          glEnable(GL_CLIP_DISTANCE2);
          glEnable(GL_CLIP_DISTANCE3);
          glEnable(GL_CLIP_DISTANCE4);
          glEnable(GL_CLIP_DISTANCE5);
        }
        else
        {
          glDisable(GL_CLIP_DISTANCE0);
          glDisable(GL_CLIP_DISTANCE1);
          glDisable(GL_CLIP_DISTANCE2);
          glDisable(GL_CLIP_DISTANCE3);
          glDisable(GL_CLIP_DISTANCE4);
          glDisable(GL_CLIP_DISTANCE5);
        }

        glBindBufferRange(GL_UNIFORM_BUFFER, 1, structureUniformBuffer, GLintptr(index * sizeof(RKStructureUniforms)), GLsizeiptr(sizeof(RKStructureUniforms)));
        glBindVertexArray(_vertexArrayObject[i][j]);
        check_gl_error();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, atomTextures[i][j]);
        glUniform1i(_ambientOcclusionTextureUniformLocation,0);



        glDrawElementsInstanced(GL_TRIANGLE_STRIP, _numberOfIndices[i][j], GL_UNSIGNED_SHORT, nullptr,(GLsizei)_atomShader._numberOfDrawnAtoms[i][j]);
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
}

void OpenGLAtomOrthographicImposterShader::reloadData()
{
  initializeVertexArrayObject();
}

void OpenGLAtomOrthographicImposterShader::initializeVertexArrayObject()
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

      glBufferData(GL_ARRAY_BUFFER, quad.vertices().size() * sizeof(RKVertex), quad.vertices().data(), GL_DYNAMIC_DRAW);
      check_gl_error();

      glVertexAttribPointer(_vertexPositionAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKVertex), reinterpret_cast<GLvoid*>(offsetof(RKVertex,position)));
      check_gl_error();

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer[i][j]);
      check_gl_error();
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, quad.indices().size() * sizeof(GLshort), quad.indices().data(), GL_DYNAMIC_DRAW);
      check_gl_error();

      glBindBuffer(GL_ARRAY_BUFFER, _atomShader._instancePositionBuffer[i][j]);
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

void OpenGLAtomOrthographicImposterShader::setRenderStructures(std::vector<std::vector<std::shared_ptr<RKRenderStructure>>> structures)
{
  deleteBuffers();
  _renderStructures = structures;
  generateBuffers();
}

void OpenGLAtomOrthographicImposterShader::loadShader(void)
{
  GLuint vertexShader;
  GLuint fragmentShader;


  vertexShader=compileShaderOfType(GL_VERTEX_SHADER,OpenGLAtomOrthographicImposterShader::_vertexShaderSource.c_str());
  fragmentShader=compileShaderOfType(GL_FRAGMENT_SHADER,OpenGLAtomOrthographicImposterShader::_fragmentShaderSource.c_str());

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

    _ambientOcclusionTextureUniformLocation   = glGetUniformLocation(_program, "ambientOcclusionTexture");
    _vertexPositionAttributeLocation = glGetAttribLocation(_program, "vertexPosition");
    _instancePositionAttributeLocation = glGetAttribLocation(_program, "instancePosition");
    _scaleAttributeLocation = glGetAttribLocation(_program, "instanceScale");
    _ambientColorAttributeLocation = glGetAttribLocation(_program, "instanceAmbientColor");
    _diffuseColorAttributeLocation = glGetAttribLocation(_program, "instanceDiffuseColor");
    _specularColorAttributeLocation = glGetAttribLocation(_program, "instanceSpecularColor");

    if (_ambientOcclusionTextureUniformLocation < 0) qDebug() << "Shader did not contain the 'ambientOcclusionTexture' attribute.";
    if (_vertexPositionAttributeLocation < 0) qDebug() << "Shader did not contain the 'vertexPosition' attribute.";
    if (_instancePositionAttributeLocation < 0) qDebug() << "Shader did not contain the 'instancePosition' attribute.";
    if (_scaleAttributeLocation < 0) qDebug() << "Shader did not contain the 'instanceScale' attribute.";
    if (_ambientColorAttributeLocation < 0) qDebug() << "Shader did not contain the 'instanceAmbientColor' attribute.";
    if (_diffuseColorAttributeLocation < 0) qDebug() << "Shader did not contain the 'instanceAmbientColor' attribute.";
    if (_specularColorAttributeLocation < 0) qDebug() << "Shader did not contain the 'instanceSpecularColor' attribute.";

    glDeleteShader(vertexShader);
    check_gl_error();
    glDeleteShader(fragmentShader);
    check_gl_error();
  }
}

const std::string  OpenGLAtomOrthographicImposterShader::_vertexShaderSource =
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
  flat vec4 instancePosition;
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
  vec4 scale = structureUniforms.atomScaleFactor * instanceScale;


  if (structureUniforms.colorAtomsWithBondColor)
  {
    vs_out.ambient=lightUniforms.lights[0].ambient * structureUniforms.bondAmbientColor;
    vs_out.diffuse=lightUniforms.lights[0].diffuse * structureUniforms.bondDiffuseColor;
    vs_out.specular=lightUniforms.lights[0].specular * structureUniforms.bondSpecularColor;
  }
  else
  {
    vs_out.ambient=lightUniforms.lights[0].ambient * structureUniforms.atomAmbientColor * instanceAmbientColor;
    vs_out.diffuse=lightUniforms.lights[0].diffuse * structureUniforms.atomDiffuseColor * instanceDiffuseColor;
    vs_out.specular=lightUniforms.lights[0].specular * structureUniforms.atomSpecularColor * instanceSpecularColor;
  }

  vs_out.N = vec3(0,0,1);
  vs_out.instancePosition = instancePosition;

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

const std::string  OpenGLAtomOrthographicImposterShader::_fragmentShaderSource =
OpenGLVersionStringLiteral +
OpenGLFrameUniformBlockStringLiteral +
OpenGLStructureUniformBlockStringLiteral +
OpenGLLightUniformBlockStringLiteral +
R"foo(
// Inputs from vertex shader
in VS_OUT
{
  smooth vec4 eye_position;
  smooth vec2 texcoords;
  flat vec4 instancePosition;
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

uniform sampler2D ambientOcclusionTexture;

out vec4 vFragColor;

vec2 textureCoordinateForSphereSurfacePosition(vec3 sphereSurfacePosition)
{
  vec3 absoluteSphereSurfacePosition = abs(sphereSurfacePosition);
  float d = absoluteSphereSurfacePosition.x + absoluteSphereSurfacePosition.y + absoluteSphereSurfacePosition.z;

  return (sphereSurfacePosition.z > 0.0) ? sphereSurfacePosition.xy / d : sign(sphereSurfacePosition.xy) * ( 1.0 - absoluteSphereSurfacePosition.yx / d);
}

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
  float x = fs_in.texcoords.x;
  float y = fs_in.texcoords.y;
  float zz = 1.0 - x*x - y*y;

  if (zz <= 0.0)
    discard;

  float z = sqrt(zz);
  vec4 pos = fs_in.eye_position;
  pos.z += fs_in.sphere_radius.z*z;

  if(structureUniforms.clipAtomsAtUnitCell)
  {
    vec3 vertexPosition = (fs_in.ambientOcclusionTransformMatrix * (fs_in.sphere_radius * vec4(x,y,z,1.0))).xyz;
    vec4 position = vec4(fs_in.instancePosition.xyz + vertexPosition.xyz,1.0);
    if (dot(structureUniforms.clipPlaneLeft,position)< 0.0) discard;
    if (dot(structureUniforms.clipPlaneRight,position)< 0.0) discard;
    if (dot(structureUniforms.clipPlaneTop,position)< 0.0) discard;
    if (dot(structureUniforms.clipPlaneBottom,position)< 0.0) discard;
    if (dot(structureUniforms.clipPlaneFront,position)< 0.0) discard;
    if (dot(structureUniforms.clipPlaneBack,position)< 0.0) discard;
  }

  pos = frameUniforms.projectionMatrix *   pos;
  gl_FragDepth = 0.5*(pos.z / pos.w)+0.5;

  // Normalize the incoming N, L and V vectors
  vec3 N = vec3(x,y,z);
  vec3 L = normalize(fs_in.L);
  vec3 V = normalize(fs_in.V);

  // Calculate R locally
  vec3 R = reflect(-L, N);

  // Compute the diffuse and specular components for each fragment
  vec3 ambient = fs_in.ambient.xyz;
  vec3 diffuse = max(dot(N, L), 0.0) * fs_in.diffuse.xyz;
  vec3 specular = pow(max(dot(R, V), 0.0), lightUniforms.lights[0].shininess + structureUniforms.atomShininess) * fs_in.specular.xyz;

  float ao = 1.0;

  if (structureUniforms.ambientOcclusion)
  {
    float patchSize=structureUniforms.ambientOcclusionPatchSize;
    vec3 t1 = (fs_in.ambientOcclusionTransformMatrix * vec4(N,1.0)).xyz;
    vec2 m2 = (vec2(patchSize*(fs_in.k1+0.5),patchSize*(fs_in.k2+0.5))+0.5*(patchSize-1.0)*(textureCoordinateForSphereSurfacePosition(t1)))*structureUniforms.ambientOcclusionInverseTextureSize;
    ao = texture(ambientOcclusionTexture,m2).r;
  }

  vec4 color= vec4(ao * (ambient.xyz + diffuse.xyz + specular.xyz), 1.0);

  if (structureUniforms.atomHDR)
  {
    vec4 vLdrColor = 1.0 - exp2(-color * structureUniforms.atomHDRExposure);
    vLdrColor.a = 1.0;
    color= vLdrColor;
  }

  vec3 hsv = rgb2hsv(color.xyz);
  hsv.x = hsv.x * structureUniforms.changeHueSaturationValue.x;
  hsv.y = hsv.y * structureUniforms.changeHueSaturationValue.y;
  hsv.z = hsv.z * structureUniforms.changeHueSaturationValue.z;
  vFragColor = vec4(hsv2rgb(hsv),1.0);
}
)foo";
