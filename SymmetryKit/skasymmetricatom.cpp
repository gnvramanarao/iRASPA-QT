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

#include "skasymmetricatom.h"

SKAsymmetricAtom::SKAsymmetricAtom(): _displayName("C"), _elementIdentifier(6)
{

}

SKAsymmetricAtom::SKAsymmetricAtom(QString displayName, int elementIdentifier): _displayName(displayName), _elementIdentifier(elementIdentifier)
{

}

SKAsymmetricAtom::~SKAsymmetricAtom()
{
}



QDataStream &operator<<(QDataStream &stream, const std::shared_ptr<SKAsymmetricAtom> &asymmetricAtom)
{
  stream << asymmetricAtom->_versionNumber;
  stream << asymmetricAtom->_asymmetricIndex;
  stream << asymmetricAtom->_displayName;
  stream << asymmetricAtom->_position;
  stream << asymmetricAtom->_charge;
  stream << static_cast<typename std::underlying_type<SKAsymmetricAtom::Hybridization>::type>(asymmetricAtom->_hybridization);
  stream << asymmetricAtom->_uniqueForceFieldName;
  stream << asymmetricAtom->_elementIdentifier;
  stream << asymmetricAtom->_color;
  stream << asymmetricAtom->_drawRadius;

  stream << asymmetricAtom->_bondDistanceCriteria;
  stream << asymmetricAtom->_potentialParameters;
  stream << asymmetricAtom->_tag;
  stream << asymmetricAtom->_isFixed;
  stream << asymmetricAtom->_isVisible;
  stream << asymmetricAtom->_isVisibleEnabled;

  stream << asymmetricAtom->_serialNumber;
  stream << asymmetricAtom->_remotenessIndicator;
  stream << asymmetricAtom->_branchDesignator;
  stream << asymmetricAtom->_asymetricID;
  stream << asymmetricAtom->_alternateLocationIndicator;
  stream << asymmetricAtom->_residueName;
  stream << asymmetricAtom->_chainIdentifier;
  stream << asymmetricAtom->_residueSequenceNumber;
  stream << asymmetricAtom->_codeForInsertionOfResidues;
  stream << asymmetricAtom->_occupancy;
  stream << asymmetricAtom->_temperaturefactor;
  stream << asymmetricAtom->_segmentIdentifier;

  stream << asymmetricAtom->_ligandAtom;
  stream << asymmetricAtom->_backBoneAtom;
  stream << asymmetricAtom->_fractional;
  stream << asymmetricAtom->_solvent;

  stream << asymmetricAtom->_copies;
  return stream;
}

QDataStream &operator>>(QDataStream &stream, std::shared_ptr<SKAsymmetricAtom> &asymmetricAtom)
{
  qint64 versionNumber;
  stream >> versionNumber;
  if(versionNumber > asymmetricAtom->_versionNumber)
  {
    throw InvalidArchiveVersionException(__FILE__, __LINE__, "SKAsymmetricAtom");
  }

  stream >> asymmetricAtom->_asymmetricIndex;
  stream >> asymmetricAtom->_displayName;
  stream >> asymmetricAtom->_position;
  stream >> asymmetricAtom->_charge;
  if(versionNumber >= 2)
  {
    qint64 hybridization;
    stream >> hybridization;
    asymmetricAtom->_hybridization = SKAsymmetricAtom::Hybridization(hybridization);
  }
  stream >> asymmetricAtom->_uniqueForceFieldName;
  stream >> asymmetricAtom->_elementIdentifier;
  stream >> asymmetricAtom->_color;
  stream >> asymmetricAtom->_drawRadius;

  stream >> asymmetricAtom->_bondDistanceCriteria;
  stream >> asymmetricAtom->_potentialParameters;
  stream >> asymmetricAtom->_tag;
  stream >> asymmetricAtom->_isFixed;
  stream >> asymmetricAtom->_isVisible;
  stream >> asymmetricAtom->_isVisibleEnabled;

  stream >> asymmetricAtom->_serialNumber;
  stream >> asymmetricAtom->_remotenessIndicator;
  stream >> asymmetricAtom->_branchDesignator;
  stream >> asymmetricAtom->_asymetricID;
  stream >> asymmetricAtom->_alternateLocationIndicator;
  stream >> asymmetricAtom->_residueName;
  stream >> asymmetricAtom->_chainIdentifier;
  stream >> asymmetricAtom->_residueSequenceNumber;
  stream >> asymmetricAtom->_codeForInsertionOfResidues;
  stream >> asymmetricAtom->_occupancy;
  stream >> asymmetricAtom->_temperaturefactor;
  stream >> asymmetricAtom->_segmentIdentifier;

  stream >> asymmetricAtom->_ligandAtom;
  stream >> asymmetricAtom->_backBoneAtom;
  stream >> asymmetricAtom->_fractional;
  stream >> asymmetricAtom->_solvent;

  stream >> asymmetricAtom->_copies;

  for(std::shared_ptr<SKAtomCopy> atom: asymmetricAtom->_copies)
  {
    atom->_parent = asymmetricAtom;
  }
  return stream;
}
