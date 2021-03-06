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
#include <set>
#include <map>
#include <unordered_map>
#include <type_traits>
#include <qdebug.h>
#include <algorithm>
#include <map>
#include <unordered_set>

qint64 SKBondSetController::_versionNumber = 2;

SKBondSetController::SKBondSetController(std::shared_ptr<SKAtomTreeController> atomTreeController):_atomTreecontroller(atomTreeController)
{

}


struct SKAysmmetricBondCompare
{
   bool operator () (const std::shared_ptr<SKAsymmetricBond> & s1, const std::shared_ptr<SKAsymmetricBond>& s2) const
   {
      if(s1->atom1()->elementIdentifier() == s2->atom1()->elementIdentifier())
      {
        if(s1->atom2()->elementIdentifier() == s2->atom2()->elementIdentifier())
        {
          if(s1->atom1()->tag() == s2->atom1()->tag())
          {
            return (s1->atom2()->tag() < s2->atom2()->tag());
          }
          else
          {
             return (s1->atom1()->tag() < s2->atom1()->tag());
          }
        }
        else
        {
          return (s1->atom2()->elementIdentifier() > s2->atom2()->elementIdentifier());
        }
      }
      else
      {
        return (s1->atom1()->elementIdentifier() > s2->atom1()->elementIdentifier());
      }
    }
};

std::vector<std::shared_ptr<SKBond>> SKBondSetController::getBonds()
{
  std::vector<std::shared_ptr<SKBond>> bonds;

  int count = getNumberOfBonds();
  bonds.reserve(count);

  for(std::shared_ptr<SKAsymmetricBond> asymmetricBond: _arrangedObjects)
  {
    std::vector<std::shared_ptr<SKBond>> copies =  asymmetricBond->copies();
    bonds.insert( bonds.end(), copies.begin(), copies.end() );
  }

  return bonds;
}

size_t SKBondSetController::getNumberOfBonds()
{
  size_t count = 0;
  for(std::shared_ptr<SKAsymmetricBond> asymmetricBond: _arrangedObjects)
  {
     count += asymmetricBond->copies().size();
  }
  return count;
}

void SKBondSetController::setBonds(std::vector<std::shared_ptr<SKBond>> &bonds)
{
  this->_arrangedObjects.clear();

  std::unordered_set<std::shared_ptr<SKAsymmetricBond>, SKAsymmetricBond::KeyHash, SKAsymmetricBond::KeyEqual> asymmetricBonds;

  std::transform(bonds.begin(),bonds.end(),std::inserter(asymmetricBonds, asymmetricBonds.begin()),[](std::shared_ptr<SKBond> b) -> std::shared_ptr<SKAsymmetricBond>
          {return std::make_shared<SKAsymmetricBond>(SKAsymmetricBond(b->atom1()->parent(), b->atom2()->parent()));});


  // partition the bonds
  for(std::shared_ptr<SKBond> &bond: bonds)
  {
    std::shared_ptr<SKAsymmetricBond> asymmetricBond = std::make_shared<SKAsymmetricBond>(SKAsymmetricBond(bond->atom1()->parent(), bond->atom2()->parent()));

    std::unordered_set<std::shared_ptr<SKAsymmetricBond>, SKAsymmetricBond::KeyHash, SKAsymmetricBond::KeyEqual>::iterator it = asymmetricBonds.find(asymmetricBond);
    if (it != asymmetricBonds.end())
    {
      it->get()->copies().push_back(bond);
    }
  }

  _arrangedObjects.clear();
  std::copy(asymmetricBonds.begin(), asymmetricBonds.end(),std::back_inserter(_arrangedObjects));
  std::sort(_arrangedObjects.begin(), _arrangedObjects.end(), SKAysmmetricBondCompare());
}

void SKBondSetController::insertBonds(std::vector<std::shared_ptr<SKAsymmetricBond>> bonds, std::set<int> indexSet)
{
  int bondIndex=0;
  for(int index: indexSet)
  {
    _arrangedObjects.insert(_arrangedObjects.begin() + index, bonds[bondIndex]);
    bondIndex++;
  }
}

void SKBondSetController::deleteBonds(std::set<int> indexSet)
{
  std::vector<int> reversedIndexSet;
  std::reverse_copy(indexSet.begin(), indexSet.end(), std::back_inserter(reversedIndexSet));

  for (int index : reversedIndexSet)
  {
    _arrangedObjects.erase(_arrangedObjects.begin() + index);
  }
}

void SKBondSetController::addSelectedObjects(std::set<int> selection)
{
 _selectedIndexSet.insert(selection.begin(), selection.end());
}


void SKBondSetController::setTags()
{
  int asymmetricBondIndex = 0;
  for(std::shared_ptr<SKAsymmetricBond> asymmetricBond: _arrangedObjects)
  {
    asymmetricBond->setAsymmetricIndex(asymmetricBondIndex);
    asymmetricBondIndex++;
  }
}

BondSelection SKBondSetController::selection() const
{
  BondSelection s;
  for(int index: _selectedIndexSet)
  {
    s.push_back(std::make_pair(_arrangedObjects[index], index));
  }
  return s;
}

void  SKBondSetController::setSelection(BondSelection selection)
{
  _selectedIndexSet.clear();
  for(const auto [bondItem, index] : selection)
  {
    _selectedIndexSet.insert(index);
  }
}

