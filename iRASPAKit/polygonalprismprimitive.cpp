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

PolygonalPrismPrimitive::PolygonalPrismPrimitive()
{

}

PolygonalPrismPrimitive::PolygonalPrismPrimitive(const PolygonalPrismPrimitive &polygonalPrismPrimitive): Structure(polygonalPrismPrimitive)
{
}

PolygonalPrismPrimitive::PolygonalPrismPrimitive(std::shared_ptr<Structure> s): Structure(s)
{
  _cell = std::make_shared<SKCell>(*s->cell());

  if(dynamic_cast<Crystal*>(s.get()) ||
     dynamic_cast<CrystalEllipsoidPrimitive*>(s.get()) ||
     dynamic_cast<CrystalCylinderPrimitive*>(s.get()) ||
     dynamic_cast<CrystalPolygonalPrismPrimitive*>(s.get()))
  {
    convertAsymmetricAtomsToFractional();
  }

  expandSymmetry();
  _atomsTreeController->setTags();
  reComputeBoundingBox();
  computeBonds();
}

// MARK: Rendering
// =====================================================================

// MARK: Rendering selection
// =====================================================================


// MARK: Filtering
// =====================================================================

// MARK: Bounding box
// =====================================================================

SKBoundingBox PolygonalPrismPrimitive::boundingBox() const
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

// MARK: Symmetry
// =====================================================================

void PolygonalPrismPrimitive::expandSymmetry()
{
  std::vector<std::shared_ptr<SKAtomTreeNode>> asymmetricAtomNodes = _atomsTreeController->flattenedLeafNodes();

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

std::optional<std::pair<std::shared_ptr<SKCell>, double3>> PolygonalPrismPrimitive::cellForFractionalPositions()
{
  SKBoundingBox boundingBox = _cell->boundingBox() + SKBoundingBox(double3(-2,-2,-2), double3(2,2,2));
  SKCell cell = SKCell(boundingBox);
  return std::make_pair(std::make_shared<SKCell>(cell), cell.inverseUnitCell() * (boundingBox.minimum()));
}

std::optional<std::pair<std::shared_ptr<SKCell>, double3>> PolygonalPrismPrimitive::cellForCartesianPositions()
{
  return std::nullopt;
}

std::vector<std::shared_ptr<SKAsymmetricAtom>> PolygonalPrismPrimitive::asymmetricAtomsCopiedAndTransformedToFractionalPositions()
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

std::vector<std::shared_ptr<SKAsymmetricAtom>> PolygonalPrismPrimitive::asymmetricAtomsCopiedAndTransformedToCartesianPositions()
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

std::vector<std::shared_ptr<SKAsymmetricAtom>> PolygonalPrismPrimitive::atomsCopiedAndTransformedToFractionalPositions()
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

std::vector<std::shared_ptr<SKAsymmetricAtom>> PolygonalPrismPrimitive::atomsCopiedAndTransformedToCartesianPositions()
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


// MARK: bond-computations
// =====================================================================


std::vector<RKInPerInstanceAttributesAtoms> PolygonalPrismPrimitive::renderPrimitivePolygonalPrismObjects() const
{
  std::vector<RKInPerInstanceAttributesAtoms> atomData;

  std::vector<std::shared_ptr<SKAtomTreeNode>> asymmetricAtomNodes = _atomsTreeController->flattenedLeafNodes();

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



QDataStream &operator<<(QDataStream &stream, const std::shared_ptr<PolygonalPrismPrimitive> &primitive)
{
  stream << primitive->_versionNumber;
  return stream;
}

QDataStream &operator>>(QDataStream &stream, std::shared_ptr<PolygonalPrismPrimitive> &primitive)
{
  qint64 versionNumber;
  stream >> versionNumber;
  if(versionNumber > primitive->_versionNumber)
  {
    throw InvalidArchiveVersionException(__FILE__, __LINE__, "PolygonalPrismPrimitive");
  }
  return stream;
}
