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

#include "openglambientocclusionshadowmapshader.h"
#include "glgeterror.h"
#include "rkrenderuniforms.h"
#include <vector>
#include <utility>
#include <iterator>
#include "quadgeometry.h"
#include <mathkit.h>
#include <QImage>
#include "openglatomshader.h"
#include "openglatomorthographicimpostershader.h"

OpenGLAmbientOcclusionShadowMapShader::OpenGLAmbientOcclusionShadowMapShader(OpenGLAtomShader &atomShader, OpenGLAtomOrthographicImposterShader &atomOrthographicImposterShader): _atomShader(atomShader), _atomOrthographicImposterShader(atomOrthographicImposterShader)
{

}

OpenGLAmbientOcclusionShadowMapShader::~OpenGLAmbientOcclusionShadowMapShader()
{
  _cache.clear();
}

void OpenGLAmbientOcclusionShadowMapShader::setRenderStructures(std::vector<std::vector<std::shared_ptr<RKRenderStructure>>> structures)
{
   deleteBuffers();
   _renderStructures = structures;
   generateBuffers();
}

//QCache<RKRenderStructure*, std::vector<uint16_t>> OpenGLAmbientOcclusionShadowMapShader::_cache;

void OpenGLAmbientOcclusionShadowMapShader::invalidateCachedAmbientOcclusionTexture(std::vector<std::shared_ptr<RKRenderStructure>> structures)
{
  for(std::shared_ptr<RKRenderStructure> structure : structures)
  {
    _cache.remove(structure.get());
  }
}


void OpenGLAmbientOcclusionShadowMapShader::reloadData(std::shared_ptr<RKRenderDataSource> dataSource)
{
  std::cout << "RELOAD DATA OpenGLAmbientOcclusionShadowMapShader" << std::endl;
  initializeVertexArrayObject();

  adjustAmbientOcclusionTextureSize();
  updateAmbientOcclusionTextures(dataSource, RKRenderQuality::low);
}

void OpenGLAmbientOcclusionShadowMapShader::deleteBuffers()
{
  for(size_t i=0;i<_renderStructures.size();i++)
  {
    glDeleteTextures(_renderStructures[i].size(), _generatedAmbientOcclusionTexture[i].data());
    glDeleteFramebuffers(_renderStructures[i].size(), _ambientOcclusionFrameBufferObject[i].data());
    glDeleteVertexArrays(_renderStructures[i].size(), _atomShadowMapVertexArrayObject[i].data());
    glDeleteVertexArrays(_renderStructures[i].size(), _atomAmbientOcclusionMapVertexArrayObject[i].data());
  }
}

void OpenGLAmbientOcclusionShadowMapShader::generateBuffers()
{

  _generatedAmbientOcclusionTexture.resize(_renderStructures.size());
  _ambientOcclusionFrameBufferObject.resize(_renderStructures.size());
  _atomShadowMapVertexArrayObject.resize(_renderStructures.size());
  _atomAmbientOcclusionMapVertexArrayObject.resize(_renderStructures.size());

  for(size_t i=0;i<_renderStructures.size();i++)
  {
    _generatedAmbientOcclusionTexture[i].resize(_renderStructures[i].size());
    _ambientOcclusionFrameBufferObject[i].resize(_renderStructures[i].size());
    _atomShadowMapVertexArrayObject[i].resize(_renderStructures[i].size());
    _atomAmbientOcclusionMapVertexArrayObject[i].resize(_renderStructures[i].size());
  }

  for(size_t i=0;i<_renderStructures.size();i++)
  {
    glGenTextures(_renderStructures[i].size(), _generatedAmbientOcclusionTexture[i].data());
    glGenFramebuffers(_renderStructures[i].size(), _ambientOcclusionFrameBufferObject[i].data());
    glGenVertexArrays(_renderStructures[i].size(), _atomShadowMapVertexArrayObject[i].data());
    glGenVertexArrays(_renderStructures[i].size(), _atomAmbientOcclusionMapVertexArrayObject[i].data());
  }
}

