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

#include "optional.h"
#include <vector>
#include <foundationkit.h>
#include <QString>
#include <QDataStream>
#include "skcolorset.h"


class SKColorSets
{
public:
  SKColorSets();
public:
  std::vector<SKColorSet>& colorSets() {return _colorSets;}
  SKColorSet& operator[] (size_t index) {return _colorSets[index];}
  SKColorSet* operator[] (QString);
  const SKColorSet* operator[] (QString) const;
  void append(SKColorSet colorSet) { _colorSets.push_back(colorSet);}
private:
  qint64 _versionNumber{1};
  size_t _numberOfPredefinedSets = 4;
  std::vector<SKColorSet> _colorSets;

  friend QDataStream &operator<<(QDataStream &, const SKColorSets &);
  friend QDataStream &operator>>(QDataStream &, SKColorSets &);
};

