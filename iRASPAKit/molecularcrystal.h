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

#pragma once

#include <symmetrykit.h>
#include "structure.h"

class MolecularCrystal: public Structure
{
public:
  MolecularCrystal();
  MolecularCrystal(std::shared_ptr<SKStructure> structure);

	iRASPAStructureType structureType() override final  { return iRASPAStructureType::molecularCrystal; }

	std::vector<RKInPerInstanceAttributesAtoms> renderAtoms() const override final;
  std::vector<RKInPerInstanceAttributesBonds> renderInternalBonds() const override final;
  std::vector<RKInPerInstanceAttributesAtoms> renderUnitCellSpheres() const override final;
  std::vector<RKInPerInstanceAttributesBonds> renderUnitCellCylinders() const override final;

	std::vector<RKInPerInstanceAttributesAtoms> renderSelectedAtoms() const override final;
  std::vector<RKInPerInstanceAttributesBonds> renderSelectedInternalBonds() const override final;

  std::set<int> filterCartesianAtomPositions(std::function<bool(double3)> &) override final;
  std::set<int> filterCartesianBondPositions(std::function<bool(double3)> &) override final;

  SKBoundingBox boundingBox() const final override;

  void expandSymmetry() final override;
  void expandSymmetry(std::shared_ptr<SKAsymmetricAtom> asymmetricAtom);
  void setSpaceGroupHallNumber(int HallNumber) override final;

  double bondLength(std::shared_ptr<SKBond> bond) const override final;
  double3 bondVector(std::shared_ptr<SKBond> bond) const override final;
  std::pair<double3, double3> computeChangedBondLength(std::shared_ptr<SKBond> bond, double bondlength) const override final;
  void computeBonds() final override;

	std::vector<double3> atomPositions() const override final;
	std::vector<double3> atomUnitCellPositions() const override final;
	std::vector<double2> potentialParameters() const override final;
private:
  qint64 _versionNumber{1};
  SKSpaceGroup _spaceGroup = SKSpaceGroup(1);

  friend QDataStream &operator<<(QDataStream &, const std::shared_ptr<MolecularCrystal> &);
  friend QDataStream &operator>>(QDataStream &, std::shared_ptr<MolecularCrystal> &);
};

