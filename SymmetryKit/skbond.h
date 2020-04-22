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

//class SKAsymmetricAtom;
class SKAtomCopy;


class SKBond: public std::enable_shared_from_this<SKBond>
{
public:
    enum class BoundaryType: qint64
    {
        internal = 0, external = 1
    };
    SKBond();
    SKBond(std::shared_ptr<SKAtomCopy> a, std::shared_ptr<SKAtomCopy> b, BoundaryType type=BoundaryType::internal);
    inline std::shared_ptr<SKAtomCopy> atom1() const {return this->_atom1.lock();}
    inline std::shared_ptr<SKAtomCopy> atom2() const {return this->_atom2.lock();}
    BoundaryType boundaryType() {return _boundaryType;}
    bool operator==(SKBond const& rhs) const;
    double bondLength();
    void setAtoms(std::shared_ptr<SKAtomCopy>, std::shared_ptr<SKAtomCopy>);
    int getTag1() {return _tag1;}
    int getTag2() {return _tag2;}
private:
    std::weak_ptr<SKAtomCopy> _atom1;
    std::weak_ptr<SKAtomCopy> _atom2;
    qint64 _tag1;
    qint64 _tag2;
    BoundaryType _boundaryType = BoundaryType::internal;

    friend QDataStream &operator<<(QDataStream &, const std::shared_ptr<SKBond> &);
    friend QDataStream &operator>>(QDataStream &, std::shared_ptr<SKBond> &);
};

