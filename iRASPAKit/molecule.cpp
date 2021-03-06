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

#include <algorithm>
#include "crystal.h"
#include "molecule.h"
#include "molecularcrystal.h"
#include "proteincrystal.h"
#include "protein.h"
#include "ellipsoidprimitive.h"
#include "cylinderprimitive.h"
#include "polygonalprismprimitive.h"
#include "crystalellipsoidprimitive.h"
#include "crystalcylinderprimitive.h"
#include "crystalpolygonalprismprimitive.h"

Molecule::Molecule()
{

}

Molecule::Molecule(const Molecule &molecule): Structure(molecule)
{
}

Molecule::Molecule(std::shared_ptr<SKStructure> structure): Structure(structure)
{
  expandSymmetry();
  _atomsTreeController->setTags();
}

Molecule::Molecule(std::shared_ptr<Structure> s): Structure(s)
{
  _cell = std::make_shared<SKCell>(*s->cell());

  if(dynamic_cast<Crystal*>(s.get()) ||
     dynamic_cast<CrystalEllipsoidPrimitive*>(s.get()) ||
     dynamic_cast<CrystalCylinderPrimitive*>(s.get()) ||
     dynamic_cast<CrystalPolygonalPrismPrimitive*>(s.get()))
  {
    convertAsymmetricAtomsToCartesian();
  }

  expandSymmetry();
  _atomsTreeController->setTags();
  reComputeBoundingBox();
  computeBonds();
}

std::shared_ptr<Structure> Molecule::clone()
{
  return std::make_shared<Molecule>(static_cast<const Molecule&>(*this));
}

// MARK: Rendering
// =====================================================================

std::vector<RKInPerInstanceAttributesAtoms> Molecule::renderAtoms() const
{
  std::vector<std::shared_ptr<SKAtomTreeNode>> asymmetricAtomNodes = _atomsTreeController->flattenedLeafNodes();

  std::vector<RKInPerInstanceAttributesAtoms> atomData = std::vector<RKInPerInstanceAttributesAtoms>();

  uint32_t asymmetricAtomIndex = 0;
  for (std::shared_ptr<SKAtomTreeNode> node : asymmetricAtomNodes)
  {
    if (std::shared_ptr<SKAsymmetricAtom> atom = node->representedObject())
    {
      bool isVisible = atom->isVisible();

      if(isVisible)
      {
        for (std::shared_ptr<SKAtomCopy> copy : atom->copies())
        {
          if (copy->type() == SKAtomCopy::AtomCopyType::copy)
          {
            QColor color = atom->color();

            float4 position = float4(copy->position(), 1.0);

            float4 ambient = float4(color.redF(), color.greenF(), color.blueF(), color.alphaF());
            float4 diffuse = float4(color.redF(), color.greenF(), color.blueF(), color.alphaF());

            float4 specular = float4(1.0, 1.0, 1.0, 1.0);

            double radius = atom->drawRadius() * atom->occupancy();
            float4 scale = float4(radius, radius, radius, 1.0);

            RKInPerInstanceAttributesAtoms atom1 = RKInPerInstanceAttributesAtoms(position, ambient, diffuse, specular, scale, asymmetricAtomIndex);
            atomData.push_back(atom1);
          }
        }
      }
    }
    asymmetricAtomIndex++;
  }

  return atomData;
}

