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

#include <string>
#include <vector>
#include <QString>
#include <QDataStream>
#include "skdefinitions.h"
#include "skrotationaloccurancetable.h"

class SKPointGroup
{
public:
  SKPointGroup(SKRotationalOccuranceTable table, qint64 number, QString symbol, QString schoenflies, Holohedry holohedry, Laue laue, bool centrosymmetric, bool enantiomorphic);
  static std::vector<SKPointGroup> pointGroupData;

  Holohedry holohedry() const {return _holohedry;}
  QString holohedryString() const;
  QString LaueString() const;
  QString symbol() {return _symbol;}
  QString schoenflies() {return _schoenflies;}
  bool centrosymmetric() {return _centrosymmetric;}
  bool enantiomorphic() {return _enantiomorphic;}
private:
  SKRotationalOccuranceTable _table;
  qint64 _number = 0;
  QString _symbol = "";
  QString _schoenflies = "";
  Holohedry _holohedry = Holohedry::none;
  Laue _laue = Laue::none;
  bool _centrosymmetric = false;
  bool _enantiomorphic = false;

  friend QDataStream &operator<<(QDataStream &, const SKPointGroup &);
  friend QDataStream &operator>>(QDataStream &, SKPointGroup &);
};
