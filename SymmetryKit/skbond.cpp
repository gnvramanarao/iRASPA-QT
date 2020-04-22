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

#include "skbond.h"
#include "skasymmetricatom.h"


SKBond::SKBond()
{

}

SKBond::SKBond(std::shared_ptr<SKAtomCopy> a, std::shared_ptr<SKAtomCopy> b, BoundaryType type)
{
  if(a->parent()->tag() < b->parent()->tag())
  {
      _atom1 = a;
      _atom2 = b;
  }
  else
  {
      _atom1 = b;
      _atom2 = a;
  }
  _boundaryType = type;
}

void SKBond::setAtoms(std::shared_ptr<SKAtomCopy> a, std::shared_ptr<SKAtomCopy> b)
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

bool SKBond::operator==(SKBond const& rhs) const
{
  return this == &rhs;
}

double SKBond::bondLength()
{
  return (this->atom1()->position()-this->atom2()->position()).length();
}

QDataStream &operator<<(QDataStream &stream, const std::shared_ptr<SKBond> &bond)
{
  stream << bond->atom1()->tag();
  stream << bond->atom2()->tag();
  stream << static_cast<typename std::underlying_type<SKBond::BoundaryType>::type>(bond->_boundaryType);
  return stream;
}

QDataStream &operator>>(QDataStream &stream, std::shared_ptr<SKBond> &bond)
{
  stream >> bond->_tag1;
  stream >> bond->_tag2;
  qint64 boundaryType;
  stream >> boundaryType;
  bond->_boundaryType = SKBond::BoundaryType(boundaryType);
  return stream;
}

