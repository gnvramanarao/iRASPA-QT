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
//#include <string_view>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <iomanip>
#include "characterset.h"

class Scanner
{
public:
  Scanner(QString &inputString, CharacterSet charactersToBeSkipped): _charactersToBeSkipped(charactersToBeSkipped), _string(inputString) {_scanLocation = _string.begin();}
  QString string() const {return _string;}
  QString::const_iterator scanLocation() const {return _scanLocation;}
  void setScanLocation(QString::const_iterator location) {_scanLocation = location;}
  bool scanCharacters(CharacterSet set, QString &into);
  bool scanUpToCharacters(CharacterSet set, QString &into);
  bool isAtEnd();
  bool scanDouble(double& value);
  bool scanInt(int& value);
private:
  CharacterSet _charactersToBeSkipped;
  QString _string;
  QString::const_iterator _scanLocation;
  int _previousScanLocation;

  QString::const_iterator find_first_not_of(const QString & chars, const QString & text, QString::const_iterator location);
  QString::const_iterator find_first_of(const QString & chars, const QString & text, QString::const_iterator location);
};
