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

#include "molecularcrystal.h"

MolecularCrystal::MolecularCrystal()
{

}

void MolecularCrystal::expandSymmetry()
{
	std::vector<std::shared_ptr<SKAtomTreeNode>> asymmetricAtomNodes = _atomsTreeController->flattenedLeafNodes();

	int index = 0;
	for (std::shared_ptr<SKAtomTreeNode> node : asymmetricAtomNodes)
	{
		std::vector<std::shared_ptr<SKAtomCopy>> atomCopies = std::vector<std::shared_ptr<SKAtomCopy>>{};
		if (std::shared_ptr<SKAsymmetricAtom> asymmetricAtom = node->representedObject())
		{
			std::vector<std::shared_ptr<SKAtomCopy>> atomCopies = std::vector<std::shared_ptr<SKAtomCopy>>{};

			std::vector<double3> images = _spaceGroup.listOfSymmetricPositions(asymmetricAtom->position());

			for (double3 image : images)
			{
				std::shared_ptr<SKAtomCopy> newAtom = std::make_shared<SKAtomCopy>(asymmetricAtom, double3::fract(image));
				newAtom->setType(SKAtomCopy::AtomCopyType::copy);
				atomCopies.push_back(newAtom);
			}
			asymmetricAtom->setCopies(atomCopies);
		}
	}
}

void MolecularCrystal::expandSymmetry(std::shared_ptr<SKAsymmetricAtom> asymmetricAtom)
{
	std::vector<std::shared_ptr<SKAtomCopy>> atomCopies = std::vector<std::shared_ptr<SKAtomCopy>>{};

	std::vector<double3> images = _spaceGroup.listOfSymmetricPositions(asymmetricAtom->position());

	for (double3 image : images)
	{
		std::shared_ptr<SKAtomCopy> newAtom = std::make_shared<SKAtomCopy>(asymmetricAtom, double3::fract(image));
		newAtom->setType(SKAtomCopy::AtomCopyType::copy);
		atomCopies.push_back(newAtom);
	}
	asymmetricAtom->setCopies(atomCopies);
}


std::vector<RKInPerInstanceAttributesAtoms> MolecularCrystal::renderAtoms() const
{
	int numberOfReplicas = _cell->numberOfReplicas();

	int minimumReplicaX = _cell->minimumReplicaX();
	int minimumReplicaY = _cell->minimumReplicaY();
	int minimumReplicaZ = _cell->minimumReplicaZ();

	int maximumReplicaX = _cell->maximumReplicaX();
	int maximumReplicaY = _cell->maximumReplicaY();
	int maximumReplicaZ = _cell->maximumReplicaZ();

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
					for (int k1 = minimumReplicaX;k1 <= maximumReplicaX;k1++)
					{
						for (int k2 = minimumReplicaY;k2 <= maximumReplicaY;k2++)
						{
							for (int k3 = minimumReplicaZ;k3 <= maximumReplicaZ;k3++)
							{

								float4 position = float4(copy->position() + _cell->unitCell() * double3(k1, k2, k3), w);

								float4 ambient = float4(color.redF(), color.greenF(), color.blueF(), color.alphaF());
								float4 diffuse = float4(color.redF(), color.greenF(), color.blueF(), color.alphaF());

								float4 specular = float4(1.0, 1.0, 1.0, 1.0);

								double radius = atom->drawRadius() * atom->occupancy();
								float4 scale = float4(radius, radius, radius, 1.0);

								//if(atom->occupancy()<1.0)
								//{
								//  diffuse = float4(1.0,1.0,1.0,1.0);
								//}

								RKInPerInstanceAttributesAtoms atom1 = RKInPerInstanceAttributesAtoms(position, ambient, diffuse, specular, scale);
								atomData.push_back(atom1);

							}
						}
					}
				}
			}
		}
		index++;
	}

	return atomData;
}

