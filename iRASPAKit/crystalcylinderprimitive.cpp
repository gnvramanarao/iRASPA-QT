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

CrystalCylinderPrimitive::CrystalCylinderPrimitive()
{

}

CrystalCylinderPrimitive::CrystalCylinderPrimitive(const CrystalCylinderPrimitive &crystalCylinderPrimitive): Structure(crystalCylinderPrimitive)
{
}

CrystalCylinderPrimitive::CrystalCylinderPrimitive(std::shared_ptr<Structure> s): Structure(s)
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

  if(dynamic_cast<MolecularCrystal*>(s.get()) ||
     dynamic_cast<ProteinCrystal*>(s.get()) ||
     dynamic_cast<MolecularCrystal*>(s.get()) ||
     dynamic_cast<Molecule*>(s.get()) ||
     dynamic_cast<Protein*>(s.get()) ||
     dynamic_cast<EllipsoidPrimitive*>(s.get()) ||
     dynamic_cast<CylinderPrimitive*>(s.get()) ||
     dynamic_cast<PolygonalPrismPrimitive*>(s.get()))
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

std::vector<RKInPerInstanceAttributesAtoms> CrystalCylinderPrimitive::renderCrystalPrimitiveCylinderObjects() const
{
  std::vector<RKInPerInstanceAttributesAtoms> atomData;

  int minimumReplicaX = _cell->minimumReplicaX();
  int minimumReplicaY = _cell->minimumReplicaY();
  int minimumReplicaZ = _cell->minimumReplicaZ();

  int maximumReplicaX = _cell->maximumReplicaX();
  int maximumReplicaY = _cell->maximumReplicaY();
  int maximumReplicaZ = _cell->maximumReplicaZ();

  std::vector<std::shared_ptr<SKAtomTreeNode>> asymmetricAtomNodes = _atomsTreeController->flattenedLeafNodes();

  uint32_t asymmetricAtomIndex = 0;
  for(std::shared_ptr<SKAtomTreeNode> node: asymmetricAtomNodes)
  {
    if(std::shared_ptr<SKAsymmetricAtom> atom = node->representedObject())
    {
      bool isVisible = atom->isVisible();

      if(isVisible)
      {
        for(std::shared_ptr<SKAtomCopy> copy : atom->copies())
        {
          if(copy->type() == SKAtomCopy::AtomCopyType::copy)
          {
            float4 ambient = float4(1.0,1.0,1.0,1.0);
            float4 diffuse = float4(1.0,1.0,1.0,1.0);
            float4 specular = float4(1.0,1.0,1.0,1.0);

            double radius = 1.0;
            float4 scale = float4(radius,radius,radius,1.0);

            for(int k1=minimumReplicaX;k1<=maximumReplicaX;k1++)
            {
              for(int k2=minimumReplicaY;k2<=maximumReplicaY;k2++)
              {
                for(int k3=minimumReplicaZ;k3<=maximumReplicaZ;k3++)
                {
                  float4 position = float4(_cell->unitCell() * (copy->position() + double3(k1,k2,k3)), 1.0);

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


// MARK: Rendering selection
// =====================================================================


// MARK: Filtering
// =====================================================================

// MARK: Bounding box
// =====================================================================

SKBoundingBox CrystalCylinderPrimitive::boundingBox() const
{
  double3 minimumReplica = _cell->minimumReplicas();
  double3 maximumReplica = _cell->maximumReplicas();

  double3 c0 = _cell->unitCell() * double3(minimumReplica);
  double3 c1 = _cell->unitCell() * double3(maximumReplica.x+1, minimumReplica.y,   minimumReplica.z);
  double3 c2 = _cell->unitCell() * double3(maximumReplica.x+1, maximumReplica.y+1, minimumReplica.z);
  double3 c3 = _cell->unitCell() * double3(minimumReplica.x,   maximumReplica.y+1, minimumReplica.z);
  double3 c4 = _cell->unitCell() * double3(minimumReplica.x,   minimumReplica.y,   maximumReplica.z+1);
  double3 c5 = _cell->unitCell() * double3(maximumReplica.x+1, minimumReplica.y,   maximumReplica.z+1);
  double3 c6 = _cell->unitCell() * double3(maximumReplica.x+1, maximumReplica.y+1, maximumReplica.z+1);
  double3 c7 = _cell->unitCell() * double3(minimumReplica.x,   maximumReplica.y+1, maximumReplica.z+1);

  double valuesX[8] = {c0.x, c1.x, c2.x, c3.x, c4.x, c5.x, c6.x, c7.x};
  double valuesY[8] = {c0.y, c1.y, c2.y, c3.y, c4.y, c5.y, c6.y, c7.y};
  double valuesZ[8] = {c0.z, c1.z, c2.z, c3.z, c4.z, c5.z, c6.z, c7.z};

  double3 minimum = double3(*std::min_element(valuesX,valuesX+8),
                            *std::min_element(valuesY,valuesY+8),
                            *std::min_element(valuesZ,valuesZ+8));

  double3 maximum = double3(*std::max_element(valuesX,valuesX+8),
                            *std::max_element(valuesY,valuesY+8),
                            *std::max_element(valuesZ,valuesZ+8));

  return SKBoundingBox(minimum,maximum);
}

// MARK: Symmetry
// =====================================================================

void CrystalCylinderPrimitive::expandSymmetry()
{
  std::vector<std::shared_ptr<SKAtomTreeNode>> asymmetricAtomNodes = _atomsTreeController->flattenedLeafNodes();

  for(std::shared_ptr<SKAtomTreeNode> node: asymmetricAtomNodes)
  {
    std::vector<std::shared_ptr<SKAtomCopy>> atomCopies = std::vector<std::shared_ptr<SKAtomCopy>>{};
    if(std::shared_ptr<SKAsymmetricAtom> asymmetricAtom = node->representedObject())
    {
      std::vector<std::shared_ptr<SKAtomCopy>> atomCopies = std::vector<std::shared_ptr<SKAtomCopy>>{};

      std::vector<double3> images = _spaceGroup.listOfSymmetricPositions(asymmetricAtom->position());

      for(double3 image : images)
      {
        std::shared_ptr<SKAtomCopy> newAtom = std::make_shared<SKAtomCopy>(asymmetricAtom, double3::fract(image));
        newAtom->setType(SKAtomCopy::AtomCopyType::copy);
        atomCopies.push_back(newAtom);
      }
      asymmetricAtom->setCopies(atomCopies);
    }
  }
}

std::optional<std::pair<std::shared_ptr<SKCell>, double3>> CrystalCylinderPrimitive::cellForFractionalPositions()
{
  return std::make_pair(std::make_shared<SKCell>(*_cell), double3(0.0,0.0,0.0));
}

std::optional<std::pair<std::shared_ptr<SKCell>, double3>> CrystalCylinderPrimitive::cellForCartesianPositions()
{
  return std::make_pair(std::make_shared<SKCell>(*_cell), double3(0.0,0.0,0.0));
}

std::vector<std::shared_ptr<SKAsymmetricAtom>> CrystalCylinderPrimitive::asymmetricAtomsCopiedAndTransformedToCartesianPositions()
{
  std::vector<std::shared_ptr<SKAsymmetricAtom>> atoms{};

  double3x3 unitCell = _cell->unitCell();
  const std::vector<std::shared_ptr<SKAtomTreeNode>> asymmetricAtomNodes = _atomsTreeController->flattenedLeafNodes();
  for(const std::shared_ptr<SKAtomTreeNode> node: asymmetricAtomNodes)
  {
    if(const std::shared_ptr<SKAsymmetricAtom> asymmetricAtom = node->representedObject())
    {
      double3 position = unitCell * double3::fract(asymmetricAtom->position());

      std::shared_ptr<SKAsymmetricAtom> newAsymmetricAtom = std::make_shared<SKAsymmetricAtom>(*asymmetricAtom);
      newAsymmetricAtom->setPosition(position);
      atoms.push_back(newAsymmetricAtom);
    }
  }

  return atoms;
}

std::vector<std::shared_ptr<SKAsymmetricAtom>> CrystalCylinderPrimitive::asymmetricAtomsCopiedAndTransformedToFractionalPositions()
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

std::vector<std::shared_ptr<SKAsymmetricAtom>> CrystalCylinderPrimitive::atomsCopiedAndTransformedToFractionalPositions()
{
  std::vector<std::shared_ptr<SKAsymmetricAtom>> atoms{};

  const std::vector<std::shared_ptr<SKAtomTreeNode>> asymmetricAtomNodes = _atomsTreeController->flattenedLeafNodes();
  for(const std::shared_ptr<SKAtomTreeNode> node: asymmetricAtomNodes)
  {
    if(const std::shared_ptr<SKAsymmetricAtom> asymmetricAtom = node->representedObject())
    {
      for(std::shared_ptr<SKAtomCopy> copy : asymmetricAtom->copies())
      {
        if(copy->type() == SKAtomCopy::AtomCopyType::copy)
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

std::vector<std::shared_ptr<SKAsymmetricAtom>> CrystalCylinderPrimitive::atomsCopiedAndTransformedToCartesianPositions()
{
  std::vector<std::shared_ptr<SKAsymmetricAtom>> atoms{};

  double3x3 unitCell = _cell->unitCell();
  const std::vector<std::shared_ptr<SKAtomTreeNode>> asymmetricAtomNodes = _atomsTreeController->flattenedLeafNodes();
  for(const std::shared_ptr<SKAtomTreeNode> node: asymmetricAtomNodes)
  {
    if(const std::shared_ptr<SKAsymmetricAtom> asymmetricAtom = node->representedObject())
    {
      for(std::shared_ptr<SKAtomCopy> copy : asymmetricAtom->copies())
      {
        if(copy->type() == SKAtomCopy::AtomCopyType::copy)
        {
          double3 position = unitCell * double3::fract(copy->position());
          std::shared_ptr<SKAsymmetricAtom> newAsymmetricAtom = std::make_shared<SKAsymmetricAtom>(*asymmetricAtom);
          newAsymmetricAtom->setPosition(position);
          atoms.push_back(newAsymmetricAtom);
        }
      }
    }
  }

  return atoms;
}

std::shared_ptr<Structure> CrystalCylinderPrimitive::superCell() const
{
  qDebug() << "CrystalCylinderPrimitive::superCell";

  // use the copy constructor
  std::shared_ptr<CrystalCylinderPrimitive> crystal = std::make_shared<CrystalCylinderPrimitive>(static_cast<const CrystalCylinderPrimitive&>(*this));

  int minimumReplicaX = _cell->minimumReplicaX();
  int minimumReplicaY = _cell->minimumReplicaY();
  int minimumReplicaZ = _cell->minimumReplicaZ();

  int maximumReplicaX = _cell->maximumReplicaX();
  int maximumReplicaY = _cell->maximumReplicaY();
  int maximumReplicaZ = _cell->maximumReplicaZ();

  const std::vector<std::shared_ptr<SKAtomTreeNode>> asymmetricAtomNodes = _atomsTreeController->flattenedLeafNodes();

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
                double3 position = (atomCopy->position() + double3(k1,k2,k3)) / double3(dx+1, dy+1, dz+1);

                std::shared_ptr<SKAsymmetricAtom> newAsymmetricAtom = std::make_shared<SKAsymmetricAtom>(*asymmetricAtom);
                newAsymmetricAtom->setPosition(position);
                std::shared_ptr<SKAtomCopy> newAtomCopy = std::make_shared<SKAtomCopy>(newAsymmetricAtom, position);
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



QDataStream &operator<<(QDataStream &stream, const std::shared_ptr<CrystalCylinderPrimitive> &primitive)
{
  stream << primitive->_versionNumber;
  return stream;
}

QDataStream &operator>>(QDataStream &stream, std::shared_ptr<CrystalCylinderPrimitive> &primitive)
{
  qint64 versionNumber;
  stream >> versionNumber;
  if(versionNumber > primitive->_versionNumber)
  {
    throw InvalidArchiveVersionException(__FILE__, __LINE__, "CrystalCylinderPrimitive");
  }
  return stream;
}
