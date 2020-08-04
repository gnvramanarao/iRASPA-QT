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

ProteinCrystal::ProteinCrystal()
{
}

ProteinCrystal::ProteinCrystal(const ProteinCrystal &proteinCrystal): Structure(proteinCrystal)
{
}


ProteinCrystal::ProteinCrystal(std::shared_ptr<SKStructure> structure): Structure(structure)
{
  expandSymmetry();
  _atomsTreeController->setTags();
}

ProteinCrystal::ProteinCrystal(std::shared_ptr<Structure> s): Structure(s)
{
  if(dynamic_cast<Molecule*>(s.get()) ||
     dynamic_cast<Protein*>(s.get()) ||
     dynamic_cast<EllipsoidPrimitive*>(s.get()) ||
     dynamic_cast<CylinderPrimitive*>(s.get()) ||
     dynamic_cast<PolygonalPrismPrimitive*>(s.get()))
  {
    // create a periodic cell based on the bounding-box
    _cell = std::make_shared<SKCell>(s->boundingBox());
  }
  else
  {
    _cell = std::make_shared<SKCell>(*s->cell());
  }

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

// MARK: Rendering
// =====================================================================

std::vector<RKInPerInstanceAttributesAtoms> ProteinCrystal::renderAtoms() const
{
  int minimumReplicaX = _cell->minimumReplicaX();
  int minimumReplicaY = _cell->minimumReplicaY();
  int minimumReplicaZ = _cell->minimumReplicaZ();

  int maximumReplicaX = _cell->maximumReplicaX();
  int maximumReplicaY = _cell->maximumReplicaY();
  int maximumReplicaZ = _cell->maximumReplicaZ();

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
        QColor color = atom->color();
        float4 ambient = float4(color.redF(), color.greenF(), color.blueF(), color.alphaF());
        float4 diffuse = float4(color.redF(), color.greenF(), color.blueF(), color.alphaF());

        float4 specular = float4(1.0, 1.0, 1.0, 1.0);

        double radius = atom->drawRadius() * atom->occupancy();
        float4 scale = float4(radius, radius, radius, 1.0);
        for (std::shared_ptr<SKAtomCopy> copy : atom->copies())
        {
          if (copy->type() == SKAtomCopy::AtomCopyType::copy)
          {
            for (int k1 = minimumReplicaX;k1 <= maximumReplicaX;k1++)
            {
              for (int k2 = minimumReplicaY;k2 <= maximumReplicaY;k2++)
              {
                for (int k3 = minimumReplicaZ;k3 <= maximumReplicaZ;k3++)
                {
                  float4 position = float4(copy->position() + _cell->unitCell() * double3(k1, k2, k3), 1.0);

                  RKInPerInstanceAttributesAtoms atom1 = RKInPerInstanceAttributesAtoms(position, ambient, diffuse, specular, scale, asymmetricAtomIndex);
                  atomData.push_back(atom1);
                }
              }
            }
          }
        }
      }
    }
    asymmetricAtomIndex++;
  }

  return atomData;
}

std::vector<RKInPerInstanceAttributesBonds> ProteinCrystal::renderInternalBonds() const
{
  int minimumReplicaX = _cell->minimumReplicaX();
  int minimumReplicaY = _cell->minimumReplicaY();
  int minimumReplicaZ = _cell->minimumReplicaZ();

  int maximumReplicaX = _cell->maximumReplicaX();
  int maximumReplicaY = _cell->maximumReplicaY();
  int maximumReplicaZ = _cell->maximumReplicaZ();

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
          for (int k1 = minimumReplicaX;k1 <= maximumReplicaX;k1++)
          {
            for (int k2 = minimumReplicaY;k2 <= maximumReplicaY;k2++)
            {
              for (int k3 = minimumReplicaZ;k3 <= maximumReplicaZ;k3++)
              {
                double3 pos1 = bond->atom1()->position() + _cell->unitCell() * double3(k1, k2, k3);
                double3 pos2 = bond->atom2()->position() + _cell->unitCell() * double3(k1, k2, k3);
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
        }
      }
    }
    asymmetricBondIndex++;
  }

  return data;
}