std::vector<RKInPerInstanceAttributesAtoms> MolecularCrystal::renderSelectedAtoms() const
{
	int numberOfReplicas = _cell->numberOfReplicas();

	int minimumReplicaX = _cell->minimumReplicaX();
	int minimumReplicaY = _cell->minimumReplicaY();
	int minimumReplicaZ = _cell->minimumReplicaZ();

	int maximumReplicaX = _cell->maximumReplicaX();
	int maximumReplicaY = _cell->maximumReplicaY();
	int maximumReplicaZ = _cell->maximumReplicaZ();

	std::unordered_set<std::shared_ptr<SKAtomTreeNode>> asymmetricAtomNodes = _atomsTreeController->selectedTreeNodes();

	std::vector<RKInPerInstanceAttributesAtoms> atomData = std::vector<RKInPerInstanceAttributesAtoms>();


	for (std::shared_ptr<SKAtomTreeNode> node : asymmetricAtomNodes)
	{
		if (std::shared_ptr<SKAsymmetricAtom> atom = node->representedObject())
		{
			for (std::shared_ptr<SKAtomCopy> copy : atom->copies())
			{
				if (copy->type() == SKAtomCopy::AtomCopyType::copy)
				{
					QColor color = atom->color();
					double w = atom->isVisible() ? 1.0 : -1.0;
					//std::cout << "here" << std::endl;
					for (int k1 = minimumReplicaX;k1 <= maximumReplicaX;k1++)
					{
						for (int k2 = minimumReplicaY;k2 <= maximumReplicaY;k2++)
						{
							for (int k3 = minimumReplicaZ;k3 <= maximumReplicaZ;k3++)
							{
								//std::cout << "position: " << copy->position() .x << "," << copy->position().y << ", " << copy->position().z << std::endl;

								float4 position = float4(copy->position() + _cell->unitCell() * double3(k1, k2, k3), w);

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
				}
			}
		}
	}

	return atomData;
}


std::vector<double3> MolecularCrystal::atomPositions() const
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

std::vector<double3> MolecularCrystal::atomUnitCellPositions() const
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

std::vector<double2> MolecularCrystal::potentialParameters() const
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

std::vector<RKInPerInstanceAttributesBonds> MolecularCrystal::renderInternalBonds() const
{
	int index = 0;

	int numberOfReplicas = _cell->numberOfReplicas();

	int minimumReplicaX = _cell->minimumReplicaX();
	int minimumReplicaY = _cell->minimumReplicaY();
	int minimumReplicaZ = _cell->minimumReplicaZ();

	int maximumReplicaX = _cell->maximumReplicaX();
	int maximumReplicaY = _cell->maximumReplicaY();
	int maximumReplicaZ = _cell->maximumReplicaZ();

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
							float4(pos1, w),
							float4(pos2, w),
							float4(color1.redF(), color1.greenF(), color1.blueF(), color1.alphaF()),
							float4(color2.redF(), color2.greenF(), color2.blueF(), color2.alphaF()),
							float4(drawRadius1, std::min(bond->atom1()->parent()->occupancy(), bond->atom2()->parent()->occupancy()), drawRadius2, drawRadius1 / drawRadius2));
						data.push_back(bondData);
					}
				}
			}
		}
	}

	return data;
}

