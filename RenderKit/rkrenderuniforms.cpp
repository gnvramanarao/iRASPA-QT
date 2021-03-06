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

#include "rkrenderuniforms.h"
#include "mathkit.h"

RKTransformationUniforms::RKTransformationUniforms(double4x4 projectionMatrix, double4x4 viewMatrix, double bloomLevel, double bloomPulse, int multiSampling)
{
    double4x4 mvpMatrix = projectionMatrix * viewMatrix;
    this->projectionMatrix = float4x4(projectionMatrix);
    this->viewMatrix = float4x4(viewMatrix);
    this->mvpMatrix = float4x4(mvpMatrix);
    this->shadowMatrix = float4x4(mvpMatrix);

    this->projectionMatrixInverse = float4x4(double4x4::inverse(projectionMatrix));
    this->viewMatrixInverse = float4x4(double4x4::inverse(viewMatrix));

    double3x3 normalMatrix = double3x3::transpose(double3x3::inverse(double3x3(viewMatrix)));
    this->normalMatrix = float4x4(double4x4(normalMatrix));
    this->bloomLevel = float(bloomLevel);
    this->bloomPulse = float(bloomPulse);
    this->numberOfMultiSamplePoints = float(multiSampling);
};


RKStructureUniforms::RKStructureUniforms(int sceneIdentifier, int movieIdentifier, std::shared_ptr<RKRenderStructure> structure)
{
  SKBoundingBox boundingbox = structure->cell()->boundingBox();
  double3 centerOfRotation = boundingbox.center();

  double4x4 modelMatrix = double4x4::AffinityMatrixToTransformationAroundArbitraryPointWithTranslation(double4x4(structure->orientation()),centerOfRotation, structure->origin());

  this->sceneIdentifier = int32_t(sceneIdentifier);
  this->MovieIdentifier = int32_t(movieIdentifier);

  this->colorAtomsWithBondColor = structure->colorAtomsWithBondColor();
  this->atomScaleFactor = float(structure->atomScaleFactor());
  this->modelMatrix = float4x4(modelMatrix);
  this->atomHue = structure->atomHue();
  this->atomSaturation = structure->atomSaturation();
  this->atomValue = structure->atomValue();

  this->ambientOcclusion = structure->atomAmbientOcclusion() ? GL_TRUE : GL_FALSE;
  this->ambientOcclusionPatchNumber = int32_t(structure->atomAmbientOcclusionPatchNumber());
  this->ambientOcclusionPatchSize = float(structure->atomAmbientOcclusionPatchSize());
  this->ambientOcclusionInverseTextureSize = float(1.0/double(structure->atomAmbientOcclusionTextureSize()));


  this->atomAmbient = float(structure->atomAmbientIntensity()) * float4(structure->atomAmbientColor().redF(),structure->atomAmbientColor().greenF(),
                                                                        structure->atomAmbientColor().blueF(),structure->atomAmbientColor().alphaF());
  this->atomDiffuse = float(structure->atomDiffuseIntensity()) * float4(structure->atomDiffuseColor().redF(),structure->atomDiffuseColor().greenF(),
                                                                        structure->atomDiffuseColor().blueF(),structure->atomDiffuseColor().alphaF());
  this->atomSpecular = float(structure->atomSpecularIntensity()) * float4(structure->atomSpecularColor().redF(),structure->atomSpecularColor().greenF(),
                                                                          structure->atomSpecularColor().blueF(),structure->atomSpecularColor().alphaF());
  this->atomShininess = float(structure->atomShininess());


  this->bondScaling = float(structure->bondScaleFactor());
  this->bondColorMode = int32_t(structure->bondColorMode());

  this->unitCellScaling =  float(structure->unitCellScaleFactor());
  this->unitCellDiffuseColor = float(structure->unitCellDiffuseIntensity()) * float4(structure->unitCellDiffuseColor().redF(),structure->unitCellDiffuseColor().greenF(),
                                                                                     structure->unitCellDiffuseColor().blueF(),structure->unitCellDiffuseColor().alphaF());

  this->atomHDR = structure->atomHDR() ? GL_TRUE : GL_FALSE;
  this->atomHDRExposure = float(structure->atomHDRExposure());
  this->atomSelectionIntensity = float(structure->atomSelectionIntensity());
  this->clipAtomsAtUnitCell = structure->clipAtomsAtUnitCell() ? GL_TRUE : GL_FALSE;

  this->bondHDR = structure->bondHDR() ? GL_TRUE : GL_FALSE;
  this->bondHDRExposure = float(structure->bondHDRExposure());
  this->bondSelectionIntensity = float(structure->bondSelectionIntensity());
  this->clipBondsAtUnitCell = structure->clipBondsAtUnitCell() ? GL_TRUE : GL_FALSE;

  this->bondHue = float(structure->bondHue());
  this->bondSaturation = float(structure->bondSaturation());
  this->bondValue = float(structure->bondValue());

  this->bondAmbientColor = float(structure->bondAmbientIntensity()) * float4(structure->bondAmbientColor().redF(),structure->bondAmbientColor().greenF(),
                                                                             structure->bondAmbientColor().blueF(),structure->bondAmbientColor().alphaF());
  this->bondDiffuseColor = float(structure->bondDiffuseIntensity()) * float4(structure->bondDiffuseColor().redF(),structure->bondDiffuseColor().greenF(),
                                                                             structure->bondDiffuseColor().blueF(),structure->bondDiffuseColor().alphaF());
  this->bondSpecularColor = float(structure->bondSpecularIntensity()) * float4(structure->bondSpecularColor().redF(),structure->bondSpecularColor().greenF(),
                                                                               structure->bondSpecularColor().blueF(),structure->bondSpecularColor().alphaF());
  this->bondShininess = float(structure->bondShininess());

  this->bondSelectionStripesDensity = float(structure->bondSelectionStripesDensity());
  this->bondSelectionStripesFrequency = float(structure->bondSelectionStripesFrequency());
  this->bondSelectionWorleyNoise3DFrequency = float(structure->bondSelectionWorleyNoise3DFrequency());
  this->bondSelectionWorleyNoise3DJitter = float(structure->bondSelectionWorleyNoise3DJitter());
  this->bondSelectionIntensity = float(structure->bondSelectionIntensity());
  this->bondSelectionScaling = float(std::max(1.001,structure->bondSelectionScaling())); // avoid artifacts



  double3x3 unitCell = structure->cell()->unitCell();
  double3x3 box = structure->cell()->box();
  double3 corner = unitCell * double3(double(structure->cell()->minimumReplicas().x), double(structure->cell()->minimumReplicas().y), double(structure->cell()->minimumReplicas().z));
  double3 corner2 = unitCell * double3(double(structure->cell()->maximumReplicas().x)+1.0, double(structure->cell()->maximumReplicas().y)+1.0, double(structure->cell()->maximumReplicas().z)+1.0);

  this->boxMatrix = float4x4(box);
  double3 shift = structure->cell()->unitCell() * double3(structure->cell()->minimumReplicas());
  this->boxMatrix[3][0] = float(shift.x);
  this->boxMatrix[3][1] = float(shift.y);
  this->boxMatrix[3][2] = float(shift.z);


  this->atomSelectionScaling = float(std::max(1.005,structure->atomSelectionScaling())); // avoid artifacts
  this->atomSelectionStripesDensity = float(structure->atomSelectionStripesDensity());
  this->atomSelectionStripesFrequency = float(structure->atomSelectionStripesFrequency());
  this->atomSelectionWorleyNoise3DFrequency = float(structure->atomSelectionWorleyNoise3DFrequency());
  this->atomSelectionWorleyNoise3DJitter = float(structure->atomSelectionWorleyNoise3DJitter());

  this->atomAnnotationTextColor = float4(structure->renderTextColor().redF(),structure->renderTextColor().greenF(),
                                         structure->renderTextColor().blueF(),structure->renderTextColor().alphaF());
  this->atomAnnotationTextScaling = float(structure->renderTextScaling());
  this->atomAnnotationTextDisplacement = float4(float(structure->renderTextOffset().x),
                                               float(structure->renderTextOffset().y),
                                               float(structure->renderTextOffset().z),
                                               0.0);

  // clipping planes are in object space
  double3 u_plane0 = double3::normalize(double3::cross(box[0],box[1]));
  double3 u_plane1 = double3::normalize(double3::cross(box[2],box[0]));
  double3 u_plane2 = double3::normalize(double3::cross(box[1],box[2]));
  this->clipPlaneBack = float4(u_plane0.x, u_plane0.y, u_plane0.z, -double3::dot(u_plane0,corner));
  this->clipPlaneBottom = float4(u_plane1.x, u_plane1.y, u_plane1.z, -double3::dot(u_plane1,corner));
  this->clipPlaneLeft = float4(u_plane2.x, u_plane2.y, u_plane2.z, -double3::dot(u_plane2,corner));
  this->clipPlaneFront = float4(-u_plane0.x, -u_plane0.y, -u_plane0.z, double3::dot(u_plane0,corner2));
  this->clipPlaneTop = float4(-u_plane1.x, -u_plane1.y, -u_plane1.z, double3::dot(u_plane1,corner2));
  this->clipPlaneRight = float4(-u_plane2.x, -u_plane2.y, -u_plane2.z, double3::dot(u_plane2,corner2));

  if(RKRenderPrimitiveObjectsSource *source = dynamic_cast<RKRenderPrimitiveObjectsSource*>(structure.get()))
  {
    float4x4 primitiveModelMatrix = float4x4(double4x4(source->primitiveOrientation()));
    float4x4 primitiveNormalMatrix = float4x4(double3x3(source->primitiveOrientation()).inverse().transpose());  // tranpose

    this->transformationMatrix = primitiveModelMatrix * float4x4(source->primitiveTransformationMatrix());
    this->transformationNormalMatrix = primitiveNormalMatrix * float4x4(source->primitiveTransformationMatrix().inverse().transpose()); // tranpose

    this->primitiveOpacity = source->primitiveOpacity();

    this->primitiveFrontSideHDR = source->primitiveFrontSideHDR();
    this->primitiveFrontSideHDRExposure = float(source->primitiveFrontSideHDRExposure());
    this->primitiveAmbientFrontSide = float(source->primitiveFrontSideAmbientIntensity()) * float4(source->primitiveFrontSideAmbientColor(), source->primitiveOpacity());
    this->primitiveDiffuseFrontSide = float(source->primitiveFrontSideDiffuseIntensity()) * float4(source->primitiveFrontSideDiffuseColor(), source->primitiveOpacity());
    this->primitiveSpecularFrontSide = float(source->primitiveFrontSideSpecularIntensity()) * float4(source->primitiveFrontSideSpecularColor(), source->primitiveOpacity());
    this->primitiveShininessFrontSide = float(source->primitiveFrontSideShininess());

    this->primitiveBackSideHDR = source->primitiveBackSideHDR();
    this->primitiveBackSideHDRExposure = float(source->primitiveBackSideHDRExposure());
    this->primitiveAmbientBackSide = float(source->primitiveBackSideAmbientIntensity()) * float4(source->primitiveBackSideAmbientColor(), source->primitiveOpacity());
    this->primitiveDiffuseBackSide = float(source->primitiveBackSideDiffuseIntensity()) * float4(source->primitiveBackSideDiffuseColor(), source->primitiveOpacity());
    this->primitiveSpecularBackSide = float(source->primitiveBackSideSpecularIntensity()) * float4(source->primitiveBackSideSpecularColor(), source->primitiveOpacity());
    this->primitiveShininessBackSide = float(source->primitiveBackSideShininess());
  }
}

