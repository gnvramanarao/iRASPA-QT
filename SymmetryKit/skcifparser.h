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
#include <vector>
#include <map>
#include <cwctype>
#define _USE_MATH_DEFINES
#include <cmath>
#include <optional>
#include <foundationkit.h>
#include "skparser.h"
#include "skasymmetricatom.h"
#include "skatomtreenode.h"
#include "skatomtreecontroller.h"

class SKCIFParser: public SKParser
{
public:
  SKCIFParser(QString fileContent, bool onlyAsymmetricUnitCell, CharacterSet charactersToBeSkipped): SKParser(), _scanner(fileContent, charactersToBeSkipped), _onlyAsymmetricUnitCell(onlyAsymmetricUnitCell) {}
  void startParsing() override final;
  std::optional<int> spaceGroupHallNumber() {return _spaceGroupHallNumber;}
private:
  Scanner _scanner;
  bool _onlyAsymmetricUnitCell;
  QString::const_iterator _previousScanLocation;
  std::vector<QString> _keys;
  std::map<QString,QString> _map;
  std::optional<int> _spaceGroupHallNumber;
  std::shared_ptr<SKAtomTreeController> _atomTreeController = std::make_shared<SKAtomTreeController>();
  std::vector<SKAsymmetricAtom> _atoms{};

  void ParseLine(QString);
  void parseAudit(QString& string);
  void parseiRASPA(QString& string);
  void parseChemical(QString& string);
  void parseCell(QString& string);
  void parseSymmetry(QString& string);
  void parseName(QString& string);
  void parseLoop(QString& string);
  std::optional<QString> parseValue();
  void skipComment();
  qint64 scanInt();
  double scanDouble();
  std::optional<QString> scanString();
};

