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

#include "celltreewidgetchangestructurecommand.h"
#include <QDebug>
#include <algorithm>

CellTreeWidgetChangeStructureCommand::CellTreeWidgetChangeStructureCommand(MainWindow *main_window, std::shared_ptr<ProjectTreeNode> projectTreeNode, std::vector<std::shared_ptr<iRASPAStructure>> old_iraspa_structures, int value, QUndoCommand *undoParent):
  QUndoCommand(undoParent),
  _main_window(main_window),
  _projectTreeNode(projectTreeNode),
  _old_iraspa_structures(old_iraspa_structures),
  _old_structures(),
  _value(value)
{
  setText(QString("Change structure"));

  _old_structures.clear();
  std::transform(old_iraspa_structures.begin(),old_iraspa_structures.end(),std::back_inserter(_old_structures),
                 [](std::shared_ptr<iRASPAStructure> iraspastructure) -> std::tuple<std::shared_ptr<iRASPAStructure>, std::shared_ptr<Structure>, iRASPAStructureType>
                 {return std::make_tuple(iraspastructure,iraspastructure->structure(), iraspastructure->type());});
}

void CellTreeWidgetChangeStructureCommand::redo()
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _old_iraspa_structures)
  {
    switch(_value)
    {
      case static_cast<typename std::underlying_type<Structure::CreationMethod>::type>(SKStructure::Kind::crystal):
      {
        std::shared_ptr<Crystal> crystal = std::make_shared<Crystal>(iraspa_structure->structure());
        iraspa_structure->setStructure(crystal, iRASPAStructureType::crystal);
        break;
      }
      case static_cast<typename std::underlying_type<Structure::CreationMethod>::type>(SKStructure::Kind::molecularCrystal):
      {
        std::shared_ptr<MolecularCrystal> molecularCrystal = std::make_shared<MolecularCrystal>(iraspa_structure->structure());
        iraspa_structure->setStructure(molecularCrystal, iRASPAStructureType::molecularCrystal);
        break;
      }
      case static_cast<typename std::underlying_type<Structure::CreationMethod>::type>(SKStructure::Kind::molecule):
      {
        std::shared_ptr<Molecule> molecule = std::make_shared<Molecule>(iraspa_structure->structure());
        iraspa_structure->setStructure(molecule, iRASPAStructureType::molecule);
        break;
      }
      case static_cast<typename std::underlying_type<Structure::CreationMethod>::type>(SKStructure::Kind::protein):
      {
        std::shared_ptr<Protein> protein = std::make_shared<Protein>(iraspa_structure->structure());
        iraspa_structure->setStructure(protein, iRASPAStructureType::protein);
        break;
      }
      case static_cast<typename std::underlying_type<Structure::CreationMethod>::type>(SKStructure::Kind::proteinCrystal):
      {
        std::shared_ptr<ProteinCrystal> proteinCrystal = std::make_shared<ProteinCrystal>(iraspa_structure->structure());
        iraspa_structure->setStructure(proteinCrystal, iRASPAStructureType::proteinCrystal);
        break;
      }
      case static_cast<typename std::underlying_type<Structure::CreationMethod>::type>(SKStructure::Kind::crystalEllipsoidPrimitive):
      {
        std::shared_ptr<CrystalEllipsoidPrimitive> crystalEllipsoidPrimitive = std::make_shared<CrystalEllipsoidPrimitive>(iraspa_structure->structure());
        iraspa_structure->setStructure(crystalEllipsoidPrimitive, iRASPAStructureType::crystalEllipsoidPrimitive);
        break;
      }
      case static_cast<typename std::underlying_type<Structure::CreationMethod>::type>(SKStructure::Kind::crystalCylinderPrimitive):
      {
        std::shared_ptr<CrystalCylinderPrimitive> crystalCylinderPrimitive = std::make_shared<CrystalCylinderPrimitive>(iraspa_structure->structure());
        iraspa_structure->setStructure(crystalCylinderPrimitive, iRASPAStructureType::crystalCylinderPrimitive);
        break;
      }
      case static_cast<typename std::underlying_type<Structure::CreationMethod>::type>(SKStructure::Kind::crystalPolygonalPrismPrimitive):
      {
        std::shared_ptr<CrystalPolygonalPrismPrimitive> crystalPolygonalPrismPrimitive = std::make_shared<CrystalPolygonalPrismPrimitive>(iraspa_structure->structure());
        iraspa_structure->setStructure(crystalPolygonalPrismPrimitive, iRASPAStructureType::crystalPolygonalPrismPrimitive);
        break;
      }
      case static_cast<typename std::underlying_type<Structure::CreationMethod>::type>(SKStructure::Kind::ellipsoidPrimitive):
      {
        std::shared_ptr<EllipsoidPrimitive> ellipsoidPrimitive = std::make_shared<EllipsoidPrimitive>(iraspa_structure->structure());
        iraspa_structure->setStructure(ellipsoidPrimitive, iRASPAStructureType::ellipsoidPrimitive);
        break;
      }
      case static_cast<typename std::underlying_type<Structure::CreationMethod>::type>(SKStructure::Kind::cylinderPrimitive):
      {
        std::shared_ptr<CylinderPrimitive> cylinderPrimitive = std::make_shared<CylinderPrimitive>(iraspa_structure->structure());
        iraspa_structure->setStructure(cylinderPrimitive, iRASPAStructureType::cylinderPrimitive);
        break;
      }
      case static_cast<typename std::underlying_type<Structure::CreationMethod>::type>(SKStructure::Kind::polygonalPrismPrimitive):
      {
        std::shared_ptr<PolygonalPrismPrimitive> polygonalPrismPrimitive = std::make_shared<PolygonalPrismPrimitive>(iraspa_structure->structure());
        iraspa_structure->setStructure(polygonalPrismPrimitive, iRASPAStructureType::polygonalPrismPrimitive);
        break;
      }
      default:
        break;
    }
  }

  _main_window->propagateProject(_projectTreeNode, _main_window);

  //SKBoundingBox box = _projectTreeNode->representedObject()->renderBoundingBox();
  //_projectStructure->camera()->resetForNewBoundingBox(box);

  //emit invalidateCachedAmbientOcclusionTexture(render_structures);
  //emit reloadAllViews();
}

void CellTreeWidgetChangeStructureCommand::undo()
{
  for(const auto [iraspaStructure, structure, type] : _old_structures)
  {
    iraspaStructure->setStructure(structure, type);
  }
  _main_window->propagateProject(_projectTreeNode, _main_window);
}


