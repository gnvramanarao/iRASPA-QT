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

#include "celltreewidgetcontroller.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <type_traits>
#include <tuple>
#include <QLabel>
#include <QSpinBox>
#include <mathkit.h>
#include <renderkit.h>
#include <symmetrykit.h>
#include <foundationkit.h>
#include "textfield.h"

CellTreeWidgetController::CellTreeWidgetController(QWidget* parent): QTreeWidget(parent ),
    _cellCellForm(new CellCellForm),
    _cellTransformContentForm(new CellTransformContentForm),
    _cellStructuralForm(new CellStructuralForm),
    _cellSymmetryForm(new CellSymmetryForm)
{
  //this->viewport()->setMouseTracking(true);
  this->setHeaderHidden(true);
  this->setRootIsDecorated(true);
  this->setFrameStyle(QFrame::NoFrame);
  this->setAnimated(true);
  this->setVerticalScrollMode(QTreeWidget::ScrollPerPixel);
  this->setExpandsOnDoubleClick(false);
  this->setIndentation(0);
  this->setSelectionMode(QAbstractItemView::NoSelection);

  // Cell
  //==============================================================================================
  QTreeWidgetItem* cellItem = new QTreeWidgetItem(this);
  this->addTopLevelItem(cellItem);

  pushButtonCell = new QPushButton(QString("Cell"),this);
  pushButtonCell->setIcon(QIcon(":/iRASPA/collapsed.png"));
  pushButtonCell->setStyleSheet("text-align:left;");
  setItemWidget(cellItem,0,pushButtonCell);

  QTreeWidgetItem *childCellItem = new QTreeWidgetItem(cellItem);
  this->setItemWidget(childCellItem,0, _cellCellForm);

  QObject::connect(pushButtonCell, &QPushButton::clicked, this, &CellTreeWidgetController::expandCellItem);

  pushButtonCell->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
  QFontMetrics fm(font());
  pushButtonCell->resize(size().width(), fm.height());

  _cellCellForm->cellStructureTypeComboBox->insertItem(0, "Empty");
  _cellCellForm->cellStructureTypeComboBox->insertItem(1, "Structure");
  _cellCellForm->cellStructureTypeComboBox->insertItem(2, "Crystal");
  _cellCellForm->cellStructureTypeComboBox->insertItem(3, "Molecular crystal");
  _cellCellForm->cellStructureTypeComboBox->insertItem(4, "Molecule");
  _cellCellForm->cellStructureTypeComboBox->insertItem(5, "Protein");
  _cellCellForm->cellStructureTypeComboBox->insertItem(6, "Protein crystal");
  _cellCellForm->cellStructureTypeComboBox->insertItem(7, "Protein crystal solvent");
  _cellCellForm->cellStructureTypeComboBox->insertItem(8, "Crystal solvent");
  _cellCellForm->cellStructureTypeComboBox->insertItem(9, "Molecular crystal solvent");
  _cellCellForm->cellStructureTypeComboBox->insertItem(10, "Crystal ellipse primitive");
  _cellCellForm->cellStructureTypeComboBox->insertItem(11, "Crystal cylinder primitive");
  _cellCellForm->cellStructureTypeComboBox->insertItem(12, "Crystal polygonal prism primitive");
  _cellCellForm->cellStructureTypeComboBox->insertItem(13, "Ellipse primitive");
  _cellCellForm->cellStructureTypeComboBox->insertItem(14, "Cylinder primitive");
  _cellCellForm->cellStructureTypeComboBox->insertItem(15, "Polygonal prism primitive");
  QStandardItemModel *model = qobject_cast<QStandardItemModel *>( _cellCellForm->cellStructureTypeComboBox->model());
  QStandardItem *itemEmpty = model->item(0);
  itemEmpty->setFlags(itemEmpty->flags() & ~Qt::ItemIsEnabled);
  QStandardItem *itemStructure = model->item(1);
  itemStructure->setFlags(itemStructure->flags() & ~Qt::ItemIsEnabled);
  QStandardItem *itemProteinCrystalSolvent = model->item(7);
  itemProteinCrystalSolvent->setFlags(itemProteinCrystalSolvent->flags() & ~Qt::ItemIsEnabled);
  QStandardItem *itemCrystalSolvent = model->item(8);
  itemCrystalSolvent->setFlags(itemCrystalSolvent->flags() & ~Qt::ItemIsEnabled);
  QStandardItem *itemMolecularCrystalSolvent = model->item(9);
  itemMolecularCrystalSolvent->setFlags(itemMolecularCrystalSolvent->flags() & ~Qt::ItemIsEnabled);
  QObject::connect(_cellCellForm->cellStructureTypeComboBox,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),this,&CellTreeWidgetController::setStructureType);


  QObject::connect(_cellCellForm->unitCellADoubleSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&CellTreeWidgetController::setUnitCellLengthA);
  QObject::connect(_cellCellForm->unitCellBDoubleSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&CellTreeWidgetController::setUnitCellLengthB);
  QObject::connect(_cellCellForm->unitCellCDoubleSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&CellTreeWidgetController::setUnitCellLengthC);

  QObject::connect(_cellCellForm->unitCellAlphaDoubleSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&CellTreeWidgetController::setUnitCellAngleAlpha);
  QObject::connect(_cellCellForm->unitCellBetaDoubleSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&CellTreeWidgetController::setUnitCellAngleBeta);
  QObject::connect(_cellCellForm->unitCellGammaDoubleSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&CellTreeWidgetController::setUnitCellAngleGamma);


  _cellCellForm->cellMaximumReplicaX->setRange(-INT_MAX,INT_MAX);
  _cellCellForm->cellMaximumReplicaX->setSpecialValueText(QString("Mult.Val."));
  _cellCellForm->cellMaximumReplicaY->setRange(-INT_MAX,INT_MAX);
  _cellCellForm->cellMaximumReplicaY->setSpecialValueText(QString("Mult.Val."));
  _cellCellForm->cellMaximumReplicaZ->setRange(-INT_MAX,INT_MAX);
  _cellCellForm->cellMaximumReplicaZ->setSpecialValueText(QString("Mult.Val."));

  _cellCellForm->cellMinimumReplicaX->setRange(-INT_MAX,INT_MAX);
  _cellCellForm->cellMinimumReplicaX->setSpecialValueText(QString("Mult.Val."));
  _cellCellForm->cellMinimumReplicaY->setRange(-INT_MAX,INT_MAX);
  _cellCellForm->cellMinimumReplicaY->setSpecialValueText(QString("Mult.Val."));
  _cellCellForm->cellMinimumReplicaZ->setRange(-INT_MAX,INT_MAX);
  _cellCellForm->cellMinimumReplicaZ->setSpecialValueText(QString("Mult.Val."));

  _cellCellForm->volumeDoubleSpinBox->setAttribute(Qt::WA_MacShowFocusRect, false);


  QObject::connect(_cellCellForm->cellMaximumReplicaX, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),this,&CellTreeWidgetController::setMaximumReplicasX);
  QObject::connect(_cellCellForm->cellMaximumReplicaY, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),this,&CellTreeWidgetController::setMaximumReplicasY);
  QObject::connect(_cellCellForm->cellMaximumReplicaZ, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),this,&CellTreeWidgetController::setMaximumReplicasZ);
  QObject::connect(_cellCellForm->cellMinimumReplicaX, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),this,&CellTreeWidgetController::setMinimumReplicasX);
  QObject::connect(_cellCellForm->cellMinimumReplicaY, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),this,&CellTreeWidgetController::setMinimumReplicasY);
  QObject::connect(_cellCellForm->cellMinimumReplicaZ, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),this,&CellTreeWidgetController::setMinimumReplicasZ);


  _cellCellForm->EulerAngleXDoubleSpinBox->setMinimum(-180);
  _cellCellForm->EulerAngleXDoubleSpinBox->setMaximum(180);
  _cellCellForm->EulerAngleXDial->setDoubleMinimum(-180);
  _cellCellForm->EulerAngleXDial->setDoubleMaximum(180);

  _cellCellForm->EulerAngleYDoubleSpinBox->setMinimum(-90.0);
  _cellCellForm->EulerAngleYDoubleSpinBox->setMaximum(90.0);
  _cellCellForm->EulerAngleYSlider->setDoubleMinimum(-90.0);
  _cellCellForm->EulerAngleYSlider->setDoubleMaximum(90.0);

  _cellCellForm->EulerAngleZDoubleSpinBox->setMinimum(-180.0);
  _cellCellForm->EulerAngleZDoubleSpinBox->setMaximum(180.0);
  _cellCellForm->EulerAngleZDial->setDoubleMinimum(-180);
  _cellCellForm->EulerAngleZDial->setDoubleMaximum(180);

  QObject::connect(_cellCellForm->rotationAngleDoubleSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &CellTreeWidgetController::setRotationAngle);
  QObject::connect(_cellCellForm->rotatePlusXPushButton, &QPushButton::clicked, this, &CellTreeWidgetController::rotateYawPlus);
  QObject::connect(_cellCellForm->rotatePlusZPushButton, &QPushButton::clicked, this, &CellTreeWidgetController::rotatePitchPlus);
  QObject::connect(_cellCellForm->rotatePlusYPushButton, &QPushButton::clicked, this, &CellTreeWidgetController::rotateRollPlus);
  QObject::connect(_cellCellForm->rotateMinusXPushButton, &QPushButton::clicked, this, &CellTreeWidgetController::rotateYawMinus);
  QObject::connect(_cellCellForm->rotateMinusZPushButton, &QPushButton::clicked, this, &CellTreeWidgetController::rotatePitchMinus);
  QObject::connect(_cellCellForm->rotateMinusYPushButton, &QPushButton::clicked, this, &CellTreeWidgetController::rotateRollMinus);


  QObject::connect(_cellCellForm->EulerAngleXDoubleSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &CellTreeWidgetController::setEulerAngleX);
  QObject::connect(_cellCellForm->EulerAngleYDoubleSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &CellTreeWidgetController::setEulerAngleY);
  QObject::connect(_cellCellForm->EulerAngleZDoubleSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &CellTreeWidgetController::setEulerAngleZ);
  QObject::connect(_cellCellForm->EulerAngleXDial,static_cast<void (QDoubleDial::*)(double)>(&QDoubleDial::sliderMoved),this,&CellTreeWidgetController::setEulerAngleX);
  QObject::connect(_cellCellForm->EulerAngleYSlider,static_cast<void (QDoubleSlider::*)(double)>(&QDoubleSlider::sliderMoved),this,&CellTreeWidgetController::setEulerAngleY);
  QObject::connect(_cellCellForm->EulerAngleZDial,static_cast<void (QDoubleDial::*)(double)>(&QDoubleDial::sliderMoved),this,&CellTreeWidgetController::setEulerAngleZ);

  QObject::connect(_cellCellForm->originXDoubleSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &CellTreeWidgetController::setOriginX);
  QObject::connect(_cellCellForm->originYDoubleSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &CellTreeWidgetController::setOriginY);
  QObject::connect(_cellCellForm->originZDoubleSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &CellTreeWidgetController::setOriginZ);

  // Tranform content
  //==============================================================================================
  QTreeWidgetItem* transformContentItem = new QTreeWidgetItem(this);
  this->addTopLevelItem(transformContentItem);

  pushButtonTransformContent = new QPushButton(QString("Tranform content"),this);
  pushButtonTransformContent->setIcon(QIcon(":/iRASPA/collapsed.png"));
  pushButtonTransformContent->setStyleSheet("text-align:left;");
  setItemWidget(transformContentItem,0,pushButtonTransformContent);

  QTreeWidgetItem *childTransformContentItem = new QTreeWidgetItem(transformContentItem);
  this->setItemWidget(childTransformContentItem,0, _cellTransformContentForm);

  QObject::connect(pushButtonTransformContent, &QPushButton::clicked, this, &CellTreeWidgetController::expandTransformContentItem);

  // Structural
  //==============================================================================================
  QTreeWidgetItem* structuralItem = new QTreeWidgetItem(this);
  this->addTopLevelItem(structuralItem);

  pushButtonStructural = new QPushButton(QString("Structural properties"),this);
  pushButtonStructural->setIcon(QIcon(":/iRASPA/collapsed.png"));
  pushButtonStructural->setStyleSheet("text-align:left;");
  setItemWidget(structuralItem,0,pushButtonStructural);

  QTreeWidgetItem *childStructuralItem = new QTreeWidgetItem(structuralItem);
  this->setItemWidget(childStructuralItem,0, _cellStructuralForm);

  _cellStructuralForm->massDoubleSpinBox->setReadOnly(true);

  QObject::connect(pushButtonStructural, &QPushButton::clicked, this, &CellTreeWidgetController::expandStructuralItem);

  pushButtonCell->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
  pushButtonCell->resize(size().width(), fm.height());

  QObject::connect(_cellStructuralForm->computeHeliumVoidFractionPushButton, &QPushButton::clicked, this, &CellTreeWidgetController::computeHeliumVoidFractionPushButton);

  _cellStructuralForm->probeMoleculeComboBox->insertItem(0, "Helium");
  _cellStructuralForm->probeMoleculeComboBox->insertItem(1, "Methane");
  _cellStructuralForm->probeMoleculeComboBox->insertItem(2, "Nitrogen");
  _cellStructuralForm->probeMoleculeComboBox->insertItem(3, "Hydrogen");
  _cellStructuralForm->probeMoleculeComboBox->insertItem(4, "Water");
  _cellStructuralForm->probeMoleculeComboBox->insertItem(5, "CO₂");
  _cellStructuralForm->probeMoleculeComboBox->insertItem(6, "Xenon");
  _cellStructuralForm->probeMoleculeComboBox->insertItem(7, "Krypton");
  _cellStructuralForm->probeMoleculeComboBox->insertItem(8, "Argon");
  QObject::connect(_cellStructuralForm->probeMoleculeComboBox,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),this,&CellTreeWidgetController::setFrameworkProbeMolecule);


  QObject::connect(_cellStructuralForm->computeGravimetricSurfaceAreaPushButton, &QPushButton::clicked, this, &CellTreeWidgetController::computeGravimetricSurfaceAreaPushButton);
  QObject::connect(_cellStructuralForm->computeVolumetricSurfaceAreaPushButton, &QPushButton::clicked,this, &CellTreeWidgetController::computeGravimetricSurfaceAreaPushButton);

  QObject::connect(_cellStructuralForm->numberOfChannelSystemsSpinBox, static_cast<void (CustomIntSpinBox::*)(int)>(&CustomIntSpinBox::valueChanged),this, &CellTreeWidgetController::setStructureNumberOfChannelSystems);
  QObject::connect(_cellStructuralForm->numberOfInaccessiblePocketsSpinBox, static_cast<void (CustomIntSpinBox::*)(int)>(&CustomIntSpinBox::valueChanged),this, &CellTreeWidgetController::setStructureNumberOfInaccessiblePockets);
  QObject::connect(_cellStructuralForm->dimensionalityOfPoreSystemSpinBox, static_cast<void (CustomIntSpinBox::*)(int)>(&CustomIntSpinBox::valueChanged),this, &CellTreeWidgetController::setStructureDimensionalityOfPoreSystem);

  QObject::connect(_cellStructuralForm->largestOverallCavityDiameterDoubleSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this, &CellTreeWidgetController::setStructureLargestCavityDiameter);
  QObject::connect(_cellStructuralForm->restrictingPoreDiameterDoubleSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this, &CellTreeWidgetController::setStructureRestrictingPoreLimitingDiameter);
  QObject::connect(_cellStructuralForm->largestDiameterAlongAViablePathDoubleSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this, &CellTreeWidgetController::setStructureLargestCavityDiameterAlongAviablePath);


  _cellStructuralForm->materialTypeComboBox->insertItem(0,"Unspecified");
  _cellStructuralForm->materialTypeComboBox->insertItem(1,"Silica");
  _cellStructuralForm->materialTypeComboBox->insertItem(2,"Aluminosilicate");
  _cellStructuralForm->materialTypeComboBox->insertItem(3,"Metallophosphate");
  _cellStructuralForm->materialTypeComboBox->insertItem(4,"Silicialinophosphate");
  _cellStructuralForm->materialTypeComboBox->insertItem(5,"Zeolite");
  _cellStructuralForm->materialTypeComboBox->insertItem(6,"MOF");
  _cellStructuralForm->materialTypeComboBox->insertItem(7,"ZIF");


  // Symmetry
  //==============================================================================================
  QTreeWidgetItem* symmetryItem = new QTreeWidgetItem(this);
  this->addTopLevelItem(symmetryItem);

  pushButtonSymmetry = new QPushButton(QString("Symmetry properties"),this);
  pushButtonSymmetry->setIcon(QIcon(":/iRASPA/collapsed.png"));
  pushButtonSymmetry->setStyleSheet("text-align:left;");
  setItemWidget(symmetryItem,0,pushButtonSymmetry);

  QTreeWidgetItem *childSymmetryItem = new QTreeWidgetItem(symmetryItem);
  this->setItemWidget(childSymmetryItem,0, _cellSymmetryForm);

  QObject::connect(pushButtonSymmetry, &QPushButton::clicked, this, &CellTreeWidgetController::expandSymmetryItem);

  pushButtonSymmetry->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
  pushButtonSymmetry->resize(size().width(), fm.height());

  _cellSymmetryForm->spaceGroupHallNamecomboBox->clear();
  int index=0;
  for(const SKSpaceGroupSetting& spaceGroup : SKSpaceGroup::spaceGroupData())
  {
    _cellSymmetryForm->spaceGroupHallNamecomboBox->addItem(QString::number(index) + " " + spaceGroup.HallString());
    index++;
  }

  QObject::connect(_cellSymmetryForm->spaceGroupHallNamecomboBox,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),this,&CellTreeWidgetController::setSymmetrySpaceGroupHallNumber);
  QObject::connect(_cellSymmetryForm->spaceGroupITNumberComboBox,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),this,&CellTreeWidgetController::setSymmetrySpaceGroupStandardNumber);

  _cellSymmetryForm->spaceGroupITNumberComboBox->clear();
  for(int i=0; i<=230; i++)
  {
    _cellSymmetryForm->spaceGroupITNumberComboBox->insertItem(i,QString::number(i));
  }

  QObject::connect(_cellSymmetryForm->precisionDoubleSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &CellTreeWidgetController::setSymmetryPrecision);
}


