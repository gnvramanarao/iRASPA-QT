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

#include "protein.h"

Protein::Protein()
{

}

void Protein::expandSymmetry()
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

void Protein::expandSymmetry(std::shared_ptr<SKAsymmetricAtom> asymmetricAtom)
{
	std::vector<std::shared_ptr<SKAtomCopy>> atomCopies = std::vector<std::shared_ptr<SKAtomCopy>>{};

	std::shared_ptr<SKAtomCopy> newAtom = std::make_shared<SKAtomCopy>(asymmetricAtom, asymmetricAtom->position());
	newAtom->setType(SKAtomCopy::AtomCopyType::copy);
	atomCopies.push_back(newAtom);

	asymmetricAtom->setCopies(atomCopies);
}

std::vector<RKInPerInstanceAttributesAtoms> Protein::renderAtoms() const
{
	std::vector<std::shared_ptr<SKAtomTreeNode>> asymmetricAtomNodes = _atomsTreeController->flattenedLeafNodes();

	std::vector<RKInPerInstanceAttributesAtoms> atomData = std::vector<RKInPerInstanceAttributesAtoms>();

	int index = 0;
	for (std::shared_ptr<SKAtomTreeNode> node : asymmetricAtomNodes)
	{
		if (std::shared_ptr<SKAsymmetricAtom> atom = node->representedObject())
		{
			for (std::shared_ptr<SKAtomCopy> copy : atom->copies())
			{
				copy->setAsymmetricIndex(index);

				if (copy->type() == SKAtomCopy::AtomCopyType::copy)
				{
					QColor color = atom->color();
					double w = atom->isVisible() ? 1.0 : -1.0;


					float4 position = float4(copy->position(), w);

					float4 ambient = float4(color.redF(), color.greenF(), color.blueF(), color.alphaF());
					float4 diffuse = float4(color.redF(), color.greenF(), color.blueF(), color.alphaF());

					float4 specular = float4(1.0, 1.0, 1.0, 1.0);

					double radius = atom->drawRadius() * atom->occupancy();
					float4 scale = float4(radius, radius, radius, 1.0);

					RKInPerInstanceAttributesAtoms atom1 = RKInPerInstanceAttributesAtoms(position, ambient, diffuse, specular, scale);
					atomData.push_back(atom1);
				}
			}
		}
		index++;
	}

	return atomData;
}

std::vector<RKInPerInstanceAttributesAtoms> Protein::renderSelectedAtoms() const
{
	std::unordered_set<std::shared_ptr<SKAtomTreeNode>> asymmetricAtomNodes = _atomsTreeController->selectedTreeNodes();

	std::vector<RKInPerInstanceAttributesAtoms> atomData = std::vector<RKInPerInstanceAttributesAtoms>();

	int index = 0;
	for (std::shared_ptr<SKAtomTreeNode> node : asymmetricAtomNodes)
	{
		if (std::shared_ptr<SKAsymmetricAtom> atom = node->representedObject())
		{
			for (std::shared_ptr<SKAtomCopy> copy : atom->copies())
			{
				copy->setAsymmetricIndex(index);

				if (copy->type() == SKAtomCopy::AtomCopyType::copy)
				{
					QColor color = atom->color();
					double w = atom->isVisible() ? 1.0 : -1.0;


					float4 position = float4(copy->position(), w);

					float4 ambient = float4(color.redF(), color.greenF(), color.blueF(), color.alphaF());
					float4 diffuse = float4(color.redF(), color.greenF(), color.blueF(), color.alphaF());

					float4 specular = float4(1.0, 1.0, 1.0, 1.0);

					double radius = atom->drawRadius() * atom->occupancy();
					float4 scale = float4(radius, radius, radius, 1.0);

					RKInPerInstanceAttributesAtoms atom1 = RKInPerInstanceAttributesAtoms(position, ambient, diffuse, specular, scale);
					atomData.push_back(atom1);
				}
			}
		}
		index++;
	}

	qDebug() << "Molecule renderatoms: " << atomData.size();

	return atomData;
}

std::vector<double3> Protein::atomPositions() const
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

