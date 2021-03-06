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

#include <QDataStream>
#include <unordered_set>
#include <vector>
#include <set>
#include <tuple>
#include "skbond.h"
#include "skasymmetricatom.h"
#include "skatomcopy.h"
#include "skasymmetricbond.h"
#include "skatomtreecontroller.h"

using BondSelection = std::vector<std::pair<std::shared_ptr<SKAsymmetricBond>, int>>;

class SKBondSetController
{
public:
  SKBondSetController(std::shared_ptr<SKAtomTreeController> atomTreeController);
  const std::vector<std::shared_ptr<SKAsymmetricBond>>& arrangedObjects() const {return _arrangedObjects;}
  void append(std::shared_ptr<SKAsymmetricBond> bond) {_arrangedObjects.push_back(bond);}
  void clear() {_arrangedObjects.clear();}
  size_t getNumberOfBonds();
  std::vector<std::shared_ptr<SKBond>> getBonds();
  void setBonds(std::vector<std::shared_ptr<SKBond>> &bonds);
  void insertBonds(std::vector<std::shared_ptr<SKAsymmetricBond> > bonds, std::set<int> indexSet);
  void deleteBonds(std::set<int> indexSet);
  std::set<int> &selectionIndexSet() {return _selectedIndexSet;}
  std::vector<std::shared_ptr<SKAsymmetricBond>> selectedObjects() const;
  void setSelectedObjects(std::set<int> selection) {_selectedIndexSet = selection;}
  void addSelectedObjects(std::set<int> selection);
  void setTags();
  void correctBondSelectionDueToAtomSelection();
  BondSelection selection() const;
  void setSelection(BondSelection selection);
  void deleteBonds(BondSelection selection);
  void insertSelection(BondSelection selection);
private:
  static qint64 _versionNumber;
  std::shared_ptr<SKAtomTreeController> _atomTreecontroller;

  std::vector<std::shared_ptr<SKAsymmetricBond>> _arrangedObjects;
  std::set<int> _selectedIndexSet;

  friend QDataStream &operator<<(QDataStream &, const std::shared_ptr<SKBondSetController> &);
  friend QDataStream &operator>>(QDataStream &stream, std::shared_ptr<SKBondSetController>& controller);
};
