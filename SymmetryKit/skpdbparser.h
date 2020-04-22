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

#define _USE_MATH_DEFINES
#include <cmath>
#include <QString>
#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <cwctype>
#include <optional>
#include <foundationkit.h>
#include "skparser.h"
#include "skasymmetricatom.h"
#include "skatomtreenode.h"
#include "skatomtreecontroller.h"
#include "skspacegroup.h"

class SKPDBParser: public SKParser
{
public:
  SKPDBParser(QString fileContent, bool onlyAsymmetricUnitCell, CharacterSet charactersToBeSkipped): SKParser(), _scanner(fileContent, charactersToBeSkipped), _onlyAsymmetricUnitCell(onlyAsymmetricUnitCell) {}
  void startParsing() override final;
private:
  Scanner _scanner;
  bool _onlyAsymmetricUnitCell;
  QString::const_iterator _previousScanLocation;

  std::tuple<std::shared_ptr<SKAtomTreeController>, std::shared_ptr<SKCell>,int> _frame = std::make_tuple(std::make_shared<SKAtomTreeController>(), std::make_shared<SKCell>(), 1);

  void addFrameToStructure(size_t currentMovie, size_t currentFrame);
};