std::vector<RKInPerInstanceAttributesAtoms> ProteinCrystal::renderUnitCellSpheres() const
{
  int minimumReplicaX = _cell->minimumReplicaX();
  int minimumReplicaY = _cell->minimumReplicaY();
  int minimumReplicaZ = _cell->minimumReplicaZ();

  int maximumReplicaX = _cell->maximumReplicaX();
  int maximumReplicaY = _cell->maximumReplicaY();
  int maximumReplicaZ = _cell->maximumReplicaZ();

  std::vector<RKInPerInstanceAttributesAtoms> data = std::vector<RKInPerInstanceAttributesAtoms>();

  uint32_t asymmetricBondIndex=0;
  for (int k1 = minimumReplicaX;k1 <= maximumReplicaX + 1;k1++)
  {
    for (int k2 = minimumReplicaY;k2 <= maximumReplicaY + 1;k2++)
    {
      for (int k3 = minimumReplicaZ;k3 <= maximumReplicaZ + 1;k3++)
      {
        double3 position = _cell->unitCell() * double3(k1, k2, k3); // + origin();
        float4 ambient = float4(1.0f, 1.0f, 1.0f, 1.0f);
        float4 diffuse = float4(1.0f, 1.0f, 1.0f, 1.0f);
        float4 specular = float4(1.0f, 1.0f, 1.0f, 1.0f);
        float4 scale = float4(0.1f, 0.1f, 0.1f, 1.0f);
        RKInPerInstanceAttributesAtoms sphere = RKInPerInstanceAttributesAtoms(float4(position, 1.0), ambient, diffuse, specular, scale, asymmetricBondIndex);
        data.push_back(sphere);
      }
    }
    asymmetricBondIndex++;
  }

  return data;
}

std::vector<RKInPerInstanceAttributesBonds> ProteinCrystal::renderUnitCellCylinders() const
{
  int minimumReplicaX = _cell->minimumReplicaX();
  int minimumReplicaY = _cell->minimumReplicaY();
  int minimumReplicaZ = _cell->minimumReplicaZ();

  int maximumReplicaX = _cell->maximumReplicaX();
  int maximumReplicaY = _cell->maximumReplicaY();
  int maximumReplicaZ = _cell->maximumReplicaZ();

  std::vector<RKInPerInstanceAttributesBonds> data = std::vector<RKInPerInstanceAttributesBonds>();

  for (int k1 = minimumReplicaX;k1 <= maximumReplicaX + 1;k1++)
  {
    for (int k2 = minimumReplicaY;k2 <= maximumReplicaY + 1;k2++)
    {
      for (int k3 = minimumReplicaZ;k3 <= maximumReplicaZ + 1;k3++)
      {
        if (k1 <= maximumReplicaX)
        {
          double3 position1 = _cell->unitCell() * double3(k1, k2, k3);
          double3 position2 = _cell->unitCell() * double3(k1 + 1, k2, k3);
          float4 scale = float4(0.1f, 1.0f, 0.1f, 1.0f);
          RKInPerInstanceAttributesBonds cylinder =
            RKInPerInstanceAttributesBonds(float4(position1, 1.0),
              float4(position2, 1.0),
              float4(1.0f, 1.0f, 1.0f, 1.0f),
              float4(1.0f, 1.0f, 1.0f, 1.0f),
              scale,0,0);
          data.push_back(cylinder);
        }

        if (k2 <= maximumReplicaY)
        {
          double3 position1 = _cell->unitCell() * double3(k1, k2, k3);
          double3 position2 = _cell->unitCell() * double3(k1, k2 + 1, k3);
          float4 scale = float4(0.1f, 1.0f, 0.1f, 1.0f);
          RKInPerInstanceAttributesBonds cylinder =
            RKInPerInstanceAttributesBonds(float4(position1, 1.0),
              float4(position2, 1.0),
              float4(1.0f, 1.0f, 1.0f, 1.0f),
              float4(1.0f, 1.0f, 1.0f, 1.0f),
              scale,0,0);
          data.push_back(cylinder);
        }

        if (k3 <= maximumReplicaZ)
        {
          double3 position1 = _cell->unitCell() * double3(k1, k2, k3);
          double3 position2 = _cell->unitCell() * double3(k1, k2, k3 + 1);
          float4 scale = float4(0.1, 1.0, 0.1, 1.0);
          RKInPerInstanceAttributesBonds cylinder =
            RKInPerInstanceAttributesBonds(float4(position1, 1.0),
              float4(position2, 1.0),
              float4(1.0, 1.0, 1.0, 1.0),
              float4(1.0, 1.0, 1.0, 1.0),
              scale,0,0);
          data.push_back(cylinder);
        }
      }
    }
  }

  return data;
}


