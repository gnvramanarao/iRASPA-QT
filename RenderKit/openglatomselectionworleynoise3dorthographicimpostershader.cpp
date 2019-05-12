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
  for(int i=0;i<_renderStructures.size();i++)
  {
    glDeleteBuffers(_renderStructures[i].size(), _vertexBuffer[i].data());
    glDeleteBuffers(_renderStructures[i].size(), _indexBuffer[i].data());
    glDeleteVertexArrays(_renderStructures[i].size(), _vertexArrayObject[i].data());
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

  for(int i=0;i<_renderStructures.size();i++)
  {
    glGenBuffers(_renderStructures[i].size(), _vertexBuffer[i].data());
    glGenBuffers(_renderStructures[i].size(), _indexBuffer[i].data());
    glGenVertexArrays(_renderStructures[i].size(), _vertexArrayObject[i].data());
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
  for(int i=0;i<_renderStructures.size();i++)
  {
    for(int j=0;j<_renderStructures[i].size();j++)
    {
      if(_renderStructures[i][j]->renderSelectionStyle() == RKSelectionStyle::WorleyNoise3D && _renderStructures[i][j]->drawAtoms() && _renderStructures[i][j]->isVisible())
      {
        glBindBufferRange(GL_UNIFORM_BUFFER, 1, structureUniformBuffer, GLintptr(index * sizeof(RKStructureUniforms)), GLsizeiptr(sizeof(RKStructureUniforms)));
        glBindVertexArray(_vertexArrayObject[i][j]);
        check_gl_error();

        glDrawElementsInstanced(GL_TRIANGLE_STRIP, _numberOfIndices[i][j], GL_UNSIGNED_SHORT, nullptr,(GLsizei)_atomSelectionShader._numberOfDrawnAtoms[i][j]);
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
  vec4 scale = structureUniforms.selectionScaling * structureUniforms.atomScaleFactor * instanceScale;
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

vec3 mod289(vec3 x)
{
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec3 permute(vec3 x)
{
  return mod289(((x*34.0)+1.0)*x);
}


vec2 cellular3D(vec3 P, float jitter)
{
#define K 0.142857142857 // 1/7
#define Ko 0.428571428571 // 1/2-K/2
#define K2 0.020408163265306 // 1/(7*7)
#define Kz 0.166666666667 // 1/6
#define Kzo 0.416666666667 // 1/2-1/6*2
//#define jitter 1.0 // smaller jitter gives more regular pattern

  vec3 Pi = mod(floor(P), 289.0);
  vec3 Pf = fract(P) - 0.5;

  vec3 Pfx = Pf.x + vec3(1.0, 0.0, -1.0);
  vec3 Pfy = Pf.y + vec3(1.0, 0.0, -1.0);
  vec3 Pfz = Pf.z + vec3(1.0, 0.0, -1.0);

  vec3 p = permute(Pi.x + vec3(-1.0, 0.0, 1.0));
  vec3 p1 = permute(p + Pi.y - 1.0);
  vec3 p2 = permute(p + Pi.y);
  vec3 p3 = permute(p + Pi.y + 1.0);

  vec3 p11 = permute(p1 + Pi.z - 1.0);
  vec3 p12 = permute(p1 + Pi.z);
  vec3 p13 = permute(p1 + Pi.z + 1.0);

  vec3 p21 = permute(p2 + Pi.z - 1.0);
  vec3 p22 = permute(p2 + Pi.z);
  vec3 p23 = permute(p2 + Pi.z + 1.0);

  vec3 p31 = permute(p3 + Pi.z - 1.0);
  vec3 p32 = permute(p3 + Pi.z);
  vec3 p33 = permute(p3 + Pi.z + 1.0);

  vec3 ox11 = fract(p11*K) - Ko;
  vec3 oy11 = mod(floor(p11*K), 7.0)*K - Ko;
  vec3 oz11 = floor(p11*K2)*Kz - Kzo; // p11 < 289 guaranteed

  vec3 ox12 = fract(p12*K) - Ko;
  vec3 oy12 = mod(floor(p12*K), 7.0)*K - Ko;
  vec3 oz12 = floor(p12*K2)*Kz - Kzo;

  vec3 ox13 = fract(p13*K) - Ko;
  vec3 oy13 = mod(floor(p13*K), 7.0)*K - Ko;
  vec3 oz13 = floor(p13*K2)*Kz - Kzo;

  vec3 ox21 = fract(p21*K) - Ko;
  vec3 oy21 = mod(floor(p21*K), 7.0)*K - Ko;
  vec3 oz21 = floor(p21*K2)*Kz - Kzo;

  vec3 ox22 = fract(p22*K) - Ko;
  vec3 oy22 = mod(floor(p22*K), 7.0)*K - Ko;
  vec3 oz22 = floor(p22*K2)*Kz - Kzo;

  vec3 ox23 = fract(p23*K) - Ko;
  vec3 oy23 = mod(floor(p23*K), 7.0)*K - Ko;
  vec3 oz23 = floor(p23*K2)*Kz - Kzo;

  vec3 ox31 = fract(p31*K) - Ko;
  vec3 oy31 = mod(floor(p31*K), 7.0)*K - Ko;
  vec3 oz31 = floor(p31*K2)*Kz - Kzo;

  vec3 ox32 = fract(p32*K) - Ko;
  vec3 oy32 = mod(floor(p32*K), 7.0)*K - Ko;
  vec3 oz32 = floor(p32*K2)*Kz - Kzo;

  vec3 ox33 = fract(p33*K) - Ko;
  vec3 oy33 = mod(floor(p33*K), 7.0)*K - Ko;
  vec3 oz33 = floor(p33*K2)*Kz - Kzo;

  vec3 dx11 = Pfx + jitter*ox11;
  vec3 dy11 = Pfy.x + jitter*oy11;
  vec3 dz11 = Pfz.x + jitter*oz11;

  vec3 dx12 = Pfx + jitter*ox12;
  vec3 dy12 = Pfy.x + jitter*oy12;
  vec3 dz12 = Pfz.y + jitter*oz12;

  vec3 dx13 = Pfx + jitter*ox13;
  vec3 dy13 = Pfy.x + jitter*oy13;
  vec3 dz13 = Pfz.z + jitter*oz13;

  vec3 dx21 = Pfx + jitter*ox21;
  vec3 dy21 = Pfy.y + jitter*oy21;
  vec3 dz21 = Pfz.x + jitter*oz21;

  vec3 dx22 = Pfx + jitter*ox22;
  vec3 dy22 = Pfy.y + jitter*oy22;
  vec3 dz22 = Pfz.y + jitter*oz22;

  vec3 dx23 = Pfx + jitter*ox23;
  vec3 dy23 = Pfy.y + jitter*oy23;
  vec3 dz23 = Pfz.z + jitter*oz23;

  vec3 dx31 = Pfx + jitter*ox31;
  vec3 dy31 = Pfy.z + jitter*oy31;
  vec3 dz31 = Pfz.x + jitter*oz31;

  vec3 dx32 = Pfx + jitter*ox32;
  vec3 dy32 = Pfy.z + jitter*oy32;
  vec3 dz32 = Pfz.y + jitter*oz32;

  vec3 dx33 = Pfx + jitter*ox33;
  vec3 dy33 = Pfy.z + jitter*oy33;
  vec3 dz33 = Pfz.z + jitter*oz33;

  vec3 d11 = dx11 * dx11 + dy11 * dy11 + dz11 * dz11;
  vec3 d12 = dx12 * dx12 + dy12 * dy12 + dz12 * dz12;
  vec3 d13 = dx13 * dx13 + dy13 * dy13 + dz13 * dz13;
  vec3 d21 = dx21 * dx21 + dy21 * dy21 + dz21 * dz21;
  vec3 d22 = dx22 * dx22 + dy22 * dy22 + dz22 * dz22;
  vec3 d23 = dx23 * dx23 + dy23 * dy23 + dz23 * dz23;
  vec3 d31 = dx31 * dx31 + dy31 * dy31 + dz31 * dz31;
  vec3 d32 = dx32 * dx32 + dy32 * dy32 + dz32 * dz32;
  vec3 d33 = dx33 * dx33 + dy33 * dy33 + dz33 * dz33;

  // Sort out the two smallest distances (F1, F2)
#if 0
  // Cheat and sort out only F1
  vec3 d1 = min(min(d11,d12), d13);
  vec3 d2 = min(min(d21,d22), d23);
  vec3 d3 = min(min(d31,d32), d33);
  vec3 d = min(min(d1,d2), d3);
  d.x = min(min(d.x,d.y),d.z);
  return sqrt(d.xx); // F1 duplicated, no F2 computed
#else
  // Do it right and sort out both F1 and F2
  vec3 d1a = min(d11, d12);
  d12 = max(d11, d12);
  d11 = min(d1a, d13); // Smallest now not in d12 or d13
  d13 = max(d1a, d13);
  d12 = min(d12, d13); // 2nd smallest now not in d13
  vec3 d2a = min(d21, d22);
  d22 = max(d21, d22);
  d21 = min(d2a, d23); // Smallest now not in d22 or d23
  d23 = max(d2a, d23);
  d22 = min(d22, d23); // 2nd smallest now not in d23
  vec3 d3a = min(d31, d32);
  d32 = max(d31, d32);
  d31 = min(d3a, d33); // Smallest now not in d32 or d33
  d33 = max(d3a, d33);
  d32 = min(d32, d33); // 2nd smallest now not in d33
  vec3 da = min(d11, d21);
  d21 = max(d11, d21);
  d11 = min(da, d31); // Smallest now in d11
  d31 = max(da, d31); // 2nd smallest now not in d31
  d11.xy = (d11.x < d11.y) ? d11.xy : d11.yx;
  d11.xz = (d11.x < d11.z) ? d11.xz : d11.zx; // d11.x now smallest
  d12 = min(d12, d21); // 2nd smallest now not in d21
  d12 = min(d12, d22); // nor in d22
  d12 = min(d12, d31); // nor in d31
  d12 = min(d12, d32); // nor in d32
  d11.yz = min(d11.yz,d12.xy); // nor in d12.yz
  d11.y = min(d11.y,d12.z); // Only two more to go
  d11.y = min(d11.y,d11.z); // Done! (Phew!)
  return sqrt(d11.xy); // F1, F2
#endif
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
  hsv.x = hsv.x * structureUniforms.changeHueSaturationValue.x;
  hsv.y = hsv.y * structureUniforms.changeHueSaturationValue.y;
  hsv.z = hsv.z * structureUniforms.changeHueSaturationValue.z;
  float bloomLevel = frameUniforms.bloomLevel * structureUniforms.atomHDRBloomLevel;
  vFragColor = vec4(hsv2rgb(hsv)*bloomLevel,bloomLevel);

}
)foo";

