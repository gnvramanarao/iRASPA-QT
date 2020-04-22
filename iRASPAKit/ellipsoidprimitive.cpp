#include "ellipsoidprimitive.h"

EllipsoidPrimitive::EllipsoidPrimitive()
{

}

// MARK: Rendering
// =====================================================================

std::vector<RKInPerInstanceAttributesAtoms> EllipsoidPrimitive::renderPrimitiveObjects() const
{
  std::vector<RKInPerInstanceAttributesAtoms> atomData;

  if(_primitiveIsFractional)
  {
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

  }
  else
  {
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
  }

  return atomData;
}


// MARK: Rendering selection
// =====================================================================


// MARK: Filtering
// =====================================================================

// MARK: Bounding box
// =====================================================================

// MARK: Symmetry
// =====================================================================


// MARK: bond-computations
// =====================================================================


QDataStream &operator<<(QDataStream &stream, const std::shared_ptr<EllipsoidPrimitive> &primitive)
{
  stream << primitive->_versionNumber;
  return stream;
}

QDataStream &operator>>(QDataStream &stream, std::shared_ptr<EllipsoidPrimitive> &primitive)
{
  qint64 versionNumber;
  stream >> versionNumber;
  if(versionNumber > primitive->_versionNumber)
  {
    throw InvalidArchiveVersionException(__FILE__, __LINE__, "EllipsoidPrimitive");
  }
  return stream;
}