void OpenGLAmbientOcclusionShadowMapShader::initializeVertexArrayObject()
{
 // QuadGeometry quad = QuadGeometry();

  for(size_t i=0;i<_renderStructures.size();i++)
  {
    for(size_t j=0;j<_renderStructures[i].size();j++)
    {
      glBindVertexArray(_atomShadowMapVertexArrayObject[i][j]);

      glBindBuffer(GL_ARRAY_BUFFER, _atomOrthographicImposterShader._vertexBuffer[i][j]);
      //glBufferData(GL_ARRAY_BUFFER, quad.vertices().size() * sizeof(RKVertex), quad.vertices().data(), GL_DYNAMIC_DRAW);
      check_gl_error();

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _atomOrthographicImposterShader._indexBuffer[i][j]);
     // glBufferData(GL_ELEMENT_ARRAY_BUFFER, quad.indices().size() * sizeof(GLshort), quad.indices().data(), GL_DYNAMIC_DRAW);
      check_gl_error();
      glVertexAttribPointer(_atomShadowMapVertexPositionAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKVertex),reinterpret_cast<GLvoid*>(offsetof(RKVertex,position)));

      glBindBuffer(GL_ARRAY_BUFFER, _atomShader._instancePositionBuffer[i][j]);
     // glBufferData(GL_ARRAY_BUFFER, _numberOfDrawnAtoms[i][j]*sizeof(RKInPerInstanceAttributesAtoms), atomData.data(), GL_DYNAMIC_DRAW);
      check_gl_error();
      glVertexAttribPointer(_atomShadowMapInstancePositionAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKInPerInstanceAttributesAtoms),reinterpret_cast<GLvoid*>(offsetof(RKInPerInstanceAttributesAtoms,position)));
      glVertexAttribDivisor(_atomShadowMapInstancePositionAttributeLocation, 1);

      glVertexAttribPointer(_atomShadowMapScaleAttributeLocation, 1, GL_FLOAT, GL_FALSE, sizeof(RKInPerInstanceAttributesAtoms), reinterpret_cast<GLvoid*>(offsetof(RKInPerInstanceAttributesAtoms,scale)));

      glVertexAttribDivisor(_atomShadowMapScaleAttributeLocation, 1);

      glEnableVertexAttribArray(_atomShadowMapVertexPositionAttributeLocation);
      glEnableVertexAttribArray(_atomShadowMapInstancePositionAttributeLocation);
      glEnableVertexAttribArray(_atomShadowMapScaleAttributeLocation);

      glBindBuffer(GLenum(GL_ARRAY_BUFFER), 0);
      glBindVertexArray(0);

      glBindVertexArray(_atomAmbientOcclusionMapVertexArrayObject[i][j]);



      // each crystal can have a different number of quad-slices
      glBindBuffer(GL_ARRAY_BUFFER, _atomOrthographicImposterShader._vertexBuffer[i][j]);
      check_gl_error();



      glVertexAttribPointer(_atomAmbientOcclusionMapPositionAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKVertex), reinterpret_cast<GLvoid*>(offsetof(RKVertex,position)));
      check_gl_error();

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _atomOrthographicImposterShader._indexBuffer[i][j]);
      check_gl_error();


      glBindBuffer(GL_ARRAY_BUFFER, _atomShader._instancePositionBuffer[i][j]);
      check_gl_error();


      glVertexAttribPointer(_atomAmbientOcclusionMapInstancePositionAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKInPerInstanceAttributesAtoms), reinterpret_cast<GLvoid*>(offsetof(RKInPerInstanceAttributesAtoms,position)));
      check_gl_error();
      glVertexAttribDivisor(_atomAmbientOcclusionMapInstancePositionAttributeLocation, 1);
      check_gl_error();

      glVertexAttribPointer(_atomAmbientOcclusionMapScaleAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKInPerInstanceAttributesAtoms), reinterpret_cast<GLvoid*>(offsetof(RKInPerInstanceAttributesAtoms,scale)));
      check_gl_error();
      glVertexAttribDivisor(_atomAmbientOcclusionMapScaleAttributeLocation, 1);
      check_gl_error();

      glEnableVertexAttribArray(_atomAmbientOcclusionMapPositionAttributeLocation);
      glEnableVertexAttribArray(_atomAmbientOcclusionMapInstancePositionAttributeLocation);
      glEnableVertexAttribArray(_atomAmbientOcclusionMapScaleAttributeLocation);
      check_gl_error();

      glBindBuffer(GLenum(GL_ARRAY_BUFFER), 0);
      glBindVertexArray(0);
    }
  }
}