std::vector<RKInPerInstanceAttributesBonds> Molecule::renderInternalBonds() const
{
  std::vector<RKInPerInstanceAttributesBonds> data = std::vector<RKInPerInstanceAttributesBonds>();

  const std::vector<std::shared_ptr<SKAsymmetricBond>> asymmetricBonds = _bondSetController->arrangedObjects();

  uint32_t asymmetricBondIndex = 0;
  for(std::shared_ptr<SKAsymmetricBond> asymmetricBond : asymmetricBonds)
  {
    bool isVisible = asymmetricBond->isVisible() && asymmetricBond->atom1()->isVisible() && asymmetricBond->atom2()->isVisible();

    if(isVisible)
    {
      const std::vector<std::shared_ptr<SKBond>> bonds = asymmetricBond->copies();

      for(std::shared_ptr<SKBond> bond : bonds)
      {
        if (bond->boundaryType() == SKBond::BoundaryType::internal)
        {
          QColor color1 = bond->atom1()->parent()->color();
          QColor color2 = bond->atom2()->parent()->color();

          double3 pos1 = bond->atom1()->position();
          double3 pos2 = bond->atom2()->position();
          double bondLength = (pos2 - pos1).length();
          double drawRadius1 = bond->atom1()->parent()->drawRadius() / bondLength;
          double drawRadius2 = bond->atom2()->parent()->drawRadius() / bondLength;

          RKInPerInstanceAttributesBonds bondData = RKInPerInstanceAttributesBonds(
                float4(pos1, 1.0),
                float4(pos2, 1.0),
                float4(color1.redF(), color1.greenF(), color1.blueF(), color1.alphaF()),
                float4(color2.redF(), color2.greenF(), color2.blueF(), color2.alphaF()),
                float4(drawRadius1, std::min(bond->atom1()->parent()->occupancy(), bond->atom2()->parent()->occupancy()), drawRadius2, drawRadius1 / drawRadius2),
                asymmetricBondIndex,
                static_cast<typename std::underlying_type<SKAsymmetricBond::SKBondType>::type>(asymmetricBond->getBondType()));
          data.push_back(bondData);
        }
      }
    }
    asymmetricBondIndex++;
  }

  return data;
}


// MARK: Rendering selection
// =====================================================================

std::vector<RKInPerInstanceAttributesAtoms> Molecule::renderSelectedAtoms() const
{
  std::set<std::shared_ptr<SKAtomTreeNode>> asymmetricAtomNodes = _atomsTreeController->selectedTreeNodes();

  std::vector<RKInPerInstanceAttributesAtoms> atomData = std::vector<RKInPerInstanceAttributesAtoms>();

  uint32_t asymmetricAtomIndex = 0;
  for (std::shared_ptr<SKAtomTreeNode> node : asymmetricAtomNodes)
  {
    if (std::shared_ptr<SKAsymmetricAtom> atom = node->representedObject())
    {
      bool isVisble = atom->isVisible();

      if(isVisble)
      {
        for (std::shared_ptr<SKAtomCopy> copy : atom->copies())
        {
          if (copy->type() == SKAtomCopy::AtomCopyType::copy)
          {
            QColor color = atom->color();

            float4 position = float4(copy->position(), 1.0);

            float4 ambient = float4(color.redF(), color.greenF(), color.blueF(), color.alphaF());
            float4 diffuse = float4(color.redF(), color.greenF(), color.blueF(), color.alphaF());

            float4 specular = float4(1.0, 1.0, 1.0, 1.0);

            double radius = atom->drawRadius() * atom->occupancy();
            float4 scale = float4(radius, radius, radius, 1.0);

            RKInPerInstanceAttributesAtoms atom1 = RKInPerInstanceAttributesAtoms(position, ambient, diffuse, specular, scale, asymmetricAtomIndex);
            atomData.push_back(atom1);
          }
        }
      }
    }
    asymmetricAtomIndex++;
  }

  return atomData;
}

