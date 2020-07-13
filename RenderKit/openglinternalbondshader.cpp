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

#include "openglinternalbondshader.h"
#include <QDebug>
#include <iostream>
#include "glgeterror.h"
#include "cylindergeometry.h"
#include <algorithm>
#include <type_traits>

OpenGLInternalBondShader::OpenGLInternalBondShader()
{

}

void OpenGLInternalBondShader::setRenderStructures(std::vector<std::vector<std::shared_ptr<RKRenderStructure>>> structures)
{
  deleteBuffers();
  _renderStructures = structures;
  generateBuffers();
}

void OpenGLInternalBondShader::paintGL(GLuint structureUniformBuffer)
{
  glUseProgram(_program);
  check_gl_error();

  int index = 0;
  for(size_t i=0;i<_renderStructures.size();i++)
  {
    for(size_t j=0;j<_renderStructures[i].size();j++)
    {
      glBindBufferRange(GL_UNIFORM_BUFFER, 1, structureUniformBuffer, GLintptr(index * sizeof(RKStructureUniforms)), GLsizeiptr(sizeof(RKStructureUniforms)));

      if (_renderStructures[i][j]->isUnity())
      {
        if(_renderStructures[i][j]->drawBonds() && _renderStructures[i][j]->isVisible() && _internalAllBondNumberOfIndices[i][j]>0 && _numberOfAllInternalBonds[i][j]>0)
        {
          glBindVertexArray(_vertexAllBondsArrayObject[i][j]);
          check_gl_error();

          glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(_internalAllBondNumberOfIndices[i][j]), GL_UNSIGNED_SHORT, nullptr, static_cast<GLsizei>(_numberOfAllInternalBonds[i][j]));

          check_gl_error();
          glBindVertexArray(0);
        }
      }
      else
      {
        if(_renderStructures[i][j]->drawBonds() && _renderStructures[i][j]->isVisible() && _internalSingleBondNumberOfIndices[i][j]>0 && _numberOfSingleInternalBonds[i][j]>0)
        {
          glBindVertexArray(_vertexSingleBondsArrayObject[i][j]);
          check_gl_error();

          glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(_internalSingleBondNumberOfIndices[i][j]), GL_UNSIGNED_SHORT, nullptr, static_cast<GLsizei>(_numberOfSingleInternalBonds[i][j]));

          check_gl_error();
          glBindVertexArray(0);
        }

        if(_renderStructures[i][j]->drawBonds() && _renderStructures[i][j]->isVisible() && _internalDoubleBondNumberOfIndices[i][j]>0 && _numberOfDoubleInternalBonds[i][j]>0)
        {
          glBindVertexArray(_vertexDoubleBondsArrayObject[i][j]);
          check_gl_error();

          glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(_internalDoubleBondNumberOfIndices[i][j]), GL_UNSIGNED_SHORT, nullptr, static_cast<GLsizei>(_numberOfDoubleInternalBonds[i][j]));

          check_gl_error();
          glBindVertexArray(0);
        }

        if(_renderStructures[i][j]->drawBonds() && _renderStructures[i][j]->isVisible() && _internalPartialDoubleBondNumberOfIndices[i][j]>0 && _numberOfPartialDoubleInternalBonds[i][j]>0)
        {
          glBindVertexArray(_vertexPartialDoubleBondsArrayObject[i][j]);
          check_gl_error();

          glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(_internalPartialDoubleBondNumberOfIndices[i][j]), GL_UNSIGNED_SHORT, nullptr, static_cast<GLsizei>(_numberOfPartialDoubleInternalBonds[i][j]));

          check_gl_error();
          glBindVertexArray(0);
        }

        if(_renderStructures[i][j]->drawBonds() && _renderStructures[i][j]->isVisible() && _internalTripleBondNumberOfIndices[i][j]>0 && _numberOfTripleInternalBonds[i][j]>0)
        {
          glBindVertexArray(_vertexTripleBondsArrayObject[i][j]);
          check_gl_error();

          glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(_internalTripleBondNumberOfIndices[i][j]), GL_UNSIGNED_SHORT, nullptr, static_cast<GLsizei>(_numberOfTripleInternalBonds[i][j]));

          check_gl_error();
          glBindVertexArray(0);
        }
      }
      index++;
    }
  }
  glUseProgram(0);
}