void OpenGLAmbientOcclusionShadowMapShader::loadShader(void)
{
  GLuint vertexShader;
  GLuint fragmentShader;

  vertexShader=compileShaderOfType(GL_VERTEX_SHADER,OpenGLAmbientOcclusionShadowMapShader::_vertexAmbientOcclusionShaderSource.c_str());
  fragmentShader=compileShaderOfType(GL_FRAGMENT_SHADER,OpenGLAmbientOcclusionShadowMapShader::_fragmentAmbientOcclusionShaderSource.c_str());

  if (0 != vertexShader && 0 != fragmentShader)
  {
    _ambientOcclusionProgram = glCreateProgram();
    check_gl_error();

    glAttachShader(_ambientOcclusionProgram, vertexShader);
    check_gl_error();
    glAttachShader(_ambientOcclusionProgram, fragmentShader);
    check_gl_error();

    glBindFragDataLocation(_ambientOcclusionProgram, 0, "vFragColor");

    linkProgram(_ambientOcclusionProgram);

    _atomAmbientOcclusionMapUniformLocation   = glGetUniformLocation(_ambientOcclusionProgram, "shadowMapTexture");
    _atomAmbientOcclusionMapWeightUniformLocation   = glGetUniformLocation(_ambientOcclusionProgram, "weight");

    _atomAmbientOcclusionMapPositionAttributeLocation = glGetAttribLocation(_ambientOcclusionProgram, "vertexPosition");
    _atomAmbientOcclusionMapInstancePositionAttributeLocation = glGetAttribLocation(_ambientOcclusionProgram, "instancePosition");
    _atomAmbientOcclusionMapScaleAttributeLocation = glGetAttribLocation(_ambientOcclusionProgram, "instanceScale");


    if (_atomAmbientOcclusionMapUniformLocation < 0) qDebug() << "Shader did not contain the 'shadowMapTexture' uniform.";
    if (_atomAmbientOcclusionMapWeightUniformLocation < 0) qDebug() << "Shader did not contain the 'weight' attribute.";
    if (_atomAmbientOcclusionMapPositionAttributeLocation < 0) qDebug() << "Shader did not contain the 'vertexPosition' attribute.";
    if (_atomAmbientOcclusionMapInstancePositionAttributeLocation < 0) qDebug() << "Shader did not contain the 'instancePosition' attribute.";
    if (_atomAmbientOcclusionMapScaleAttributeLocation < 0) qDebug() << "Shader did not contain the 'instanceScale' attribute.";

    glDeleteShader(vertexShader);
    check_gl_error();
    glDeleteShader(fragmentShader);
    check_gl_error();
  }

  vertexShader=compileShaderOfType(GL_VERTEX_SHADER,OpenGLAmbientOcclusionShadowMapShader::_vertexShadowMapShaderSource.c_str());
  fragmentShader=compileShaderOfType(GL_FRAGMENT_SHADER,OpenGLAmbientOcclusionShadowMapShader::_fragmentShadowMapShaderSource.c_str());

  if (0 != vertexShader && 0 != fragmentShader)
  {
    _shadowMapProgram = glCreateProgram();
    check_gl_error();

    glAttachShader(_shadowMapProgram, vertexShader);
    check_gl_error();
    glAttachShader(_shadowMapProgram, fragmentShader);
    check_gl_error();

    glBindFragDataLocation(_shadowMapProgram, 0, "vFragColor");

    linkProgram(_shadowMapProgram);

    _atomShadowMapVertexPositionAttributeLocation   = glGetAttribLocation(_shadowMapProgram, "vertexPosition");
    _atomShadowMapInstancePositionAttributeLocation   = glGetAttribLocation(_shadowMapProgram, "instancePosition");
    _atomShadowMapScaleAttributeLocation   = glGetAttribLocation(_shadowMapProgram, "instanceScale");

    if (_atomShadowMapVertexPositionAttributeLocation < 0) qDebug() << "Shader did not contain the 'vertexPosition' attribute.";
    if (_atomShadowMapInstancePositionAttributeLocation < 0) qDebug() << "Shader did not contain the 'vertexPosition' attribute.";
    if (_atomShadowMapScaleAttributeLocation < 0) qDebug() << "Shader did not contain the 'vertexPosition' attribute.";

    glDeleteShader(vertexShader);
    check_gl_error();
    glDeleteShader(fragmentShader);
    check_gl_error();
  }
}

