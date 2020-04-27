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
#include <iostream>
#include <memory>
#include <tuple>
#include "skasymmetricatom.h"


class SKAsymmetricBond: public std::enable_shared_from_this<SKAsymmetricBond>
{
public:
  enum class SKBondType: qint64
  {
    singleBond = 0, doubleBond = 1, partialDoubleBond = 2, tripleBond = 3
  };

  struct KeyHash {
   std::size_t operator()(const std::shared_ptr<SKAsymmetricBond>& k) const
   {
       size_t first = std::hash<std::shared_ptr<SKAsymmetricAtom>>{}(k->atom1());
       size_t second = std::hash<std::shared_ptr<SKAsymmetricAtom>>{}(k->atom2());
       size_t lhs = std::min(first,second);
       size_t rhs = std::max(first,second);
       return lhs ^ rhs + 0x9e3779b9 + (lhs << 6) + (lhs >> 2);
   }
  };

  struct KeyEqual {
   bool operator()(const std::shared_ptr<SKAsymmetricBond>& lhs, const std::shared_ptr<SKAsymmetricBond>& rhs) const
   {
       return lhs->atom1().get() == rhs->atom1().get() && lhs->atom2().get() == rhs->atom2().get();
   }
  };
  SKAsymmetricBond() {}
  SKAsymmetricBond(std::shared_ptr<SKAsymmetricAtom> a, std::shared_ptr<SKAsymmetricAtom> b);
  bool operator==(SKAsymmetricBond const& rhs) const;
  inline std::shared_ptr<SKAsymmetricAtom> atom1() const {return this->_atom1.lock();}
  inline std::shared_ptr<SKAsymmetricAtom> atom2() const {return this->_atom2.lock();}
  std::vector<std::shared_ptr<SKBond>>& copies()  {return _copies;}
  bool isVisible() {return _isVisible;}
  void setIsVisible(bool v) {_isVisible = v;}
  SKBondType getBondType() {return _bondType;}
  void setBondType(SKBondType type) {_bondType = type;}
  int getTag1() {return _tag1;}
  int getTag2() {return _tag2;}
  void setAtom1(std::shared_ptr<SKAsymmetricAtom> a) {_atom1 = a;}
  void setAtom2(std::shared_ptr<SKAsymmetricAtom> a) {_atom2 = a;}
  void setAsymmetricIndex(int index) {_asymmetricIndex = index;}
  int asymmetricIndex() {return _asymmetricIndex;}
private:

  std::weak_ptr<SKAsymmetricAtom> _atom1;
  std::weak_ptr<SKAsymmetricAtom> _atom2;
  qint64 _tag1;
  qint64 _tag2;
  std::vector<std::shared_ptr<SKBond>> _copies;
  bool _isVisible;
  SKBondType _bondType;
  int _asymmetricIndex = 0;
  int _bondOrder = 0;

  friend QDataStream &operator<<(QDataStream &, const std::shared_ptr<SKAsymmetricBond> &);
  friend QDataStream &operator>>(QDataStream &, std::shared_ptr<SKAsymmetricBond> &);
};