double Protein::bondLenght(std::shared_ptr<SKBond> bond)
{
	double3 pos1 = bond->atom1()->position();
	double3 pos2 = bond->atom2()->position();
	return (pos2 - pos1).length();
}

std::vector<RKInPerInstanceAttributesBonds> Protein::renderInternalBonds() const
{
	int index = 0;

	const std::unordered_set<std::shared_ptr<SKBond>> bonds = _bondSetController->arrangedObjects();

	std::vector<RKInPerInstanceAttributesBonds> data = std::vector<RKInPerInstanceAttributesBonds>();
	for (std::shared_ptr<SKBond> bond : bonds)
	{
		if (bond->atom1()->type() == SKAtomCopy::AtomCopyType::copy && bond->atom2()->type() == SKAtomCopy::AtomCopyType::copy &&
			bond->boundaryType() == SKBond::BoundaryType::internal)
		{
			QColor color1 = bond->atom1()->parent()->color();
			QColor color2 = bond->atom2()->parent()->color();
			double w = (bond->atom1()->parent()->isVisible() && bond->atom2()->parent()->isVisible()) ? 1.0 : -1.0;

			double3 pos1 = bond->atom1()->position();
			double3 pos2 = bond->atom2()->position();
			double bondLength = (pos2 - pos1).length();
			double drawRadius1 = bond->atom1()->parent()->drawRadius() / bondLength;
			double drawRadius2 = bond->atom2()->parent()->drawRadius() / bondLength;

			RKInPerInstanceAttributesBonds bondData = RKInPerInstanceAttributesBonds(
				float4(pos1, w),
				float4(pos2, w),
				float4(color1.redF(), color1.greenF(), color1.blueF(), color1.alphaF()),
				float4(color2.redF(), color2.greenF(), color2.blueF(), color2.alphaF()),
				float4(drawRadius1, std::min(bond->atom1()->parent()->occupancy(), bond->atom2()->parent()->occupancy()), drawRadius2, drawRadius1 / drawRadius2));
			data.push_back(bondData);
		}
	}

	return data;
}


SKBoundingBox Protein::boundingBox() const
{
	double3 minimum = double3(1e10, 1e10, 1e10);
	double3 maximum = double3(-1e10, -1e10, -1e10);

	qDebug() << "Molecule boundingBox";

	std::vector<std::shared_ptr<SKAtomCopy>> atoms = _atomsTreeController->atomCopies();

	if (atoms.empty())
	{
		return SKBoundingBox(double3(0.0, 0.0, 0.0), double3(0.0, 0.0, 0.0));
	}

	for (std::shared_ptr<SKAtomCopy> atom : atoms)
	{
		double3 CartesianPosition = atom->position();
		minimum.x = std::min(minimum.x, CartesianPosition.x);
		minimum.y = std::min(minimum.y, CartesianPosition.y);
		minimum.z = std::min(minimum.z, CartesianPosition.z);
		maximum.x = std::max(maximum.x, CartesianPosition.x);
		maximum.y = std::max(maximum.x, CartesianPosition.y);
		maximum.z = std::max(maximum.x, CartesianPosition.z);
	}

	return SKBoundingBox(minimum, maximum);
}

SKBoundingBox Protein::transformedBoundingBox() const
{
	SKBoundingBox currentBoundingBox = _cell->boundingBox();
	double4x4 transformation = double4x4::AffinityMatrixToTransformationAroundArbitraryPoint(double4x4(_orientation), currentBoundingBox.center());
	SKBoundingBox transformedBoundingBox = currentBoundingBox.adjustForTransformation(transformation);
	return transformedBoundingBox;
}

QDataStream &operator<<(QDataStream &stream, const std::shared_ptr<Protein> &protein)
{
  stream << protein->_versionNumber;
  return stream;
}

QDataStream &operator>>(QDataStream &stream, std::shared_ptr<Protein> &protein)
{
  qint64 versionNumber;
  stream >> versionNumber;
  if(versionNumber > protein->_versionNumber)
  {
    throw InvalidArchiveVersionException(__FILE__, __LINE__, "Protein");
  }
  return stream;
}
