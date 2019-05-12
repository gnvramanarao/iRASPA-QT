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

#pragma once

#include <mathkit.h>
#include <QtOpenGL>
#include <QColor>
#include "rkrenderkitprotocols.h"

const std::string  OpenGLVersionStringLiteral = R"foo(
#version 330
)foo";

struct RKVertex
{
  RKVertex():position(float4()),normal(float4()), st(float2()) {}
  RKVertex(float4 pos, float4 norm, float2 c): position(pos), normal(norm), st(c) {}

  float4 position;
  float4 normal;
  float2 st;
  float2 pad = float2();
};

struct RKTextVertex
{
  float4 position  = float4();
  float4 st = float4();
};

struct RKBondVertex
{
  float4 position1;
  float4 position2;
};

struct RKInPerInstanceAttributesText
{
  float4 position = float4();
  float4 scale = float4();
  float4 vertexCoordinatesData = float4();
  float4 textureCoordinatesData = float4();
};

struct RKInPerInstanceAttributesAtoms
{
  float4 position = float4();
  float4 ambient = float4();
  float4 diffuse = float4();
  float4 specular = float4();
  float4 scale = float4();
  RKInPerInstanceAttributesAtoms(float4 p, float4 a, float4 d, float4 s, float4 sc): position(p), ambient(a), diffuse(d), specular(s), scale(sc) {}
};

struct RKInPerInstanceAttributesBonds
{
  float4 position1 = float4();
  float4 position2 = float4();
  float4 color1 = float4();
  float4 color2 = float4();
  float4 scale = float4();
  RKInPerInstanceAttributesBonds() {}
  RKInPerInstanceAttributesBonds(float4 pos1, float4 pos2, float4 c1, float4 c2, float4 sc): position1(pos1), position2(pos2), color1(c1), color2(c2), scale(sc) {}
};


// Note: must be aligned at vector-length (16-bytes boundaries, 4 Floats of 4 bytes)
// current number of bytes: 512 bytes

struct RKTransformationUniforms
{
  float4x4 projectionMatrix = float4x4();
  float4x4 viewMatrix = float4x4();
  float4x4 mvpMatrix = float4x4();
  float4x4 shadowMatrix = float4x4();
  float4x4 projectionMatrixInverse = float4x4();
  float4x4 viewMatrixInverse = float4x4();
  float4x4 normalMatrix = float4x4();

  // moved 'numberOfMultiSamplePoints' to here (for downsampling when no structures are present)
  float numberOfMultiSamplePoints = 8.0;
  float intPad1;
  float intPad2;
  float intPad3;
  float bloomLevel = 1.0;
  float bloomPulse = 1.0;
  float padFloat1 = 0.0;
  float padFloat2 = 0.0;
  float4 padVector3 = float4();
  float4 padvector4 = float4();

  //RKTransformationUniforms();
  RKTransformationUniforms(double4x4 projectionMatrix, double4x4 viewMatrix, double bloomLevel, double bloomPulse, int multiSampling);
};

const std::string  OpenGLFrameUniformBlockStringLiteral = R"foo(
layout (std140) uniform FrameUniformBlock
{
  mat4 projectionMatrix;
  mat4 viewMatrix;
  mat4 mvpMatrix;
  mat4 shadowMatrix;


  mat4 projectionMatrixInverse;
  mat4 viewMatrixInverse;
  mat4 normalMatrix;
  float numberOfMultiSamplePoints;
  float padInt1;
  float padInt2;
  float padInt3;
  float bloomLevel;
  float bloomPulse;
  float padFloat1;
  float padFloat2;
  vec4 padvector3;
  vec4 padvector4;
} frameUniforms;
)foo";

// IMPORTANT: must be aligned on 256-bytes boundaries
// current number of bytes: 512 bytes
struct RKStructureUniforms
{
  int32_t sceneIdentifier = 0;
  int32_t MovieIdentifier = 0;
  float atomScaleFactor = 1.0;
  int32_t numberOfMultiSamplePoints = 8;