RKStructureUniforms::RKStructureUniforms(int sceneIdentifier, int movieIdentifier, std::shared_ptr<RKRenderStructure> structure, double4x4 inverseModelMatrix)
{
    SKBoundingBox boundingbox = structure->cell()->boundingBox();
    double3 centerOfRotation = boundingbox.center();

    // difference
    double4x4 modelMatrix2 = inverseModelMatrix * double4x4::AffinityMatrixToTransformationAroundArbitraryPointWithTranslation(double4x4(structure->orientation()),centerOfRotation, structure->origin());
    this->modelMatrix = float4x4(modelMatrix2);

    this->sceneIdentifier = int32_t(sceneIdentifier);
    this->MovieIdentifier = int32_t(movieIdentifier);

    this->colorAtomsWithBondColor = structure->colorAtomsWithBondColor();
    this->atomScaleFactor = float(structure->atomScaleFactor());
    this->modelMatrix = float4x4(modelMatrix);
    this->atomHue = structure->atomHue();
    this->atomSaturation = structure->atomSaturation();
    this->atomValue = structure->atomValue();

    this->ambientOcclusion = structure->atomAmbientOcclusion() ? GL_TRUE: GL_FALSE;
    this->ambientOcclusionPatchNumber = int32_t(structure->atomAmbientOcclusionPatchNumber());
    this->ambientOcclusionPatchSize = float(structure->atomAmbientOcclusionPatchSize());
    this->ambientOcclusionInverseTextureSize = float(1.0/double(structure->atomAmbientOcclusionTextureSize()));

    this->atomAmbient = float(structure->atomAmbientIntensity()) * float4(structure->atomAmbientColor().redF(),structure->atomAmbientColor().greenF(),
                                                                          structure->atomAmbientColor().blueF(),structure->atomAmbientColor().alphaF());
    this->atomDiffuse = float(structure->atomDiffuseIntensity()) * float4(structure->atomDiffuseColor().redF(),structure->atomDiffuseColor().greenF(),
                                                                          structure->atomDiffuseColor().blueF(),structure->atomDiffuseColor().alphaF());
    this->atomSpecular = float(structure->atomSpecularIntensity()) * float4(structure->atomSpecularColor().redF(),structure->atomSpecularColor().greenF(),
                                                                            structure->atomSpecularColor().blueF(),structure->atomSpecularColor().alphaF());
    this->atomShininess = float(structure->atomShininess());


    this->bondScaling = float(structure->bondScaleFactor());
    this->bondColorMode = int32_t(structure->bondColorMode());

    this->unitCellScaling =  float(structure->unitCellScaleFactor());
    this->unitCellDiffuseColor = float(structure->unitCellDiffuseIntensity()) * float4(structure->unitCellDiffuseColor().redF(),structure->unitCellDiffuseColor().greenF(),
                                                                                       structure->unitCellDiffuseColor().blueF(),structure->unitCellDiffuseColor().alphaF());

    this->atomHDR = structure->atomHDR() ? GL_TRUE : GL_FALSE;
    this->atomHDRExposure = float(structure->atomHDRExposure());
    this->atomSelectionIntensity = float(structure->atomSelectionIntensity());
    this->clipAtomsAtUnitCell = structure->clipAtomsAtUnitCell() ? GL_TRUE : GL_FALSE;

    this->bondHDR = structure->bondHDR() ? GL_TRUE : GL_FALSE;
    this->bondHDRExposure = float(structure->bondHDRExposure());
    this->clipBondsAtUnitCell = structure->clipBondsAtUnitCell() ? GL_TRUE : GL_FALSE;

    this->bondHue = float(structure->bondHue());
    this->bondSaturation = float(structure->bondSaturation());
    this->bondValue = float(structure->bondValue());

    this->bondAmbientColor = float(structure->bondAmbientIntensity()) * float4(structure->bondAmbientColor().redF(),structure->bondAmbientColor().greenF(),
                                                                               structure->bondAmbientColor().blueF(),structure->bondAmbientColor().alphaF());
    this->bondDiffuseColor = float(structure->bondDiffuseIntensity()) * float4(structure->bondDiffuseColor().redF(),structure->bondDiffuseColor().greenF(),
                                                                               structure->bondDiffuseColor().blueF(),structure->bondDiffuseColor().alphaF());
    this->bondSpecularColor = float(structure->bondSpecularIntensity()) * float4(structure->bondSpecularColor().redF(),structure->bondSpecularColor().greenF(),
                                                                                 structure->bondSpecularColor().blueF(),structure->bondSpecularColor().alphaF());
    this->bondShininess = float(structure->bondShininess());

    double3x3 unitCell = structure->cell()->unitCell();
    double3x3 box = structure->cell()->box();
    double3 corner = unitCell * double3(double(structure->cell()->minimumReplicas().x), double(structure->cell()->minimumReplicas().y), double(structure->cell()->minimumReplicas().z));
    double3 corner2 = unitCell * double3(double(structure->cell()->maximumReplicas().x)+1.0, double(structure->cell()->maximumReplicas().y)+1.0, double(structure->cell()->maximumReplicas().z)+1.0);

    this->boxMatrix = float4x4(box);
    double3 shift = structure->cell()->unitCell() * double3(structure->cell()->minimumReplicas());
    this->boxMatrix[3][0] = float(shift.x);
    this->boxMatrix[3][1] = float(shift.y);
    this->boxMatrix[3][2] = float(shift.z);


    this->atomSelectionStripesDensity = float(structure->atomSelectionStripesDensity());
    this->atomSelectionStripesFrequency = float(structure->atomSelectionStripesFrequency());
    this->atomSelectionWorleyNoise3DFrequency = float(structure->atomSelectionWorleyNoise3DFrequency());
    this->atomSelectionWorleyNoise3DJitter = float(structure->atomSelectionWorleyNoise3DJitter());
    this->atomSelectionScaling = float(std::max(1.001,structure->atomSelectionScaling())); // avoid artifacts
    this->atomSelectionIntensity = float(structure->atomSelectionIntensity());

    this->atomAnnotationTextColor = float4(structure->renderTextColor().redF(),structure->renderTextColor().greenF(),
                                           structure->renderTextColor().blueF(),structure->renderTextColor().alphaF());
    this->atomAnnotationTextScaling = float(structure->renderTextScaling());
    this->atomAnnotationTextDisplacement = float4(float(structure->renderTextOffset().x),
                                                 float(structure->renderTextOffset().y),
                                                 float(structure->renderTextOffset().z),
                                                 0.0);

    this->bondSelectionStripesDensity = float(structure->atomSelectionStripesDensity());
    this->bondSelectionStripesFrequency = float(structure->atomSelectionStripesFrequency());
    this->bondSelectionWorleyNoise3DFrequency = float(structure->atomSelectionWorleyNoise3DFrequency());
    this->bondSelectionWorleyNoise3DJitter = float(structure->atomSelectionWorleyNoise3DJitter());
    this->bondSelectionScaling = float(std::max(1.001,structure->atomSelectionScaling())); // avoid artifacts
    this->bondSelectionIntensity = float(structure->bondSelectionIntensity());


    // clipping planes are in object space
    double3 u_plane0 = double3::normalize(double3::cross(box[0],box[1]));
    double3 u_plane1 = double3::normalize(double3::cross(box[2],box[0]));
    double3 u_plane2 = double3::normalize(double3::cross(box[1],box[2]));
    this->clipPlaneBack = float4(u_plane0.x, u_plane0.y, u_plane0.z, -double3::dot(u_plane0,corner));
    this->clipPlaneBottom = float4(u_plane1.x, u_plane1.y, u_plane1.z, -double3::dot(u_plane1,corner));
    this->clipPlaneLeft = float4(u_plane2.x, u_plane2.y, u_plane2.z, -double3::dot(u_plane2,corner));
    this->clipPlaneFront = float4(-u_plane0.x, -u_plane0.y, -u_plane0.z, double3::dot(u_plane0,corner2));
    this->clipPlaneTop = float4(-u_plane1.x, -u_plane1.y, -u_plane1.z, double3::dot(u_plane1,corner2));
    this->clipPlaneRight = float4(-u_plane2.x, -u_plane2.y, -u_plane2.z, double3::dot(u_plane2,corner2));
}