std::vector<RKInPerInstanceAttributesBonds> Molecule::renderSelectedInternalBonds() const
{
  std::vector<RKInPerInstanceAttributesBonds> data =  std::vector<RKInPerInstanceAttributesBonds>();

  const std::vector<std::shared_ptr<SKAsymmetricBond>> asymmetricBonds = _bondSetController->arrangedObjects();

  for(int asymmetricBondIndex: _bondSetController->selectionIndexSet())
  {
    std::shared_ptr<SKAsymmetricBond> asymmetricBond = _bondSetController->arrangedObjects()[asymmetricBondIndex];
    bool isVisible = asymmetricBond->isVisible() && asymmetricBond->atom1()->isVisible()  && asymmetricBond->atom2()->isVisible();

    if(isVisible)
    {
      const std::vector<std::shared_ptr<SKBond>> bonds = asymmetricBond->copies();
      for(std::shared_ptr<SKBond> bond : bonds)
      {
        if(bond->atom1()->type() == SKAtomCopy::AtomCopyType::copy && bond->atom2()->type() == SKAtomCopy::AtomCopyType::copy && bond->boundaryType() == SKBond::BoundaryType::internal)
        {
          QColor color1 = bond->atom1()->parent()->color();
          QColor color2 = bond->atom2()->parent()->color();

          double3 pos1 = bond->atom1()->position();
          double3 pos2 = bond->atom2()->position();
          double bondLength = (pos2-pos1).length();
          double drawRadius1 = bond->atom1()->parent()->drawRadius()/bondLength;
          double drawRadius2 = bond->atom2()->parent()->drawRadius()/bondLength;

          RKInPerInstanceAttributesBonds bondData = RKInPerInstanceAttributesBonds(
                     float4(pos1,1.0),
                     float4(pos2,1.0),
                     float4(color1.redF(),color1.greenF(),color1.blueF(),color1.alphaF()),
                     float4(color2.redF(),color2.greenF(),color2.blueF(),color2.alphaF()),
                     float4(drawRadius1, std::min(bond->atom1()->parent()->occupancy(),bond->atom2()->parent()->occupancy()), drawRadius2, drawRadius1/drawRadius2),
                     asymmetricBondIndex,
                     static_cast<typename std::underlying_type<SKAsymmetricBond::SKBondType>::type>(asymmetricBond->getBondType()));
          data.push_back(bondData);
        }
      }
    }
  }

  return data;
}

// MARK: Filtering
// =====================================================================

std::set<int> Molecule::filterCartesianAtomPositions(std::function<bool(double3)> & closure)
{
  std::set<int> data;

  double4x4 rotationMatrix = double4x4::AffinityMatrixToTransformationAroundArbitraryPoint(double4x4(_orientation), boundingBox().center());

  std::vector<std::shared_ptr<SKAtomTreeNode>> asymmetricAtomNodes = _atomsTreeController->flattenedLeafNodes();

  uint32_t asymmetricAtomIndex = 0;
  for(std::shared_ptr<SKAtomTreeNode> node: asymmetricAtomNodes)
  {
    if(std::shared_ptr<SKAsymmetricAtom> atom = node->representedObject())
    {
      if (atom->isVisible())
      {
        for(std::shared_ptr<SKAtomCopy> copy : atom->copies())
        {
          if(copy->type() == SKAtomCopy::AtomCopyType::copy)
          {
            double3 cartesianPosition = copy->position();

            double4 position = rotationMatrix * double4(cartesianPosition.x, cartesianPosition.y, cartesianPosition.z, 1.0);

            double3 absoluteCartesianPosition = double3(position.x,position.y,position.z) + _origin;
            if(closure(absoluteCartesianPosition))
            {
              data.insert(asymmetricAtomIndex);
            }
          }
        }
      }
    }
    asymmetricAtomIndex++;
  }

  return data;
}


std::set<int> Molecule::filterCartesianBondPositions(std::function<bool(double3)> &closure)
{
  std::set<int> data;

  const std::vector<std::shared_ptr<SKAsymmetricBond>> asymmetricBonds = _bondSetController->arrangedObjects();

  double4x4 rotationMatrix = double4x4::AffinityMatrixToTransformationAroundArbitraryPoint(double4x4(_orientation), boundingBox().center());

  uint32_t asymmetricBondIndex = 0;
  for(std::shared_ptr<SKAsymmetricBond> asymmetricBond : asymmetricBonds)
  {
    bool isVisible = asymmetricBond->isVisible() && asymmetricBond->atom1()->isVisible()  && asymmetricBond->atom2()->isVisible();

    if (isVisible)
    {
      const std::vector<std::shared_ptr<SKBond>> bonds = asymmetricBond->copies();
      for(std::shared_ptr<SKBond> bond : bonds)
      {
        double3 cartesianPosition = 0.5 * (bond->atom1()->position()+bond->atom2()->position());

        double4 position = rotationMatrix * double4(cartesianPosition.x, cartesianPosition.y, cartesianPosition.z, 1.0);
        double3 absoluteCartesianPosition = double3(position.x,position.y,position.z) + _origin;
        if (closure(absoluteCartesianPosition))
        {
          data.insert(asymmetricBondIndex);
        }
      }
    }
    asymmetricBondIndex++;
  }

  return data;
}