  int32_t ambientOcclusion = GL_FALSE;
  int32_t ambientOcclusionPatchNumber = 64;
  float ambientOcclusionPatchSize = 16.0;
  float ambientOcclusionInverseTextureSize = 1.0/1024.0;

  float4 changeHueSaturationValue = float4(1.0, 1.0, 1.0, 1.0);

  int32_t atomHDR = GL_TRUE;
  float atomHDRExposure = 1.5;
  float atomHDRBloomLevel = 0.5;
  int32_t clipAtomsAtUnitCell = GL_FALSE;

  float4 atomAmbient = float4(1.0, 1.0, 1.0, 1.0);
  float4 atomDiffuse = float4(1.0, 1.0, 1.0, 1.0);
  float4 atomSpecular = float4(1.0, 1.0, 1.0, 1.0);
  float atomShininess = 4.0;

  float bondHue = 0.0;
  float bondSaturation = 0.0;
  float bondValue = 0.0;

  //----------------------------------------  128 bytes boundary

  int32_t bondHDR = GL_TRUE;
  float bondHDRExposure = 1.5;
  float bondHDRBloomLevel = 1.0;
  int32_t clipBondsAtUnitCell = GL_FALSE;


  float4 bondAmbientColor = float4(1.0, 1.0, 1.0, 1.0);
  float4 bondDiffuseColor = float4(1.0, 1.0, 1.0, 1.0);
  float4 bondSpecularColor = float4(1.0, 1.0, 1.0, 1.0);

  float bondShininess = 4.0;
  float bondScaling = 1.0;
  int32_t bondColorMode = 0;

  float unitCellScaling = 1.0;
  float4 unitCellDiffuseColor = float4(1.0, 1.0, 1.0, 1.0);

  float4 clipPlaneLeft = float4(1.0, 1.0, 1.0, 1.0);
  float4 clipPlaneRight = float4(1.0, 1.0, 1.0, 1.0);

  //----------------------------------------  256 bytes boundary

  float4 clipPlaneTop = float4(1.0, 1.0, 1.0, 1.0);
  float4 clipPlaneBottom = float4(1.0, 1.0, 1.0, 1.0);

  float4 clipPlaneFront = float4(1.0, 1.0, 1.0, 1.0);
  float4 clipPlaneBack = float4(1.0, 1.0, 1.0, 1.0);

  float4x4 modelMatrix = float4x4(double4x4());

  //----------------------------------------  384 bytes boundary

  float4x4 boxMatrix = float4x4();
  float atomSelectionStripesDensity = 0.25;
  float atomSelectionStripesFrequency = 12.0;
  float atomSelectionWorleyNoise3DFrequency = 2.0;
  float atomSelectionWorleyNoise3DJitter = 0.0;

  float4 atomAnnotationTextDisplacement = float4();
  float4 atomAnnotationTextColor = float4(0.0,0.0,0.0,1.0);
  float atomAnnotationTextScaling = 1.0;
  float bondAnnotationTextScaling = 1.0;
  float selectionScaling = 1.25;
  int32_t colorAtomsWithBondColor = GL_FALSE;

  //----------------------------------------  512 bytes boundary

  float4x4 transformationMatrix = float4x4();
  float4x4 transformationNormalMatrix = float4x4();

  float4 primitiveAmbientFrontSide = float4(0.0,0.0,0.0,1.0);
  float4 primitiveDiffuseFrontSide = float4(0.0,0.0,0.0,1.0);
  float4 primitiveSpecularFrontSide = float4(0.0,0.0,0.0,1.0);
  int32_t primitiveFrontSideHDR = 1;
  float primitiveFrontSideHDRExposure = 1.5;
  float pad3 = 0.0;
  float primitiveShininessFrontSide = 4.0;