RKShadowUniforms::RKShadowUniforms(double4x4 projectionMatrix, double4x4 viewMatrix, double4x4 modelMatrix)
{
  double4x4 ViewToOpenGLDepthTextureMatrix = double4x4(double4(0.5, 0.0, 0.0, 0.0), double4(0.0, 0.5, 0.0, 0.0), double4(0.0, 0.0, 0.5, 0.0), double4(0.5, 0.5, 0.5, 1.0));

  double4x4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;
  this->projectionMatrix = float4x4(projectionMatrix);
  this->viewMatrix = float4x4(viewMatrix * modelMatrix);
  this->shadowMatrix = float4x4(ViewToOpenGLDepthTextureMatrix * mvpMatrix);

  this->normalMatrix = float4x4(double4x4(double3x3::transpose(double3x3::inverse(double3x3(viewMatrix * modelMatrix)))));
}

RKIsosurfaceUniforms::RKIsosurfaceUniforms()
{

}

RKIsosurfaceUniforms::RKIsosurfaceUniforms(std::shared_ptr<RKRenderStructure> structure)
{
  double3x3 unitCellMatrix = structure->cell()->unitCell();
  this->unitCellMatrix = float4x4(unitCellMatrix);
  this->unitCellNormalMatrix = float4x4(double3x3::transpose(double3x3::inverse(unitCellMatrix)));


  this->frontHDR = structure->adsorptionSurfaceFrontSideHDR() ? 1 : 0;
  this->frontHDRExposure = float(structure->adsorptionSurfaceFrontSideHDRExposure());
  this->ambientBackSide = float(structure->adsorptionSurfaceBackSideAmbientIntensity()) * float4(structure->adsorptionSurfaceBackSideAmbientColor().redF(),
                                                                                                 structure->adsorptionSurfaceBackSideAmbientColor().greenF(),
                                                                                                 structure->adsorptionSurfaceBackSideAmbientColor().blueF(),
                                                                                                 structure->adsorptionSurfaceOpacity());
  this->diffuseBackSide = float(structure->adsorptionSurfaceBackSideDiffuseIntensity()) * float4(structure->adsorptionSurfaceBackSideDiffuseColor().redF(),
                                                                                                 structure->adsorptionSurfaceBackSideDiffuseColor().greenF(),
                                                                                                 structure->adsorptionSurfaceBackSideDiffuseColor().blueF(),
                                                                                                 structure->adsorptionSurfaceOpacity());
  this->specularBackSide = float(structure->adsorptionSurfaceBackSideSpecularIntensity()) * float4(structure->adsorptionSurfaceBackSideSpecularColor().redF(),
                                                                                                   structure->adsorptionSurfaceBackSideSpecularColor().greenF(),
                                                                                                   structure->adsorptionSurfaceBackSideSpecularColor().blueF(),
                                                                                                   structure->adsorptionSurfaceOpacity());
  this->shininessBackSide = float(structure->adsorptionSurfaceBackSideShininess());

  this->backHDR = structure->adsorptionSurfaceBackSideHDR() ? 1 : 0;
  this->backHDRExposure = float(structure->adsorptionSurfaceBackSideHDRExposure());
  this->ambientFrontSide = float(structure->adsorptionSurfaceFrontSideAmbientIntensity()) * float4(structure->adsorptionSurfaceFrontSideAmbientColor().redF(),
                                                                                                   structure->adsorptionSurfaceFrontSideAmbientColor().greenF(),
                                                                                                   structure->adsorptionSurfaceFrontSideAmbientColor().blueF(),
                                                                                                   structure->adsorptionSurfaceOpacity());
  this->diffuseFrontSide = float(structure->adsorptionSurfaceFrontSideDiffuseIntensity()) * float4(structure->adsorptionSurfaceFrontSideDiffuseColor().redF(),
                                                                                                   structure->adsorptionSurfaceFrontSideDiffuseColor().greenF(),
                                                                                                   structure->adsorptionSurfaceFrontSideDiffuseColor().blueF(),
                                                                                                   structure->adsorptionSurfaceOpacity());
  this->specularFrontSide = float(structure->adsorptionSurfaceFrontSideSpecularIntensity()) * float4(structure->adsorptionSurfaceFrontSideSpecularColor().redF(),
                                                                                                     structure->adsorptionSurfaceFrontSideSpecularColor().greenF(),
                                                                                                     structure->adsorptionSurfaceFrontSideSpecularColor().blueF(),
                                                                                                     structure->adsorptionSurfaceOpacity());
  this->shininessFrontSide = float(structure->adsorptionSurfaceFrontSideShininess());
};