void OpenGLAmbientOcclusionShadowMapShader::adjustAmbientOcclusionTextureSize()
{
    GLint maxSize = 0;



      for(size_t i=0;i<_renderStructures.size();i++)
      {
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxSize);
        for(size_t j=0;j<_renderStructures[i].size();j++)
        {
          int numberOfAtoms = _renderStructures[i][j]->renderAtoms().size();

          /*
          switch(numberOfAtoms)
          {
          case 0...64:
            _renderStructures[i][j]->atomAmbientOcclusionTextureSize = std::min(256,int(maxSize));
          case 65...256:
            structure.atomAmbientOcclusionTextureSize = min(512,int(maxSize));
          case 257...1024:
            structure.atomAmbientOcclusionTextureSize = min(1024,int(maxSize));
          case 1025...65536:
            structure.atomAmbientOcclusionTextureSize = min(2048,int(maxSize))
          case 65537...524288:;
            structure.atomAmbientOcclusionTextureSize = min(4096,int(maxSize));
          default:
            structure.atomAmbientOcclusionTextureSize = min(8192,int(maxSize));
          }
          */

          _renderStructures[i][j]->setAtomAmbientOcclusionTextureSize(1024);

          _renderStructures[i][j]->setAtomAmbientOcclusionPatchNumber(int(sqrt(double(numberOfAtoms)))+1);
          _renderStructures[i][j]->setAtomAmbientOcclusionPatchSize(_renderStructures[i][j]->atomAmbientOcclusionTextureSize()/
                  _renderStructures[i][j]->atomAmbientOcclusionPatchNumber());


          glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _ambientOcclusionFrameBufferObject[i][j]);

          glDrawBuffer(GL_COLOR_ATTACHMENT0);


          glBindTexture(GL_TEXTURE_2D, _generatedAmbientOcclusionTexture[i][j]);
          glTexParameteri(GL_TEXTURE_2D, GLenum(GL_TEXTURE_MAG_FILTER), GLint(GL_LINEAR));
          glTexParameteri(GL_TEXTURE_2D, GLenum(GL_TEXTURE_MIN_FILTER), GLint(GL_LINEAR));
          glTexParameteri(GL_TEXTURE_2D, GLenum(GL_TEXTURE_WRAP_S), GLint(GL_CLAMP_TO_EDGE));
          glTexParameteri(GL_TEXTURE_2D, GLenum(GL_TEXTURE_WRAP_T), GLint(GL_CLAMP_TO_EDGE));
          glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, _renderStructures[i][j]->atomAmbientOcclusionTextureSize(), _renderStructures[i][j]->atomAmbientOcclusionTextureSize(), 0, GL_RED, GL_HALF_FLOAT, nullptr);
          glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _generatedAmbientOcclusionTexture[i][j], 0);
          glBindTexture(GL_TEXTURE_2D, 0);

          // check framebuffer completeness
          GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
          if (status != GL_FRAMEBUFFER_COMPLETE)
          {
            std::cout << "initializeAmbientOcclusionMapFrameBuffer fatal error: framebuffer incomplete" << std::endl;
          }
          glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
      }
}