void CellTreeWidgetController::expandCellItem()
{
  QModelIndex index = indexFromItem(topLevelItem(0),0);
  if(this->isExpanded(index))
  {
    this->collapse(index);
    pushButtonCell->setIcon(QIcon(":/iRASPA/collapsed.png"));
  }
  else
  {
    this->expand(index);
    pushButtonCell->setIcon(QIcon(":/iRASPA/expanded.png"));
  }
}

void CellTreeWidgetController::expandTransformContentItem()
{
  QModelIndex index = indexFromItem(topLevelItem(1),0);
  if(this->isExpanded(index))
  {
    this->collapse(index);
    pushButtonTransformContent->setIcon(QIcon(":/iRASPA/collapsed.png"));
  }
  else
  {
    this->expand(index);
    pushButtonTransformContent->setIcon(QIcon(":/iRASPA/expanded.png"));
  }
}

void CellTreeWidgetController::expandStructuralItem()
{
  QModelIndex index = indexFromItem(topLevelItem(2),0);
  if(this->isExpanded(index))
  {
    this->collapse(index);
    pushButtonStructural->setIcon(QIcon(":/iRASPA/collapsed.png"));
  }
  else
  {
    this->expand(index);
    pushButtonStructural->setIcon(QIcon(":/iRASPA/expanded.png"));
  }
}

void CellTreeWidgetController::expandSymmetryItem()
{
  QModelIndex index = indexFromItem(topLevelItem(3),0);
  if(this->isExpanded(index))
  {
    this->collapse(index);
    pushButtonSymmetry->setIcon(QIcon(":/iRASPA/collapsed.png"));
  }
  else
  {
    this->expand(index);
    pushButtonSymmetry->setIcon(QIcon(":/iRASPA/expanded.png"));
  }
}

void CellTreeWidgetController::setProject(std::shared_ptr<ProjectTreeNode> projectTreeNode)
{
  _projectTreeNode = projectTreeNode;
  _projectStructure = nullptr;
  if (projectTreeNode)
  {
    if(std::shared_ptr<iRASPAProject> iraspaProject = projectTreeNode->representedObject())
    {
      if(std::shared_ptr<Project> project = iraspaProject->project())
      {
        if (std::shared_ptr<ProjectStructure> projectStructure = std::dynamic_pointer_cast<ProjectStructure>(project))
        {
          _projectStructure = projectStructure;
          _iraspa_structures = projectStructure->flattenediRASPAStructures();
          reloadData();
        }
      }
    }
  }
}


void CellTreeWidgetController::setFlattenedSelectedFrames(std::vector<std::shared_ptr<iRASPAStructure>> iraspa_structures)
{
  _iraspa_structures = iraspa_structures;
  reloadData();
}

void CellTreeWidgetController::reloadData()
{
  reloadCellProperties();
  reloadTransformContentProperties();
  reloadStructureProperties();
  reloadSymmetryProperties();
}

void CellTreeWidgetController::reloadSelection()
{

}


// Cell/Bounding properties
//========================================================================================================================================

void CellTreeWidgetController::reloadCellProperties()
{
	reloadStructureType();
  reloadBoundingBox();

  reloadUnitCell();
  reloadUnitCellLengths();
  reloadUnitCellAngles();

  reloadCellVolume();
  reloadCellPerpendicularWidthX();
  reloadCellPerpendicularWidthY();
  reloadCellPerpendicularWidthZ();

  reloadCellMaximumReplicasX();
  reloadCellMinimumReplicasX();
  reloadCellMaximumReplicasY();
  reloadCellMinimumReplicasY();
  reloadCellMaximumReplicasZ();
  reloadCellMinimumReplicasZ();

  reloadRotationAngle();
  reloadEulerAngles();

  reloadCellOriginX();
  reloadCellOriginY();
  reloadCellOriginZ();
}


void CellTreeWidgetController::reloadStructureType()
{
  if(_projectTreeNode)
  {
    _cellCellForm->cellStructureTypeComboBox->setEnabled(_projectTreeNode->isEditable());
  }
  else
  {
    _cellCellForm->cellStructureTypeComboBox->setEnabled(false);
  }

  if (!_iraspa_structures.empty())
	{
    if (std::optional<iRASPAStructureType> type = structureType())
		{
			if (int index = _cellCellForm->cellStructureTypeComboBox->findText("Multiple values"); index >= 0)
			{
				whileBlocking(_cellCellForm->cellStructureTypeComboBox)->removeItem(index);
			}
			whileBlocking(_cellCellForm->cellStructureTypeComboBox)->setCurrentIndex(int(*type));
		}
		else
		{
			if (int index = _cellCellForm->cellStructureTypeComboBox->findText("Multiple values"); index < 0)
			{
				whileBlocking(_cellCellForm->cellStructureTypeComboBox)->addItem("Multiple values");
			}
			whileBlocking(_cellCellForm->cellStructureTypeComboBox)->setCurrentText("Multiple values");
		}
	}
}

void CellTreeWidgetController::reloadBoundingBox()
{
  _cellCellForm->cellBoundingBoxMaxXDoubleSpinBox->setReadOnly(true);
  _cellCellForm->cellBoundingBoxMaxYDoubleSpinBox->setReadOnly(true);
  _cellCellForm->cellBoundingBoxMaxZDoubleSpinBox->setReadOnly(true);
  _cellCellForm->cellBoundingBoxMinXDoubleSpinBox->setReadOnly(true);
  _cellCellForm->cellBoundingBoxMinYDoubleSpinBox->setReadOnly(true);
  _cellCellForm->cellBoundingBoxMinZDoubleSpinBox->setReadOnly(true);
  if(_projectTreeNode)
  {
    _cellCellForm->cellBoundingBoxMaxXDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _cellCellForm->cellBoundingBoxMaxYDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _cellCellForm->cellBoundingBoxMaxZDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _cellCellForm->cellBoundingBoxMinXDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _cellCellForm->cellBoundingBoxMinYDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _cellCellForm->cellBoundingBoxMinZDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    SKBoundingBox overAllBoundingBox = boundingBox();

    whileBlocking(_cellCellForm->cellBoundingBoxMaxXDoubleSpinBox)->setValue(overAllBoundingBox.maximum().x);
    whileBlocking(_cellCellForm->cellBoundingBoxMaxYDoubleSpinBox)->setValue(overAllBoundingBox.maximum().y);
    whileBlocking(_cellCellForm->cellBoundingBoxMaxZDoubleSpinBox)->setValue(overAllBoundingBox.maximum().z);

    whileBlocking(_cellCellForm->cellBoundingBoxMinXDoubleSpinBox)->setValue(overAllBoundingBox.minimum().x);
    whileBlocking(_cellCellForm->cellBoundingBoxMinYDoubleSpinBox)->setValue(overAllBoundingBox.minimum().y);
    whileBlocking(_cellCellForm->cellBoundingBoxMinZDoubleSpinBox)->setValue(overAllBoundingBox.minimum().z);
  }
}

