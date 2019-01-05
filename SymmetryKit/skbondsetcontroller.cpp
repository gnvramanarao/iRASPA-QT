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

#include "skbondsetcontroller.h"
#include "skasymmetricatom.h"
#include <set>
#include <map>
#include <unordered_map>

SKBondSetController::SKBondSetController(std::shared_ptr<SKAtomTreeController> atomTreeController):_atomTreecontroller(atomTreeController)
{

}


struct SKAysmmetricBondCompare
{
   bool operator () (const std::shared_ptr<SKBond> & s1, const std::shared_ptr<SKBond>& s2) const
   {
      if(s1->atom1()->parent()->elementIdentifier() == s2->atom1()->parent()->elementIdentifier())
      {
        if(s1->atom2()->parent()->elementIdentifier() == s2->atom2()->parent()->elementIdentifier())
        {
          if(s1->atom1()->parent()->tag() == s2->atom1()->parent()->tag())
          {
            return (s1->atom2()->parent()->tag() < s2->atom2()->parent()->tag());
          }
          else
          {
             return (s1->atom1()->parent()->tag() < s2->atom1()->parent()->tag());
          }
        }
        else
        {
          return (s1->atom2()->parent()->elementIdentifier() > s2->atom2()->parent()->elementIdentifier());
        }
      }
      else
      {
        return (s1->atom1()->parent()->elementIdentifier() > s2->atom1()->parent()->elementIdentifier());
      }
    }
} ;

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

void SKBondSetController::sort()
{
  //std::unordered_map<std::shared_ptr<SKAsymmetricBond>, std::shared_ptr<SKBond>, KeyHash, KeyEqual > map{};
  std::set<std::shared_ptr<SKBond>, SKAysmmetricBondCompare > map{};

  for (const std::shared_ptr<SKBond>& bond : _arrangedObjects)
  {
    if(bond->atom1()->type() == SKAtomCopy::AtomCopyType::copy && bond->atom2()->type() == SKAtomCopy::AtomCopyType::copy)
    {
       //std::cout << "parent: " << bond->atom1()->parent() << ", " << bond->atom2()->parent() << std::endl;
       //std::shared_ptr<SKAsymmetricBond> asymmetricBond = std::make_shared<SKAsymmetricBond>(bond->atom1()->parent(), bond->atom2()->parent());
       //map.insert({asymmetricBond,bond});
       map.insert(bond);
    }
  }

  _rearrangedObjects.clear();
  //for(const auto& [key, value] : map)
  for(auto & key : map)
  {
     //std::cout << "put in set: " << key->atom1()->tag() << ", " << key->atom2()->tag() << std::endl;
     _rearrangedObjects.push_back(key);
  }
}

QDataStream &operator<<(QDataStream &stream, const std::shared_ptr<SKBondSetController> &controller)
{
  stream << controller->_versionNumber;
  stream << static_cast<qint64>(controller->_arrangedObjects.size());
  for(const std::shared_ptr<SKBond>& bond : controller->_arrangedObjects)
  {
    stream << bond->atom1()->tag();
    stream << bond->atom2()->tag();
    stream << bond->boundaryType();
  }
  return stream;
}

QDataStream &operator>>(QDataStream &stream, std::shared_ptr<SKBondSetController>& controller)
{
  qint64 versionNumber;
  stream >> versionNumber;
  if(versionNumber > controller->_versionNumber)
  {
    throw InvalidArchiveVersionException(__FILE__, __LINE__, "SKBondSetController");
  }

  std::vector<std::shared_ptr<SKAtomCopy>> atomCopies = controller->_atomTreecontroller->allAtomCopies();
  qint64 index=0;
  for(std::shared_ptr<SKAtomCopy> atom: atomCopies)
  {
    atom->setTag(index);
    index++;
  }

  qint64 vecSize;
  controller->_arrangedObjects.clear();
  stream >> vecSize;
  qint64 atomtag1;
  qint64 atomtag2;
  SKBond::BoundaryType type;
  while(vecSize--)
  {
    stream >> atomtag1;
    stream >> atomtag2;
    stream >> type;
    std::shared_ptr<SKBond> bond = std::make_shared<SKBond>(atomCopies[atomtag1],atomCopies[atomtag2], type);
    controller->_arrangedObjects.insert(bond);
  }
  controller->sort();
  return stream;
}
