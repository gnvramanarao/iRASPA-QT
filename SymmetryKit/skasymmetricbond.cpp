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

#include <qdebug.h>
#include "skasymmetricbond.h"

SKAsymmetricBond::SKAsymmetricBond(std::shared_ptr<SKAsymmetricAtom> a, std::shared_ptr<SKAsymmetricAtom> b)
{
  if(a->tag() < b->tag())
  {
     _atom1 = a;
     _atom2 = b;
  }
  else
  {
     _atom1 = b;
     _atom2 = a;
  }
}

bool SKAsymmetricBond::operator==(SKAsymmetricBond const& rhs) const
{
  return (this->atom1().get() == rhs.atom1().get() && this->atom2().get() == rhs.atom2().get()) ||
         (this->atom1().get() == rhs.atom2().get() && this->atom2().get() == rhs.atom1().get());
}



QDataStream &operator<<(QDataStream &stream, const std::shared_ptr<SKAsymmetricBond> &asymmetricBond)
{
  stream << asymmetricBond->atom1()->tag();
  stream << asymmetricBond->atom2()->tag();
  stream << asymmetricBond->_copies;
  stream << static_cast<typename std::underlying_type<SKAsymmetricBond::SKBondType>::type>(asymmetricBond->_bondType);
  stream << asymmetricBond->_isVisible;
  return stream;
}

QDataStream &operator>>(QDataStream &stream, std::shared_ptr<SKAsymmetricBond> &asymmetricBond)
{
  stream >> asymmetricBond->_tag1;
  stream >> asymmetricBond->_tag2;
  stream >> asymmetricBond->_copies;
  qint64 bondType;
  stream >> bondType;
  asymmetricBond->_bondType = SKAsymmetricBond::SKBondType(bondType);
  stream >> asymmetricBond->_isVisible;

  return stream;
}