void  OpenGLAmbientOcclusionShadowMapShader::updateAmbientOcclusionTextures(std::shared_ptr<RKRenderDataSource> dataSource, RKRenderQuality quality)
{
  GLuint shadowMapFrameUniformBuffer = 0;
  GLuint structureAmbientOcclusionUniformBuffer = 0;
  GLuint shadowMapFrameBufferObject = 0;
  GLuint shadowMapDepthTexture = 0;

  if(dataSource)
  {
      std::cout << "UPDATE AMBIENT OCCLUSION" << std::endl;

  glGenBuffers(1, &structureAmbientOcclusionUniformBuffer);
  glGenBuffers(1, &shadowMapFrameUniformBuffer);
  glGenFramebuffers(1, &shadowMapFrameBufferObject);
  glGenTextures(1, &shadowMapDepthTexture);

  glBindBufferBase(GL_UNIFORM_BUFFER, 1, structureAmbientOcclusionUniformBuffer);
  glUniformBlockBinding(_shadowMapProgram, glGetUniformBlockIndex(_shadowMapProgram, "StructureUniformBlock"), 1);
  glUniformBlockBinding(_ambientOcclusionProgram, glGetUniformBlockIndex(_ambientOcclusionProgram, "StructureUniformBlock"), 1);


  glBindBufferBase(GLenum(GL_UNIFORM_BUFFER), 2, shadowMapFrameUniformBuffer);
  glUniformBlockBinding(_shadowMapProgram, glGetUniformBlockIndex(_shadowMapProgram, "ShadowUniformBlock"), 2);
  glUniformBlockBinding(_ambientOcclusionProgram, glGetUniformBlockIndex(_ambientOcclusionProgram, "ShadowUniformBlock"), 2);



  // create Framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFrameBufferObject);

  // only create depth-texture
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);

  glBindTexture(GL_TEXTURE_2D, shadowMapDepthTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, 2048, 2048, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMapDepthTexture, 0);

  glBindTexture(GLenum(GL_TEXTURE_2D), 0);

  // check framebuffer completeness
  GLenum status = glCheckFramebufferStatus(GLenum(GL_FRAMEBUFFER));
  if (status != GL_FRAMEBUFFER_COMPLETE)
  {
    std::cout << "initializeShadowMapFrameBuffer fatal error: framebuffer incomplete " << status << std::endl;
  }
  glBindFramebuffer(GLenum(GL_FRAMEBUFFER), 0);

  //std::vector<std::shared_ptr<RKRenderStructure>> structures = flatten(_renderStructures);


  for(int i=0;i<_renderStructures.size();i++)
  {
    for(int j=0;j<_renderStructures[i].size();j++)
    {
      if(_renderStructures[i][j]->atomAmbientOcclusion() && _renderStructures[i][j]->isVisible())
      {
        if(_cache.contains(_renderStructures[i][j].get())) // case cached
        {
           std::vector<uint16_t>* textureData = _cache.object(_renderStructures[i][j].get());
           glBindTexture(GL_TEXTURE_2D, _generatedAmbientOcclusionTexture[i][j]);
           glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, _renderStructures[i][j]->atomAmbientOcclusionTextureSize(),
                        _renderStructures[i][j]->atomAmbientOcclusionTextureSize(), 0, GL_RED, GL_HALF_FLOAT, textureData->data());
           glBindTexture(GLenum(GL_TEXTURE_2D), 0);
           std::cout << "USING CACHE!!!" << std::endl;
        }
        else
        {
          // create vector of structure-uniforms in advance

          double4x4 modelMatrix = double4x4::AffinityMatrixToTransformationAroundArbitraryPointWithTranslation(double4x4(_renderStructures[i][j]->orientation()), _renderStructures[i][j]->cell()->boundingBox().center(), _renderStructures[i][j]->origin());

          std::vector<RKStructureUniforms> structureUniforms = std::vector<RKStructureUniforms>();
          structureUniforms.reserve(_renderStructures[i].size());

          for(size_t k=0; k < _renderStructures[i].size(); k++)
          {
            RKStructureUniforms structureUniform = RKStructureUniforms(i, int(k), _renderStructures[i][k], double4x4::inverse(modelMatrix));
            structureUniforms.push_back(structureUniform);
          }

          glBindBuffer(GL_UNIFORM_BUFFER, structureAmbientOcclusionUniformBuffer);
          glBufferData(GL_UNIFORM_BUFFER, sizeof(RKStructureUniforms) * _renderStructures[i].size(), structureUniforms.data(), GL_DYNAMIC_DRAW);
          glBindBuffer(GL_UNIFORM_BUFFER, 0);

          SKBoundingBox boundingBox = dataSource->renderBoundingBox();
          double largestRadius = boundingBox.boundingSphereRadius();
          double3 centerOfScene = boundingBox.minimum() + (boundingBox.maximum() - boundingBox.minimum()) * 0.5;
          double3 eye = double3(centerOfScene.x, centerOfScene.y, centerOfScene.z + largestRadius);

          double boundingBoxAspectRatio = fabs(boundingBox.maximum().x - boundingBox.minimum().x) / abs(boundingBox.maximum().y - boundingBox.minimum().y);

          double left,right,top,bottom;
          if (boundingBoxAspectRatio < 1.0)
          {
            left =  -largestRadius/boundingBoxAspectRatio;
            right = largestRadius/boundingBoxAspectRatio;
            top = largestRadius/boundingBoxAspectRatio;
            bottom = -largestRadius/boundingBoxAspectRatio;

          }
          else
          {
            left = -largestRadius;
            right = largestRadius;
            top = largestRadius;
            bottom = -largestRadius;
          }

          double near1 = 0.0;
          double far1 = 2.0 * largestRadius;

          simd_quatd *directions = simd_quatd::data360;
          double *weights = simd_quatd::weights360;

          std::vector<RKShadowUniforms> shadowMapFrameUniforms = std::vector<RKShadowUniforms>();
          shadowMapFrameUniforms.reserve(360);

          // the fixed set of directions are randomly, but deterministically, distorted to remove artifacts
          srand48(0);

          for(int k=0;k<360;k++)
          {
            simd_quatd smallChangeQ = simd_quatd::smallRandomQuaternion(0.5*10.0*M_PI/180.0);
            simd_quatd q = smallChangeQ * directions[k];

            double4x4 modelMatrix = double4x4::AffinityMatrixToTransformationAroundArbitraryPoint(double4x4(q), centerOfScene);


            double4x4 viewMatrix = RKCamera::GluLookAt(eye,centerOfScene,double3(0, 1, 0));
            double4x4 projectionMatrix = RKCamera::glFrustumfOrthographic(left, right, bottom, top, near1, far1);

            RKShadowUniforms shadowMapFrameUniform = RKShadowUniforms(projectionMatrix, viewMatrix, modelMatrix);
            shadowMapFrameUniforms.push_back(shadowMapFrameUniform);
          }

          glBindBuffer(GL_UNIFORM_BUFFER, shadowMapFrameUniformBuffer);
          glBufferData(GL_UNIFORM_BUFFER, sizeof(RKShadowUniforms) * 360, shadowMapFrameUniforms.data(), GL_DYNAMIC_DRAW);
          glBindBuffer(GL_UNIFORM_BUFFER, 0);

          glBindBufferRange(GL_UNIFORM_BUFFER, 1, structureAmbientOcclusionUniformBuffer, i * sizeof(RKStructureUniforms), sizeof(RKStructureUniforms));


          // clear the ambient-occlusion texture
          int textureSize = _renderStructures[i][j]->atomAmbientOcclusionTextureSize();
          glViewport(0,0,textureSize,textureSize);

          glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _ambientOcclusionFrameBufferObject[i][j]);


          GLuint clearColor[4] = {0, 0, 0, 0};
          glClearBufferuiv(GL_COLOR, 0, clearColor);

          glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);

          for(int k=0;k<360;k++)
          {
            glEnable(GL_DEPTH_TEST);
            glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

            // create shadow map for direction d_k
            glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFrameBufferObject);

            glClear(GL_DEPTH_BUFFER_BIT);

            glViewport(0,0,2048,2048);

            glUseProgram(_shadowMapProgram);


            for(int l=0;l<_renderStructures[i].size();l++)
            {
              if (_renderStructures[i][l]->isVisible() && _atomShader._numberOfDrawnAtoms[i][l] > 0)
              {
                glBindVertexArray(_atomShadowMapVertexArrayObject[i][l]);

                glBindBufferRange(GL_UNIFORM_BUFFER, 2, shadowMapFrameUniformBuffer, k * sizeof(RKShadowUniforms), sizeof(RKShadowUniforms));

                glBindBufferRange(GL_UNIFORM_BUFFER, 1, structureAmbientOcclusionUniformBuffer, l * sizeof(RKStructureUniforms), sizeof(RKStructureUniforms));

                glDrawElementsInstanced(GL_TRIANGLE_STRIP, GLsizei(4),GLenum(GL_UNSIGNED_SHORT), nullptr, _atomShader._numberOfDrawnAtoms[i][l]);

                glBindVertexArray(0);
              }
            }

            glUseProgram(0);
            glBindFramebuffer(GLenum(GL_FRAMEBUFFER),0);

            glViewport(0,0,textureSize,textureSize);


            glBindFramebuffer(GL_FRAMEBUFFER, _ambientOcclusionFrameBufferObject[i][j]);
            glDrawBuffer(GL_COLOR_ATTACHMENT0);

            glDisable(GL_DEPTH_TEST);
            glEnable(GL_BLEND);
            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_ONE, GL_ONE);
            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

            glUseProgram(_ambientOcclusionProgram);


            if(_renderStructures[i][j]->isVisible() && _atomShader._numberOfDrawnAtoms[i][j] > 0)
            {
              glBindVertexArray(_atomAmbientOcclusionMapVertexArrayObject[i][j]);

              glBindBufferRange(GL_UNIFORM_BUFFER, 2, shadowMapFrameUniformBuffer, k * sizeof(RKShadowUniforms), sizeof(RKShadowUniforms));

              glBindBufferRange(GL_UNIFORM_BUFFER, 1, structureAmbientOcclusionUniformBuffer, j * sizeof(RKStructureUniforms), sizeof(RKStructureUniforms));


              glActiveTexture(GL_TEXTURE0);
              glBindTexture(GL_TEXTURE_2D, shadowMapDepthTexture);
              glUniform1i(_atomAmbientOcclusionMapUniformLocation, 0);
              glUniform1f(_atomAmbientOcclusionMapWeightUniformLocation, float(4.0*weights[k]/360.0));

              glDrawElementsInstanced(GL_TRIANGLE_STRIP, 4,GL_UNSIGNED_SHORT, nullptr, _atomShader._numberOfDrawnAtoms[i][j]);

              glBindVertexArray(0);
            }
            glBindTexture(GLenum(GL_TEXTURE_2D), 0);

            glUseProgram(0);

            glDisable(GLenum(GL_BLEND));
            glEnable(GLenum(GL_DEPTH_TEST));


          }


          glBindTexture(GL_TEXTURE_2D, _generatedAmbientOcclusionTexture[i][j]);
          std::vector<uint16_t>* textureData = new std::vector<uint16_t>(textureSize * textureSize);
          glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_HALF_FLOAT, textureData->data());
          glBindTexture(GL_TEXTURE_2D, 0);
          _cache.insert(_renderStructures[i][j].get(), textureData);

          //std::cout << "Draw with filled vector: " << _renderStructures[i][j]->atomAmbientOcclusion() << std::endl;
          //std::cout << "vector size: " << _renderStructures[i][j]->atomAmbientOcclusionTextureSize() << std::endl;

          //glTexImage2D(GLenum(GL_TEXTURE_2D), 0, GLint(GL_R16F), _renderStructures[i][j]->atomAmbientOcclusionTextureSize(),
          //             _renderStructures[i][j]->atomAmbientOcclusionTextureSize(), 0, GL_RED, GL_HALF_FLOAT, emptyData.data());
          //glTexImage2D(GLenum(GL_TEXTURE_2D), 0, GLint(GL_R32F), _renderStructures[i][j]->atomAmbientOcclusionTextureSize(),
          //             _renderStructures[i][j]->atomAmbientOcclusionTextureSize(), 0, GL_RED, GL_FLOAT, emptyData.data());

         // glTexImage2D(GLenum(GL_TEXTURE_2D), 0, GLint(GL_R16F), _renderStructures[i][j]->atomAmbientOcclusionTextureSize(),
         //              _renderStructures[i][j]->atomAmbientOcclusionTextureSize(), 0, GL_RED, GL_HALF_FLOAT, emptyData.data());