// MARK: Rendering selection
// =====================================================================

std::vector<RKInPerInstanceAttributesAtoms> ProteinCrystal::renderSelectedAtoms() const
{
  int minimumReplicaX = _cell->minimumReplicaX();
  int minimumReplicaY = _cell->minimumReplicaY();
  int minimumReplicaZ = _cell->minimumReplicaZ();

  int maximumReplicaX = _cell->maximumReplicaX();
  int maximumReplicaY = _cell->maximumReplicaY();
  int maximumReplicaZ = _cell->maximumReplicaZ();

  std::unordered_set<std::shared_ptr<SKAtomTreeNode>> asymmetricAtomNodes = _atomsTreeController->selectedTreeNodes();

  std::vector<RKInPerInstanceAttributesAtoms> atomData = std::vector<RKInPerInstanceAttributesAtoms>();

  uint32_t asymmetricAtomIndex=0;
  for (std::shared_ptr<SKAtomTreeNode> node : asymmetricAtomNodes)
  {
    if (std::shared_ptr<SKAsymmetricAtom> atom = node->representedObject())
    {
      bool isVisible = atom->isVisible();

      if(isVisible)
      {
        QColor color = atom->color();
        float4 ambient = float4(color.redF(), color.greenF(), color.blueF(), color.alphaF());
        float4 diffuse = float4(color.redF(), color.greenF(), color.blueF(), color.alphaF());

        float4 specular = float4(1.0, 1.0, 1.0, 1.0);
        double radius = atom->drawRadius() * atom->occupancy();
        float4 scale = float4(radius, radius, radius, 1.0);

        for (std::shared_ptr<SKAtomCopy> copy : atom->copies())
        {
          if (copy->type() == SKAtomCopy::AtomCopyType::copy)
          {
            for (int k1 = minimumReplicaX;k1 <= maximumReplicaX;k1++)
            {
              for (int k2 = minimumReplicaY;k2 <= maximumReplicaY;k2++)
              {
                for (int k3 = minimumReplicaZ;k3 <= maximumReplicaZ;k3++)
                {
                  float4 position = float4(copy->position() + _cell->unitCell() * double3(k1, k2, k3), 1.0);

                  RKInPerInstanceAttributesAtoms atom1 = RKInPerInstanceAttributesAtoms(position, ambient, diffuse, specular, scale, asymmetricAtomIndex);
                  atomData.push_back(atom1);
                }
              }
            }
          }
        }
      }
    }
    asymmetricAtomIndex++;
  }

  return atomData;
}

