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

#include <vector>
#define GL_GLEXT_PROTOTYPES
#include <QtOpenGL>
#include <QGLFunctions>
#include "openglshader.h"
#include "openglatompickingshader.h"
#include "openglinternalbondshader.h"
#include "rkrenderkitprotocols.h"
#include "cappedcylindersinglebondgeometry.h"
#include "cappedcylinderdoublebondgeometry.h"
#include "cappedcylinderpartialdoublebondgeometry.h"
#include "cappedcylindertriplebondgeometry.h"

class OpenGLInternalBondPickingShader: public OpenGLShader
{
public:
  OpenGLInternalBondPickingShader(OpenGLAtomPickingShader &atomPickingShader);
  void loadShader(void) override final;

  void setRenderStructures(std::vector<std::vector<std::shared_ptr<RKRenderStructure>>> structures);
  void paintGL(int width,int height,GLuint structureUniformBuffer);

  void reloadData();
  void initializeVertexArrayObject();

  void deleteBuffers();
  void generateBuffers();

  GLuint program() {return _program;}
private:
  GLuint _program;
  OpenGLAtomPickingShader &_atomPickingShader;

  std::vector<std::vector<std::shared_ptr<RKRenderStructure>>> _renderStructures;

  std::vector<std::vector<int>> _numberOfAllInternalBonds;
  std::vector<std::vector<int>> _internalAllBondNumberOfIndices;
  std::vector<std::vector<GLuint>> _vertexAllBondsInstanceBuffer;
  std::vector<std::vector<GLuint>> _vertexAllBondsArrayObject;
  std::vector<std::vector<GLuint>> _vertexAllBondsBuffer;
  std::vector<std::vector<GLuint>> _indexAllBondsBuffer;

  std::vector<std::vector<int>> _numberOfSingleInternalBonds;
  std::vector<std::vector<int>> _internalSingleBondNumberOfIndices;
  std::vector<std::vector<GLuint>> _vertexSingleBondsInstanceBuffer;
  std::vector<std::vector<GLuint>> _vertexSingleBondsArrayObject;
  std::vector<std::vector<GLuint>> _vertexSingleBondsBuffer;
  std::vector<std::vector<GLuint>> _indexSingleBondsBuffer;

  std::vector<std::vector<int>> _numberOfDoubleInternalBonds;
  std::vector<std::vector<int>> _internalDoubleBondNumberOfIndices;
  std::vector<std::vector<GLuint>> _vertexDoubleBondsInstanceBuffer;
  std::vector<std::vector<GLuint>> _vertexDoubleBondsArrayObject;
  std::vector<std::vector<GLuint>> _vertexDoubleBondsBuffer;
  std::vector<std::vector<GLuint>> _indexDoubleBondsBuffer;

  std::vector<std::vector<int>> _numberOfPartialDoubleInternalBonds;
  std::vector<std::vector<int>> _internalPartialDoubleBondNumberOfIndices;
  std::vector<std::vector<GLuint>> _vertexPartialDoubleBondsInstanceBuffer;
  std::vector<std::vector<GLuint>> _vertexPartialDoubleBondsArrayObject;
  std::vector<std::vector<GLuint>> _vertexPartialDoubleBondsBuffer;
  std::vector<std::vector<GLuint>> _indexPartialDoubleBondsBuffer;


  std::vector<std::vector<int>> _numberOfTripleInternalBonds;
  std::vector<std::vector<int>> _internalTripleBondNumberOfIndices;
  std::vector<std::vector<GLuint>> _vertexTripleBondsInstanceBuffer;
  std::vector<std::vector<GLuint>> _vertexTripleBondsArrayObject;
  std::vector<std::vector<GLuint>> _vertexTripleBondsBuffer;
  std::vector<std::vector<GLuint>> _indexTripleBondsBuffer;

  GLint _vertexPositionAttributeLocation;
  GLint _instancePositionFirstAtomAttributeLocation;
  GLint _instancePositionSecondAtomAttributeLocation;
  GLint _pickingTagAttributeLocation;

  static const std::string _vertexShaderSource;
  static const std::string _fragmentShaderSource;
};