  float4 primitiveAmbientBackSide = float4(0.0,0.0,0.0,1.0);
  float4 primitiveDiffuseBackSide = float4(0.0,0.0,0.0,1.0);
  float4 primitiveSpecularBackSide = float4(0.0,0.0,0.0,1.0);
  int32_t primitiveBackSideHDR = 1;
  float primitiveBackSideHDRExposure = 1.5;
  float pad6 = 0.0;
  float primitiveShininessBackSide = 4.0;

  RKStructureUniforms() {}
  RKStructureUniforms(int sceneIdentifier, int movieIdentifier, std::shared_ptr<RKRenderStructure> structure);
  RKStructureUniforms(int sceneIdentifier, int movieIdentifier, std::shared_ptr<RKRenderStructure> structure, double4x4 inverseModelMatrix);
};

const std::string  OpenGLStructureUniformBlockStringLiteral = R"foo(
layout (std140) uniform StructureUniformBlock
{
    int sceneIdentifier;
    int MovieIdentifier;
    float atomScaleFactor;
    int numberOfMultiSamplePoints;

    bool ambientOcclusion;
    int ambientOcclusionPatchNumber;
    float ambientOcclusionPatchSize;
    float ambientOcclusionInverseTextureSize;

    vec4 changeHueSaturationValue;

    bool atomHDR;
    float atomHDRExposure;
    float atomHDRBloomLevel;
    bool clipAtomsAtUnitCell;

    vec4 atomAmbientColor;
    vec4 atomDiffuseColor;
    vec4 atomSpecularColor;
    float atomShininess;

    float bondHue;
    float bondSaturation;
    float bondValue;

    //----------------------------------------  128 bytes boundary

    bool bondHDR;
    float bondHDRExposure;
    float bondHDRBloomLevel;
    bool clipBondsAtUnitCell;

    vec4 bondAmbientColor;
    vec4 bondDiffuseColor;
    vec4 bondSpecularColor;

    float bondShininess;
    float bondScaling;
    int bondColorMode;
    float unitCellScaling;
    vec4 unitCellColor;

    vec4 clipPlaneLeft;
    vec4 clipPlaneRight;

    //----------------------------------------  256 bytes boundary

    vec4 clipPlaneTop;
    vec4 clipPlaneBottom;
    vec4 clipPlaneFront;
    vec4 clipPlaneBack;

    mat4 modelMatrix;

    //----------------------------------------  384 bytes boundary

    mat4 boxMatrix;
    float atomSelectionStripesDensity;
    float atomSelectionStripesFrequency;
    float atomSelectionWorleyNoise3DFrequency;
    float atomSelectionWorleyNoise3DJitter;

    vec4 atomAnnotationTextDisplacement;
    vec4 atomAnnotationTextColor;
    float atomAnnotationTextScaling;
    float bondAnnotationTextScaling;
    float selectionScaling;
    bool colorAtomsWithBondColor;

    //----------------------------------------  512 bytes boundary

    mat4 transformationMatrix;
    mat4 transformationNormalMatrix;

    vec4 primitiveAmbientFrontSide;
    vec4 primitiveDiffuseFrontSide;
    vec4 primitiveSpecularFrontSide;
    int primitiveFrontSideHDR;
    float primitiveFrontSideHDRExposure;
    float pad3;
    float primitiveShininessFrontSide;

    vec4 primitiveAmbientBackSide;
    vec4 primitiveDiffuseBackSide;
    vec4 primitiveSpecularBackSide;
    int primitiveBackSideHDR;
    float primitiveBackSideHDRExposure;
    float pad6;
    float primitiveShininessBackSide;
} structureUniforms;
)foo";

// IMPORTANT: must be aligned on 256-bytes boundaries
// current number of bytes: 256 bytes
struct RKShadowUniforms
{
  float4x4 projectionMatrix = float4x4();
  float4x4 viewMatrix = float4x4();
  float4x4 shadowMatrix = float4x4();
  float4x4 normalMatrix = float4x4();

  RKShadowUniforms();
  RKShadowUniforms(double4x4 projectionMatrix, double4x4 viewMatrix, double4x4 modelMatrix);
};

