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

#include <iostream>
#include <vector>
#include <unordered_set>
#include <mathkit.h>
#include <type_traits>
#include <foundationkit.h>
#include "skbond.h"

class SKAsymmetricAtom;

class SKAtomCopy
{
public:
    SKAtomCopy(): _position(), _tag(0), _type(AtomCopyType::copy), _parent() {}
    SKAtomCopy(std::shared_ptr<SKAsymmetricAtom> asymmetricParentAtom, double3 position): _position(position), _tag(0), _type(AtomCopyType::copy), _parent(asymmetricParentAtom) {}
    enum class AtomCopyType: qint64
    {
      copy = 2, duplicate = 3
    };
    const std::shared_ptr<SKAsymmetricAtom> parent() const {return this->_parent.lock();}
    double3 position() {return _position;}
    AtomCopyType type() {return _type;}
    void setType(AtomCopyType type) {_type = type;}
    qint64 tag() {return _tag;}
    void setTag(qint64 tag) {_tag = tag;}
    qint64 asymmetricIndex() {return _asymmetricIndex;}
    void setAsymmetricIndex(qint64 value) {_asymmetricIndex = value;}
private:
    qint64 _versionNumber{1};
    struct Hash
    {
      template <typename T> std::size_t operator() (T* const &p) const
      {
        return std::hash<T>()(*p);
      }
    };
    struct Compare
    {
      template <typename T> size_t operator() (T* const &a, T* const &b) const
      {
        return *a == *b;
      }
    };
    double3 _position;
    std::unordered_set<SKBond*, SKAtomCopy::Hash, SKAtomCopy::Compare> _bonds;
    qint64 _tag;
    AtomCopyType _type;
    std::weak_ptr<SKAsymmetricAtom> _parent;
    qint64 _asymmetricIndex;

    friend QDataStream &operator<<(QDataStream &, const std::shared_ptr<SKAtomCopy> &);
    friend QDataStream &operator>>(QDataStream &, std::shared_ptr<SKAtomCopy> &);

    friend QDataStream &operator<<(QDataStream &, const std::shared_ptr<SKAsymmetricAtom> &);
    friend QDataStream &operator>>(QDataStream &, std::shared_ptr<SKAsymmetricAtom> &);

    friend QDataStream &operator<<(QDataStream &, const std::shared_ptr<SKBond> &);
    friend QDataStream &operator>>(QDataStream &, std::shared_ptr<SKBond> &);
};