std::vector<RKInPerInstanceAttributesBonds> ProteinCrystal::renderSelectedInternalBonds() const
{
  int minimumReplicaX = _cell->minimumReplicaX();
  int minimumReplicaY = _cell->minimumReplicaY();
  int minimumReplicaZ = _cell->minimumReplicaZ();

  int maximumReplicaX = _cell->maximumReplicaX();
  int maximumReplicaY = _cell->maximumReplicaY();
  int maximumReplicaZ = _cell->maximumReplicaZ();

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

          for(int k1=minimumReplicaX;k1<=maximumReplicaX;k1++)
          {
            for(int k2=minimumReplicaY;k2<=maximumReplicaY;k2++)
            {
              for(int k3=minimumReplicaZ;k3<=maximumReplicaZ;k3++)
              {
                double3 pos1 = bond->atom1()->position() + _cell->unitCell() *double3(k1,k2,k3);
                double3 pos2 = bond->atom2()->position() + _cell->unitCell() * double3(k1,k2,k3);
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
      }
    }
  }

  return data;
}


// MARK: Filtering
// =====================================================================

std::set<int> ProteinCrystal::filterCartesianAtomPositions(std::function<bool(double3)> & closure)
{
  std::set<int> data;

  int minimumReplicaX = _cell->minimumReplicaX();
  int minimumReplicaY = _cell->minimumReplicaY();
  int minimumReplicaZ = _cell->minimumReplicaZ();

  int maximumReplicaX = _cell->maximumReplicaX();
  int maximumReplicaY = _cell->maximumReplicaY();
  int maximumReplicaZ = _cell->maximumReplicaZ();

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
            double3 pos = copy->position();
            for(int k1=minimumReplicaX;k1<=maximumReplicaX;k1++)
            {
              for(int k2=minimumReplicaY;k2<=maximumReplicaY;k2++)
              {
                for(int k3=minimumReplicaZ;k3<=maximumReplicaZ;k3++)
                {
                  double3 cartesianPosition = pos + _cell->convertToCartesian(double3(double(k1), double(k2), double(k3)));

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
        }
      }
    }
    asymmetricAtomIndex++;
  }

  return data;
}

std::set<int> ProteinCrystal::filterCartesianBondPositions(std::function<bool(double3)> &closure)
{
  std::set<int> data;

  int minimumReplicaX = _cell->minimumReplicaX();
  int minimumReplicaY = _cell->minimumReplicaY();
  int minimumReplicaZ = _cell->minimumReplicaZ();

  int maximumReplicaX = _cell->maximumReplicaX();
  int maximumReplicaY = _cell->maximumReplicaY();
  int maximumReplicaZ = _cell->maximumReplicaZ();

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
        for(int k1=minimumReplicaX;k1<=maximumReplicaX;k1++)
        {
          for(int k2=minimumReplicaY;k2<=maximumReplicaY;k2++)
          {
            for(int k3=minimumReplicaZ;k3<=maximumReplicaZ;k3++)
            {
              double3 pos1 = bond->atom1()->position() + _cell->unitCell() * double3(k1,k2,k3);
              double3 pos2 = bond->atom2()->position() + _cell->unitCell() * double3(k1,k2,k3);

              double3 dr = (pos2 -pos1);

              double3 cartesianPosition1 = pos1 + 0.5 * dr ;
              double4 position1 = rotationMatrix * double4(cartesianPosition1.x, cartesianPosition1.y, cartesianPosition1.z, 1.0);
              double3 absoluteCartesianPosition1 = double3(position1.x,position1.y,position1.z) + _origin;
              if (closure(absoluteCartesianPosition1))
              {
                data.insert(asymmetricBondIndex);
              }

              double3 cartesianPosition2 = pos2 - 0.5 * dr ;
              double4 position2 = rotationMatrix * double4(cartesianPosition2.x, cartesianPosition2.y, cartesianPosition2.z, 1.0);
              double3 absoluteCartesianPosition2 = double3(position2.x,position2.y,position2.z) + _origin;
              if (closure(absoluteCartesianPosition2))
              {
                data.insert(asymmetricBondIndex);
              }
            }
          }
        }
      }
    }
    asymmetricBondIndex++;
  }

  return data;
}