void CellTreeWidgetController::reloadUnitCellLengths()
{
  _cellCellForm->unitCellADoubleSpinBox->setReadOnly(true);
  _cellCellForm->unitCellBDoubleSpinBox->setReadOnly(true);
  _cellCellForm->unitCellCDoubleSpinBox->setReadOnly(true);
  if(_projectTreeNode)
  {
    _cellCellForm->unitCellADoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _cellCellForm->unitCellBDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _cellCellForm->unitCellCDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = unitCellLengthA())
    {
      whileBlocking(_cellCellForm->unitCellADoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellCellForm->unitCellADoubleSpinBox)->setText("Mult. Val.");
    }

    if (std::optional<double> value = unitCellLengthB())
    {
      whileBlocking(_cellCellForm->unitCellBDoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellCellForm->unitCellBDoubleSpinBox)->setText("Mult. Val.");
    }

    if (std::optional<double> value = unitCellLengthC())
    {
      whileBlocking(_cellCellForm->unitCellCDoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellCellForm->unitCellCDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}
void CellTreeWidgetController::reloadUnitCellAngles()
{
  _cellCellForm->unitCellAlphaDoubleSpinBox->setReadOnly(true);
  _cellCellForm->unitCellBetaDoubleSpinBox->setReadOnly(true);
  _cellCellForm->unitCellGammaDoubleSpinBox->setReadOnly(true);
  if(_projectTreeNode)
  {
    _cellCellForm->unitCellAlphaDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _cellCellForm->unitCellBetaDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _cellCellForm->unitCellGammaDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = unitCellAngleAlpha())
    {
      whileBlocking(_cellCellForm->unitCellAlphaDoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellCellForm->unitCellAlphaDoubleSpinBox)->setText("Mult. Val.");
    }

    if (std::optional<double> value = unitCellAngleBeta())
    {
      whileBlocking(_cellCellForm->unitCellBetaDoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellCellForm->unitCellBetaDoubleSpinBox)->setText("Mult. Val.");
    }

    if (std::optional<double> value = unitCellAngleGamma())
    {
      whileBlocking(_cellCellForm->unitCellGammaDoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellCellForm->unitCellGammaDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}

void CellTreeWidgetController::reloadUnitCell()
{
  _cellCellForm->unitCellAXDoubleSpinBox->setReadOnly(true);
  _cellCellForm->unitCellAYDoubleSpinBox->setReadOnly(true);
  _cellCellForm->unitCellAZDoubleSpinBox->setReadOnly(true);
  _cellCellForm->unitCellBXDoubleSpinBox->setReadOnly(true);
  _cellCellForm->unitCellBYDoubleSpinBox->setReadOnly(true);
  _cellCellForm->unitCellBZDoubleSpinBox->setReadOnly(true);
  _cellCellForm->unitCellBXDoubleSpinBox->setReadOnly(true);
  _cellCellForm->unitCellBYDoubleSpinBox->setReadOnly(true);
  _cellCellForm->unitCellBZDoubleSpinBox->setReadOnly(true);
  if(_projectTreeNode)
  {
    _cellCellForm->unitCellAXDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _cellCellForm->unitCellAYDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _cellCellForm->unitCellAZDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _cellCellForm->unitCellBXDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _cellCellForm->unitCellBYDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _cellCellForm->unitCellBZDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _cellCellForm->unitCellCXDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _cellCellForm->unitCellCYDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _cellCellForm->unitCellCZDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = unitCellAX())
    {
      whileBlocking(_cellCellForm->unitCellAXDoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellCellForm->unitCellAXDoubleSpinBox)->setText("Mult. Val.");
    }

    if (std::optional<double> value = unitCellAY())
    {
      whileBlocking(_cellCellForm->unitCellAYDoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellCellForm->unitCellAYDoubleSpinBox)->setText("Mult. Val.");
    }

    if (std::optional<double> value = unitCellAZ())
    {
      whileBlocking(_cellCellForm->unitCellAZDoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellCellForm->unitCellAZDoubleSpinBox)->setText("Mult. Val.");
    }

    if (std::optional<double> value = unitCellBX())
    {
      whileBlocking(_cellCellForm->unitCellBXDoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellCellForm->unitCellBXDoubleSpinBox)->setText("Mult. Val.");
    }

    if (std::optional<double> value = unitCellBY())
    {
      whileBlocking(_cellCellForm->unitCellBYDoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellCellForm->unitCellBYDoubleSpinBox)->setText("Mult. Val.");
    }

    if (std::optional<double> value = unitCellBZ())
    {
      whileBlocking(_cellCellForm->unitCellBZDoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellCellForm->unitCellBZDoubleSpinBox)->setText("Mult. Val.");
    }

    if (std::optional<double> value = unitCellCX())
    {
      whileBlocking(_cellCellForm->unitCellCXDoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellCellForm->unitCellCXDoubleSpinBox)->setText("Mult. Val.");
    }

    if (std::optional<double> value = unitCellCY())
    {
      whileBlocking(_cellCellForm->unitCellCYDoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellCellForm->unitCellCYDoubleSpinBox)->setText("Mult. Val.");
    }

    if (std::optional<double> value = unitCellCZ())
    {
      whileBlocking(_cellCellForm->unitCellCZDoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellCellForm->unitCellCZDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}

void CellTreeWidgetController::reloadCellVolume()
{
  _cellCellForm->volumeDoubleSpinBox->setReadOnly(true);

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = unitCellVolume())
    {
      whileBlocking(_cellCellForm->volumeDoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellCellForm->volumeDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}

void CellTreeWidgetController::reloadCellPerpendicularWidthX()
{
  _cellCellForm->perpendicularWidthXDoubleSpinBox->setReadOnly(true);

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = unitCellPerpendicularWidthX())
    {
      whileBlocking(_cellCellForm->perpendicularWidthXDoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellCellForm->perpendicularWidthXDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}

void CellTreeWidgetController::reloadCellPerpendicularWidthY()
{
  _cellCellForm->perpendicularWidthYDoubleSpinBox->setReadOnly(true);

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = unitCellPerpendicularWidthY())
    {
      whileBlocking(_cellCellForm->perpendicularWidthYDoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellCellForm->perpendicularWidthYDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}

void CellTreeWidgetController::reloadCellPerpendicularWidthZ()
{
  _cellCellForm->perpendicularWidthZDoubleSpinBox->setReadOnly(true);

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = unitCellPerpendicularWidthZ())
    {
      whileBlocking(_cellCellForm->perpendicularWidthZDoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellCellForm->perpendicularWidthZDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}

void CellTreeWidgetController::reloadCellMaximumReplicasX()
{
  _cellCellForm->cellMaximumReplicaX->setReadOnly(true);
  if(_projectTreeNode)
  {
    _cellCellForm->cellMaximumReplicaX->setReadOnly(!_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<int> value = maximumReplicasX())
    {
      whileBlocking(_cellCellForm->cellMaximumReplicaX)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellCellForm->cellMaximumReplicaX)->setText("Mult. Val.");
    }
  }
}

void CellTreeWidgetController::reloadCellMinimumReplicasX()
{
  _cellCellForm->cellMinimumReplicaX->setReadOnly(true);
  if(_projectTreeNode)
  {
    _cellCellForm->cellMinimumReplicaX->setReadOnly(!_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<int> value = minimumReplicasX())
    {
      whileBlocking(_cellCellForm->cellMinimumReplicaX)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellCellForm->cellMinimumReplicaX)->setText("Mult. Val.");
    }
  }
}

void CellTreeWidgetController::reloadCellMaximumReplicasY()
{
  _cellCellForm->cellMaximumReplicaY->setReadOnly(true);
  if(_projectTreeNode)
  {
    _cellCellForm->cellMaximumReplicaY->setReadOnly(!_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<int> value = maximumReplicasY())
    {
      whileBlocking(_cellCellForm->cellMaximumReplicaY)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellCellForm->cellMaximumReplicaY)->setText("Mult. Val.");
    }
  }
}

void CellTreeWidgetController::reloadCellMinimumReplicasY()
{
  _cellCellForm->cellMinimumReplicaY->setReadOnly(true);
  if(_projectTreeNode)
  {
    _cellCellForm->cellMinimumReplicaY->setReadOnly(!_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<int> value = minimumReplicasY())
    {
      whileBlocking(_cellCellForm->cellMinimumReplicaY)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellCellForm->cellMinimumReplicaY)->setText("Mult. Val.");
    }
  }
}

void CellTreeWidgetController::reloadCellMaximumReplicasZ()
{
  _cellCellForm->cellMaximumReplicaZ->setReadOnly(true);
  if(_projectTreeNode)
  {
    _cellCellForm->cellMaximumReplicaZ->setReadOnly(!_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<int> value = maximumReplicasZ())
    {
      whileBlocking(_cellCellForm->cellMaximumReplicaZ)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellCellForm->cellMaximumReplicaZ)->setText("Mult. Val.");
    }
  }
}

void CellTreeWidgetController::reloadCellMinimumReplicasZ()
{
  _cellCellForm->cellMinimumReplicaZ->setReadOnly(true);
  if(_projectTreeNode)
  {
    _cellCellForm->cellMinimumReplicaZ->setReadOnly(!_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<int> value = minimumReplicasZ())
    {
      whileBlocking(_cellCellForm->cellMinimumReplicaZ)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellCellForm->cellMinimumReplicaZ)->setText("Mult. Val.");
    }
  }
}

void CellTreeWidgetController::reloadRotationAngle()
{
  _cellCellForm->rotationAngleDoubleSpinBox->setReadOnly(true);
  _cellCellForm->rotatePlusXPushButton->setEnabled(false);
  _cellCellForm->rotatePlusYPushButton->setEnabled(false);
  _cellCellForm->rotatePlusZPushButton->setEnabled(false);
  _cellCellForm->rotateMinusXPushButton->setEnabled(false);
  _cellCellForm->rotateMinusYPushButton->setEnabled(false);
  _cellCellForm->rotateMinusZPushButton->setEnabled(false);
  if(_projectTreeNode)
  {
    _cellCellForm->rotationAngleDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());

  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> angle = rotationAngle())
    {
      whileBlocking(_cellCellForm->rotationAngleDoubleSpinBox)->setValue(*angle);

      _cellCellForm->rotatePlusXPushButton->setEnabled(_projectTreeNode->isEditable());
      _cellCellForm->rotatePlusYPushButton->setEnabled(_projectTreeNode->isEditable());
      _cellCellForm->rotatePlusZPushButton->setEnabled(_projectTreeNode->isEditable());
      _cellCellForm->rotateMinusXPushButton->setEnabled(_projectTreeNode->isEditable());
      _cellCellForm->rotateMinusYPushButton->setEnabled(_projectTreeNode->isEditable());
      _cellCellForm->rotateMinusZPushButton->setEnabled(_projectTreeNode->isEditable());

      _cellCellForm->rotatePlusXPushButton->setText("Rotate +" + QString::number(*angle));
      _cellCellForm->rotatePlusYPushButton->setText("Rotate +" + QString::number(*angle));
      _cellCellForm->rotatePlusZPushButton->setText("Rotate +" + QString::number(*angle));
      _cellCellForm->rotateMinusXPushButton->setText("Rotate -" + QString::number(*angle));
      _cellCellForm->rotateMinusYPushButton->setText("Rotate -" + QString::number(*angle));
      _cellCellForm->rotateMinusZPushButton->setText("Rotate -" + QString::number(*angle));
    }
    else
    {
      whileBlocking(_cellCellForm->rotationAngleDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}

// QT-bug: disabling and re-enabling does not work for QDial and 'sliderMoved'
void CellTreeWidgetController::reloadEulerAngles()
{
  _cellCellForm->EulerAngleXDoubleSpinBox->setReadOnly(true);
  _cellCellForm->EulerAngleXDial->setEnabled(false);
  _cellCellForm->EulerAngleYDoubleSpinBox->setReadOnly(true);
  _cellCellForm->EulerAngleYSlider->setEnabled(false);
  _cellCellForm->EulerAngleZDoubleSpinBox->setReadOnly(true);
  _cellCellForm->EulerAngleZDial->setEnabled(false);
  if(_projectTreeNode)
  {
    _cellCellForm->EulerAngleXDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _cellCellForm->EulerAngleXDial->setEnabled(_projectTreeNode->isEditable());
    _cellCellForm->EulerAngleYDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _cellCellForm->EulerAngleYSlider->setEnabled(_projectTreeNode->isEditable());
    _cellCellForm->EulerAngleZDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _cellCellForm->EulerAngleZDial->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> angle = EulerAngleX())
    {
      whileBlocking(_cellCellForm->EulerAngleXDial)->setDoubleValue(*angle);
      whileBlocking(_cellCellForm->EulerAngleXDoubleSpinBox)->setValue(*angle);
    }
    else
    {
      whileBlocking(_cellCellForm->EulerAngleXDoubleSpinBox)->setText("Mult. Val.");
    }

    if (std::optional<double> angle = EulerAngleY())
    {
      whileBlocking(_cellCellForm->EulerAngleYDoubleSpinBox)->setValue(*angle);
      whileBlocking(_cellCellForm->EulerAngleYSlider)->setDoubleValue(*angle);
    }
    else
    {
      whileBlocking(_cellCellForm->EulerAngleYDoubleSpinBox)->setText("Mult. Val.");
    }

    if (std::optional<double> angle = EulerAngleZ())
    {
      whileBlocking(_cellCellForm->EulerAngleZDoubleSpinBox)->setValue(*angle);
      whileBlocking(_cellCellForm->EulerAngleZDial)->setDoubleValue(*angle);
    }
    else
    {
      whileBlocking(_cellCellForm->EulerAngleZDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}


void CellTreeWidgetController::reloadCellOriginX()
{
  _cellCellForm->originXDoubleSpinBox->setReadOnly(true);
  if(_projectTreeNode)
  {
    _cellCellForm->originXDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = originX())
    {
      whileBlocking(_cellCellForm->originXDoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellCellForm->originXDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}

void CellTreeWidgetController::reloadCellOriginY()
{
  _cellCellForm->originYDoubleSpinBox->setReadOnly(true);
  if(_projectTreeNode)
  {
    _cellCellForm->originYDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = originY())
    {
      whileBlocking(_cellCellForm->originYDoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellCellForm->originYDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}

void CellTreeWidgetController::reloadCellOriginZ()
{
  _cellCellForm->originZDoubleSpinBox->setReadOnly(true);
  if(_projectTreeNode)
  {
    _cellCellForm->originZDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = originZ())
    {
      whileBlocking(_cellCellForm->originZDoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellCellForm->originZDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}

void CellTreeWidgetController::setStructureType(int value)
{
  std::vector<std::pair<std::shared_ptr<iRASPAStructure>,std::shared_ptr<iRASPAStructure>>> data;

  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    switch(value)
    {
      case static_cast<typename std::underlying_type<Structure::CreationMethod>::type>(SKStructure::Kind::crystal):
      {
        std::shared_ptr<Crystal> crystal = std::make_shared<Crystal>(iraspa_structure->structure());
        std::shared_ptr<iRASPAStructure> newiRASPAStructure = std::make_shared<iRASPAStructure>(crystal);
        data.push_back(std::make_pair(iraspa_structure, newiRASPAStructure));
        break;
      }
      case static_cast<typename std::underlying_type<Structure::CreationMethod>::type>(SKStructure::Kind::molecularCrystal):
      {
        std::shared_ptr<MolecularCrystal> molecularCrystal = std::make_shared<MolecularCrystal>(iraspa_structure->structure());
        std::shared_ptr<iRASPAStructure> newiRASPAStructure = std::make_shared<iRASPAStructure>(molecularCrystal);
        data.push_back(std::make_pair(iraspa_structure, newiRASPAStructure));
        break;
      }
      case static_cast<typename std::underlying_type<Structure::CreationMethod>::type>(SKStructure::Kind::molecule):
      {
        std::shared_ptr<Molecule> molecule = std::make_shared<Molecule>(iraspa_structure->structure());
        std::shared_ptr<iRASPAStructure> newiRASPAStructure = std::make_shared<iRASPAStructure>(molecule);
        data.push_back(std::make_pair(iraspa_structure, newiRASPAStructure));
        break;
      }
      case static_cast<typename std::underlying_type<Structure::CreationMethod>::type>(SKStructure::Kind::protein):
      {
        std::shared_ptr<Protein> protein = std::make_shared<Protein>(iraspa_structure->structure());
        std::shared_ptr<iRASPAStructure> newiRASPAStructure = std::make_shared<iRASPAStructure>(protein);
        data.push_back(std::make_pair(iraspa_structure, newiRASPAStructure));
        break;
      }
      case static_cast<typename std::underlying_type<Structure::CreationMethod>::type>(SKStructure::Kind::proteinCrystal):
      {
        std::shared_ptr<ProteinCrystal> proteinCrystal = std::make_shared<ProteinCrystal>(iraspa_structure->structure());
        std::shared_ptr<iRASPAStructure> newiRASPAStructure = std::make_shared<iRASPAStructure>(proteinCrystal);
        data.push_back(std::make_pair(iraspa_structure, newiRASPAStructure));
        break;
      }
      case static_cast<typename std::underlying_type<Structure::CreationMethod>::type>(SKStructure::Kind::crystalEllipsoidPrimitive):
      {
        std::shared_ptr<CrystalEllipsoidPrimitive> crystalEllipsoidPrimitive = std::make_shared<CrystalEllipsoidPrimitive>(iraspa_structure->structure());
        std::shared_ptr<iRASPAStructure> newiRASPAStructure = std::make_shared<iRASPAStructure>(crystalEllipsoidPrimitive);
        data.push_back(std::make_pair(iraspa_structure, newiRASPAStructure));
        break;
      }
      case static_cast<typename std::underlying_type<Structure::CreationMethod>::type>(SKStructure::Kind::crystalCylinderPrimitive):
      {
        std::shared_ptr<CrystalCylinderPrimitive> crystalCylinderPrimitive = std::make_shared<CrystalCylinderPrimitive>(iraspa_structure->structure());
        std::shared_ptr<iRASPAStructure> newiRASPAStructure = std::make_shared<iRASPAStructure>(crystalCylinderPrimitive);
        data.push_back(std::make_pair(iraspa_structure, newiRASPAStructure));
        break;
      }
      case static_cast<typename std::underlying_type<Structure::CreationMethod>::type>(SKStructure::Kind::crystalPolygonalPrismPrimitive):
      {
        std::shared_ptr<CrystalPolygonalPrismPrimitive> crystalPolygonalPrismPrimitive = std::make_shared<CrystalPolygonalPrismPrimitive>(iraspa_structure->structure());
        std::shared_ptr<iRASPAStructure> newiRASPAStructure = std::make_shared<iRASPAStructure>(crystalPolygonalPrismPrimitive);
        data.push_back(std::make_pair(iraspa_structure, newiRASPAStructure));
        break;
      }
      case static_cast<typename std::underlying_type<Structure::CreationMethod>::type>(SKStructure::Kind::ellipsoidPrimitive):
      {
        std::shared_ptr<EllipsoidPrimitive> ellipsoidPrimitive = std::make_shared<EllipsoidPrimitive>(iraspa_structure->structure());
        std::shared_ptr<iRASPAStructure> newiRASPAStructure = std::make_shared<iRASPAStructure>(ellipsoidPrimitive);
        data.push_back(std::make_pair(iraspa_structure, newiRASPAStructure));
        break;
      }
      case static_cast<typename std::underlying_type<Structure::CreationMethod>::type>(SKStructure::Kind::cylinderPrimitive):
      {
        std::shared_ptr<CylinderPrimitive> cylinderPrimitive = std::make_shared<CylinderPrimitive>(iraspa_structure->structure());
        std::shared_ptr<iRASPAStructure> newiRASPAStructure = std::make_shared<iRASPAStructure>(cylinderPrimitive);
        data.push_back(std::make_pair(iraspa_structure, newiRASPAStructure));
        break;
      }
      case static_cast<typename std::underlying_type<Structure::CreationMethod>::type>(SKStructure::Kind::polygonalPrismPrimitive):
      {
        std::shared_ptr<PolygonalPrismPrimitive> polygonalPrismPrimitive = std::make_shared<PolygonalPrismPrimitive>(iraspa_structure->structure());
        std::shared_ptr<iRASPAStructure> newiRASPAStructure = std::make_shared<iRASPAStructure>(polygonalPrismPrimitive);
        data.push_back(std::make_pair(iraspa_structure, newiRASPAStructure));
        break;
      }
      default:
        break;
    }
  }
  replaceStructure(data);
}

void CellTreeWidgetController::replaceStructure(std::vector<std::pair<std::shared_ptr<iRASPAStructure>,std::shared_ptr<iRASPAStructure>>> data)
{
  for(std::pair<std::shared_ptr<iRASPAStructure>,std::shared_ptr<iRASPAStructure>> tuple : data)
  {
    iRASPAStructure::swapRepresentedObjects(std::get<0>(tuple), std::get<1>(tuple));
  }

  std::vector<std::shared_ptr<RKRenderStructure>> render_structures{};
  std::transform(_iraspa_structures.begin(),_iraspa_structures.end(),std::back_inserter(render_structures),
                  [](std::shared_ptr<iRASPAStructure> iraspastructure) -> std::shared_ptr<RKRenderStructure> {return iraspastructure->structure();});

  SKBoundingBox box = _projectStructure->renderBoundingBox();
  _projectStructure->camera()->resetForNewBoundingBox(box);

  emit invalidateCachedAmbientOcclusionTexture(render_structures);
  emit reloadAllViews();
}

std::optional<iRASPAStructureType> CellTreeWidgetController::structureType()
{
  if (_iraspa_structures.empty())
	{
    return std::nullopt;
	}
  std::unordered_set<iRASPAStructureType, enum_hash> set = std::unordered_set<iRASPAStructureType, enum_hash>{};
  for (std::shared_ptr<iRASPAStructure> iraspa_structure : _iraspa_structures)
	{
    iRASPAStructureType value = iraspa_structure->structure()->structureType();
    set.emplace(value);
	}

	if (set.size() == 1)
	{
    iRASPAStructureType value = *set.begin();
    return value;
	}
  return std::nullopt;
}

SKBoundingBox CellTreeWidgetController::boundingBox()
{
  if(!_iraspa_structures.empty())
  {
    double3 minimum = double3(DBL_MAX,DBL_MAX, DBL_MAX);
    double3 maximum = double3(-DBL_MAX,-DBL_MAX, -DBL_MAX);
    for(std::shared_ptr<iRASPAStructure> iraspa_structure : _iraspa_structures)
    {
      std::shared_ptr<Structure> structure = iraspa_structure->structure();
      SKBoundingBox transformedBoundingBox = structure->transformedBoundingBox();

      minimum.x = std::min(minimum.x, transformedBoundingBox.minimum().x + structure->origin().x);
      minimum.y = std::min(minimum.y, transformedBoundingBox.minimum().y + structure->origin().y);
      minimum.z = std::min(minimum.z, transformedBoundingBox.minimum().z + structure->origin().z);
      maximum.x = std::max(maximum.x, transformedBoundingBox.maximum().x + structure->origin().x);
      maximum.y = std::max(maximum.y, transformedBoundingBox.maximum().y + structure->origin().y);
      maximum.z = std::max(maximum.z, transformedBoundingBox.maximum().z + structure->origin().z);
    }
    return SKBoundingBox(minimum,maximum);
  }
  return SKBoundingBox(double3(0.0,0.0,0.0),double3(0.0,0.0,0.0));
}

std::optional<double> CellTreeWidgetController::unitCellAX()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    set.insert(iraspa_structure->structure()->cell()->unitCell().ax);
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }
  return std::nullopt;
}


std::optional<double> CellTreeWidgetController::unitCellAY()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    set.insert(iraspa_structure->structure()->cell()->unitCell().ay);
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }
  return std::nullopt;
}

std::optional<double> CellTreeWidgetController::unitCellAZ()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    set.insert(iraspa_structure->structure()->cell()->unitCell().az);
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }
  return std::nullopt;
}


std::optional<double> CellTreeWidgetController::unitCellBX()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    set.insert(iraspa_structure->structure()->cell()->unitCell().bx);
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }
  return std::nullopt;
}

std::optional<double> CellTreeWidgetController::unitCellBY()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    set.insert(iraspa_structure->structure()->cell()->unitCell().by);
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }
  return std::nullopt;
}

std::optional<double> CellTreeWidgetController::unitCellBZ()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    set.insert(iraspa_structure->structure()->cell()->unitCell().bz);
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }
  return std::nullopt;
}

