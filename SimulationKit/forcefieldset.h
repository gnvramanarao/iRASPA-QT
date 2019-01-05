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

#include <map>
#include <QString>
#include <tuple>
#include <vector>
#include <set>
#include "forcefieldtype.h"
#include <iostream>

enum class ForceFieldSchemeOrder: qint64
{
  elementOnly = 0, forceFieldFirst = 1, forceFieldOnly = 2, multiple_values = 3
};

class ForceFieldSet
{
public:
  ForceFieldSet();
  ForceFieldSet(QString name, ForceFieldSet& forcefieldset, bool editable=false);
  QString displayName() const {return _displayName;}

  ForceFieldType* operator[] (const QString name);
  ForceFieldType& operator[] (size_t sortNumber) {return _atomTypeList[sortNumber];}

  std::vector<ForceFieldType>& atomTypeList() {return _atomTypeList;}
  void insert(int index, ForceFieldType& forceFieldType) {_atomTypeList.insert(_atomTypeList.begin()+index, forceFieldType);}
  void duplicate(size_t index);
  bool editable() {return _editable;}
  QString uniqueName(int atomicNumber);
private:
  qint64 _versionNumber{1};
  QString _displayName = "Default";
  bool _editable = false;
  std::vector<ForceFieldType> _atomTypeList{};

  static std::vector<ForceFieldType> _defaultForceField;

  friend QDataStream &operator<<(QDataStream &, const ForceFieldSet &);
  friend QDataStream &operator>>(QDataStream &, ForceFieldSet &);
};