// MARK: Bounding box
// =====================================================================

SKBoundingBox ProteinCrystal::boundingBox() const
{
  double3 minimum = double3(1e10, 1e10, 1e10);
  double3 maximum = double3(-1e10, -1e10, -1e10);

  if (_drawUnitCell)
  {
    minimum = _cell->enclosingBoundingBox().minimum();
    maximum = _cell->enclosingBoundingBox().maximum();
  }

  int minimumReplicaX = _cell->minimumReplicaX();
  int minimumReplicaY = _cell->minimumReplicaY();
  int minimumReplicaZ = _cell->minimumReplicaZ();

  int maximumReplicaX = _cell->maximumReplicaX();
  int maximumReplicaY = _cell->maximumReplicaY();
  int maximumReplicaZ = _cell->maximumReplicaZ();

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
          for (int k1 = minimumReplicaX;k1 <= maximumReplicaX;k1++)
          {
            for (int k2 = minimumReplicaY;k2 <= maximumReplicaY;k2++)
            {
              for (int k3 = minimumReplicaZ;k3 <= maximumReplicaZ;k3++)
              {

                double3 CartesianPosition = pos + _cell->unitCell() *  double3(k1, k2, k3);
                minimum.x = std::min(minimum.x, CartesianPosition.x);
                minimum.y = std::min(minimum.y, CartesianPosition.y);
                minimum.z = std::min(minimum.z, CartesianPosition.z);
                maximum.x = std::max(maximum.x, CartesianPosition.x);
                maximum.y = std::max(maximum.y, CartesianPosition.y);
                maximum.z = std::max(maximum.z, CartesianPosition.z);
              }
            }
          }
        }
      }
    }
  }

  return SKBoundingBox(minimum, maximum);
}


// MARK: Symmetry
// =====================================================================

void ProteinCrystal::expandSymmetry()
{
  std::vector<std::shared_ptr<SKAtomTreeNode>> asymmetricAtomNodes = _atomsTreeController->flattenedLeafNodes();

  for (std::shared_ptr<SKAtomTreeNode> node : asymmetricAtomNodes)
  {
    std::vector<std::shared_ptr<SKAtomCopy>> atomCopies = std::vector<std::shared_ptr<SKAtomCopy>>{};
    if (std::shared_ptr<SKAsymmetricAtom> asymmetricAtom = node->representedObject())
    {
      std::vector<std::shared_ptr<SKAtomCopy>> atomCopies = std::vector<std::shared_ptr<SKAtomCopy>>{};

      double3 fractionalPosition = _cell->convertToFractional(asymmetricAtom->position());
      std::vector<double3> images = _spaceGroup.listOfSymmetricPositions(fractionalPosition);

      for (double3 image : images)
      {
        double3 CartesianPosition = _cell->convertToCartesian(image);
        std::shared_ptr<SKAtomCopy> newAtom = std::make_shared<SKAtomCopy>(asymmetricAtom, CartesianPosition);
        newAtom->setType(SKAtomCopy::AtomCopyType::copy);
        atomCopies.push_back(newAtom);
      }
      asymmetricAtom->setCopies(atomCopies);
    }
  }
}

void ProteinCrystal::expandSymmetry(std::shared_ptr<SKAsymmetricAtom> asymmetricAtom)
{
  std::vector<std::shared_ptr<SKAtomCopy>> atomCopies = std::vector<std::shared_ptr<SKAtomCopy>>{};

  double3 fractionalPosition = _cell->convertToFractional(asymmetricAtom->position());
  std::vector<double3> images = _spaceGroup.listOfSymmetricPositions(fractionalPosition);

  for (double3 image : images)
  {
    double3 CartesianPosition = _cell->convertToCartesian(double3::fract(image));
    std::shared_ptr<SKAtomCopy> newAtom = std::make_shared<SKAtomCopy>(asymmetricAtom, CartesianPosition);
    newAtom->setType(SKAtomCopy::AtomCopyType::copy);
    atomCopies.push_back(newAtom);
  }
  asymmetricAtom->setCopies(atomCopies);
}

