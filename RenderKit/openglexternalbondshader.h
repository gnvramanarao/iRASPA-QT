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
#include "rkrenderkitprotocols.h"
#include "cappedcylindersinglebondgeometry.h"
#include "cappedcylinderdoublebondgeometry.h"
#include "cappedcylinderpartialdoublebondgeometry.h"
#include "cappedcylindertriplebondgeometry.h"

class OpenGLExternalBondShader: public OpenGLShader
{
public:
  OpenGLExternalBondShader();
  void loadShader(void) override final;
  void deleteBuffers();
  void generateBuffers();

  void paintGL(GLuint structureUniformBuffer);

  void reloadData();
  void initializeVertexArrayObject();
  void setRenderStructures(std::vector<std::vector<std::shared_ptr<RKRenderStructure>>> structures);
  GLuint program() {return _program;}
  GLuint stencilProgram() {return _programStencil;}
  GLuint boxProgram() {return _programBox;}
private:
  GLuint _program;
  GLuint _programStencil;
  GLuint _programBox;
  std::vector<std::vector<std::shared_ptr<RKRenderStructure>>> _renderStructures;

  std::vector<std::vector<int>> _numberOfAllExternalBonds;
  std::vector<std::vector<int>> _externalAllBondNumberOfIndices;
  std::vector<std::vector<GLuint>> _vertexAllBondsArrayObject;
  std::vector<std::vector<GLuint>> _vertexAllBondsStencilArrayObject;
  std::vector<std::vector<GLuint>> _vertexAllBondsInstanceBuffer;
  std::vector<std::vector<GLuint>> _vertexAllBondsBuffer;
  std::vector<std::vector<GLuint>> _indexAllBondsBuffer;

  std::vector<std::vector<int>> _numberOfSingleExternalBonds;
  std::vector<std::vector<int>> _externalSingleBondNumberOfIndices;
  std::vector<std::vector<GLuint>> _vertexSingleBondsInstanceBuffer;
  std::vector<std::vector<GLuint>> _vertexSingleBondsArrayObject;
  std::vector<std::vector<GLuint>> _vertexSingleBondsStencilArrayObject;
  std::vector<std::vector<GLuint>> _vertexSingleBondsBuffer;
  std::vector<std::vector<GLuint>> _indexSingleBondsBuffer;

  std::vector<std::vector<int>> _numberOfDoubleExternalBonds;
  std::vector<std::vector<int>> _externalDoubleBondNumberOfIndices;
  std::vector<std::vector<GLuint>> _vertexDoubleBondsInstanceBuffer;
  std::vector<std::vector<GLuint>> _vertexDoubleBondsArrayObject;
  std::vector<std::vector<GLuint>> _vertexDoubleBondsStencilArrayObject;
  std::vector<std::vector<GLuint>> _vertexDoubleBondsBuffer;
  std::vector<std::vector<GLuint>> _indexDoubleBondsBuffer;

  std::vector<std::vector<int>> _numberOfPartialDoubleExternalBonds;
  std::vector<std::vector<int>> _externalPartialDoubleBondNumberOfIndices;
  std::vector<std::vector<GLuint>> _vertexPartialDoubleBondsInstanceBuffer;
  std::vector<std::vector<GLuint>> _vertexPartialDoubleBondsArrayObject;
  std::vector<std::vector<GLuint>> _vertexPartialDoubleBondsStencilArrayObject;
  std::vector<std::vector<GLuint>> _vertexPartialDoubleBondsBuffer;
  std::vector<std::vector<GLuint>> _indexPartialDoubleBondsBuffer;

  std::vector<std::vector<int>> _numberOfTripleExternalBonds;
  std::vector<std::vector<int>> _externalTripleBondNumberOfIndices;
  std::vector<std::vector<GLuint>> _vertexTripleBondsInstanceBuffer;
  std::vector<std::vector<GLuint>> _vertexTripleBondsArrayObject;
  std::vector<std::vector<GLuint>> _vertexTripleBondsStencilArrayObject;
  std::vector<std::vector<GLuint>> _vertexTripleBondsBuffer;
  std::vector<std::vector<GLuint>> _indexTripleBondsBuffer;

  GLint _vertexNormalAttributeLocation;
  GLint _vertexPositionAttributeLocation;
  GLint _instancePositionFirstAtomAttributeLocation;
  GLint _instancePositionSecondAtomAttributeLocation;
  GLint _instanceColorFirstAtomAttributeLocation;
  GLint _instanceColorSecondAtomAttributeLocation;
  GLint _instanceScaleAttributeLocation;

  GLint _vertexStencilNormalAttributeLocation;
  GLint _vertexStencilPositionAttributeLocation;
  GLint _instanceStencilPositionFirstAtomAttributeLocation;
  GLint _instanceStencilPositionSecondAtomAttributeLocation;
  GLint _instanceStencilColorFirstAtomAttributeLocation;
  GLint _instanceStencilColorSecondAtomAttributeLocation;
  GLint _instanceStencilScaleAttributeLocation;

  GLint _boxVertexPositionAttributeLocation;
  GLint _boxVertexNormalAttributeLocation;

  GLuint _boxVertexArray;
  GLuint _boxVertexBuffer;
  GLuint _boxIndexBuffer;

  static const std::string _vertexShaderSource;
  static const std::string _fragmentShaderSource;

  static const std::string _vertexShaderSourceStencil;
  static const std::string _fragmentShaderSourceStencil;

  static const std::string _vertexShaderSourceBox;
  static const std::string _fragmentShaderSourceBox;
};