// MARK: Symmetry
// =====================================================================


void Molecule::expandSymmetry()
{
	std::vector<std::shared_ptr<SKAtomTreeNode>> asymmetricAtomNodes = _atomsTreeController->flattenedLeafNodes();

	int index = 0;
	for (std::shared_ptr<SKAtomTreeNode> node : asymmetricAtomNodes)
	{
		std::vector<std::shared_ptr<SKAtomCopy>> atomCopies = std::vector<std::shared_ptr<SKAtomCopy>>{};
		if (std::shared_ptr<SKAsymmetricAtom> asymmetricAtom = node->representedObject())
		{
			std::vector<std::shared_ptr<SKAtomCopy>> atomCopies = std::vector<std::shared_ptr<SKAtomCopy>>{};

			std::shared_ptr<SKAtomCopy> newAtom = std::make_shared<SKAtomCopy>(asymmetricAtom, asymmetricAtom->position());
			newAtom->setType(SKAtomCopy::AtomCopyType::copy);
			atomCopies.push_back(newAtom);
			
			asymmetricAtom->setCopies(atomCopies);
		}
	}
}

void Molecule::expandSymmetry(std::shared_ptr<SKAsymmetricAtom> asymmetricAtom)
{
	std::vector<std::shared_ptr<SKAtomCopy>> atomCopies = std::vector<std::shared_ptr<SKAtomCopy>>{};

	std::shared_ptr<SKAtomCopy> newAtom = std::make_shared<SKAtomCopy>(asymmetricAtom, asymmetricAtom->position());
	newAtom->setType(SKAtomCopy::AtomCopyType::copy);
	atomCopies.push_back(newAtom);

	asymmetricAtom->setCopies(atomCopies);
}

std::shared_ptr<Structure> Molecule::flattenHierarchy() const
{
  std::shared_ptr<Molecule> crystal = std::make_shared<Molecule>(static_cast<const Molecule&>(*this));

  const std::vector<std::shared_ptr<SKAtomTreeNode>> asymmetricAtomNodes = _atomsTreeController->flattenedLeafNodes();

  for(const std::shared_ptr<SKAtomTreeNode> node: asymmetricAtomNodes)
  {
    if(const std::shared_ptr<SKAsymmetricAtom> asymmetricAtom = node->representedObject())
    {
      std::shared_ptr<SKAsymmetricAtom> newAsymmetricAtom = std::make_shared<SKAsymmetricAtom>(*asymmetricAtom);
      for(std::shared_ptr<SKAtomCopy> atomCopy : asymmetricAtom->copies())
      {
        std::shared_ptr<SKAtomCopy> newAtomCopy = std::make_shared<SKAtomCopy>(newAsymmetricAtom, atomCopy->position());
        newAsymmetricAtom->copies().push_back(newAtomCopy);
      }
      std::shared_ptr<SKAtomTreeNode> atomTreeNode = std::make_shared<SKAtomTreeNode>(newAsymmetricAtom);
      crystal->atomsTreeController()->appendToRootnodes(atomTreeNode);
    }
  }

  crystal->atomsTreeController()->setTags();
  crystal->reComputeBoundingBox();
  crystal->computeBonds();

  return crystal;
}