std::optional<std::pair<std::shared_ptr<SKCell>, double3>> ProteinCrystal::cellForFractionalPositions()
{
  return std::make_pair(std::make_shared<SKCell>(*_cell), double3(0.0,0.0,0.0));
}

std::optional<std::pair<std::shared_ptr<SKCell>, double3>> ProteinCrystal::cellForCartesianPositions()
{
  return std::make_pair(std::make_shared<SKCell>(*_cell), double3(0.0,0.0,0.0));
}

std::vector<std::shared_ptr<SKAsymmetricAtom>> ProteinCrystal::asymmetricAtomsCopiedAndTransformedToCartesianPositions()
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

std::vector<std::shared_ptr<SKAsymmetricAtom>> ProteinCrystal::asymmetricAtomsCopiedAndTransformedToFractionalPositions()
{
  std::vector<std::shared_ptr<SKAsymmetricAtom>> atoms{};

  double3x3 inverseUnitCell = _cell->inverseUnitCell();
  const std::vector<std::shared_ptr<SKAtomTreeNode>> asymmetricAtomNodes = _atomsTreeController->flattenedLeafNodes();
  for(const std::shared_ptr<SKAtomTreeNode> node: asymmetricAtomNodes)
  {
    if(const std::shared_ptr<SKAsymmetricAtom> asymmetricAtom = node->representedObject())
    {
      std::shared_ptr<SKAsymmetricAtom> newAsymmetricAtom  = std::make_shared<SKAsymmetricAtom>(*asymmetricAtom);
      double3 position = inverseUnitCell * newAsymmetricAtom->position();
      newAsymmetricAtom->setPosition(position);
      atoms.push_back(newAsymmetricAtom);
    }
  }

  return atoms;
}

std::vector<std::shared_ptr<SKAsymmetricAtom>> ProteinCrystal::atomsCopiedAndTransformedToCartesianPositions()
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
          std::shared_ptr<SKAsymmetricAtom> newAsymmetricAtom  = std::make_shared<SKAsymmetricAtom>(*asymmetricAtom);
          newAsymmetricAtom->setPosition(copy->position());
          atoms.push_back(newAsymmetricAtom);
        }
      }
    }
  }

  return atoms;
}

std::vector<std::shared_ptr<SKAsymmetricAtom>> ProteinCrystal::atomsCopiedAndTransformedToFractionalPositions()
{
  std::vector<std::shared_ptr<SKAsymmetricAtom>> atoms{};

  double3x3 inverseUnitCell = _cell->inverseUnitCell();
  const std::vector<std::shared_ptr<SKAtomTreeNode>> asymmetricAtomNodes = _atomsTreeController->flattenedLeafNodes();
  for(const std::shared_ptr<SKAtomTreeNode> node: asymmetricAtomNodes)
  {
    if(const std::shared_ptr<SKAsymmetricAtom> asymmetricAtom = node->representedObject())
    {
      for (std::shared_ptr<SKAtomCopy> copy : asymmetricAtom->copies())
      {
        if (copy->type() == SKAtomCopy::AtomCopyType::copy)
        {
          std::shared_ptr<SKAsymmetricAtom> newAsymmetricAtom  = std::make_shared<SKAsymmetricAtom>(*asymmetricAtom);
          double3 position = inverseUnitCell * copy->position();
          newAsymmetricAtom->setPosition(position);
          atoms.push_back(newAsymmetricAtom);
        }
      }
    }
  }

  return atoms;
}