/*
          let dataLength: Int = textureSize * textureSize * 2

          glBindTexture(GLenum(GL_TEXTURE_2D), generatedAmbientOcclusionTexture[i][j])

          // storing in NSCache
          var ambientOcclusionTexture: [CUnsignedChar] = [CUnsignedChar](repeating: 0, count: dataLength)
          glGetTexImage(GLenum(GL_TEXTURE_2D), 0, GLenum(GL_RED), GLenum(GL_HALF_FLOAT), &ambientOcclusionTexture)

          let ambientOcclusionTextureData: Data = Data(bytes: &ambientOcclusionTexture, count: dataLength)
          //self.cachedAmbientOcclusionTextures[structure] = ambientOcclusionTextureData
          self.cachedAmbientOcclusionTextures.setObject(ambientOcclusionTextureData as AnyObject, forKey: structure)
*/
          //glBindTexture(GL_TEXTURE_2D, 0);

        }
      }
    }
  }

  glDeleteTextures(1, &shadowMapDepthTexture);
  glDeleteFramebuffers(1, &shadowMapFrameBufferObject);
  glDeleteBuffers(1, &shadowMapFrameUniformBuffer);
  glDeleteBuffers(1, &structureAmbientOcclusionUniformBuffer);
  }
}

const std::string OpenGLAmbientOcclusionShadowMapShader::_vertexAmbientOcclusionShaderSource =
std::string(R"foo(
#version 330

layout (std140) uniform ShadowUniformBlock
{
  mat4 projectionMatrix;
  mat4 viewMatrix;
  mat4 shadowMatrix;
  mat4 normalMatrix;
} shadowUniforms;

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
  bool pad;
} structureUniforms;