std::optional<std::pair<std::shared_ptr<SKCell>, double3>> Molecule::cellForFractionalPositions()
{
  SKBoundingBox boundingBox = _cell->boundingBox() + SKBoundingBox(double3(-2,-2,-2), double3(2,2,2));
  SKCell cell = SKCell(boundingBox);
  return std::make_pair(std::make_shared<SKCell>(cell), cell.inverseUnitCell() * (boundingBox.minimum()));
}

std::optional<std::pair<std::shared_ptr<SKCell>, double3>> Molecule::cellForCartesianPositions()
{
  return std::nullopt;
}

std::vector<std::shared_ptr<SKAsymmetricAtom>> Molecule::asymmetricAtomsCopiedAndTransformedToFractionalPositions()
{
  std::vector<std::shared_ptr<SKAsymmetricAtom>> atoms{};

  SKBoundingBox boundingBox = _cell->boundingBox() + SKBoundingBox(double3(-2,-2,-2), double3(2,2,2));
  double3x3 inverseUnitCell = SKCell(boundingBox).inverseUnitCell();
  const std::vector<std::shared_ptr<SKAtomTreeNode>> asymmetricAtomNodes = _atomsTreeController->flattenedLeafNodes();
  for(const std::shared_ptr<SKAtomTreeNode> node: asymmetricAtomNodes)
  {
    if(const std::shared_ptr<SKAsymmetricAtom> asymmetricAtom = node->representedObject())
    {
      std::shared_ptr<SKAsymmetricAtom> newAsymmetricAtom = std::make_shared<SKAsymmetricAtom>(*asymmetricAtom);
      newAsymmetricAtom->setPosition(inverseUnitCell * asymmetricAtom->position());
      atoms.push_back(newAsymmetricAtom);
    }
  }

  return atoms;
}

std::vector<std::shared_ptr<SKAsymmetricAtom>> Molecule::asymmetricAtomsCopiedAndTransformedToCartesianPositions()
{
  std::vector<std::shared_ptr<SKAsymmetricAtom>> atoms{};

  const std::vector<std::shared_ptr<SKAtomTreeNode>> asymmetricAtomNodes = _atomsTreeController->flattenedLeafNodes();
  for(const std::shared_ptr<SKAtomTreeNode> node: asymmetricAtomNodes)
  {
    if(const std::shared_ptr<SKAsymmetricAtom> asymmetricAtom = node->representedObject())
    {
      atoms.push_back(std::make_shared<SKAsymmetricAtom>(*asymmetricAtom));
    }
  }

  return atoms;
}

std::vector<std::shared_ptr<SKAsymmetricAtom>> Molecule::atomsCopiedAndTransformedToFractionalPositions()
{
  std::vector<std::shared_ptr<SKAsymmetricAtom>> atoms{};

  SKBoundingBox boundingBox = _cell->boundingBox() + SKBoundingBox(double3(-2,-2,-2), double3(2,2,2));
  double3x3 inverseUnitCell = SKCell(boundingBox).inverseUnitCell();
  const std::vector<std::shared_ptr<SKAtomTreeNode>> asymmetricAtomNodes = _atomsTreeController->flattenedLeafNodes();
  for(const std::shared_ptr<SKAtomTreeNode> node: asymmetricAtomNodes)
  {
    if(const std::shared_ptr<SKAsymmetricAtom> asymmetricAtom = node->representedObject())
    {
      for (std::shared_ptr<SKAtomCopy> copy : asymmetricAtom->copies())
      {
        if (copy->type() == SKAtomCopy::AtomCopyType::copy)
        {
          std::shared_ptr<SKAsymmetricAtom> newAsymmetricAtom = std::make_shared<SKAsymmetricAtom>(*asymmetricAtom);
          newAsymmetricAtom->setPosition(inverseUnitCell * copy->position());
          atoms.push_back(newAsymmetricAtom);
        }
      }
    }
  }

  return atoms;
}

