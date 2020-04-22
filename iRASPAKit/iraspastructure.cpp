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

#include "iraspastructure.h"

QString iRASPAStructure::displayName() const
{
  return _displayName;
}

QDataStream &operator<<(QDataStream &stream, const std::shared_ptr<iRASPAStructure> &iraspa_structure)
{
  stream << static_cast<typename std::underlying_type<iRASPAStructureType>::type>(iraspa_structure->_rawValue);
  switch(iraspa_structure->_rawValue)
  {
  case iRASPAStructureType::none:
    break;
  case iRASPAStructureType::structure:
    stream << iraspa_structure->_structure;
    break;
  case iRASPAStructureType::crystal:
    stream << std::dynamic_pointer_cast<Crystal>(iraspa_structure->_structure);
    stream << iraspa_structure->_structure;
    break;
  case iRASPAStructureType::molecularCrystal:
    stream << std::dynamic_pointer_cast<MolecularCrystal>(iraspa_structure->_structure);
    stream << iraspa_structure->_structure;
    break;
  case iRASPAStructureType::proteinCrystal:
    stream << std::dynamic_pointer_cast<ProteinCrystal>(iraspa_structure->_structure);
    stream << iraspa_structure->_structure;
    break;
  case iRASPAStructureType::molecule:
    stream << std::dynamic_pointer_cast<Molecule>(iraspa_structure->_structure);
    stream << iraspa_structure->_structure;
    break;
  case iRASPAStructureType::protein:
    stream << std::dynamic_pointer_cast<Protein>(iraspa_structure->_structure);
    stream << iraspa_structure->_structure;
    break;
  case iRASPAStructureType::proteinCrystalSolvent:
    break;
  case iRASPAStructureType::crystalSolvent:
    break;
  case iRASPAStructureType::molecularCrystalSolvent:
    break;
  case iRASPAStructureType::ellipsoidPrimitive:
    stream << std::dynamic_pointer_cast<EllipsoidPrimitive>(iraspa_structure->_structure);
    stream << iraspa_structure->_structure;
    break;
  case iRASPAStructureType::cylinderPrimitive:
    stream << std::dynamic_pointer_cast<CylinderPrimitive>(iraspa_structure->_structure);
    stream << iraspa_structure->_structure;
    break;
  case iRASPAStructureType::polygonalPrismPrimitive:
    stream << std::dynamic_pointer_cast<PolygonalPrismPrimitive>(iraspa_structure->_structure);
    stream << iraspa_structure->_structure;
    break;
  }

  return stream;
}

QDataStream &operator>>(QDataStream &stream, std::shared_ptr<iRASPAStructure> &iraspa_structure)
{
  qint64 type;
  stream >> type;
  iraspa_structure->_rawValue = iRASPAStructureType(type);

  switch(iraspa_structure->_rawValue)
  {
    case iRASPAStructureType::none:
      break;
    case iRASPAStructureType::structure:
    {
      std::shared_ptr<Structure> structure = std::make_shared<Structure>();
      stream >> structure;
      iraspa_structure->_structure = structure;
      stream >> iraspa_structure->_structure;
      break;
    }
    case iRASPAStructureType::crystal:
    {
      std::shared_ptr<Crystal> crystal = std::make_shared<Crystal>();
      stream >> crystal;
      iraspa_structure->_structure = crystal;
      stream >> iraspa_structure->_structure;
      break;
    }
    case iRASPAStructureType::molecularCrystal:
    {
      std::shared_ptr<MolecularCrystal> molecularCrystal = std::make_shared<MolecularCrystal>();
      stream >> molecularCrystal;
      iraspa_structure->_structure = molecularCrystal;
      stream >> iraspa_structure->_structure;
      break;
    }
    case iRASPAStructureType::proteinCrystal:
    {
      std::shared_ptr<ProteinCrystal> proteinCrystal = std::make_shared<ProteinCrystal>();
      stream >> proteinCrystal;
      iraspa_structure->_structure = proteinCrystal;
      stream >> iraspa_structure->_structure;
      break;
    }
    case iRASPAStructureType::molecule:
    {
      std::shared_ptr<Molecule> molecule = std::make_shared<Molecule>();
      stream >> molecule;
      iraspa_structure->_structure = molecule;
      stream >> iraspa_structure->_structure;
      break;
    }
    case iRASPAStructureType::protein:
    {
      std::shared_ptr<Protein> protein = std::make_shared<Protein>();
      stream >> protein;
      iraspa_structure->_structure = protein;
      stream >> iraspa_structure->_structure;
      break;
    }
    case iRASPAStructureType::proteinCrystalSolvent:
      break;
    case iRASPAStructureType::crystalSolvent:
      break;
    case iRASPAStructureType::molecularCrystalSolvent:
      break;
    case iRASPAStructureType::ellipsoidPrimitive:
    {
      std::shared_ptr<EllipsoidPrimitive> primitive = std::make_shared<EllipsoidPrimitive>();
      stream >> primitive;
      iraspa_structure->_structure = primitive;
      stream >> iraspa_structure->_structure;
      break;
    }
    case iRASPAStructureType::cylinderPrimitive:
    {
      std::shared_ptr<CylinderPrimitive> primitive = std::make_shared<CylinderPrimitive>();
      stream >> primitive;
      iraspa_structure->_structure = primitive;
      stream >> iraspa_structure->_structure;
      break;
    }
    case iRASPAStructureType::polygonalPrismPrimitive:
    {
      std::shared_ptr<PolygonalPrismPrimitive> primitive = std::make_shared<PolygonalPrismPrimitive>();
      stream >> primitive;
      iraspa_structure->_structure = primitive;
      stream >> iraspa_structure->_structure;
      break;
    }
  }

  return stream;
}