// Inputs from vertex shader
out VS_OUT
{
  flat vec4 atomCenterPosition;
  smooth vec2 texcoords;
  flat vec4 sphere_radius;
} vs_out;

in vec4 vertexPosition;
in vec4 instancePosition;
in vec4 instanceScale;


void main()
{
  vs_out.atomCenterPosition = structureUniforms.modelMatrix * instancePosition;
  vs_out.sphere_radius = structureUniforms.atomScaleFactor * instanceScale;

  int patchNumber=structureUniforms.ambientOcclusionPatchNumber;
  float patchSize=structureUniforms.ambientOcclusionPatchSize;
  int iid = gl_InstanceID;
  float k1=iid%patchNumber;
  float k2=iid/patchNumber;

  vec2 offset = vec2(patchSize,patchSize)*vec2(k1,k2)*structureUniforms.ambientOcclusionInverseTextureSize;

  vec2 origin = offset * 2.0 - 1.0;  // make beteen -1.0 and 1.0

  float tmp = 2.0*patchSize*structureUniforms.ambientOcclusionInverseTextureSize;

  vs_out.texcoords = vertexPosition.xy;

  vec4 pos = vec4(origin + tmp * (vertexPosition.xy*0.5+vec2(0.5)),0.0,1.0);

  gl_Position = pos;
}
)foo");


const std::string OpenGLAmbientOcclusionShadowMapShader::_fragmentAmbientOcclusionShaderSource =