void OpenGLInternalBondShader::reloadData()
{
  initializeVertexArrayObject();
}

void OpenGLInternalBondShader::initializeVertexArrayObject()
{
  CappedCylinderSingleBondGeometry singleBondCylinder =  CappedCylinderSingleBondGeometry(1.0,41);
  CappedCylinderDoubleBondGeometry doubleBondCylinder =  CappedCylinderDoubleBondGeometry(1.0,41);
  CappedCylinderPartialDoubleBondGeometry partialDoubleBondCylinder =  CappedCylinderPartialDoubleBondGeometry(1.0,41);
  CappedCylinderTripleBondGeometry tripleBondCylinder =  CappedCylinderTripleBondGeometry(1.0,41);

  for(size_t i=0;i<_renderStructures.size();i++)
  {
    for(size_t j=0;j<_renderStructures[i].size();j++)
    {
      std::vector<RKInPerInstanceAttributesBonds> bondInstanceData = _renderStructures[i][j]->renderInternalBonds();
      _numberOfAllInternalBonds[i][j] = bondInstanceData.size();
      _internalAllBondNumberOfIndices[i][j] = singleBondCylinder.indices().size();

      std::vector<RKInPerInstanceAttributesBonds> singleBondInstanceData;
      int32_t singleBondType = static_cast<typename std::underlying_type<SKAsymmetricBond::SKBondType>::type>(SKAsymmetricBond::SKBondType::singleBond);
      std::copy_if(bondInstanceData.begin(), bondInstanceData.end(), std::back_inserter(singleBondInstanceData),
                   [singleBondType](RKInPerInstanceAttributesBonds &i){return i.type == singleBondType;});
      _numberOfSingleInternalBonds[i][j] = singleBondInstanceData.size();
      _internalSingleBondNumberOfIndices[i][j] = singleBondCylinder.indices().size();

      std::vector<RKInPerInstanceAttributesBonds> doubleBondInstanceData;
      int32_t doubleBondType = static_cast<typename std::underlying_type<SKAsymmetricBond::SKBondType>::type>(SKAsymmetricBond::SKBondType::doubleBond);
      std::copy_if(bondInstanceData.begin(), bondInstanceData.end(), std::back_inserter(doubleBondInstanceData),
                   [doubleBondType](RKInPerInstanceAttributesBonds &i){return i.type == doubleBondType;});
      _numberOfDoubleInternalBonds[i][j] = doubleBondInstanceData.size();
      _internalDoubleBondNumberOfIndices[i][j] = doubleBondCylinder.indices().size();

      std::vector<RKInPerInstanceAttributesBonds> partialDoubleBondInstanceData;
      int32_t partialDoubleBondType = static_cast<typename std::underlying_type<SKAsymmetricBond::SKBondType>::type>(SKAsymmetricBond::SKBondType::partialDoubleBond);
      std::copy_if(bondInstanceData.begin(), bondInstanceData.end(), std::back_inserter(partialDoubleBondInstanceData),
                   [partialDoubleBondType](RKInPerInstanceAttributesBonds &i){return i.type == partialDoubleBondType;});
      _numberOfPartialDoubleInternalBonds[i][j] = partialDoubleBondInstanceData.size();
      _internalPartialDoubleBondNumberOfIndices[i][j] = singleBondCylinder.indices().size();

      std::vector<RKInPerInstanceAttributesBonds> tripleBondInstanceData;
      int32_t tripleBondType = static_cast<typename std::underlying_type<SKAsymmetricBond::SKBondType>::type>(SKAsymmetricBond::SKBondType::tripleBond);
      std::copy_if(bondInstanceData.begin(), bondInstanceData.end(), std::back_inserter(tripleBondInstanceData),
                   [tripleBondType](RKInPerInstanceAttributesBonds &i){return i.type == tripleBondType;});
      _numberOfTripleInternalBonds[i][j] = tripleBondInstanceData.size();
      _internalTripleBondNumberOfIndices[i][j] = tripleBondCylinder.indices().size();


      glBindVertexArray(_vertexAllBondsArrayObject[i][j]);
      check_gl_error();

      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ARRAY_BUFFER, _vertexAllBondsBuffer[i][j]);
      check_gl_error();

      if(singleBondCylinder.vertices().size()>0)
      {
        glBufferData(GL_ARRAY_BUFFER, singleBondCylinder.vertices().size()*sizeof(RKVertex), singleBondCylinder.vertices().data(), GL_DYNAMIC_DRAW);
        check_gl_error();
      }

      glVertexAttribPointer(_vertexPositionAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKVertex), (GLvoid *)offsetof(RKVertex,position));
      glVertexAttribPointer(_vertexNormalAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKVertex), (GLvoid *)offsetof(RKVertex,normal));
      check_gl_error();

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexAllBondsBuffer[i][j]);
      if(singleBondCylinder.indices().size()>0)
      {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, singleBondCylinder.indices().size() * sizeof(GLshort), singleBondCylinder.indices().data(), GL_DYNAMIC_DRAW);
        check_gl_error();
      }

      glBindBuffer(GL_ARRAY_BUFFER, _vertexAllBondsInstanceBuffer[i][j]);

      if(_numberOfAllInternalBonds[i][j]>0)
      {
        glBufferData(GL_ARRAY_BUFFER, _numberOfAllInternalBonds[i][j]*sizeof(RKInPerInstanceAttributesBonds), bondInstanceData.data(), GL_DYNAMIC_DRAW);
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



      glBindVertexArray(_vertexSingleBondsArrayObject[i][j]);
      check_gl_error();


      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ARRAY_BUFFER, _vertexSingleBondsBuffer[i][j]);
      check_gl_error();

      if(singleBondCylinder.vertices().size()>0)
      {
        glBufferData(GL_ARRAY_BUFFER, singleBondCylinder.vertices().size()*sizeof(RKVertex), singleBondCylinder.vertices().data(), GL_DYNAMIC_DRAW);
        check_gl_error();
      }

      glVertexAttribPointer(_vertexPositionAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKVertex), (GLvoid *)offsetof(RKVertex,position));
      glVertexAttribPointer(_vertexNormalAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKVertex), (GLvoid *)offsetof(RKVertex,normal));
      check_gl_error();

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexSingleBondsBuffer[i][j]);
      if(singleBondCylinder.indices().size()>0)
      {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, singleBondCylinder.indices().size() * sizeof(GLshort), singleBondCylinder.indices().data(), GL_DYNAMIC_DRAW);
        check_gl_error();
      }

      glBindBuffer(GL_ARRAY_BUFFER, _vertexSingleBondsInstanceBuffer[i][j]);

      if(_numberOfSingleInternalBonds[i][j]>0)
      {
        glBufferData(GL_ARRAY_BUFFER, _numberOfSingleInternalBonds[i][j]*sizeof(RKInPerInstanceAttributesBonds), singleBondInstanceData.data(), GL_DYNAMIC_DRAW);
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


      glBindVertexArray(_vertexDoubleBondsArrayObject[i][j]);
      check_gl_error();

      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ARRAY_BUFFER, _vertexDoubleBondsBuffer[i][j]);
      check_gl_error();

      if(doubleBondCylinder.vertices().size()>0)
      {
        glBufferData(GL_ARRAY_BUFFER, doubleBondCylinder.vertices().size()*sizeof(RKVertex), doubleBondCylinder.vertices().data(), GL_DYNAMIC_DRAW);
        check_gl_error();
      }

      glVertexAttribPointer(_vertexPositionAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKVertex), (GLvoid *)offsetof(RKVertex,position));
      glVertexAttribPointer(_vertexNormalAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKVertex), (GLvoid *)offsetof(RKVertex,normal));
      check_gl_error();

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexDoubleBondsBuffer[i][j]);
      if(doubleBondCylinder.indices().size()>0)
      {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, doubleBondCylinder.indices().size() * sizeof(GLshort), doubleBondCylinder.indices().data(), GL_DYNAMIC_DRAW);
        check_gl_error();
      }

      glBindBuffer(GL_ARRAY_BUFFER, _vertexDoubleBondsInstanceBuffer[i][j]);

      if(_numberOfDoubleInternalBonds[i][j]>0)
      {
        glBufferData(GL_ARRAY_BUFFER, _numberOfDoubleInternalBonds[i][j]*sizeof(RKInPerInstanceAttributesBonds), doubleBondInstanceData.data(), GL_DYNAMIC_DRAW);
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



      glBindVertexArray(_vertexPartialDoubleBondsArrayObject[i][j]);
      check_gl_error();

      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ARRAY_BUFFER, _vertexPartialDoubleBondsBuffer[i][j]);
      check_gl_error();

      if(partialDoubleBondCylinder.vertices().size()>0)
      {
        glBufferData(GL_ARRAY_BUFFER, partialDoubleBondCylinder.vertices().size()*sizeof(RKVertex), partialDoubleBondCylinder.vertices().data(), GL_DYNAMIC_DRAW);
        check_gl_error();
      }

      glVertexAttribPointer(_vertexPositionAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKVertex), (GLvoid *)offsetof(RKVertex,position));
      glVertexAttribPointer(_vertexNormalAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKVertex), (GLvoid *)offsetof(RKVertex,normal));
      check_gl_error();

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexPartialDoubleBondsBuffer[i][j]);
      if(partialDoubleBondCylinder.indices().size()>0)
      {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, partialDoubleBondCylinder.indices().size() * sizeof(GLshort), partialDoubleBondCylinder.indices().data(), GL_DYNAMIC_DRAW);
        check_gl_error();
      }

      glBindBuffer(GL_ARRAY_BUFFER, _vertexPartialDoubleBondsInstanceBuffer[i][j]);

      if(_numberOfPartialDoubleInternalBonds[i][j]>0)
      {
        glBufferData(GL_ARRAY_BUFFER, _numberOfPartialDoubleInternalBonds[i][j]*sizeof(RKInPerInstanceAttributesBonds), partialDoubleBondInstanceData.data(), GL_DYNAMIC_DRAW);
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


      glBindVertexArray(_vertexTripleBondsArrayObject[i][j]);
      check_gl_error();

      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ARRAY_BUFFER, _vertexTripleBondsBuffer[i][j]);
      check_gl_error();

      if(tripleBondCylinder.vertices().size()>0)
      {
        glBufferData(GL_ARRAY_BUFFER, tripleBondCylinder.vertices().size()*sizeof(RKVertex), tripleBondCylinder.vertices().data(), GL_DYNAMIC_DRAW);
        check_gl_error();
      }

      glVertexAttribPointer(_vertexPositionAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKVertex), (GLvoid *)offsetof(RKVertex,position));
      glVertexAttribPointer(_vertexNormalAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(RKVertex), (GLvoid *)offsetof(RKVertex,normal));
      check_gl_error();

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexTripleBondsBuffer[i][j]);
      if(tripleBondCylinder.indices().size()>0)
      {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, tripleBondCylinder.indices().size() * sizeof(GLshort), tripleBondCylinder.indices().data(), GL_DYNAMIC_DRAW);
        check_gl_error();
      }

      glBindBuffer(GL_ARRAY_BUFFER, _vertexTripleBondsInstanceBuffer[i][j]);

      if(_numberOfTripleInternalBonds[i][j]>0)
      {
        glBufferData(GL_ARRAY_BUFFER, _numberOfTripleInternalBonds[i][j]*sizeof(RKInPerInstanceAttributesBonds), tripleBondInstanceData.data(), GL_DYNAMIC_DRAW);
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

    }
  }
}