std::optional<double> CellTreeWidgetController::unitCellCX()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    set.insert(iraspa_structure->structure()->cell()->unitCell().cx);
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }
  return std::nullopt;
}

std::optional<double> CellTreeWidgetController::unitCellCY()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    set.insert(iraspa_structure->structure()->cell()->unitCell().cy);
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }
  return std::nullopt;
}

std::optional<double> CellTreeWidgetController::unitCellCZ()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    set.insert(iraspa_structure->structure()->cell()->unitCell().cz);
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }
  return std::nullopt;
}

std::optional<double> CellTreeWidgetController::unitCellLengthA()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    set.insert(iraspa_structure->structure()->cell()->a());
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }
  return std::nullopt;
}

void CellTreeWidgetController::setUnitCellLengthA(double value)
{
  if(!_iraspa_structures.empty())
  {
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      iraspa_structure->structure()->cell()->setLengthA(value);
      iraspa_structure->structure()->reComputeBoundingBox();
    }

    SKBoundingBox box = _projectStructure->renderBoundingBox();
    _projectStructure->camera()->resetForNewBoundingBox(box);

    std::vector<std::shared_ptr<RKRenderStructure>> render_structures{};
    std::transform(_iraspa_structures.begin(),_iraspa_structures.end(),std::back_inserter(render_structures),
                    [](std::shared_ptr<iRASPAStructure> iraspastructure) -> std::shared_ptr<RKRenderStructure> {return iraspastructure->structure();});

    emit invalidateCachedAmbientOcclusionTexture(render_structures);
    emit rendererReloadData();

    reloadBoundingBox();
    reloadUnitCell();
  }
}

std::optional<double> CellTreeWidgetController::unitCellLengthB()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    set.insert(iraspa_structure->structure()->cell()->b());
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }
  return std::nullopt;
}

void CellTreeWidgetController::setUnitCellLengthB(double value)
{
  if(!_iraspa_structures.empty())
  {
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      iraspa_structure->structure()->cell()->setLengthB(value);
      iraspa_structure->structure()->reComputeBoundingBox();
    }

    SKBoundingBox box = _projectStructure->renderBoundingBox();
    _projectStructure->camera()->resetForNewBoundingBox(box);

    std::vector<std::shared_ptr<RKRenderStructure>> render_structures{};
    std::transform(_iraspa_structures.begin(),_iraspa_structures.end(),std::back_inserter(render_structures),
                    [](std::shared_ptr<iRASPAStructure> iraspastructure) -> std::shared_ptr<RKRenderStructure> {return iraspastructure->structure();});

    emit invalidateCachedAmbientOcclusionTexture(render_structures);
    emit rendererReloadData();

    reloadBoundingBox();
    reloadUnitCell();
  }
}

std::optional<double> CellTreeWidgetController::unitCellLengthC()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    set.insert(iraspa_structure->structure()->cell()->c());
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }
  return std::nullopt;
}

void CellTreeWidgetController::setUnitCellLengthC(double value)
{
  if(!_iraspa_structures.empty())
  {
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      iraspa_structure->structure()->cell()->setLengthC(value);
      iraspa_structure->structure()->reComputeBoundingBox();
    }

    SKBoundingBox box = _projectStructure->renderBoundingBox();
    _projectStructure->camera()->resetForNewBoundingBox(box);


    std::vector<std::shared_ptr<RKRenderStructure>> render_structures{};
    std::transform(_iraspa_structures.begin(),_iraspa_structures.end(),std::back_inserter(render_structures),
                    [](std::shared_ptr<iRASPAStructure> iraspastructure) -> std::shared_ptr<RKRenderStructure> {return iraspastructure->structure();});

    emit invalidateCachedAmbientOcclusionTexture(render_structures);
    emit rendererReloadData();

    reloadBoundingBox();
    reloadUnitCell();
  }
}

std::optional<double> CellTreeWidgetController::unitCellAngleAlpha()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    set.insert(iraspa_structure->structure()->cell()->alpha() * 180.0/M_PI);
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }
  return std::nullopt;
}

void CellTreeWidgetController::setUnitCellAngleAlpha(double value)
{
  if(!_iraspa_structures.empty())
  {
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      iraspa_structure->structure()->cell()->setAlphaAngle(value * M_PI / 180.0);
      iraspa_structure->structure()->reComputeBoundingBox();
    }

    SKBoundingBox box = _projectStructure->renderBoundingBox();
    _projectStructure->camera()->resetForNewBoundingBox(box);

    std::vector<std::shared_ptr<RKRenderStructure>> render_structures{};
    std::transform(_iraspa_structures.begin(),_iraspa_structures.end(),std::back_inserter(render_structures),
                    [](std::shared_ptr<iRASPAStructure> iraspastructure) -> std::shared_ptr<RKRenderStructure> {return iraspastructure->structure();});

    emit invalidateCachedAmbientOcclusionTexture(render_structures);
    emit rendererReloadData();

    reloadBoundingBox();
    reloadUnitCell();
  }
}