std::vector<std::shared_ptr<SKAsymmetricAtom>> Molecule::atomsCopiedAndTransformedToCartesianPositions()
{
  std::vector<std::shared_ptr<SKAsymmetricAtom>> atoms{};

  const std::vector<std::shared_ptr<SKAtomTreeNode>> asymmetricAtomNodes = _atomsTreeController->flattenedLeafNodes();
  for(const std::shared_ptr<SKAtomTreeNode> node: asymmetricAtomNodes)
  {
    if(const std::shared_ptr<SKAsymmetricAtom> asymmetricAtom = node->representedObject())
    {
      for (std::shared_ptr<SKAtomCopy> copy : asymmetricAtom->copies())
      {
        if (copy->type() == SKAtomCopy::AtomCopyType::copy)
        {
          std::shared_ptr<SKAsymmetricAtom> newAsymmetricAtom = std::make_shared<SKAsymmetricAtom>(*asymmetricAtom);
          newAsymmetricAtom->setPosition(copy->position());
          atoms.push_back(newAsymmetricAtom);
        }
      }
    }
  }

  return atoms;
}


std::vector<double3> Molecule::atomPositions() const
{
	std::vector<std::shared_ptr<SKAtomTreeNode>> asymmetricAtomNodes = _atomsTreeController->flattenedLeafNodes();

	std::vector<double3> atomData = std::vector<double3>();


	for (std::shared_ptr<SKAtomTreeNode> node : asymmetricAtomNodes)
	{
		if (std::shared_ptr<SKAsymmetricAtom> atom = node->representedObject())
		{
			for (std::shared_ptr<SKAtomCopy> copy : atom->copies())
			{
				if (copy->type() == SKAtomCopy::AtomCopyType::copy)
				{
					double3 pos = copy->position();
					
					double4x4 rotationMatrix = double4x4::AffinityMatrixToTransformationAroundArbitraryPoint(double4x4(_orientation), boundingBox().center());

					double4 position = rotationMatrix * double4(pos.x, pos.y, pos.z, 1.0);

					atomData.push_back(double3(position.x, position.y, position.z));
				}
			}
		}
	}

	return atomData;
}

double Molecule::bondLength(std::shared_ptr<SKBond> bond) const
{
	double3 pos1 = bond->atom1()->position();
	double3 pos2 = bond->atom2()->position();
	return (pos2 - pos1).length();
}

double3 Molecule::bondVector(std::shared_ptr<SKBond> bond) const
{
  double3 pos1 = bond->atom1()->position();
  double3 pos2 = bond->atom2()->position();
  double3 ds = pos2 - pos1;
  return ds;
}

std::pair<double3, double3> Molecule::computeChangedBondLength(std::shared_ptr<SKBond> bond, double bondLength) const
{
  double3 pos1 = bond->atom1()->position();
  std::shared_ptr<SKAsymmetricAtom> asymmetricAtom1 = bond->atom1()->parent();

  double3 pos2 = bond->atom2()->position();
  std::shared_ptr<SKAsymmetricAtom> asymmetricAtom2 = bond->atom2()->parent();

  double oldBondLength = this->bondLength(bond);

  double3 bondVector = this->bondVector(bond).normalise();

  bool isAllFixed1 = asymmetricAtom1->isFixed().x && asymmetricAtom1->isFixed().y && asymmetricAtom1->isFixed().z;
  bool isAllFixed2 = asymmetricAtom2->isFixed().x && asymmetricAtom2->isFixed().y && asymmetricAtom2->isFixed().z;

  if(!isAllFixed1 && !isAllFixed2)
  {
    double3 newPos1 = pos1 - 0.5 * (bondLength - oldBondLength) * bondVector;
    double3 newPos2 = pos2 + 0.5 * (bondLength - oldBondLength) * bondVector;
    return std::make_pair(newPos1, newPos2);
  }
  else if(isAllFixed1 && !isAllFixed2)
  {
    double3 newPos2 = pos1 + bondLength * bondVector;
    return std::make_pair(pos1, newPos2);
  }
  else if(!isAllFixed1 && isAllFixed2)
  {
    double3 newPos1 = pos2 - bondLength * bondVector;
    return std::make_pair(newPos1, pos2);
  }

  return std::make_pair(pos1,pos2);
}