void OpenGLInternalBondShader::deleteBuffers()
{
  for(size_t i=0;i<_renderStructures.size();i++)
  {
    glDeleteVertexArrays(static_cast<GLsizei>(_renderStructures[i].size()), _vertexAllBondsArrayObject[i].data());
    glDeleteBuffers(static_cast<GLsizei>(_renderStructures[i].size()), _vertexAllBondsInstanceBuffer[i].data());
    glDeleteBuffers(static_cast<GLsizei>(_renderStructures[i].size()), _vertexAllBondsBuffer[i].data());
    glDeleteBuffers(static_cast<GLsizei>(_renderStructures[i].size()), _indexAllBondsBuffer[i].data());

    glDeleteVertexArrays(static_cast<GLsizei>(_renderStructures[i].size()), _vertexSingleBondsArrayObject[i].data());
    glDeleteBuffers(static_cast<GLsizei>(_renderStructures[i].size()), _vertexSingleBondsInstanceBuffer[i].data());
    glDeleteBuffers(static_cast<GLsizei>(_renderStructures[i].size()), _vertexSingleBondsBuffer[i].data());
    glDeleteBuffers(static_cast<GLsizei>(_renderStructures[i].size()), _indexSingleBondsBuffer[i].data());

    glDeleteVertexArrays(static_cast<GLsizei>(_renderStructures[i].size()), _vertexDoubleBondsArrayObject[i].data());
    glDeleteBuffers(static_cast<GLsizei>(_renderStructures[i].size()), _vertexDoubleBondsInstanceBuffer[i].data());
    glDeleteBuffers(static_cast<GLsizei>(_renderStructures[i].size()), _vertexDoubleBondsBuffer[i].data());
    glDeleteBuffers(static_cast<GLsizei>(_renderStructures[i].size()), _indexDoubleBondsBuffer[i].data());

    glDeleteVertexArrays(static_cast<GLsizei>(_renderStructures[i].size()), _vertexPartialDoubleBondsArrayObject[i].data());
    glDeleteBuffers(static_cast<GLsizei>(_renderStructures[i].size()), _vertexPartialDoubleBondsInstanceBuffer[i].data());
    glDeleteBuffers(static_cast<GLsizei>(_renderStructures[i].size()), _vertexPartialDoubleBondsBuffer[i].data());
    glDeleteBuffers(static_cast<GLsizei>(_renderStructures[i].size()), _indexPartialDoubleBondsBuffer[i].data());

    glDeleteVertexArrays(static_cast<GLsizei>(_renderStructures[i].size()), _vertexTripleBondsArrayObject[i].data());
    glDeleteBuffers(static_cast<GLsizei>(_renderStructures[i].size()), _vertexTripleBondsInstanceBuffer[i].data());
    glDeleteBuffers(static_cast<GLsizei>(_renderStructures[i].size()), _vertexTripleBondsBuffer[i].data());
    glDeleteBuffers(static_cast<GLsizei>(_renderStructures[i].size()), _indexTripleBondsBuffer[i].data());
  }
}