std::optional<double> CellTreeWidgetController::unitCellAngleBeta()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    set.insert(iraspa_structure->structure()->cell()->beta() * 180.0/M_PI);
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }
  return std::nullopt;
}

void CellTreeWidgetController::setUnitCellAngleBeta(double value)
{
  if(!_iraspa_structures.empty())
  {
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      iraspa_structure->structure()->cell()->setBetaAngle(value * M_PI / 180.0);
      iraspa_structure->structure()->reComputeBoundingBox();
    }

    SKBoundingBox box = _projectStructure->renderBoundingBox();
    _projectStructure->camera()->resetForNewBoundingBox(box);

    std::vector<std::shared_ptr<RKRenderStructure>> render_structures{};
    std::transform(_iraspa_structures.begin(),_iraspa_structures.end(),std::back_inserter(render_structures),
                    [](std::shared_ptr<iRASPAStructure> iraspastructure) -> std::shared_ptr<RKRenderStructure> {return iraspastructure->structure();});

    emit invalidateCachedAmbientOcclusionTexture(render_structures);
    emit rendererReloadData();

    reloadBoundingBox();
    reloadUnitCell();
  }
}

std::optional<double> CellTreeWidgetController::unitCellAngleGamma()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    set.insert(iraspa_structure->structure()->cell()->gamma() * 180.0/M_PI);
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }
  return std::nullopt;
}

void CellTreeWidgetController::setUnitCellAngleGamma(double value)
{
  if(!_iraspa_structures.empty())
  {
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      iraspa_structure->structure()->cell()->setGammaAngle(value * M_PI / 180.0);
      iraspa_structure->structure()->reComputeBoundingBox();
    }

    SKBoundingBox box = _projectStructure->renderBoundingBox();
    _projectStructure->camera()->resetForNewBoundingBox(box);

    std::vector<std::shared_ptr<RKRenderStructure>> render_structures{};
    std::transform(_iraspa_structures.begin(),_iraspa_structures.end(),std::back_inserter(render_structures),
                    [](std::shared_ptr<iRASPAStructure> iraspastructure) -> std::shared_ptr<RKRenderStructure> {return iraspastructure->structure();});

    emit invalidateCachedAmbientOcclusionTexture(render_structures);
    emit rendererReloadData();

    reloadBoundingBox();
    reloadUnitCell();
  }
}


std::optional<double> CellTreeWidgetController::unitCellVolume()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    set.insert(iraspa_structure->structure()->cell()->volume());
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }

  return std::nullopt;
}


std::optional<double> CellTreeWidgetController::unitCellPerpendicularWidthX()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    set.insert(iraspa_structure->structure()->cell()->perpendicularWidths().x);
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }

  return std::nullopt;
}

std::optional<double> CellTreeWidgetController::unitCellPerpendicularWidthY()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    set.insert(iraspa_structure->structure()->cell()->perpendicularWidths().y);
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }

  return std::nullopt;
}


std::optional<double> CellTreeWidgetController::unitCellPerpendicularWidthZ()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    set.insert(iraspa_structure->structure()->cell()->perpendicularWidths().z);
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }

  return std::nullopt;
}


std::optional<int> CellTreeWidgetController::maximumReplicasX()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<int> set = std::unordered_set<int>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    set.insert(iraspa_structure->structure()->cell()->maximumReplicaX());
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }

  return std::nullopt;
}

void CellTreeWidgetController::setMaximumReplicasX(int value)
{
  if(!_iraspa_structures.empty())
  {
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      if (value >= iraspa_structure->structure()->cell()->minimumReplicaX())
      {
        iraspa_structure->structure()->cell()->setMaximumReplicaX(value);
      }
    }

    SKBoundingBox box = _projectStructure->renderBoundingBox();
    _projectStructure->camera()->resetForNewBoundingBox(box);

    std::vector<std::shared_ptr<RKRenderStructure>> render_structures{};
    std::transform(_iraspa_structures.begin(),_iraspa_structures.end(),std::back_inserter(render_structures),
                    [](std::shared_ptr<iRASPAStructure> iraspastructure) -> std::shared_ptr<RKRenderStructure> {return iraspastructure->structure();});

    emit invalidateCachedAmbientOcclusionTexture(render_structures);
    emit rendererReloadData();

    reloadCellMaximumReplicasX();
    reloadBoundingBox();
  }
}

std::optional<int> CellTreeWidgetController::maximumReplicasY()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<int> set = std::unordered_set<int>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    set.insert(iraspa_structure->structure()->cell()->maximumReplicaY());
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }

  return std::nullopt;
}

void CellTreeWidgetController::setMaximumReplicasY(int value)
{
  if(!_iraspa_structures.empty())
  {
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      if (value >= iraspa_structure->structure()->cell()->minimumReplicaY())
      {
        iraspa_structure->structure()->cell()->setMaximumReplicaY(value);
      }
    }

    SKBoundingBox box = _projectStructure->renderBoundingBox();
    _projectStructure->camera()->resetForNewBoundingBox(box);

    std::vector<std::shared_ptr<RKRenderStructure>> render_structures{};
    std::transform(_iraspa_structures.begin(),_iraspa_structures.end(),std::back_inserter(render_structures),
                    [](std::shared_ptr<iRASPAStructure> iraspastructure) -> std::shared_ptr<RKRenderStructure> {return iraspastructure->structure();});

    emit invalidateCachedAmbientOcclusionTexture(render_structures);
    emit rendererReloadData();

    reloadCellMaximumReplicasY();
    reloadBoundingBox();
  }
}

std::optional<int> CellTreeWidgetController::maximumReplicasZ()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<int> set = std::unordered_set<int>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    set.insert(iraspa_structure->structure()->cell()->maximumReplicaZ());
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }

  return std::nullopt;
}

void CellTreeWidgetController::setMaximumReplicasZ(int value)
{
  if(!_iraspa_structures.empty())
  {
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      if (value >= iraspa_structure->structure()->cell()->minimumReplicaZ())
      {
        iraspa_structure->structure()->cell()->setMaximumReplicaZ(value);
      }
    }

    SKBoundingBox box = _projectStructure->renderBoundingBox();
    _projectStructure->camera()->resetForNewBoundingBox(box);

    std::vector<std::shared_ptr<RKRenderStructure>> render_structures{};
    std::transform(_iraspa_structures.begin(),_iraspa_structures.end(),std::back_inserter(render_structures),
                    [](std::shared_ptr<iRASPAStructure> iraspastructure) -> std::shared_ptr<RKRenderStructure> {return iraspastructure->structure();});

    emit invalidateCachedAmbientOcclusionTexture(render_structures);
    emit rendererReloadData();

    reloadCellMaximumReplicasZ();
    reloadBoundingBox();
  }
}

std::optional<int> CellTreeWidgetController::minimumReplicasX()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<int> set = std::unordered_set<int>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    set.insert(iraspa_structure->structure()->cell()->minimumReplicaX());
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }

  return std::nullopt;
}

void CellTreeWidgetController::setMinimumReplicasX(int value)
{
  if(!_iraspa_structures.empty())
  {
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      if (value <= iraspa_structure->structure()->cell()->maximumReplicaX())
      {
        iraspa_structure->structure()->cell()->setMinimumReplicaX(value);
      }
    }

    SKBoundingBox box = _projectStructure->renderBoundingBox();
    _projectStructure->camera()->resetForNewBoundingBox(box);

    std::vector<std::shared_ptr<RKRenderStructure>> render_structures{};
    std::transform(_iraspa_structures.begin(),_iraspa_structures.end(),std::back_inserter(render_structures),
                    [](std::shared_ptr<iRASPAStructure> iraspastructure) -> std::shared_ptr<RKRenderStructure> {return iraspastructure->structure();});

    emit invalidateCachedAmbientOcclusionTexture(render_structures);
    emit rendererReloadData();

    reloadCellMinimumReplicasX();
    reloadBoundingBox();
  }
}

std::optional<int> CellTreeWidgetController::minimumReplicasY()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<int> set = std::unordered_set<int>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    set.insert(iraspa_structure->structure()->cell()->minimumReplicaY());
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }

  return std::nullopt;
}

void CellTreeWidgetController::setMinimumReplicasY(int value)
{
  if(!_iraspa_structures.empty())
  {
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      if (value <= iraspa_structure->structure()->cell()->maximumReplicaY())
      {
        iraspa_structure->structure()->cell()->setMinimumReplicaY(value);
      }
    }

    SKBoundingBox box = _projectStructure->renderBoundingBox();
    _projectStructure->camera()->resetForNewBoundingBox(box);

    std::vector<std::shared_ptr<RKRenderStructure>> render_structures{};
    std::transform(_iraspa_structures.begin(),_iraspa_structures.end(),std::back_inserter(render_structures),
                    [](std::shared_ptr<iRASPAStructure> iraspastructure) -> std::shared_ptr<RKRenderStructure> {return iraspastructure->structure();});

    emit invalidateCachedAmbientOcclusionTexture(render_structures);
    emit rendererReloadData();

    reloadCellMinimumReplicasY();
    reloadBoundingBox();
  }
}

std::optional<int> CellTreeWidgetController::minimumReplicasZ()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<int> set = std::unordered_set<int>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    set.insert(iraspa_structure->structure()->cell()->minimumReplicaZ());
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }

  return std::nullopt;
}

void CellTreeWidgetController::setMinimumReplicasZ(int value)
{
  if(!_iraspa_structures.empty())
  {
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      if (value <= iraspa_structure->structure()->cell()->maximumReplicaZ())
      {
        iraspa_structure->structure()->cell()->setMinimumReplicaZ(value);
      }
    }

    SKBoundingBox box = _projectStructure->renderBoundingBox();
    _projectStructure->camera()->resetForNewBoundingBox(box);

    std::vector<std::shared_ptr<RKRenderStructure>> render_structures{};
    std::transform(_iraspa_structures.begin(),_iraspa_structures.end(),std::back_inserter(render_structures),
                    [](std::shared_ptr<iRASPAStructure> iraspastructure) -> std::shared_ptr<RKRenderStructure> {return iraspastructure->structure();});

    emit invalidateCachedAmbientOcclusionTexture(render_structures);
    emit rendererReloadData();

    reloadCellMinimumReplicasZ();
    reloadBoundingBox();
  }
}

std::optional<double> CellTreeWidgetController::rotationAngle()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }

  std::set<double> set = std::set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double angle = iraspa_structure->structure()->rotationDelta();
    set.insert(angle);
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }
  return std::nullopt;
}

void CellTreeWidgetController::setRotationAngle(double angle)
{
  if(!_iraspa_structures.empty())
  {
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      iraspa_structure->structure()->setRotationDelta(angle);
    }
    reloadRotationAngle();
  }
}

void CellTreeWidgetController::rotateYawPlus()
{
  if(!_iraspa_structures.empty())
  {
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      double rotationDelta = iraspa_structure->structure()->rotationDelta();
      simd_quatd orientation = iraspa_structure->structure()->orientation();
      simd_quatd dq = simd_quatd::yaw(rotationDelta);
      simd_quatd newOrientation = orientation *dq;
      iraspa_structure->structure()->setOrientation(newOrientation);
      iraspa_structure->structure()->reComputeBoundingBox();
    }

    SKBoundingBox box = _projectStructure->renderBoundingBox();
    _projectStructure->camera()->resetForNewBoundingBox(box);

    emit rendererReloadData();

    reloadBoundingBox();
    reloadEulerAngles();
  }
}

void CellTreeWidgetController::rotateYawMinus()
{
  if(!_iraspa_structures.empty())
  {
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      double rotationDelta = iraspa_structure->structure()->rotationDelta();
      simd_quatd orientation = iraspa_structure->structure()->orientation();
      simd_quatd dq = simd_quatd::yaw(-rotationDelta);
      simd_quatd newOrientation = orientation *dq;
      iraspa_structure->structure()->setOrientation(newOrientation);
      iraspa_structure->structure()->reComputeBoundingBox();
    }

    SKBoundingBox box = _projectStructure->renderBoundingBox();
    _projectStructure->camera()->resetForNewBoundingBox(box);

    emit rendererReloadData();

    reloadBoundingBox();
    reloadEulerAngles();
  }
}

void CellTreeWidgetController::rotatePitchPlus()
{
  if(!_iraspa_structures.empty())
  {
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      double rotationDelta = iraspa_structure->structure()->rotationDelta();
      simd_quatd orientation = iraspa_structure->structure()->orientation();
      simd_quatd dq = simd_quatd::pitch(rotationDelta);
      simd_quatd newOrientation = orientation *dq;
      iraspa_structure->structure()->setOrientation(newOrientation);
      iraspa_structure->structure()->reComputeBoundingBox();
    }

    SKBoundingBox box = _projectStructure->renderBoundingBox();
    _projectStructure->camera()->resetForNewBoundingBox(box);

    emit rendererReloadData();

    reloadBoundingBox();
    reloadEulerAngles();
  }
}

void CellTreeWidgetController::rotatePitchMinus()
{
  if(!_iraspa_structures.empty())
  {
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      double rotationDelta = iraspa_structure->structure()->rotationDelta();
      simd_quatd orientation = iraspa_structure->structure()->orientation();
      simd_quatd dq = simd_quatd::pitch(-rotationDelta);
      simd_quatd newOrientation = orientation *dq;
      iraspa_structure->structure()->setOrientation(newOrientation);
      iraspa_structure->structure()->reComputeBoundingBox();
    }

    SKBoundingBox box = _projectStructure->renderBoundingBox();
    _projectStructure->camera()->resetForNewBoundingBox(box);

    emit rendererReloadData();

    reloadBoundingBox();
    reloadEulerAngles();
  }
}