std::string(R"foo(
#version 330

layout (std140) uniform ShadowUniformBlock
{
  mat4 projectionMatrix;
  mat4 viewMatrix;
  mat4 shadowMatrix;
  mat4 normalMatrix;
} shadowUniforms;

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
  bool pad;
} structureUniforms;


vec3 coordinateFromTexturePosition(vec2 texturePosition)
{
  vec2 absoluteTexturePosition = abs(texturePosition);
  float h = 1.0 - absoluteTexturePosition.s - absoluteTexturePosition.t;

  if (h >= 0.0)
  {
    return vec3(texturePosition.s, texturePosition.t, h);
  }
  else
  {
    return vec3(sign(texturePosition.s) * (1.0 - absoluteTexturePosition.t), sign(texturePosition.t) * (1.0 - absoluteTexturePosition.s), h);
  }
}

uniform sampler2DShadow shadowMapTexture;

uniform float weight;

out float vFragColor;
//flat in int instanceId;

// Inputs from vertex shader
in VS_OUT
{
  flat vec4 atomCenterPosition;
  smooth vec2 texcoords;
  flat vec4 sphere_radius;
} fs_in;




void main()
{
  float patchSize=structureUniforms.ambientOcclusionPatchSize;
  ivec2 impostorSpaceCoordinate = ivec2(gl_FragCoord.x,gl_FragCoord.y)%int(patchSize);     // ambient-Occlusion coordinate 0..pathSize-1
  vec2 newImpostorSpaceCoordinate = (2.0*vec2(impostorSpaceCoordinate)/(patchSize-1.0)-1.0);          // imposter coordinate -1.0..1.0

  vec3 imposterXYZ =  normalize(coordinateFromTexturePosition(newImpostorSpaceCoordinate));
  vec3 currentSphereSurfaceCoordinate = fs_in.sphere_radius.xyz*imposterXYZ;

  vec3 pos = currentSphereSurfaceCoordinate.xyz +  fs_in.atomCenterPosition.xyz;

  vec4 shadowCoordinate = shadowUniforms.shadowMatrix * vec4(pos,1.0);

  vec4 normal= shadowUniforms.normalMatrix * vec4(imposterXYZ.xyz,1.0);

  if(normal.z<0.0) discard;

  // Write final color to the framebuffer
  vFragColor = weight*normal.z*textureProj(shadowMapTexture, shadowCoordinate);
}
)foo");

const std::string OpenGLAmbientOcclusionShadowMapShader::_vertexShadowMapShaderSource =
std::string(R"foo(
#version 330

layout (std140) uniform ShadowUniformBlock
{
  mat4 projectionMatrix;
  mat4 viewMatrix;
  mat4 shadowMatrix;
  mat4 normalMatrix;
} shadowUniforms;

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
  bool clipAtomssAtUnitCell;

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
  bool pad;
} structureUniforms;

in vec4 vertexPosition;
in vec4 instancePosition;
in float instanceScale;

// Inputs from vertex shader
out VS_OUT
{
  flat float sphere_radius;
  smooth vec2 texcoord;
  flat vec4 eye_position;
} vs_out;

void main(void)
{
  float sphere_radius = structureUniforms.atomScaleFactor * instanceScale;
  vs_out.eye_position = shadowUniforms.viewMatrix * structureUniforms.modelMatrix * instancePosition;
  vs_out.sphere_radius = sphere_radius;

  vs_out.texcoord = vertexPosition.xy;
  vec4 pos = shadowUniforms.viewMatrix * structureUniforms.modelMatrix * instancePosition;
  pos.xy += sphere_radius * vertexPosition.xy;
  gl_Position = shadowUniforms.projectionMatrix * pos;
}
)foo");

const std::string OpenGLAmbientOcclusionShadowMapShader::_fragmentShadowMapShaderSource =
std::string(R"foo(
#version 330

layout (std140) uniform ShadowUniformBlock
{
  mat4 projectionMatrix;
  mat4 viewMatrix;
  mat4 shadowMatrix;
  mat4 normalMatrix;
} shadowUniforms;

// Inputs from vertex shader
in VS_OUT
{
  flat float sphere_radius;
  smooth vec2 texcoord;
  flat vec4 eye_position;
} fs_in;

void main(void)
{
  float x = fs_in.texcoord.x;
  float y = fs_in.texcoord.y;
  float zz = 1.0 - x*x - y*y;

  if (zz <= 0.0)
    discard;

  vec4 pos = fs_in.eye_position;
  pos = shadowUniforms.projectionMatrix * pos;
  gl_FragDepth = 0.5*(pos.z / pos.w)+0.5;
}
)foo");