std::shared_ptr<Structure> ProteinCrystal::superCell() const
{
  qDebug() << "Crystal::superCell";

  // use the copy constructor
  std::shared_ptr<ProteinCrystal> crystal = std::make_shared<ProteinCrystal>(static_cast<const ProteinCrystal&>(*this));

  int minimumReplicaX = _cell->minimumReplicaX();
  int minimumReplicaY = _cell->minimumReplicaY();
  int minimumReplicaZ = _cell->minimumReplicaZ();

  int maximumReplicaX = _cell->maximumReplicaX();
  int maximumReplicaY = _cell->maximumReplicaY();
  int maximumReplicaZ = _cell->maximumReplicaZ();

  const std::vector<std::shared_ptr<SKAtomTreeNode>> asymmetricAtomNodes = _atomsTreeController->flattenedLeafNodes();

  double3x3 unitCell = _cell->unitCell();
  double3x3 inverseUnitCell = _cell->inverseUnitCell();
  int dx = maximumReplicaX-minimumReplicaX;
  int dy = maximumReplicaY-minimumReplicaY;
  int dz = maximumReplicaZ-minimumReplicaZ;
  for(int k1=0;k1<=dx;k1++)
  {
    for(int k2=0;k2<=dy;k2++)
    {
      for(int k3=0;k3<=dz;k3++)
      {
        for(const std::shared_ptr<SKAtomTreeNode> node: asymmetricAtomNodes)
        {
          if(const std::shared_ptr<SKAsymmetricAtom> asymmetricAtom = node->representedObject())
          {
            for(const std::shared_ptr<SKAtomCopy> atomCopy : asymmetricAtom->copies())
            {
              if(atomCopy->type() == SKAtomCopy::AtomCopyType::copy)
              {
                double3 fractionalPosition = (inverseUnitCell * atomCopy->position() + double3(k1,k2,k3)) / double3(dx+1, dy+1, dz+1);
                double3 CartesianPosition = unitCell * fractionalPosition;

                std::shared_ptr<SKAsymmetricAtom> newAsymmetricAtom = std::make_shared<SKAsymmetricAtom>(*asymmetricAtom);
                newAsymmetricAtom->setPosition(CartesianPosition);
                std::shared_ptr<SKAtomCopy> newAtomCopy = std::make_shared<SKAtomCopy>(newAsymmetricAtom, CartesianPosition);
                newAsymmetricAtom->setCopies({newAtomCopy});
                std::shared_ptr<SKAtomTreeNode> atomTreeNode = std::make_shared<SKAtomTreeNode>(newAsymmetricAtom);
                crystal->atomsTreeController()->appendToRootnodes(atomTreeNode);
              }
            }
          }
        }
      }
    }
  }

  crystal->setCell(_cell->superCell());
  crystal->setOrigin(this->origin() + _cell->unitCell() * double3(minimumReplicaX, minimumReplicaY, minimumReplicaZ));
  crystal->atomsTreeController()->setTags();
  crystal->reComputeBoundingBox();
  crystal->computeBonds();

  return crystal;
}


// MARK: bond-computations
// =====================================================================






std::vector<double3> ProteinCrystal::atomPositions() const
{
	int numberOfReplicas = _cell->numberOfReplicas();

	int minimumReplicaX = _cell->minimumReplicaX();
	int minimumReplicaY = _cell->minimumReplicaY();
	int minimumReplicaZ = _cell->minimumReplicaZ();

	int maximumReplicaX = _cell->maximumReplicaX();
	int maximumReplicaY = _cell->maximumReplicaY();
	int maximumReplicaZ = _cell->maximumReplicaZ();

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
					for (int k1 = minimumReplicaX;k1 <= maximumReplicaX;k1++)
					{
						for (int k2 = minimumReplicaY;k2 <= maximumReplicaY;k2++)
						{
							for (int k3 = minimumReplicaZ;k3 <= maximumReplicaZ;k3++)
							{
								double4x4 rotationMatrix = double4x4::AffinityMatrixToTransformationAroundArbitraryPoint(double4x4(_orientation), boundingBox().center());

								double3 fractionalPosition = pos + _cell->unitCell()  * double3(double(k1), double(k2), double(k3));
								double3 cartesianPosition = _cell->convertToCartesian(fractionalPosition);

								double4 position = rotationMatrix * double4(cartesianPosition.x, cartesianPosition.y, cartesianPosition.z, 1.0);

								atomData.push_back(double3(position.x, position.y, position.z));
							}
						}
					}
				}
			}
		}
	}

	return atomData;
}