void CellTreeWidgetController::rotateRollPlus()
{
  if(!_iraspa_structures.empty())
  {
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      double rotationDelta = iraspa_structure->structure()->rotationDelta();
      simd_quatd orientation = iraspa_structure->structure()->orientation();
      simd_quatd dq = simd_quatd::roll(rotationDelta);
      simd_quatd newOrientation = orientation *dq;
      iraspa_structure->structure()->setOrientation(newOrientation);
      iraspa_structure->structure()->reComputeBoundingBox();
    }

    SKBoundingBox box = _projectStructure->renderBoundingBox();
    _projectStructure->camera()->resetForNewBoundingBox(box);

    emit rendererReloadData();

    reloadBoundingBox();
    reloadEulerAngles();
  }
}

void CellTreeWidgetController::rotateRollMinus()
{
  if(!_iraspa_structures.empty())
  {
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      double rotationDelta = iraspa_structure->structure()->rotationDelta();
      simd_quatd orientation = iraspa_structure->structure()->orientation();
      simd_quatd dq = simd_quatd::roll(-rotationDelta);
      simd_quatd newOrientation = orientation *dq;
      iraspa_structure->structure()->setOrientation(newOrientation);
      iraspa_structure->structure()->reComputeBoundingBox();
    }

    SKBoundingBox box = _projectStructure->renderBoundingBox();
    _projectStructure->camera()->resetForNewBoundingBox(box);

    emit rendererReloadData();

    reloadBoundingBox();
    reloadEulerAngles();
  }
}

std::optional<double> CellTreeWidgetController::EulerAngleX()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }

  std::set<double> set = std::set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    simd_quatd orientation = iraspa_structure->structure()->orientation();
    double EulerAngle = orientation.EulerAngles().x * 180.0 / M_PI;
    set.insert(EulerAngle);
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }
  return std::nullopt;
}

void CellTreeWidgetController::setEulerAngleX(double angle)
{
  if(!_iraspa_structures.empty())
  {
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      simd_quatd orientation = iraspa_structure->structure()->orientation();
      double3 EulerAngles = orientation.EulerAngles();
      EulerAngles.x = angle * M_PI / 180.0;
      simd_quatd newOrientation = simd_quatd(EulerAngles);
      iraspa_structure->structure()->setOrientation(newOrientation);
      iraspa_structure->structure()->reComputeBoundingBox();
    }

    SKBoundingBox box = _projectStructure->renderBoundingBox();
    _projectStructure->camera()->resetForNewBoundingBox(box);

    std::vector<std::shared_ptr<RKRenderStructure>> render_structures{};
    std::transform(_iraspa_structures.begin(),_iraspa_structures.end(),std::back_inserter(render_structures),
                    [](std::shared_ptr<iRASPAStructure> iraspastructure) -> std::shared_ptr<RKRenderStructure> {return iraspastructure->structure();});

    emit invalidateCachedAmbientOcclusionTexture(render_structures);
    emit rendererReloadData();

    reloadBoundingBox();
    reloadEulerAngles();
  }
}

std::optional<double> CellTreeWidgetController::EulerAngleY()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }

  std::set<double> set = std::set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    simd_quatd orientation = iraspa_structure->structure()->orientation();
    int EulerAngle = orientation.EulerAngles().y * 180.0 / M_PI;
    set.insert(EulerAngle);
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }
  return std::nullopt;
}

void CellTreeWidgetController::setEulerAngleY(double angle)
{
  if(!_iraspa_structures.empty())
  {
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      simd_quatd orientation = iraspa_structure->structure()->orientation();
      double3 EulerAngles = orientation.EulerAngles();
      EulerAngles.y = angle * M_PI / 180.0;
      simd_quatd newOrientation = simd_quatd(EulerAngles);
      iraspa_structure->structure()->setOrientation(newOrientation);
      iraspa_structure->structure()->reComputeBoundingBox();
    }

    SKBoundingBox box = _projectStructure->renderBoundingBox();
    _projectStructure->camera()->resetForNewBoundingBox(box);

    std::vector<std::shared_ptr<RKRenderStructure>> render_structures{};
    std::transform(_iraspa_structures.begin(),_iraspa_structures.end(),std::back_inserter(render_structures),
                    [](std::shared_ptr<iRASPAStructure> iraspastructure) -> std::shared_ptr<RKRenderStructure> {return iraspastructure->structure();});

    emit invalidateCachedAmbientOcclusionTexture(render_structures);
    emit rendererReloadData();

    reloadBoundingBox();
    reloadEulerAngles();
  }
}

std::optional<double> CellTreeWidgetController::EulerAngleZ()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }

  std::set<double> set = std::set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    simd_quatd orientation = iraspa_structure->structure()->orientation();
    int EulerAngle = orientation.EulerAngles().z * 180.0 / M_PI;
    set.insert(EulerAngle);
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }
  return std::nullopt;
}

void CellTreeWidgetController::setEulerAngleZ(double angle)
{
  if(!_iraspa_structures.empty())
  {
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      simd_quatd orientation = iraspa_structure->structure()->orientation();
      double3 EulerAngles = orientation.EulerAngles();
      EulerAngles.z = angle * M_PI / 180.0;
      simd_quatd newOrientation = simd_quatd(EulerAngles);
      iraspa_structure->structure()->setOrientation(newOrientation);
      iraspa_structure->structure()->reComputeBoundingBox();
    }

    SKBoundingBox box = _projectStructure->renderBoundingBox();
    _projectStructure->camera()->resetForNewBoundingBox(box);

    std::vector<std::shared_ptr<RKRenderStructure>> render_structures{};
    std::transform(_iraspa_structures.begin(),_iraspa_structures.end(),std::back_inserter(render_structures),
                    [](std::shared_ptr<iRASPAStructure> iraspastructure) -> std::shared_ptr<RKRenderStructure> {return iraspastructure->structure();});

    emit invalidateCachedAmbientOcclusionTexture(render_structures);
    emit rendererReloadData();

    reloadBoundingBox();
    reloadEulerAngles();
  }
}


std::optional<double>  CellTreeWidgetController::originX()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    set.insert(iraspa_structure->structure()->origin().x);
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }

  return std::nullopt;
}

void CellTreeWidgetController::setOriginX(double value)
{
  if(!_iraspa_structures.empty())
  {
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      iraspa_structure->structure()->setOriginX(value);
    }

    SKBoundingBox box = _projectStructure->renderBoundingBox();
    _projectStructure->camera()->resetForNewBoundingBox(box);

    std::vector<std::shared_ptr<RKRenderStructure>> render_structures{};
    std::transform(_iraspa_structures.begin(),_iraspa_structures.end(),std::back_inserter(render_structures),
                    [](std::shared_ptr<iRASPAStructure> iraspastructure) -> std::shared_ptr<RKRenderStructure> {return iraspastructure->structure();});

    emit invalidateCachedAmbientOcclusionTexture(render_structures);
    emit rendererReloadData();

    reloadBoundingBox();
  }
}


std::optional<double>  CellTreeWidgetController::originY()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    set.insert(iraspa_structure->structure()->origin().y);
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }

  return std::nullopt;
}

void CellTreeWidgetController::setOriginY(double value)
{
  if(!_iraspa_structures.empty())
  {
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      iraspa_structure->structure()->setOriginY(value);
    }

    SKBoundingBox box = _projectStructure->renderBoundingBox();
    _projectStructure->camera()->resetForNewBoundingBox(box);

    std::vector<std::shared_ptr<RKRenderStructure>> render_structures{};
    std::transform(_iraspa_structures.begin(),_iraspa_structures.end(),std::back_inserter(render_structures),
                    [](std::shared_ptr<iRASPAStructure> iraspastructure) -> std::shared_ptr<RKRenderStructure> {return iraspastructure->structure();});

    emit invalidateCachedAmbientOcclusionTexture(render_structures);
    emit rendererReloadData();

    reloadBoundingBox();
  }
}


std::optional<double>  CellTreeWidgetController::originZ()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    set.insert(iraspa_structure->structure()->origin().z);
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }

  return std::nullopt;
}

void CellTreeWidgetController::setOriginZ(double value)
{
  if(!_iraspa_structures.empty())
  {
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      iraspa_structure->structure()->setOriginZ(value);
    }

    SKBoundingBox box = _projectStructure->renderBoundingBox();
    _projectStructure->camera()->resetForNewBoundingBox(box);

    std::vector<std::shared_ptr<RKRenderStructure>> render_structures{};
    std::transform(_iraspa_structures.begin(),_iraspa_structures.end(),std::back_inserter(render_structures),
                    [](std::shared_ptr<iRASPAStructure> iraspastructure) -> std::shared_ptr<RKRenderStructure> {return iraspastructure->structure();});

    emit invalidateCachedAmbientOcclusionTexture(render_structures);
    emit rendererReloadData();

    reloadBoundingBox();
  }
}


// Transform content properties
//========================================================================================================================================

void CellTreeWidgetController::reloadTransformContentProperties()
{
  reloadFlipAxesProperties();
  reloadShiftAxesProperties();

  _cellTransformContentForm->applyPushButton->setEnabled(false);
}

void CellTreeWidgetController::reloadFlipAxesProperties()
{
  _cellTransformContentForm->flipAxisACheckBox->setEnabled(false);
  _cellTransformContentForm->flipAxisBCheckBox->setEnabled(false);
  _cellTransformContentForm->flipAxisCCheckBox->setEnabled(false);
}

void CellTreeWidgetController::reloadShiftAxesProperties()
{
  _cellTransformContentForm->shiftAxisADoubleSpinBox->setEnabled(false);
  _cellTransformContentForm->shiftAxisBDoubleSpinBox->setEnabled(false);
  _cellTransformContentForm->shiftAxisCDoubleSpinBox->setEnabled(false);
}

// Structural properties
//========================================================================================================================================

void CellTreeWidgetController::reloadStructureProperties()
{
  reloadStructuralMaterialType();
  reloadStructuralMass();
  reloadStructuralDensity();
  reloadStructuralHeliumVoidFraction();
  reloadStructuralSpecificVolume();
  reloadStructuralAccessiblePoreVolume();
  reloadFrameworkProbeMoleculePopupBox();
  reloadStructuralVolumetricSurfaceArea();
  reloadStructuralGravimetricSurfaceArea();
  reloadStructuralNumberOfChannelSystems();
  reloadStructuralNumberOfInaccessiblePockets();
  reloadStructuralDimensionalityOfPoreSystem();
  reloadStructuralLargestOverallCavityDiameter();
  reloadStructuralRestrictingPoreDiameter();
  reloadStructuralLargestDiamtereAlongAViablePath();

  _cellStructuralForm->computeGravimetricSurfaceAreaPushButton->setEnabled(false);
  _cellStructuralForm->computeVolumetricSurfaceAreaPushButton->setEnabled(false);
  _cellStructuralForm->computeHeliumVoidFractionPushButton->setEnabled(false);
  if(_projectTreeNode)
  {
    _cellStructuralForm->computeGravimetricSurfaceAreaPushButton->setEnabled(_projectTreeNode->isEditable());
    _cellStructuralForm->computeVolumetricSurfaceAreaPushButton->setEnabled(_projectTreeNode->isEditable());
    _cellStructuralForm->computeHeliumVoidFractionPushButton->setEnabled(_projectTreeNode->isEditable());
  }
}

void CellTreeWidgetController::reloadStructuralMaterialType()
{
  _cellStructuralForm->materialTypeComboBox->setEnabled(false);
  if(_projectTreeNode)
  {
    _cellStructuralForm->materialTypeComboBox->setEnabled(_projectTreeNode->isEditable());
  }
  if(!_iraspa_structures.empty())
  {

  }
}