void OpenGLInternalBondShader::generateBuffers()
{
  _numberOfAllInternalBonds.resize(_renderStructures.size());
  _internalAllBondNumberOfIndices.resize(_renderStructures.size());
  _vertexAllBondsArrayObject.resize(_renderStructures.size());
  _vertexAllBondsInstanceBuffer.resize(_renderStructures.size());
  _vertexAllBondsBuffer.resize(_renderStructures.size());
  _indexAllBondsBuffer.resize(_renderStructures.size());

  _numberOfSingleInternalBonds.resize(_renderStructures.size());
  _internalSingleBondNumberOfIndices.resize(_renderStructures.size());
  _vertexSingleBondsArrayObject.resize(_renderStructures.size());
  _vertexSingleBondsInstanceBuffer.resize(_renderStructures.size());
  _vertexSingleBondsBuffer.resize(_renderStructures.size());
  _indexSingleBondsBuffer.resize(_renderStructures.size());

  _numberOfDoubleInternalBonds.resize(_renderStructures.size());
  _internalDoubleBondNumberOfIndices.resize(_renderStructures.size());
  _vertexDoubleBondsArrayObject.resize(_renderStructures.size());
  _vertexDoubleBondsInstanceBuffer.resize(_renderStructures.size());
  _vertexDoubleBondsBuffer.resize(_renderStructures.size());
  _indexDoubleBondsBuffer.resize(_renderStructures.size());

  _numberOfPartialDoubleInternalBonds.resize(_renderStructures.size());
  _internalPartialDoubleBondNumberOfIndices.resize(_renderStructures.size());
  _vertexPartialDoubleBondsArrayObject.resize(_renderStructures.size());
  _vertexPartialDoubleBondsInstanceBuffer.resize(_renderStructures.size());
  _vertexPartialDoubleBondsBuffer.resize(_renderStructures.size());
  _indexPartialDoubleBondsBuffer.resize(_renderStructures.size());

  _numberOfTripleInternalBonds.resize(_renderStructures.size());
  _internalTripleBondNumberOfIndices.resize(_renderStructures.size());
  _vertexTripleBondsArrayObject.resize(_renderStructures.size());
  _vertexTripleBondsInstanceBuffer.resize(_renderStructures.size());
  _vertexTripleBondsBuffer.resize(_renderStructures.size());
  _indexTripleBondsBuffer.resize(_renderStructures.size());

  for(size_t i=0;i<_renderStructures.size();i++)
  {
    _numberOfAllInternalBonds[i].resize(_renderStructures[i].size());
    _internalAllBondNumberOfIndices[i].resize(_renderStructures[i].size());
    _vertexAllBondsArrayObject[i].resize(_renderStructures[i].size());
    _vertexAllBondsInstanceBuffer[i].resize(_renderStructures[i].size());
    _vertexAllBondsBuffer[i].resize(_renderStructures[i].size());
    _indexAllBondsBuffer[i].resize(_renderStructures[i].size());

    _numberOfSingleInternalBonds[i].resize(_renderStructures[i].size());
    _internalSingleBondNumberOfIndices[i].resize(_renderStructures[i].size());
    _vertexSingleBondsArrayObject[i].resize(_renderStructures[i].size());
    _vertexSingleBondsInstanceBuffer[i].resize(_renderStructures[i].size());
    _vertexSingleBondsBuffer[i].resize(_renderStructures[i].size());
    _indexSingleBondsBuffer[i].resize(_renderStructures[i].size());

    _numberOfDoubleInternalBonds[i].resize(_renderStructures[i].size());
    _internalDoubleBondNumberOfIndices[i].resize(_renderStructures[i].size());
    _vertexDoubleBondsArrayObject[i].resize(_renderStructures[i].size());
    _vertexDoubleBondsInstanceBuffer[i].resize(_renderStructures[i].size());
    _vertexDoubleBondsBuffer[i].resize(_renderStructures[i].size());
    _indexDoubleBondsBuffer[i].resize(_renderStructures[i].size());

    _numberOfPartialDoubleInternalBonds[i].resize(_renderStructures[i].size());
    _internalPartialDoubleBondNumberOfIndices[i].resize(_renderStructures[i].size());
    _vertexPartialDoubleBondsArrayObject[i].resize(_renderStructures[i].size());
    _vertexPartialDoubleBondsInstanceBuffer[i].resize(_renderStructures[i].size());
    _vertexPartialDoubleBondsBuffer[i].resize(_renderStructures[i].size());
    _indexPartialDoubleBondsBuffer[i].resize(_renderStructures[i].size());

    _numberOfTripleInternalBonds[i].resize(_renderStructures[i].size());
    _internalTripleBondNumberOfIndices[i].resize(_renderStructures[i].size());
    _vertexTripleBondsArrayObject[i].resize(_renderStructures[i].size());
    _vertexTripleBondsInstanceBuffer[i].resize(_renderStructures[i].size());
    _vertexTripleBondsBuffer[i].resize(_renderStructures[i].size());
    _indexTripleBondsBuffer[i].resize(_renderStructures[i].size());
  }

  for(size_t i=0;i<_renderStructures.size();i++)
  {
    glGenVertexArrays(static_cast<GLsizei>(_renderStructures[i].size()), _vertexAllBondsArrayObject[i].data());
    glGenBuffers(static_cast<GLsizei>(_renderStructures[i].size()), _vertexAllBondsInstanceBuffer[i].data());
    glGenBuffers(static_cast<GLsizei>(_renderStructures[i].size()), _vertexAllBondsBuffer[i].data());
    glGenBuffers(static_cast<GLsizei>(_renderStructures[i].size()), _indexAllBondsBuffer[i].data());

    glGenVertexArrays(static_cast<GLsizei>(_renderStructures[i].size()), _vertexSingleBondsArrayObject[i].data());
    glGenBuffers(static_cast<GLsizei>(_renderStructures[i].size()), _vertexSingleBondsInstanceBuffer[i].data());
    glGenBuffers(static_cast<GLsizei>(_renderStructures[i].size()), _vertexSingleBondsBuffer[i].data());
    glGenBuffers(static_cast<GLsizei>(_renderStructures[i].size()), _indexSingleBondsBuffer[i].data());

    glGenVertexArrays(static_cast<GLsizei>(_renderStructures[i].size()), _vertexDoubleBondsArrayObject[i].data());
    glGenBuffers(static_cast<GLsizei>(_renderStructures[i].size()), _vertexDoubleBondsInstanceBuffer[i].data());
    glGenBuffers(static_cast<GLsizei>(_renderStructures[i].size()), _vertexDoubleBondsBuffer[i].data());
    glGenBuffers(static_cast<GLsizei>(_renderStructures[i].size()), _indexDoubleBondsBuffer[i].data());

    glGenVertexArrays(static_cast<GLsizei>(_renderStructures[i].size()), _vertexPartialDoubleBondsArrayObject[i].data());
    glGenBuffers(static_cast<GLsizei>(_renderStructures[i].size()), _vertexPartialDoubleBondsInstanceBuffer[i].data());
    glGenBuffers(static_cast<GLsizei>(_renderStructures[i].size()), _vertexPartialDoubleBondsBuffer[i].data());
    glGenBuffers(static_cast<GLsizei>(_renderStructures[i].size()), _indexPartialDoubleBondsBuffer[i].data());

    glGenVertexArrays(static_cast<GLsizei>(_renderStructures[i].size()), _vertexTripleBondsArrayObject[i].data());
    glGenBuffers(static_cast<GLsizei>(_renderStructures[i].size()), _vertexTripleBondsInstanceBuffer[i].data());
    glGenBuffers(static_cast<GLsizei>(_renderStructures[i].size()), _vertexTripleBondsBuffer[i].data());
    glGenBuffers(static_cast<GLsizei>(_renderStructures[i].size()), _indexTripleBondsBuffer[i].data());
  }
}

void OpenGLInternalBondShader::loadShader(void)
{
  GLuint vertexShader;
  GLuint fragmentShader;

  vertexShader=compileShaderOfType(GL_VERTEX_SHADER,OpenGLInternalBondShader::_vertexShaderSource.c_str());
  fragmentShader=compileShaderOfType(GL_FRAGMENT_SHADER,OpenGLInternalBondShader::_fragmentShaderSource.c_str());

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
}

const std::string  OpenGLInternalBondShader::_vertexShaderSource =
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

  gl_Position = frameUniforms.mvpMatrix * structureUniforms.modelMatrix * (orientationMatrix * (scale * pos) + pos1);
}
)foo";

const std::string  OpenGLInternalBondShader::_fragmentShaderSource =
OpenGLVersionStringLiteral +
OpenGLFrameUniformBlockStringLiteral +
OpenGLStructureUniformBlockStringLiteral +
OpenGLLightUniformBlockStringLiteral +
OpenGLRGBHSVStringLiteral +
R"foo(

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