std::vector<RKInPerInstanceAttributesAtoms> MolecularCrystal::renderUnitCellSpheres() const
{
	int minimumReplicaX = _cell->minimumReplicaX();
	int minimumReplicaY = _cell->minimumReplicaY();
	int minimumReplicaZ = _cell->minimumReplicaZ();

	int maximumReplicaX = _cell->maximumReplicaX();
	int maximumReplicaY = _cell->maximumReplicaY();
	int maximumReplicaZ = _cell->maximumReplicaZ();

	std::vector<RKInPerInstanceAttributesAtoms> data = std::vector<RKInPerInstanceAttributesAtoms>();

	for (int k1 = minimumReplicaX;k1 <= maximumReplicaX + 1;k1++)
	{
		for (int k2 = minimumReplicaY;k2 <= maximumReplicaY + 1;k2++)
		{
			for (int k3 = minimumReplicaZ;k3 <= maximumReplicaZ + 1;k3++)
			{
				double3 position = _cell->unitCell() * double3(k1, k2, k3); // + origin();
				float4 ambient = float4(1.0, 1.0, 1.0, 1.0);
				float4 diffuse = float4(1.0, 1.0, 1.0, 1.0);
				float4 specular = float4(1.0, 1.0, 1.0, 1.0);
				float4 scale = float4(0.1, 0.1, 0.1, 1.0);
				RKInPerInstanceAttributesAtoms sphere = RKInPerInstanceAttributesAtoms(float4(position, 1.0), ambient, diffuse, specular, scale);
				data.push_back(sphere);
			}
		}
	}

	return data;
}

std::vector<RKInPerInstanceAttributesBonds> MolecularCrystal::renderUnitCellCylinders() const
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
					double3 position1 = _cell->unitCell() * double3(k1, k2, k3); // + origin();
					double3 position2 = _cell->unitCell() * double3(k1 + 1, k2, k3); // + origin();
					float4 scale = float4(0.1, 1.0, 0.1, 1.0);
					RKInPerInstanceAttributesBonds cylinder =
						RKInPerInstanceAttributesBonds(float4(position1, 1.0),
							float4(position2, 1.0),
							float4(1.0, 1.0, 1.0, 1.0),
							float4(1.0, 1.0, 1.0, 1.0),
							scale);
					data.push_back(cylinder);
				}

				if (k2 <= maximumReplicaY)
				{
					double3 position1 = _cell->unitCell() * double3(k1, k2, k3); // + origin();
					double3 position2 = _cell->unitCell() * double3(k1, k2 + 1, k3); // + origin();
					float4 scale = float4(0.1, 1.0, 0.1, 1.0);
					RKInPerInstanceAttributesBonds cylinder =
						RKInPerInstanceAttributesBonds(float4(position1, 1.0),
							float4(position2, 1.0),
							float4(1.0, 1.0, 1.0, 1.0),
							float4(1.0, 1.0, 1.0, 1.0),
							scale);
					data.push_back(cylinder);
				}

				if (k3 <= maximumReplicaZ)
				{
					double3 position1 = _cell->unitCell() * double3(k1, k2, k3); // + origin();
					double3 position2 = _cell->unitCell() * double3(k1, k2, k3 + 1); // + origin();
					float4 scale = float4(0.1, 1.0, 0.1, 1.0);
					RKInPerInstanceAttributesBonds cylinder =
						RKInPerInstanceAttributesBonds(float4(position1, 1.0),
							float4(position2, 1.0),
							float4(1.0, 1.0, 1.0, 1.0),
							float4(1.0, 1.0, 1.0, 1.0),
							scale);
					data.push_back(cylinder);
				}
			}
		}
	}

	return data;
}

SKBoundingBox MolecularCrystal::boundingBox() const
{
	double3 minimum = double3(1e10, 1e10, 1e10);
	double3 maximum = double3(-1e10, -1e10, -1e10);

	if (_drawUnitCell)
	{
		minimum = _cell->enclosingBoundingBox().minimum();
		maximum = _cell->enclosingBoundingBox().maximum();
	}

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

								double3 CartesianPosition = pos + _cell->unitCell() *  double3(k1, k2, k3);
								minimum.x = std::min(minimum.x, CartesianPosition.x);
								minimum.y = std::min(minimum.y, CartesianPosition.y);
								minimum.z = std::min(minimum.z, CartesianPosition.z);
								maximum.x = std::max(maximum.x, CartesianPosition.x);
								maximum.y = std::max(maximum.x, CartesianPosition.y);
								maximum.z = std::max(maximum.x, CartesianPosition.z);
							}
						}
					}
				}
			}
		}
	}

	return SKBoundingBox(minimum, maximum);
}