std::vector<double3> ProteinCrystal::atomUnitCellPositions() const
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
					atomData.push_back(copy->position());
				}
			}
		}
	}

	return atomData;
}

std::vector<double2> ProteinCrystal::potentialParameters() const
{

	std::vector<std::shared_ptr<SKAtomTreeNode>> asymmetricAtomNodes = _atomsTreeController->flattenedLeafNodes();

	std::vector<double2> atomData = std::vector<double2>();

	for (std::shared_ptr<SKAtomTreeNode> node : asymmetricAtomNodes)
	{
		if (std::shared_ptr<SKAsymmetricAtom> atom = node->representedObject())
		{
			for (std::shared_ptr<SKAtomCopy> copy : atom->copies())
			{
				if (copy->type() == SKAtomCopy::AtomCopyType::copy)
				{
					double2 parameter = atom->potentialParameters();
					atomData.push_back(parameter);
				}
			}
		}
	}

	return atomData;
}


void ProteinCrystal::computeBonds()
{
	std::vector<std::shared_ptr<SKAtomCopy>> copies = _atomsTreeController->atomCopies();

  std::vector<std::shared_ptr<SKBond>> bonds;

  for (size_t i = 0;i < copies.size();i++)
	{
		copies[i]->setType(SKAtomCopy::AtomCopyType::copy);
		double3 posA = _cell->unitCell() * copies[i]->position();
		int elementIdentifierA = copies[i]->parent()->elementIdentifier();
		double covalentRadiusA = PredefinedElements::predefinedElements[elementIdentifierA]._covalentRadius;
    for (size_t j = i + 1;j < copies.size();j++)
		{
			if ((copies[i]->parent()->occupancy() == 1.0 && copies[j]->parent()->occupancy() == 1.0) ||
				(copies[i]->parent()->occupancy() < 1.0 && copies[j]->parent()->occupancy() < 1.0))
			{
				double3 posB = _cell->unitCell() * copies[j]->position();
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

double ProteinCrystal::bondLength(std::shared_ptr<SKBond> bond) const
{
  double3 pos1 = bond->atom1()->position();
  double3 pos2 = bond->atom2()->position();
  double3 ds = pos2 - pos1;
  return (_cell->applyFullCellBoundaryCondition(ds)).length();
}

double3 ProteinCrystal::bondVector(std::shared_ptr<SKBond> bond) const
{
  double3 pos1 = bond->atom1()->position();
  double3 pos2 = bond->atom2()->position();
  double3 ds = pos2 - pos1;
  return _cell->applyFullCellBoundaryCondition(ds);
}

std::pair<double3, double3> ProteinCrystal::computeChangedBondLength(std::shared_ptr<SKBond> bond, double bondLength) const
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



void ProteinCrystal::setSpaceGroupHallNumber(int HallNumber)
{
	_spaceGroup = SKSpaceGroup(HallNumber);

	expandSymmetry();
  _atomsTreeController->setTags();

	computeBonds();
}

QDataStream &operator<<(QDataStream &stream, const std::shared_ptr<ProteinCrystal> &proteinCrystal)
{
  stream << proteinCrystal->_versionNumber;
  return stream;
}

QDataStream &operator>>(QDataStream &stream, std::shared_ptr<ProteinCrystal> &proteinCrystal)
{
  qint64 versionNumber;
  stream >> versionNumber;
  if(versionNumber > proteinCrystal->_versionNumber)
  {
    throw InvalidArchiveVersionException(__FILE__, __LINE__, "ProteinCrystal");
  }
  return stream;
}