const std::string  OpenGLShadowUniformBlockStringLiteral = R"foo(
layout (std140) uniform ShadowUniformBlock
{
  mat4 projectionMatrix;
  mat4 viewMatrix;
  mat4 shadowMatrix;
  mat4 normalMatrix;
} shadowUniforms;
)foo";

struct RKIsosurfaceUniforms
{
  float4x4 unitCellMatrix = float4x4();
  float4x4 unitCellNormalMatrix = float4x4();

  float4 ambientFrontSide = float4(0.0f, 0.0f, 0.0f, 1.0f);
  float4 diffuseFrontSide = float4(0.588235f, 0.670588f, 0.729412f, 1.0f);
  float4 specularFrontSide = float4(1.0f, 1.0f, 1.0f, 1.0f);
  int32_t frontHDR = GL_TRUE;
  float frontHDRExposure = 1.5;
  float pad3 = 0.0;
  float shininessFrontSide = 4.0;

  float4 ambientBackSide = float4(0.0f, 0.0f, 0.0f, 1.0f);
  float4 diffuseBackSide = float4(0.588235f, 0.670588f, 0.729412f, 1.0f);
  float4 specularBackSide = float4(0.9f, 0.9f, 0.9f, 1.0f);
  int32_t backHDR = GL_TRUE;
  float backHDRExposure = 1.5;
  float pad6 = 0.0;
  float shininessBackSide = 4.0;

  RKIsosurfaceUniforms();
  RKIsosurfaceUniforms(std::shared_ptr<RKRenderStructure> structure);
};

const std::string  OpenGLIsosurfaceUniformBlockStringLiteral = R"foo(
layout (std140) uniform IsosurfaceUniformBlock
{
  mat4 unitCellMatrix;
  mat4 unitCellNormalMatrix;

  vec4 ambientFrontSide;
  vec4 diffuseFrontSide;
  vec4 specularFrontSide;
  bool frontHDR;
  float frontHDRExposure;
  float pad3;
  float shininessFrontSide;

  vec4 ambientBackSide;
  vec4 diffuseBackSide;
  vec4 specularBackSide;
  bool backHDR;
  float backHDRExposure;
  float pad6;
  float shininessBackSide;
} isosurfaceUniforms;
)foo";

struct RKLightUniform
{
  float4 position = float4(0.0, 0.0, 100.0, 0.0);  // w=0 directional light, w=1.0 positional light
  float4 ambient = float4(1.0, 1.0, 1.0, 1.0);
  float4 diffuse = float4(1.0, 1.0, 1.0, 1.0);
  float4 specular = float4(1.0, 1.0, 1.0, 1.0);

  float4 spotDirection = float4(1.0, 1.0, 1.0, 0.0);
  float constantAttenuation = 1.0;
  float linearAttenuation = 1.0;
  float quadraticAttenuation = 1.0;
  float spotCutoff = 1.0;

  float spotExponent = 1.0;
  float shininess = 4.0;
  float pad1 = 0.0;
  float pad2 = 0.0;

  float pad3 = 0.0;
  float pad4 = 0.0;
  float pad5 = 0.0;
  float pad6 = 0.0;

  RKLightUniform();
  RKLightUniform(std::shared_ptr<RKRenderDataSource> project, int lightIndex);
};

struct RKLightsUniforms
{
  RKLightsUniforms() {}
  RKLightsUniforms(std::shared_ptr<RKRenderDataSource> project);
  std::vector<RKLightUniform> lights{RKLightUniform(),RKLightUniform(),RKLightUniform(),RKLightUniform()};
};

const std::string  OpenGLLightUniformBlockStringLiteral = R"foo(
struct Light
{
    vec4 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;

    vec4  spotDirection;

    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
    float spotCutoff;

    float spotExponent;
    float shininess;
    float pad1;
    float pad2;
    float pad3;
    float pad4;
    float pad5;
    float pad6;
};

layout (std140) uniform LightsUniformBlock
{
    Light lights[4];
} lightUniforms;
)foo";