SKBoundingBox MolecularCrystal::transformedBoundingBox() const
{
	if (_drawUnitCell)
	{
		return this->boundingBox();
	}

	SKBoundingBox currentBoundingBox = _cell->boundingBox();
	double4x4 transformation = double4x4::AffinityMatrixToTransformationAroundArbitraryPoint(double4x4(_orientation), currentBoundingBox.center());
	SKBoundingBox transformedBoundingBox = currentBoundingBox.adjustForTransformation(transformation);
	return transformedBoundingBox;
}

void MolecularCrystal::setTags()
{
	std::vector<std::shared_ptr<SKAtomTreeNode>> asymmetricAtomNodes = _atomsTreeController->flattenedLeafNodes();

	int index = 0;
	int nodeIndex = 0;
	for (std::shared_ptr<SKAtomTreeNode> node : asymmetricAtomNodes)
	{
		if (std::shared_ptr<SKAsymmetricAtom> atom = node->representedObject())
		{
			for (std::shared_ptr<SKAtomCopy> copy : atom->copies())
			{
				copy->setTag(index);
				copy->setAsymmetricIndex(nodeIndex);
				index++;
			}
			atom->setTag(nodeIndex);
			atom->setAsymmetricIndex(nodeIndex);
			nodeIndex++;
		}
	}
}



void MolecularCrystal::computeBonds()
{
	std::vector<std::shared_ptr<SKAtomCopy>> copies = _atomsTreeController->atomCopies();

	_bondSetController->clear();
	for (int i = 0;i < copies.size();i++)
	{
		copies[i]->setType(SKAtomCopy::AtomCopyType::copy);
		double3 posA = _cell->unitCell() * copies[i]->position();
		int elementIdentifierA = copies[i]->parent()->elementIdentifier();
		double covalentRadiusA = PredefinedElements::predefinedElements[elementIdentifierA]._covalentRadius;
		for (int j = i + 1;j < copies.size();j++)
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
					_bondSetController->append(bond);
				}
			}
		}
	}

	int numberOfBonds = 0;
	for (std::shared_ptr<SKBond> bond : _bondSetController->arrangedObjects())
	{
		if (bond->atom1()->type() == SKAtomCopy::AtomCopyType::copy && bond->atom2()->type() == SKAtomCopy::AtomCopyType::copy)
			numberOfBonds++;
	}

	setTags();
	_bondSetController->sort();
	//std::cout << "Number of bonds: " << numberOfBonds << std::endl;
}

double MolecularCrystal::bondLenght(std::shared_ptr<SKBond> bond)
{
	double3 pos1 = bond->atom1()->position();
	double3 pos2 = bond->atom2()->position();
	double3 ds = pos2 - pos1;
	ds.x -= floor(ds.x + 0.5);
	ds.y -= floor(ds.y + 0.5);
	ds.z -= floor(ds.z + 0.5);
	return (_cell->unitCell() * ds).length();

}


void MolecularCrystal::setSpaceGroupHallNumber(int HallNumber)
{
	_spaceGroup = SKSpaceGroup(HallNumber);

	std::cout << "Crystal spacegroup set to: " << HallNumber << std::endl;

	expandSymmetry();
	setTags();

	computeBonds();
}

QDataStream &operator<<(QDataStream &stream, const std::shared_ptr<MolecularCrystal> &molecularCrystal)
{
  stream << molecularCrystal->_versionNumber;
  return stream;
}

QDataStream &operator>>(QDataStream &stream, std::shared_ptr<MolecularCrystal> &molecularCrystal)
{
  qint64 versionNumber;
  stream >> versionNumber;
  if(versionNumber > molecularCrystal->_versionNumber)
  {
    throw InvalidArchiveVersionException(__FILE__, __LINE__, "MolecularCrystal");
  }
  return stream;
}