RKLightUniform::RKLightUniform(std::shared_ptr<RKRenderDataSource> project, int lightIndex)
{
  std::vector<std::shared_ptr<RKLight>> lights = project->renderLights();
  std::shared_ptr<RKLight> light = lights[lightIndex];
  this->ambient = float(light->ambientIntensity()) * float4(light->ambientColor().redF(),light->ambientColor().greenF(),light->ambientColor().blueF(),light->ambientColor().alphaF());
  this->diffuse = float(light->diffuseIntensity()) * float4(light->diffuseColor().redF(),light->diffuseColor().greenF(),light->diffuseColor().blueF(),light->diffuseColor().alphaF());
  this->specular = float(light->specularIntensity()) * float4(light->specularColor().redF(),light->specularColor().greenF(),light->specularColor().blueF(),light->specularColor().alphaF());
  this->shininess = float(light->shininess());
};

RKLightUniform::RKLightUniform()
{

};

// Note: must be aligned at vector-length (16-bytes boundaries, 4 Floats of 4 bytes)
// current number of bytes: 512 bytes
RKLightsUniforms::RKLightsUniforms(std::shared_ptr<RKRenderDataSource> project)
{
  if(project)
  {
    std::vector<std::shared_ptr<RKLight>> lights = project->renderLights();
    std::shared_ptr<RKLight> light = lights[0];
    this->lights[0].ambient = float(light->ambientIntensity()) * float4(light->ambientColor().redF(),light->ambientColor().greenF(),light->ambientColor().blueF(),light->ambientColor().alphaF());
    this->lights[0].diffuse = float(light->diffuseIntensity()) * float4(light->diffuseColor().redF(),light->diffuseColor().greenF(),light->diffuseColor().blueF(),light->diffuseColor().alphaF());
    this->lights[0].specular = float(light->specularIntensity()) * float4(light->specularColor().redF(),light->specularColor().greenF(),light->specularColor().blueF(),light->specularColor().alphaF());
    this->lights[0].shininess = float(light->shininess());
  }
};



