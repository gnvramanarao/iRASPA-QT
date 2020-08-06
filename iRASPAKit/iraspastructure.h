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

#include <QString>
#include <QDebug>
#include <typeinfo>
#include "structure.h"
#include "crystal.h"
#include "molecularcrystal.h"
#include "iraspakitprotocols.h"
#include "proteincrystal.h"
#include "molecule.h"
#include "protein.h"
#include "ellipsoidprimitive.h"
#include "cylinderprimitive.h"
#include "polygonalprismprimitive.h"
#include "crystalellipsoidprimitive.h"
#include "crystalcylinderprimitive.h"
#include "crystalpolygonalprismprimitive.h"

class Movie;

class iRASPAStructure: public std::enable_shared_from_this<iRASPAStructure>, public DisplayableProtocol
{
public:
  iRASPAStructure():_rawValue(iRASPAStructureType::none), _structure(std::make_shared<Structure>()) {}
  iRASPAStructure(std::shared_ptr<Structure> structure):_rawValue(iRASPAStructureType::structure), _structure(structure) {}
  iRASPAStructure(std::shared_ptr<Crystal> crystal):_rawValue(iRASPAStructureType::crystal), _structure(crystal) {}
  iRASPAStructure(std::shared_ptr<MolecularCrystal> molecularCrystal):_rawValue(iRASPAStructureType::molecularCrystal), _structure(molecularCrystal) {}
  iRASPAStructure(std::shared_ptr<ProteinCrystal> proteinCrystal):_rawValue(iRASPAStructureType::proteinCrystal), _structure(proteinCrystal) {}
  iRASPAStructure(std::shared_ptr<Molecule> molecule):_rawValue(iRASPAStructureType::molecule), _structure(molecule) {}
  iRASPAStructure(std::shared_ptr<Protein> protein):_rawValue(iRASPAStructureType::protein), _structure(protein) {}
  iRASPAStructure(std::shared_ptr<EllipsoidPrimitive> ellipsoidPrimitive):_rawValue(iRASPAStructureType::ellipsoidPrimitive), _structure(ellipsoidPrimitive) {}
  iRASPAStructure(std::shared_ptr<CylinderPrimitive> cylinderPrimitive):_rawValue(iRASPAStructureType::cylinderPrimitive), _structure(cylinderPrimitive) {}
  iRASPAStructure(std::shared_ptr<PolygonalPrismPrimitive> polygonalPrismPrimitive):_rawValue(iRASPAStructureType::polygonalPrismPrimitive), _structure(polygonalPrismPrimitive) {}
  iRASPAStructure(std::shared_ptr<CrystalEllipsoidPrimitive> crystalEllipsoidPrimitive):_rawValue(iRASPAStructureType::crystalEllipsoidPrimitive), _structure(crystalEllipsoidPrimitive) {}
  iRASPAStructure(std::shared_ptr<CrystalCylinderPrimitive> crystalCylinderPrimitive):_rawValue(iRASPAStructureType::crystalCylinderPrimitive), _structure(crystalCylinderPrimitive) {}
  iRASPAStructure(std::shared_ptr<CrystalPolygonalPrismPrimitive> crystalPolygonalPrismPrimitive):_rawValue(iRASPAStructureType::crystalPolygonalPrismPrimitive), _structure(crystalPolygonalPrismPrimitive) {}

  inline std::shared_ptr<Structure> structure() const {return _structure;}
  inline iRASPAStructureType type() const {return _rawValue;}
  inline void setStructure(std::shared_ptr<Structure> structure) {_structure = structure;}
  inline void setStructure(std::shared_ptr<Structure> structure, iRASPAStructureType rawValue) {_structure = structure; _rawValue = rawValue;}
  static void swapRepresentedObjects(std::shared_ptr<iRASPAStructure> s1, std::shared_ptr<iRASPAStructure> s2);
  QString displayName() const override final;
  void setParent(std::weak_ptr<Movie> parent) {_parent = parent;}
  std::weak_ptr<Movie> parent() {return _parent;}
  virtual ~iRASPAStructure() {}
protected:
  iRASPAStructureType _rawValue;
  std::weak_ptr<Movie> _parent{};
  std::shared_ptr<Structure> _structure;
  QString _displayName = QString("structure");

  friend QDataStream &operator<<(QDataStream &, const std::shared_ptr<iRASPAStructure> &);
  friend QDataStream &operator>>(QDataStream &, std::shared_ptr<iRASPAStructure> &);
};
