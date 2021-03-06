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

#include <QString>
#include <string>
#include <mathkit.h>
#include <vector>
#include <set>
#include <optional>
#include "skasymmetricatom.h"
#include "skcell.h"

class SKStructure
{
public:
  SKStructure(): cell(std::make_shared<SKCell>()) {};

  enum class Kind: qint64
  {
    unknown = 0, structure = 1,
    crystal = 2, molecularCrystal = 3, molecule = 4, protein = 5, proteinCrystal = 6,
    proteinCrystalSolvent = 7, crystalSolvent = 8, molecularCrystalSolvent = 9,
    crystalEllipsoidPrimitive = 10, crystalCylinderPrimitive = 11, crystalPolygonalPrismPrimitive = 12,
    ellipsoidPrimitive = 13, cylinderPrimitive = 14, polygonalPrismPrimitive = 15
  };

  Kind kind = Kind::crystal;
  std::vector<std::shared_ptr<SKAsymmetricAtom>> atoms;
  std::set<QString> unknownAtoms;

  std::optional<QString> displayName;
  std::shared_ptr<SKCell> cell;
  std::optional<int> spaceGroupHallNumber;
  bool drawUnitCell = false;
  bool periodic = false;

  std::optional<QString> creationDate;
  std::optional<QString> creationMethod;
  std::optional<QString> chemicalFormulaSum;
  std::optional<QString> chemicalFormulaStructural;
  std::optional<int> cellFormulaUnitsZ;

  std::optional<int> numberOfChannels;
  std::optional<int> numberOfPockets;
  std::optional<int> dimensionality;
  std::optional<double> Di;
  std::optional<double> Df;
  std::optional<double> Dif;
};