SKBoundingBox Molecule::boundingBox() const
{
	double3 minimum = double3(1e10, 1e10, 1e10);
	double3 maximum = double3(-1e10, -1e10, -1e10);

	std::vector<std::shared_ptr<SKAtomCopy>> atoms = _atomsTreeController->atomCopies();

	if (atoms.empty())
	{
		return SKBoundingBox(double3(0.0,0.0,0.0), double3(0.0, 0.0, 0.0));
	}

	for (std::shared_ptr<SKAtomCopy> atom : atoms)
	{
		double3 CartesianPosition = atom->position();
		minimum.x = std::min(minimum.x, CartesianPosition.x);
		minimum.y = std::min(minimum.y, CartesianPosition.y);
		minimum.z = std::min(minimum.z, CartesianPosition.z);
		maximum.x = std::max(maximum.x, CartesianPosition.x);
    maximum.y = std::max(maximum.y, CartesianPosition.y);
    maximum.z = std::max(maximum.z, CartesianPosition.z);
	}

	return SKBoundingBox(minimum, maximum);
}


void Molecule::computeBonds()
{
  std::vector<std::shared_ptr<SKAtomCopy>> copies = _atomsTreeController->atomCopies();

  std::vector<std::shared_ptr<SKBond>> bonds;

  for (size_t i = 0;i < copies.size();i++)
  {
    copies[i]->setType(SKAtomCopy::AtomCopyType::copy);
    double3 posA = copies[i]->position();
    int elementIdentifierA = copies[i]->parent()->elementIdentifier();
    double covalentRadiusA = PredefinedElements::predefinedElements[elementIdentifierA]._covalentRadius;
    for (size_t j = i + 1;j < copies.size();j++)
    {
      if ((copies[i]->parent()->occupancy() == 1.0 && copies[j]->parent()->occupancy() == 1.0) ||
        (copies[i]->parent()->occupancy() < 1.0 && copies[j]->parent()->occupancy() < 1.0))
      {
        double3 posB = copies[j]->position();
        int elementIdentifierB = copies[j]->parent()->elementIdentifier();
        double covalentRadiusB = PredefinedElements::predefinedElements[elementIdentifierB]._covalentRadius;

        double length = (posA - posB).length();

        if (length < covalentRadiusA + covalentRadiusB + 0.56)
        {
          if (length < 0.1)
          {
            copies[i]->setType(SKAtomCopy::AtomCopyType::duplicate);
          }

          std::shared_ptr<SKBond> bond = std::make_shared<SKBond>(copies[i], copies[j]);
          bonds.push_back(bond);
        }
      }
    }
  }

  std::vector<std::shared_ptr<SKBond>> filtered_bonds;
  std::copy_if (bonds.begin(), bonds.end(), std::back_inserter(filtered_bonds), [](std::shared_ptr<SKBond> i){return i->atom1()->type() == SKAtomCopy::AtomCopyType::copy && i->atom2()->type() == SKAtomCopy::AtomCopyType::copy;} );
  _bondSetController->setBonds(filtered_bonds);
}

double3x3 Molecule::unitCell()
{
  SKCell boundaryBoxCell = SKCell(_cell->boundingBox());
  return boundaryBoxCell.unitCell();
}

QDataStream &operator<<(QDataStream &stream, const std::shared_ptr<Molecule> &molecule)
{
  stream << molecule->_versionNumber;
  return stream;
}

QDataStream &operator>>(QDataStream &stream, std::shared_ptr<Molecule> &molecule)
{
  qint64 versionNumber;
  stream >> versionNumber;
  if(versionNumber > molecule->_versionNumber)
  {
    throw InvalidArchiveVersionException(__FILE__, __LINE__, "Molecule");
  }
  return stream;
}