void SKBondSetController::insertSelection(BondSelection selection)
{
  _selectedIndexSet.clear();
  for(const auto [bondItem, index] : selection)
  {
    _arrangedObjects.insert(_arrangedObjects.begin() + index, bondItem);
    _selectedIndexSet.insert(index);
  }
}

void SKBondSetController::deleteBonds(BondSelection selection)
{
  for (const auto [bondItem, index] : selection)
  {
    _arrangedObjects.erase(_arrangedObjects.begin() + index);
  }
}

std::vector<std::shared_ptr<SKAsymmetricBond>> SKBondSetController::selectedObjects() const
{
  std::vector<std::shared_ptr<SKAsymmetricBond>> objects;
  for(int index: _selectedIndexSet)
  {
    objects.push_back(_arrangedObjects[index]);
  }
  return objects;
}

void SKBondSetController::correctBondSelectionDueToAtomSelection()
{
  std::set<std::shared_ptr<SKAtomTreeNode>> selectedTreeAtoms = _atomTreecontroller->selectedTreeNodes();
  std::set<std::shared_ptr<SKAsymmetricAtom>> selectedAtoms;
  std::transform(selectedTreeAtoms.begin(),selectedTreeAtoms.end(),std::inserter(selectedAtoms, selectedAtoms.begin()),[](std::shared_ptr<SKAtomTreeNode> node) -> std::shared_ptr<SKAsymmetricAtom>
                        {return node->representedObject();});

  int bondIndex=0;
  for(std::shared_ptr<SKAsymmetricBond> bond : _arrangedObjects)
  {
    // if one the atoms of the bond is selected then so must be the bond
    const bool Atom1isSelected = selectedAtoms.find(bond->atom1()) != selectedAtoms.end();
    const bool Atom2isSelected = selectedAtoms.find(bond->atom2()) != selectedAtoms.end();
    if(Atom1isSelected || Atom2isSelected)
    {
      _selectedIndexSet.insert(bondIndex);
    }
    bondIndex++;
  }
}

QDataStream &operator<<(QDataStream &stream, const std::shared_ptr<SKBondSetController> &controller)
{
  stream << controller->_versionNumber;
  stream << controller->_arrangedObjects;

  return stream;
}

QDataStream &operator>>(QDataStream &stream, std::shared_ptr<SKBondSetController>& controller)
{
  qint32 versionNumber;
  stream >> versionNumber;

  if(versionNumber > controller->_versionNumber)
  {
    throw InvalidArchiveVersionException(__FILE__, __LINE__, "SKBondSetController");
  }

  std::vector<std::shared_ptr<SKAsymmetricAtom>> asymmetricAtoms = controller->_atomTreecontroller->flattenedObjects();
  std::vector<std::shared_ptr<SKAtomCopy>> atomCopies = controller->_atomTreecontroller->allAtomCopies();

  if(versionNumber==0)
  {
    qint32 versionNumber2;
    stream >> versionNumber2;

    if(versionNumber2 > 1)
    {
      stream >> controller->_arrangedObjects;

      // fill in the atoms from the tags
      for(std::shared_ptr<SKAsymmetricBond> &bond: controller->_arrangedObjects)
      {
        int atom1Tag = bond->getTag1();
        int atom2Tag = bond->getTag2();
        bond->setAtom1(asymmetricAtoms[atom1Tag]);
        bond->setAtom2(asymmetricAtoms[atom2Tag]);

        for(std::shared_ptr<SKBond> &bond: bond->copies())
        {
          int tag1 = bond->getTag1();
          int tag2 = bond->getTag2();
          bond->setAtoms(atomCopies[tag1],atomCopies[tag2]);
        }
      }
    }
    else
    {
      qint64 index=0;
      for(std::shared_ptr<SKAtomCopy> atom: atomCopies)
      {
        atom->setTag(index);
        index++;
      }

      qint64 vecSize;
      std::vector<std::shared_ptr<SKBond>> bonds;
      stream >> vecSize;
      qint64 atomtag1;
      qint64 atomtag2;
      qint64 type;
      while(vecSize--)
      {
        stream >> atomtag1;
        stream >> atomtag2;
        qDebug() << "atomtags" << atomtag1 << ", " << atomtag2;
        stream >> type;
        std::shared_ptr<SKBond> bond = std::make_shared<SKBond>(atomCopies[atomtag1],atomCopies[atomtag2], SKBond::BoundaryType(type));
        bonds.push_back(bond);
      }
      controller->setBonds(bonds);
    }
  }
  else
  {
    stream >> controller->_arrangedObjects;

    // fill in the atoms from the tags
    for(std::shared_ptr<SKAsymmetricBond> &bond: controller->_arrangedObjects)
    {
      int atom1Tag = bond->getTag1();
      int atom2Tag = bond->getTag2();
      bond->setAtom1(asymmetricAtoms[atom1Tag]);
      bond->setAtom2(asymmetricAtoms[atom2Tag]);

      for(std::shared_ptr<SKBond> &bond: bond->copies())
      {
        int tag1 = bond->getTag1();
        int tag2 = bond->getTag2();
        bond->setAtoms(atomCopies[tag1],atomCopies[tag2]);
      }
    }
  }

  return stream;
}