void CellTreeWidgetController::reloadStructuralMass()
{
  _cellStructuralForm->massDoubleSpinBox->setReadOnly(true);
  if(_projectTreeNode)
  {
    _cellStructuralForm->massDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = structuralMass())
    {
      whileBlocking(_cellStructuralForm->massDoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellStructuralForm->massDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}

void CellTreeWidgetController::reloadStructuralDensity()
{
  _cellStructuralForm->densityDoubleSpinBox->setReadOnly(true);
  if(_projectTreeNode)
  {
    _cellStructuralForm->densityDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = structuralDensity())
    {
      whileBlocking(_cellStructuralForm->densityDoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellStructuralForm->densityDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}

void CellTreeWidgetController::reloadStructuralHeliumVoidFraction()
{
  _cellStructuralForm->heliumVoidFractionDoubleSpinBox->setReadOnly(true);
  if(_projectTreeNode)
  {
    _cellStructuralForm->heliumVoidFractionDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = structureHeliumVoidFraction())
    {
      whileBlocking(_cellStructuralForm->heliumVoidFractionDoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellStructuralForm->heliumVoidFractionDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}

void CellTreeWidgetController::reloadStructuralSpecificVolume()
{
  _cellStructuralForm->specificVolumeDoubleSpinBox->setReadOnly(true);
  if(_projectTreeNode)
  {
    _cellStructuralForm->specificVolumeDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = structureSpecificVolume())
    {
      whileBlocking(_cellStructuralForm->specificVolumeDoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellStructuralForm->specificVolumeDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}

void CellTreeWidgetController::reloadStructuralAccessiblePoreVolume()
{
  _cellStructuralForm->accessiblePoreVolumeDoubleSpinBox->setReadOnly(true);
  if(_projectTreeNode)
  {
    _cellStructuralForm->accessiblePoreVolumeDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = structureAccessiblePoreVolume())
    {
      whileBlocking(_cellStructuralForm->accessiblePoreVolumeDoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellStructuralForm->accessiblePoreVolumeDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}

void CellTreeWidgetController::reloadFrameworkProbeMoleculePopupBox()
{
  _cellStructuralForm->probeMoleculeComboBox->setEnabled(false);
  if(_projectTreeNode)
  {
    _cellStructuralForm->probeMoleculeComboBox->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<Structure::ProbeMolecule> type=frameworkProbeMolecule())
    {
      if(int index = _cellStructuralForm->probeMoleculeComboBox->findText("Mult. Val."); index>=0)
      {
        whileBlocking(_cellStructuralForm->probeMoleculeComboBox)->removeItem(index);
      }
      if(int(*type)<0)
      {
       whileBlocking(_cellStructuralForm->probeMoleculeComboBox)->setCurrentIndex(int(Structure::ProbeMolecule::multiple_values));
      }
      else
      {
        whileBlocking(_cellStructuralForm->probeMoleculeComboBox)->setCurrentIndex(int(*type));
      }
    }
    else
    {
      if(int index = _cellStructuralForm->probeMoleculeComboBox->findText("Mult. Val."); index<0)
      {
        whileBlocking(_cellStructuralForm->probeMoleculeComboBox)->addItem("Mult. Val.");
      }
      whileBlocking(_cellStructuralForm->probeMoleculeComboBox)->setCurrentText("Mult. Val.");
    }
  }
}

void CellTreeWidgetController::reloadStructuralVolumetricSurfaceArea()
{
  _cellStructuralForm->volumetricSurfaceAreaDoubleSpinBox->setReadOnly(true);
  if(_projectTreeNode)
  {
    _cellStructuralForm->volumetricSurfaceAreaDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = structureVolumetricNitrogenSurfaceArea())
    {
      whileBlocking(_cellStructuralForm->volumetricSurfaceAreaDoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellStructuralForm->volumetricSurfaceAreaDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}

void CellTreeWidgetController::reloadStructuralGravimetricSurfaceArea()
{
  _cellStructuralForm->gravimetricSurfaceAreaDoubleSpinBox->setReadOnly(true);
  if(_projectTreeNode)
  {
    _cellStructuralForm->gravimetricSurfaceAreaDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = structureGravimetricNitrogenSurfaceArea())
    {
      whileBlocking(_cellStructuralForm->gravimetricSurfaceAreaDoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellStructuralForm->gravimetricSurfaceAreaDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}

void CellTreeWidgetController::reloadStructuralNumberOfChannelSystems()
{
  _cellStructuralForm->numberOfChannelSystemsSpinBox->setReadOnly(true);
  if(_projectTreeNode)
  {
    _cellStructuralForm->numberOfChannelSystemsSpinBox->setReadOnly(!_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<int> value = structureNumberOfChannelSystems())
    {
      whileBlocking(_cellStructuralForm->numberOfChannelSystemsSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellStructuralForm->numberOfChannelSystemsSpinBox)->setText("Mult. Val.");
    }
  }
}

void CellTreeWidgetController::reloadStructuralNumberOfInaccessiblePockets()
{
  _cellStructuralForm->numberOfInaccessiblePocketsSpinBox->setReadOnly(true);
  if(_projectTreeNode)
  {
    _cellStructuralForm->numberOfInaccessiblePocketsSpinBox->setReadOnly(!_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<int> value = structureNumberOfInaccessiblePockets())
    {
      whileBlocking(_cellStructuralForm->numberOfInaccessiblePocketsSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellStructuralForm->numberOfInaccessiblePocketsSpinBox)->setText("Mult. Val.");
    }
  }
}

void CellTreeWidgetController::reloadStructuralDimensionalityOfPoreSystem()
{
  _cellStructuralForm->dimensionalityOfPoreSystemSpinBox->setReadOnly(true);
  if(_projectTreeNode)
  {
    _cellStructuralForm->dimensionalityOfPoreSystemSpinBox->setReadOnly(!_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<int> value = structureDimensionalityOfPoreSystem())
    {
      whileBlocking(_cellStructuralForm->dimensionalityOfPoreSystemSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellStructuralForm->dimensionalityOfPoreSystemSpinBox)->setText("Mult. Val.");
    }
  }
}

void CellTreeWidgetController::reloadStructuralLargestOverallCavityDiameter()
{
  _cellStructuralForm->largestOverallCavityDiameterDoubleSpinBox->setReadOnly(true);
  if(_projectTreeNode)
  {
    _cellStructuralForm->largestOverallCavityDiameterDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = structureLargestCavityDiameter())
    {
      whileBlocking(_cellStructuralForm->largestOverallCavityDiameterDoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellStructuralForm->largestOverallCavityDiameterDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}

void CellTreeWidgetController::reloadStructuralRestrictingPoreDiameter()
{
  _cellStructuralForm->restrictingPoreDiameterDoubleSpinBox->setReadOnly(true);
  if(_projectTreeNode)
  {
    _cellStructuralForm->restrictingPoreDiameterDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = structureRestrictingPoreLimitingDiameter())
    {
      whileBlocking(_cellStructuralForm->restrictingPoreDiameterDoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellStructuralForm->restrictingPoreDiameterDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}

void CellTreeWidgetController::reloadStructuralLargestDiamtereAlongAViablePath()
{
  _cellStructuralForm->largestDiameterAlongAViablePathDoubleSpinBox->setReadOnly(true);
  if(_projectTreeNode)
  {
    _cellStructuralForm->largestDiameterAlongAViablePathDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = structureLargestCavityDiameterAlongAviablePath())
    {
      whileBlocking(_cellStructuralForm->largestDiameterAlongAViablePathDoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellStructuralForm->largestDiameterAlongAViablePathDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}


std::optional<double> CellTreeWidgetController::structuralMass()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->structureMass();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

std::optional<double> CellTreeWidgetController::structuralDensity()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->structureDensity();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

std::optional<double> CellTreeWidgetController::structureHeliumVoidFraction()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->structureHeliumVoidFraction();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}


std::optional<double> CellTreeWidgetController::structureSpecificVolume()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->structureSpecificVolume();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

std::optional<double> CellTreeWidgetController::structureAccessiblePoreVolume()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->structureAccessiblePoreVolume();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void CellTreeWidgetController::setFrameworkProbeMolecule(int value)
{
  if(value>=0 && value<int(Structure::ProbeMolecule::multiple_values))
  {
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      iraspa_structure->structure()->setFrameworkProbeMolecule(Structure::ProbeMolecule(value));
      iraspa_structure->structure()->recheckRepresentationStyle();
    }

    std::vector<std::shared_ptr<RKRenderStructure>> render_structures{};
    std::transform(_iraspa_structures.begin(),_iraspa_structures.end(),std::back_inserter(render_structures),
                    [](std::shared_ptr<iRASPAStructure> iraspastructure) -> std::shared_ptr<RKRenderStructure> {return iraspastructure->structure();});

    emit computeNitrogenSurfaceArea(render_structures);
    this->reloadStructureProperties();

    reloadFrameworkProbeMoleculePopupBox();
  }
}

std::optional<Structure::ProbeMolecule> CellTreeWidgetController::frameworkProbeMolecule()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<Structure::ProbeMolecule, enum_hash> set = std::unordered_set<Structure::ProbeMolecule, enum_hash>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    Structure::ProbeMolecule value = iraspa_structure->structure()->frameworkProbeMolecule();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

std::optional<double> CellTreeWidgetController::structureVolumetricNitrogenSurfaceArea()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->structureVolumetricNitrogenSurfaceArea();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

std::optional<double> CellTreeWidgetController::structureGravimetricNitrogenSurfaceArea()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->structureGravimetricNitrogenSurfaceArea();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

std::optional<int> CellTreeWidgetController::structureNumberOfChannelSystems()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<int> set = std::unordered_set<int>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->structureNumberOfChannelSystems();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void CellTreeWidgetController::setStructureNumberOfChannelSystems(int value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setStructureNumberOfChannelSystems(value);
  }
}

std::optional<int> CellTreeWidgetController::structureNumberOfInaccessiblePockets()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<int> set = std::unordered_set<int>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    int value = iraspa_structure->structure()->structureNumberOfInaccessiblePockets();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void CellTreeWidgetController::setStructureNumberOfInaccessiblePockets(int value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setStructureNumberOfInaccessiblePockets(value);
  }
}

std::optional<int> CellTreeWidgetController::structureDimensionalityOfPoreSystem()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<int> set = std::unordered_set<int>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    int value = iraspa_structure->structure()->structureDimensionalityOfPoreSystem();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void CellTreeWidgetController::setStructureDimensionalityOfPoreSystem(int value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setStructureDimensionalityOfPoreSystem(value);
  }
}

std::optional<double> CellTreeWidgetController::structureLargestCavityDiameter()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->structureLargestCavityDiameter();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void CellTreeWidgetController::setStructureLargestCavityDiameter(double value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setStructureLargestCavityDiameter(value);
  }
}

std::optional<double> CellTreeWidgetController::structureRestrictingPoreLimitingDiameter()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->structureRestrictingPoreLimitingDiameter();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void CellTreeWidgetController::setStructureRestrictingPoreLimitingDiameter(double value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setStructureRestrictingPoreLimitingDiameter(value);
  }
}

std::optional<double> CellTreeWidgetController::structureLargestCavityDiameterAlongAviablePath()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->structureLargestCavityDiameterAlongAviablePath();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void CellTreeWidgetController::setStructureLargestCavityDiameterAlongAviablePath(double value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setStructureLargestCavityDiameterAlongAviablePath(value);
  }
}

// Symmetry properties
//========================================================================================================================================

void CellTreeWidgetController::reloadSymmetryProperties()
{
  reloadSpaceGroupHallName();
  reloadSpaceGroupITNumber();
  reloadSpaceGroupHolohedry();
  reloadSpaceGroupQualifier();
  reloadSpaceGroupPrecision();
  reloadSpaceGroupCenteringType();
  reloadSpaceGroupCenteringVectors();
  reloadSpaceGroupInversion();
  reloadSpaceGroupInversionCenter();
  reloadSpaceGroupCentroSymmetric();
  reloadSpaceGroupEnantiomorphic();
  reloadSpaceGroupLaueGroup();
  reloadSpaceGroupPointGroup();
  reloadSpaceGroupSchoenfliesSymbol();
  reloadSpaceGroupSymmorphicity();
}


void CellTreeWidgetController::reloadSpaceGroupHallName()
{
  _cellSymmetryForm->spaceGroupHallNamecomboBox->setEnabled(false);
  if(_projectTreeNode)
  {
    _cellSymmetryForm->spaceGroupHallNamecomboBox->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<int> type = symmetrySpaceGroupHallNumber())
    {
      if(int index = _cellSymmetryForm->spaceGroupHallNamecomboBox->findText("Multiple values"); index>=0)
      {
        whileBlocking(_cellSymmetryForm->spaceGroupHallNamecomboBox)->removeItem(index);
      }
      whileBlocking(_cellSymmetryForm->spaceGroupHallNamecomboBox)->setCurrentIndex(int(*type));
    }
    else
    {
      if(int index = _cellSymmetryForm->spaceGroupHallNamecomboBox->findText("Multiple values"); index<0)
      {
        whileBlocking(_cellSymmetryForm->spaceGroupHallNamecomboBox)->addItem("Multiple values");
      }
      whileBlocking(_cellSymmetryForm->spaceGroupHallNamecomboBox)->setCurrentText("Multiple values");
    }
  }
}

void CellTreeWidgetController::reloadSpaceGroupITNumber()
{
  _cellSymmetryForm->spaceGroupITNumberComboBox->setEnabled(false);
  if(_projectTreeNode)
  {
    _cellSymmetryForm->spaceGroupITNumberComboBox->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<int> type = symmetrySpaceGroupStamdardNumber())
    {
      if(int index = _cellSymmetryForm->spaceGroupITNumberComboBox->findText("Multiple values"); index>=0)
      {
        whileBlocking(_cellSymmetryForm->spaceGroupITNumberComboBox)->removeItem(index);
      }
      whileBlocking(_cellSymmetryForm->spaceGroupITNumberComboBox)->setCurrentIndex(int(*type));
    }
    else
    {
      if(int index = _cellSymmetryForm->spaceGroupITNumberComboBox->findText("Multiple values"); index<0)
      {
        whileBlocking(_cellSymmetryForm->spaceGroupITNumberComboBox)->addItem("Multiple values");
      }
      whileBlocking(_cellSymmetryForm->spaceGroupITNumberComboBox)->setCurrentText("Multiple values");
    }
  }
}

void CellTreeWidgetController::reloadSpaceGroupHolohedry()
{
  _cellSymmetryForm->holohedryLineEdit->setReadOnly(true);
  if(_projectTreeNode)
  {
    _cellSymmetryForm->holohedryLineEdit->setReadOnly(!_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<QString> string = symmetryHolohedryString())
    {
      whileBlocking(_cellSymmetryForm->holohedryLineEdit)->setText(*string);
    }
    else
    {
      whileBlocking(_cellSymmetryForm->holohedryLineEdit)->setText("Mult. Val.");
    }
  }
}

void CellTreeWidgetController::reloadSpaceGroupQualifier()
{
  _cellSymmetryForm->qualifierLineEdit->setReadOnly(true);
  if(_projectTreeNode)
  {
    _cellSymmetryForm->qualifierLineEdit->setReadOnly(!_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<QString> string = symmetryQualifierString())
    {
      whileBlocking(_cellSymmetryForm->qualifierLineEdit)->setText(*string);
    }
    else
    {
      whileBlocking(_cellSymmetryForm->qualifierLineEdit)->setText("Mult. Val.");
    }
  }
}

void CellTreeWidgetController::reloadSpaceGroupPrecision()
{
  _cellSymmetryForm->precisionDoubleSpinBox->setReadOnly(true);
  if(_projectTreeNode)
  {
    _cellSymmetryForm->precisionDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = symmetryPrecision())
    {
      whileBlocking(_cellSymmetryForm->precisionDoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellSymmetryForm->precisionDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}

void CellTreeWidgetController::reloadSpaceGroupCenteringType()
{
  _cellSymmetryForm->centeringLineEdit->setReadOnly(true);
  if(_projectTreeNode)
  {
    _cellSymmetryForm->centeringLineEdit->setReadOnly(!_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<QString> value = symmetryCenteringString())
    {
      whileBlocking(_cellSymmetryForm->centeringLineEdit)->setText(*value);
    }
    else
    {
      whileBlocking(_cellSymmetryForm->centeringLineEdit)->setText("Mult. Val.");
    }
  }
}

void CellTreeWidgetController::reloadSpaceGroupCenteringVectors()
{
  _cellSymmetryForm->centerintVector1LineEdit->setReadOnly(true);
  _cellSymmetryForm->centerintVector2LineEdit->setReadOnly(true);
  _cellSymmetryForm->centerintVector3LineEdit->setReadOnly(true);
  _cellSymmetryForm->centerintVector4LineEdit->setReadOnly(true);
  if(_projectTreeNode)
  {
    _cellSymmetryForm->centerintVector1LineEdit->setReadOnly(!_projectTreeNode->isEditable());
    _cellSymmetryForm->centerintVector2LineEdit->setReadOnly(!_projectTreeNode->isEditable());
    _cellSymmetryForm->centerintVector3LineEdit->setReadOnly(!_projectTreeNode->isEditable());
    _cellSymmetryForm->centerintVector4LineEdit->setReadOnly(!_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<int> value = symmetrySpaceGroupHallNumber())
    {
      std::vector<QString> latticeVector =  SKSpaceGroup::latticeTranslationStrings(*value);
      whileBlocking(_cellSymmetryForm->centerintVector1LineEdit)->setText(latticeVector[0]);
      whileBlocking(_cellSymmetryForm->centerintVector2LineEdit)->setText(latticeVector[1]);
      whileBlocking(_cellSymmetryForm->centerintVector3LineEdit)->setText(latticeVector[2]);
      whileBlocking(_cellSymmetryForm->centerintVector4LineEdit)->setText(latticeVector[3]);
    }
    else
    {
      whileBlocking(_cellSymmetryForm->centerintVector1LineEdit)->setText("Mult. Val.");
      whileBlocking(_cellSymmetryForm->centerintVector2LineEdit)->setText("Mult. Val.");
      whileBlocking(_cellSymmetryForm->centerintVector3LineEdit)->setText("Mult. Val.");
      whileBlocking(_cellSymmetryForm->centerintVector4LineEdit)->setText("Mult. Val.");
    }
  }
}

void CellTreeWidgetController::reloadSpaceGroupInversion()
{
  _cellSymmetryForm->inversionLineEdit->setReadOnly(true);
  if(_projectTreeNode)
  {
    _cellSymmetryForm->inversionLineEdit->setReadOnly(!_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<bool> type = symmetryInversion())
    {
      whileBlocking(_cellSymmetryForm->inversionLineEdit)->setText(*type ? "yes" : "no");
    }
    else
    {
      whileBlocking(_cellSymmetryForm->inversionLineEdit)->setText("Multiple values");
    }
  }
}

void CellTreeWidgetController::reloadSpaceGroupInversionCenter()
{
  _cellSymmetryForm->inversionCenterLineEdit->setReadOnly(true);
  if(_projectTreeNode)
  {
    _cellSymmetryForm->inversionCenterLineEdit->setReadOnly(!_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<QString> value = symmetryInversionCenterString())
    {
      whileBlocking(_cellSymmetryForm->inversionCenterLineEdit)->setText(*value);
    }
    else
    {
      whileBlocking(_cellSymmetryForm->inversionCenterLineEdit)->setText("Mult. Val.");
    }
  }
}

void CellTreeWidgetController::reloadSpaceGroupCentroSymmetric()
{
  _cellSymmetryForm->centrosymmetricLineEdit->setReadOnly(true);
  if(_projectTreeNode)
  {
    _cellSymmetryForm->centrosymmetricLineEdit->setReadOnly(!_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<bool> type = symmetryCentrosymmetric())
    {
      whileBlocking(_cellSymmetryForm->centrosymmetricLineEdit)->setText(*type ? "yes" : "no");
    }
    else
    {
      whileBlocking(_cellSymmetryForm->centrosymmetricLineEdit)->setText("Multiple values");
    }
  }
}

void CellTreeWidgetController::reloadSpaceGroupEnantiomorphic()
{
  _cellSymmetryForm->enantiomorphicLineEdit->setReadOnly(true);
  if(_projectTreeNode)
  {
    _cellSymmetryForm->enantiomorphicLineEdit->setReadOnly(!_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<bool> type = symmetryEnatiomorphic())
    {
      whileBlocking(_cellSymmetryForm->enantiomorphicLineEdit)->setText(*type ? "yes" : "no");
    }
    else
    {
      whileBlocking(_cellSymmetryForm->enantiomorphicLineEdit)->setText("Multiple values");
    }
  }
}

void CellTreeWidgetController::reloadSpaceGroupLaueGroup()
{
  _cellSymmetryForm->LaueGroupLineEdit->setReadOnly(true);
  if(_projectTreeNode)
  {
    _cellSymmetryForm->LaueGroupLineEdit->setReadOnly(!_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<int> type = symmetryPointGroup())
    {
      QString name = SKPointGroup::pointGroupData[*type].LaueString();
      whileBlocking(_cellSymmetryForm->LaueGroupLineEdit)->setText(name);
    }
    else
    {
      whileBlocking(_cellSymmetryForm->LaueGroupLineEdit)->setText("Multiple values");
    }
  }
}

void CellTreeWidgetController::reloadSpaceGroupPointGroup()
{
  _cellSymmetryForm->pointGroupLineEdit->setReadOnly(true);
  if(_projectTreeNode)
  {
    _cellSymmetryForm->pointGroupLineEdit->setReadOnly(!_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<int> type = symmetryPointGroup())
    {
      QString name = SKPointGroup::pointGroupData[*type].symbol();
      whileBlocking(_cellSymmetryForm->pointGroupLineEdit)->setText(name);
    }
    else
    {
      whileBlocking(_cellSymmetryForm->pointGroupLineEdit)->setText("Multiple values");
    }
  }
}

void CellTreeWidgetController::reloadSpaceGroupSchoenfliesSymbol()
{
  _cellSymmetryForm->SchoenfliesLineEdit->setReadOnly(true);
  if(_projectTreeNode)
  {
    _cellSymmetryForm->SchoenfliesLineEdit->setReadOnly(!_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<int> type = symmetryPointGroup())
    {
      QString name = SKPointGroup::pointGroupData[*type].schoenflies();
      whileBlocking(_cellSymmetryForm->SchoenfliesLineEdit)->setText(name);
    }
    else
    {
      whileBlocking(_cellSymmetryForm->SchoenfliesLineEdit)->setText("Multiple values");
    }
  }
}

void CellTreeWidgetController::reloadSpaceGroupSymmorphicity()
{
  _cellSymmetryForm->symmorphicityLineEdit->setReadOnly(true);
  if(_projectTreeNode)
  {
    _cellSymmetryForm->symmorphicityLineEdit->setReadOnly(!_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<QString> string = symmetrySymmorphicity())
    {
      whileBlocking(_cellSymmetryForm->symmorphicityLineEdit)->setText(*string);
    }
    else
    {
      whileBlocking(_cellSymmetryForm->symmorphicityLineEdit)->setText("Multiple values");
    }
  }
}

std::optional<int> CellTreeWidgetController::symmetrySpaceGroupHallNumber()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<int> set = std::unordered_set<int>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    int value = iraspa_structure->structure()->spaceGroup().spaceGroupSetting().HallNumber();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void CellTreeWidgetController::setSymmetrySpaceGroupHallNumber(int value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setSpaceGroupHallNumber(value);
  }

  std::vector<std::shared_ptr<RKRenderStructure>> render_structures{};
  std::transform(_iraspa_structures.begin(),_iraspa_structures.end(),std::back_inserter(render_structures),
                  [](std::shared_ptr<iRASPAStructure> iraspastructure) -> std::shared_ptr<RKRenderStructure> {return iraspastructure->structure();});

  emit invalidateCachedAmbientOcclusionTexture(render_structures);
  emit rendererReloadData();

  reloadSymmetryProperties();
}

std::optional<int> CellTreeWidgetController::symmetrySpaceGroupStamdardNumber()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<int> set = std::unordered_set<int>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    int value = iraspa_structure->structure()->spaceGroup().spaceGroupSetting().number();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void CellTreeWidgetController::setSymmetrySpaceGroupStandardNumber(int value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    int HallNumber = SKSpaceGroup::spaceGroupHallData[value].front();
    iraspa_structure->structure()->setSpaceGroupHallNumber(HallNumber);
  }

  std::vector<std::shared_ptr<RKRenderStructure>> render_structures{};
  std::transform(_iraspa_structures.begin(),_iraspa_structures.end(),std::back_inserter(render_structures),
                  [](std::shared_ptr<iRASPAStructure> iraspastructure) -> std::shared_ptr<RKRenderStructure> {return iraspastructure->structure();});

  emit invalidateCachedAmbientOcclusionTexture(render_structures);
  emit rendererReloadData();

  reloadSymmetryProperties();
}

std::optional<QString> CellTreeWidgetController::symmetryHolohedryString()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::set<QString> set = std::set<QString>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    int pointGroup = iraspa_structure->structure()->spaceGroup().spaceGroupSetting().pointGroup();
    QString value = SKPointGroup::pointGroupData[pointGroup].holohedryString();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

std::optional<QString> CellTreeWidgetController::symmetryQualifierString()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::set<QString> set = std::set<QString>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    QString value = iraspa_structure->structure()->spaceGroup().spaceGroupSetting().qualifier();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

std::optional<double> CellTreeWidgetController::symmetryPrecision()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->cell()->precision();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void CellTreeWidgetController::setSymmetryPrecision(double value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->cell()->setPrecision(value);
  }
}

std::optional<QString> CellTreeWidgetController::symmetryCenteringString()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::set<QString> set = std::set<QString>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    QString value = iraspa_structure->structure()->spaceGroup().spaceGroupSetting().centringString();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

std::optional<bool> CellTreeWidgetController::symmetryInversion()
{
    if(_iraspa_structures.empty())
    {
      return std::nullopt;
    }
    std::unordered_set<bool> set = std::unordered_set<bool>{};
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      bool inversion = iraspa_structure->structure()->spaceGroup().spaceGroupSetting().inversionAtOrigin();
      set.insert(inversion);
    }

    if(set.size() == 1)
    {
      return *set.begin();
    }
    return std::nullopt;
}


std::optional<QString> CellTreeWidgetController::symmetryInversionCenterString()
{
    if(_iraspa_structures.empty())
    {
      return std::nullopt;
    }
    std::set<QString> set = std::set<QString>{};
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      int HallNumber = iraspa_structure->structure()->spaceGroup().spaceGroupSetting().HallNumber();
      QString string = SKSpaceGroup::inversionCenterString(HallNumber);
      set.insert(string);
    }

    if(set.size() == 1)
    {
      return *set.begin();
    }
    return std::nullopt;
}

std::optional<bool> CellTreeWidgetController::symmetryCentrosymmetric()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<bool> set = std::unordered_set<bool>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    int pointGroup = iraspa_structure->structure()->spaceGroup().spaceGroupSetting().pointGroup();
    bool centroSymmetric = SKPointGroup::pointGroupData[pointGroup].centrosymmetric();
    set.insert(pointGroup);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

std::optional<bool> CellTreeWidgetController::symmetryEnatiomorphic()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<bool> set = std::unordered_set<bool>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    int pointGroup = iraspa_structure->structure()->spaceGroup().spaceGroupSetting().pointGroup();
    bool centroSymmetric = SKPointGroup::pointGroupData[pointGroup].enantiomorphic();
    set.insert(pointGroup);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}


std::optional<int> CellTreeWidgetController::symmetryPointGroup()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<int> set = std::unordered_set<int>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    int pointGroup = iraspa_structure->structure()->spaceGroup().spaceGroupSetting().pointGroup();
    set.insert(pointGroup);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}


std::optional<QString> CellTreeWidgetController::symmetrySymmorphicity()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::set<QString> set = std::set<QString>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    QString symmorphicity = iraspa_structure->structure()->spaceGroup().spaceGroupSetting().symmorphicityString();
    set.insert(symmorphicity);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void CellTreeWidgetController::computeHeliumVoidFractionPushButton()
{
  std::vector<std::shared_ptr<RKRenderStructure>> render_structures{};
  std::transform(_iraspa_structures.begin(),_iraspa_structures.end(),std::back_inserter(render_structures),
                  [](std::shared_ptr<iRASPAStructure> iraspastructure) -> std::shared_ptr<RKRenderStructure> {return iraspastructure->structure();});

  emit computeHeliumVoidFraction(render_structures);
  this->reloadStructureProperties();
}

void CellTreeWidgetController::computeGravimetricSurfaceAreaPushButton()
{
  std::vector<std::shared_ptr<RKRenderStructure>> render_structures{};
  std::transform(_iraspa_structures.begin(),_iraspa_structures.end(),std::back_inserter(render_structures),
                  [](std::shared_ptr<iRASPAStructure> iraspastructure) -> std::shared_ptr<RKRenderStructure> {return iraspastructure->structure();});

  emit computeNitrogenSurfaceArea(render_structures);
  this->reloadStructureProperties();
}

