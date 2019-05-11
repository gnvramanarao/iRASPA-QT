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
#include <iostream>
#include <math.h>
#include <QLabel>
#include <QSpinBox>
#include <mathkit.h>
#include <renderkit.h>
#include <symmetrykit.h>
#include <foundationkit.h>
#include "textfield.h"

CellTreeWidgetController::CellTreeWidgetController(QWidget* parent): QTreeWidget(parent ),
    _cellCellForm(new CellCellForm),
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

  QObject::connect(_cellCellForm->originXDoubleSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &CellTreeWidgetController::setOriginX);
  QObject::connect(_cellCellForm->originYDoubleSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &CellTreeWidgetController::setOriginY);
  QObject::connect(_cellCellForm->originZDoubleSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &CellTreeWidgetController::setOriginZ);

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

void CellTreeWidgetController::expandStructuralItem()
{
  QModelIndex index = indexFromItem(topLevelItem(1),0);
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
  QModelIndex index = indexFromItem(topLevelItem(2),0);
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
  if (projectTreeNode)
  {
    if(std::shared_ptr<iRASPAProject> iraspaProject = projectTreeNode->representedObject())
    {
      if(std::shared_ptr<Project> project = iraspaProject->project())
      {
        if (std::shared_ptr<ProjectStructure> projectStructure = std::dynamic_pointer_cast<ProjectStructure>(project))
        {
          _projectStructure = projectStructure;
          reloadData();
          return;
        }
      }
    }
  }
  _projectStructure = nullptr;
}


void CellTreeWidgetController::setStructures(std::vector<std::shared_ptr<Structure>> structures)
{
  _structures = structures;
  reloadData();
}

void CellTreeWidgetController::reloadData()
{
   reloadCellProperties();
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

  reloadCellOriginX();
  reloadCellOriginY();
  reloadCellOriginZ();
}

void CellTreeWidgetController::reloadStructureType()
{
	_cellCellForm->cellStructureTypeComboBox->setEnabled(false);
	if (!_structures.empty())
	{
		if (stdx::optional<iRASPAStructureType> type = structureType())
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
  if(!_structures.empty())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = unitCellLengthA())
    {
      whileBlocking(_cellCellForm->unitCellADoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellCellForm->unitCellADoubleSpinBox)->setText("Mult. Val.");
    }

    if (stdx::optional<double> value = unitCellLengthB())
    {
      whileBlocking(_cellCellForm->unitCellBDoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellCellForm->unitCellBDoubleSpinBox)->setText("Mult. Val.");
    }

    if (stdx::optional<double> value = unitCellLengthC())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = unitCellAngleAlpha())
    {
      whileBlocking(_cellCellForm->unitCellAlphaDoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellCellForm->unitCellAlphaDoubleSpinBox)->setText("Mult. Val.");
    }

    if (stdx::optional<double> value = unitCellAngleBeta())
    {
      whileBlocking(_cellCellForm->unitCellBetaDoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellCellForm->unitCellBetaDoubleSpinBox)->setText("Mult. Val.");
    }

    if (stdx::optional<double> value = unitCellAngleGamma())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = unitCellAX())
    {
      whileBlocking(_cellCellForm->unitCellAXDoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellCellForm->unitCellAXDoubleSpinBox)->setText("Mult. Val.");
    }

    if (stdx::optional<double> value = unitCellAY())
    {
      whileBlocking(_cellCellForm->unitCellAYDoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellCellForm->unitCellAYDoubleSpinBox)->setText("Mult. Val.");
    }

    if (stdx::optional<double> value = unitCellAZ())
    {
      whileBlocking(_cellCellForm->unitCellAZDoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellCellForm->unitCellAZDoubleSpinBox)->setText("Mult. Val.");
    }

    if (stdx::optional<double> value = unitCellBX())
    {
      whileBlocking(_cellCellForm->unitCellBXDoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellCellForm->unitCellBXDoubleSpinBox)->setText("Mult. Val.");
    }

    if (stdx::optional<double> value = unitCellBY())
    {
      whileBlocking(_cellCellForm->unitCellBYDoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellCellForm->unitCellBYDoubleSpinBox)->setText("Mult. Val.");
    }

    if (stdx::optional<double> value = unitCellBZ())
    {
      whileBlocking(_cellCellForm->unitCellBZDoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellCellForm->unitCellBZDoubleSpinBox)->setText("Mult. Val.");
    }

    if (stdx::optional<double> value = unitCellCX())
    {
      whileBlocking(_cellCellForm->unitCellCXDoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellCellForm->unitCellCXDoubleSpinBox)->setText("Mult. Val.");
    }

    if (stdx::optional<double> value = unitCellCY())
    {
      whileBlocking(_cellCellForm->unitCellCYDoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellCellForm->unitCellCYDoubleSpinBox)->setText("Mult. Val.");
    }

    if (stdx::optional<double> value = unitCellCZ())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = unitCellVolume())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = unitCellPerpendicularWidthX())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = unitCellPerpendicularWidthY())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = unitCellPerpendicularWidthZ())
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
  if(!_structures.empty())
  {
    if (stdx::optional<int> value = maximumReplicasX())
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
  if(!_structures.empty())
  {
    if (stdx::optional<int> value = minimumReplicasX())
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
  if(!_structures.empty())
  {
    if (stdx::optional<int> value = maximumReplicasY())
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
  if(!_structures.empty())
  {
    if (stdx::optional<int> value = minimumReplicasY())
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
  if(!_structures.empty())
  {
    if (stdx::optional<int> value = maximumReplicasZ())
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
  if(!_structures.empty())
  {
    if (stdx::optional<int> value = minimumReplicasZ())
    {
      whileBlocking(_cellCellForm->cellMinimumReplicaZ)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellCellForm->cellMinimumReplicaZ)->setText("Mult. Val.");
    }
  }
}

void CellTreeWidgetController::reloadCellOriginX()
{
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = originX())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = originY())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = originZ())
    {
      whileBlocking(_cellCellForm->originZDoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellCellForm->originZDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}


stdx::optional<iRASPAStructureType> CellTreeWidgetController::structureType()
{
	if (_structures.empty())
	{
		return stdx::nullopt;
	}
    std::unordered_set<iRASPAStructureType, enum_hash> set = std::unordered_set<iRASPAStructureType, enum_hash>{};
	for (std::shared_ptr<Structure> structure : _structures)
	{
		iRASPAStructureType value = structure->structureType();
        set.emplace(value);
	}

	if (set.size() == 1)
	{
      iRASPAStructureType value = *set.begin();
      return value;
	}
	return stdx::nullopt;
}

SKBoundingBox CellTreeWidgetController::boundingBox()
{
  if(!_structures.empty())
  {
    double3 minimum = double3(DBL_MAX,DBL_MAX, DBL_MAX);
    double3 maximum = double3(-DBL_MAX,-DBL_MAX, -DBL_MAX);
    for(std::shared_ptr<Structure> structure : _structures)
    {
      SKBoundingBox transformedBoundingBox = structure->transformedBoundingBox();

      std::cout << "TEST-CHECK: " << transformedBoundingBox.minimum().x << ", "
                   << transformedBoundingBox.minimum().y << ", "
                      << transformedBoundingBox.minimum().z << std::endl;

      minimum.x = std::min(minimum.x, transformedBoundingBox.minimum().x + structure->origin().x);
      minimum.y = std::min(minimum.y, transformedBoundingBox.minimum().y + structure->origin().y);
      minimum.z = std::min(minimum.z, transformedBoundingBox.minimum().z + structure->origin().z);
      maximum.x = std::max(maximum.x, transformedBoundingBox.maximum().x + structure->origin().x);
      maximum.y = std::max(maximum.y, transformedBoundingBox.maximum().y + structure->origin().y);
      maximum.z = std::max(maximum.z, transformedBoundingBox.maximum().z + structure->origin().z);
    }
    std::cout << "MINIMUM: " << minimum.x << ", " << minimum.y << ", " << minimum.z << std::endl;
    return SKBoundingBox(minimum,maximum);
  }
  return SKBoundingBox(double3(0.0,0.0,0.0),double3(0.0,0.0,0.0));
}

stdx::optional<double> CellTreeWidgetController::unitCellAX()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    set.insert(structure->cell()->unitCell().ax);
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }
  return stdx::nullopt;
}


stdx::optional<double> CellTreeWidgetController::unitCellAY()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    set.insert(structure->cell()->unitCell().ay);
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }
  return stdx::nullopt;
}

stdx::optional<double> CellTreeWidgetController::unitCellAZ()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    set.insert(structure->cell()->unitCell().az);
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }
  return stdx::nullopt;
}


stdx::optional<double> CellTreeWidgetController::unitCellBX()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    set.insert(structure->cell()->unitCell().bx);
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }
  return stdx::nullopt;
}

stdx::optional<double> CellTreeWidgetController::unitCellBY()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    set.insert(structure->cell()->unitCell().by);
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }
  return stdx::nullopt;
}

stdx::optional<double> CellTreeWidgetController::unitCellBZ()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    set.insert(structure->cell()->unitCell().bz);
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }
  return stdx::nullopt;
}

stdx::optional<double> CellTreeWidgetController::unitCellCX()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    set.insert(structure->cell()->unitCell().cx);
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }
  return stdx::nullopt;
}

stdx::optional<double> CellTreeWidgetController::unitCellCY()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    set.insert(structure->cell()->unitCell().cy);
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }
  return stdx::nullopt;
}

stdx::optional<double> CellTreeWidgetController::unitCellCZ()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    set.insert(structure->cell()->unitCell().cz);
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }
  return stdx::nullopt;
}

stdx::optional<double> CellTreeWidgetController::unitCellLengthA()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    set.insert(structure->cell()->a());
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }
  return stdx::nullopt;
}

void CellTreeWidgetController::setUnitCellLengthA(double value)
{
  if(!_structures.empty())
  {
    for(std::shared_ptr<Structure> structure: _structures)
    {
      structure->cell()->setLengthA(value);
      structure->reComputeBoundingBox();
    }

    SKBoundingBox box = _projectStructure->renderBoundingBox();
    _projectStructure->camera()->resetForNewBoundingBox(box);

    emit invalidateCachedAmbientOcclusionTexture(std::vector<std::shared_ptr<RKRenderStructure>>{_structures.begin(), _structures.end()});
    emit rendererReloadData();

    reloadBoundingBox();
    reloadUnitCell();
  }
}

stdx::optional<double> CellTreeWidgetController::unitCellLengthB()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    set.insert(structure->cell()->b());
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }
  return stdx::nullopt;
}

void CellTreeWidgetController::setUnitCellLengthB(double value)
{
  if(!_structures.empty())
  {
    for(std::shared_ptr<Structure> structure: _structures)
    {
      structure->cell()->setLengthB(value);
      structure->reComputeBoundingBox();
    }

    SKBoundingBox box = _projectStructure->renderBoundingBox();
    _projectStructure->camera()->resetForNewBoundingBox(box);

    emit invalidateCachedAmbientOcclusionTexture(std::vector<std::shared_ptr<RKRenderStructure>>{_structures.begin(), _structures.end()});
    emit rendererReloadData();

    reloadBoundingBox();
    reloadUnitCell();
  }
}

stdx::optional<double> CellTreeWidgetController::unitCellLengthC()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    set.insert(structure->cell()->c());
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }
  return stdx::nullopt;
}

void CellTreeWidgetController::setUnitCellLengthC(double value)
{
  if(!_structures.empty())
  {
    for(std::shared_ptr<Structure> structure: _structures)
    {
      structure->cell()->setLengthC(value);
      structure->reComputeBoundingBox();
    }

    SKBoundingBox box = _projectStructure->renderBoundingBox();
    _projectStructure->camera()->resetForNewBoundingBox(box);

    emit invalidateCachedAmbientOcclusionTexture(std::vector<std::shared_ptr<RKRenderStructure>>{_structures.begin(), _structures.end()});
    emit rendererReloadData();

    reloadBoundingBox();
    reloadUnitCell();
  }
}

stdx::optional<double> CellTreeWidgetController::unitCellAngleAlpha()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    set.insert(structure->cell()->alpha() * 180.0/M_PI);
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }
  return stdx::nullopt;
}

void CellTreeWidgetController::setUnitCellAngleAlpha(double value)
{
  if(!_structures.empty())
  {
    for(std::shared_ptr<Structure> structure: _structures)
    {
      structure->cell()->setAlphaAngle(value * M_PI / 180.0);
      structure->reComputeBoundingBox();
    }

    SKBoundingBox box = _projectStructure->renderBoundingBox();
    _projectStructure->camera()->resetForNewBoundingBox(box);

    emit invalidateCachedAmbientOcclusionTexture(std::vector<std::shared_ptr<RKRenderStructure>>{_structures.begin(), _structures.end()});
    emit rendererReloadData();

    reloadBoundingBox();
    reloadUnitCell();
  }
}

stdx::optional<double> CellTreeWidgetController::unitCellAngleBeta()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    set.insert(structure->cell()->beta() * 180.0/M_PI);
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }
  return stdx::nullopt;
}

void CellTreeWidgetController::setUnitCellAngleBeta(double value)
{
  if(!_structures.empty())
  {
    for(std::shared_ptr<Structure> structure: _structures)
    {
      structure->cell()->setBetaAngle(value * M_PI / 180.0);
      structure->reComputeBoundingBox();
    }

    SKBoundingBox box = _projectStructure->renderBoundingBox();
    _projectStructure->camera()->resetForNewBoundingBox(box);

    emit invalidateCachedAmbientOcclusionTexture(std::vector<std::shared_ptr<RKRenderStructure>>{_structures.begin(), _structures.end()});
    emit rendererReloadData();

    reloadBoundingBox();
    reloadUnitCell();
  }
}

stdx::optional<double> CellTreeWidgetController::unitCellAngleGamma()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    set.insert(structure->cell()->gamma() * 180.0/M_PI);
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }
  return stdx::nullopt;
}

void CellTreeWidgetController::setUnitCellAngleGamma(double value)
{
  if(!_structures.empty())
  {
    for(std::shared_ptr<Structure> structure: _structures)
    {
      structure->cell()->setGammaAngle(value * M_PI / 180.0);
      structure->reComputeBoundingBox();
    }

    SKBoundingBox box = _projectStructure->renderBoundingBox();
    _projectStructure->camera()->resetForNewBoundingBox(box);

    emit invalidateCachedAmbientOcclusionTexture(std::vector<std::shared_ptr<RKRenderStructure>>{_structures.begin(), _structures.end()});
    emit rendererReloadData();

    reloadBoundingBox();
    reloadUnitCell();
  }
}


stdx::optional<double> CellTreeWidgetController::unitCellVolume()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    set.insert(structure->cell()->volume());
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }

  return stdx::nullopt;
}


stdx::optional<double> CellTreeWidgetController::unitCellPerpendicularWidthX()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    set.insert(structure->cell()->perpendicularWidths().x);
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }

  return stdx::nullopt;
}

stdx::optional<double> CellTreeWidgetController::unitCellPerpendicularWidthY()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    set.insert(structure->cell()->perpendicularWidths().y);
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }

  return stdx::nullopt;
}


stdx::optional<double> CellTreeWidgetController::unitCellPerpendicularWidthZ()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    set.insert(structure->cell()->perpendicularWidths().z);
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }

  return stdx::nullopt;
}


stdx::optional<int> CellTreeWidgetController::maximumReplicasX()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<int> set = std::unordered_set<int>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    set.insert(structure->cell()->maximumReplicaX());
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }

  return stdx::nullopt;
}

void CellTreeWidgetController::setMaximumReplicasX(int value)
{
  if(!_structures.empty())
  {
    for(std::shared_ptr<Structure> structure: _structures)
    {
      if (value >= structure->cell()->minimumReplicaX())
      {
        structure->cell()->setMaximumReplicaX(value);
      }
    }

    SKBoundingBox box = _projectStructure->renderBoundingBox();
    _projectStructure->camera()->resetForNewBoundingBox(box);

    emit invalidateCachedAmbientOcclusionTexture(std::vector<std::shared_ptr<RKRenderStructure>>{_structures.begin(), _structures.end()});
    emit rendererReloadData();

    reloadCellMaximumReplicasX();
    reloadBoundingBox();
  }
}

stdx::optional<int> CellTreeWidgetController::maximumReplicasY()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<int> set = std::unordered_set<int>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    set.insert(structure->cell()->maximumReplicaY());
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }

  return stdx::nullopt;
}

void CellTreeWidgetController::setMaximumReplicasY(int value)
{
  if(!_structures.empty())
  {
    for(std::shared_ptr<Structure> structure: _structures)
    {
      if (value >= structure->cell()->minimumReplicaY())
      {
        structure->cell()->setMaximumReplicaY(value);
      }
    }

    SKBoundingBox box = _projectStructure->renderBoundingBox();
    _projectStructure->camera()->resetForNewBoundingBox(box);

    emit invalidateCachedAmbientOcclusionTexture(std::vector<std::shared_ptr<RKRenderStructure>>{_structures.begin(), _structures.end()});
    emit rendererReloadData();

    reloadCellMaximumReplicasY();
    reloadBoundingBox();
  }
}

stdx::optional<int> CellTreeWidgetController::maximumReplicasZ()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<int> set = std::unordered_set<int>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    set.insert(structure->cell()->maximumReplicaZ());
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }

  return stdx::nullopt;
}

void CellTreeWidgetController::setMaximumReplicasZ(int value)
{
  if(!_structures.empty())
  {
    for(std::shared_ptr<Structure> structure: _structures)
    {
      if (value >= structure->cell()->minimumReplicaZ())
      {
        structure->cell()->setMaximumReplicaZ(value);
      }
    }

    SKBoundingBox box = _projectStructure->renderBoundingBox();
    _projectStructure->camera()->resetForNewBoundingBox(box);

    emit invalidateCachedAmbientOcclusionTexture(std::vector<std::shared_ptr<RKRenderStructure>>{_structures.begin(), _structures.end()});
    emit rendererReloadData();

    reloadCellMaximumReplicasZ();
    reloadBoundingBox();
  }
}

stdx::optional<int> CellTreeWidgetController::minimumReplicasX()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<int> set = std::unordered_set<int>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    set.insert(structure->cell()->minimumReplicaX());
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }

  return stdx::nullopt;
}

void CellTreeWidgetController::setMinimumReplicasX(int value)
{
  if(!_structures.empty())
  {
    for(std::shared_ptr<Structure> structure: _structures)
    {
      if (value <= structure->cell()->maximumReplicaX())
      {
        structure->cell()->setMinimumReplicaX(value);
      }
    }

    SKBoundingBox box = _projectStructure->renderBoundingBox();
    _projectStructure->camera()->resetForNewBoundingBox(box);

    emit invalidateCachedAmbientOcclusionTexture(std::vector<std::shared_ptr<RKRenderStructure>>{_structures.begin(), _structures.end()});
    emit rendererReloadData();

    reloadCellMinimumReplicasX();
    reloadBoundingBox();
  }
}

stdx::optional<int> CellTreeWidgetController::minimumReplicasY()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<int> set = std::unordered_set<int>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    set.insert(structure->cell()->minimumReplicaY());
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }

  return stdx::nullopt;
}

void CellTreeWidgetController::setMinimumReplicasY(int value)
{
  if(!_structures.empty())
  {
    for(std::shared_ptr<Structure> structure: _structures)
    {
      if (value <= structure->cell()->maximumReplicaY())
      {
        structure->cell()->setMinimumReplicaY(value);
      }
    }

    SKBoundingBox box = _projectStructure->renderBoundingBox();
    _projectStructure->camera()->resetForNewBoundingBox(box);

    emit invalidateCachedAmbientOcclusionTexture(std::vector<std::shared_ptr<RKRenderStructure>>{_structures.begin(), _structures.end()});
    emit rendererReloadData();

    reloadCellMinimumReplicasY();
    reloadBoundingBox();
  }
}

stdx::optional<int> CellTreeWidgetController::minimumReplicasZ()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<int> set = std::unordered_set<int>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    set.insert(structure->cell()->minimumReplicaZ());
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }

  return stdx::nullopt;
}

void CellTreeWidgetController::setMinimumReplicasZ(int value)
{
  if(!_structures.empty())
  {
    for(std::shared_ptr<Structure> structure: _structures)
    {
      if (value <= structure->cell()->maximumReplicaZ())
      {
        structure->cell()->setMinimumReplicaZ(value);
      }
    }

    SKBoundingBox box = _projectStructure->renderBoundingBox();
    _projectStructure->camera()->resetForNewBoundingBox(box);

    emit invalidateCachedAmbientOcclusionTexture(std::vector<std::shared_ptr<RKRenderStructure>>{_structures.begin(), _structures.end()});
    emit rendererReloadData();

    reloadCellMinimumReplicasZ();
    reloadBoundingBox();
  }
}

stdx::optional<double>  CellTreeWidgetController::originX()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    set.insert(structure->origin().x);
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }

  return stdx::nullopt;
}

void CellTreeWidgetController::setOriginX(double value)
{
  if(!_structures.empty())
  {
    for(std::shared_ptr<Structure> structure: _structures)
    {
      structure->setOriginX(value);
    }

    SKBoundingBox box = _projectStructure->renderBoundingBox();
    _projectStructure->camera()->resetForNewBoundingBox(box);

    emit invalidateCachedAmbientOcclusionTexture(std::vector<std::shared_ptr<RKRenderStructure>>{_structures.begin(), _structures.end()});
    emit rendererReloadData();

    reloadBoundingBox();
  }
}


stdx::optional<double>  CellTreeWidgetController::originY()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    set.insert(structure->origin().y);
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }

  return stdx::nullopt;
}

void CellTreeWidgetController::setOriginY(double value)
{
  if(!_structures.empty())
  {
    for(std::shared_ptr<Structure> structure: _structures)
    {
      structure->setOriginY(value);
    }

    SKBoundingBox box = _projectStructure->renderBoundingBox();
    _projectStructure->camera()->resetForNewBoundingBox(box);

    emit invalidateCachedAmbientOcclusionTexture(std::vector<std::shared_ptr<RKRenderStructure>>{_structures.begin(), _structures.end()});
    emit rendererReloadData();

    reloadBoundingBox();
  }
}


stdx::optional<double>  CellTreeWidgetController::originZ()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    set.insert(structure->origin().z);
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }

  return stdx::nullopt;
}

void CellTreeWidgetController::setOriginZ(double value)
{
  if(!_structures.empty())
  {
    for(std::shared_ptr<Structure> structure: _structures)
    {
      structure->setOriginZ(value);
    }

    SKBoundingBox box = _projectStructure->renderBoundingBox();
    _projectStructure->camera()->resetForNewBoundingBox(box);

    emit invalidateCachedAmbientOcclusionTexture(std::vector<std::shared_ptr<RKRenderStructure>>{_structures.begin(), _structures.end()});
    emit rendererReloadData();

    reloadBoundingBox();
  }
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
}

void CellTreeWidgetController::reloadStructuralMaterialType()
{
  if(!_structures.empty())
  {

  }
}

void CellTreeWidgetController::reloadStructuralMass()
{
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = structuralMass())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = structuralDensity())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = structureHeliumVoidFraction())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = structureSpecificVolume())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = structureAccessiblePoreVolume())
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
  if(!_structures.empty())
  {
    if (stdx::optional<ProbeMolecule> type=frameworkProbeMolecule())
    {
      if(int index = _cellStructuralForm->probeMoleculeComboBox->findText("Mult. Val."); index>=0)
      {
        whileBlocking(_cellStructuralForm->probeMoleculeComboBox)->removeItem(index);
      }
      if(int(*type)<0)
      {
       whileBlocking(_cellStructuralForm->probeMoleculeComboBox)->setCurrentIndex(int(ProbeMolecule::multiple_values));
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = structureVolumetricNitrogenSurfaceArea())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = structureGravimetricNitrogenSurfaceArea())
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
  if(!_structures.empty())
  {
    if (stdx::optional<int> value = structureNumberOfChannelSystems())
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
  if(!_structures.empty())
  {
    if (stdx::optional<int> value = structureNumberOfInaccessiblePockets())
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
  if(!_structures.empty())
  {
    if (stdx::optional<int> value = structureDimensionalityOfPoreSystem())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = structureLargestCavityDiameter())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = structureRestrictingPoreLimitingDiameter())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = structureLargestCavityDiameterAlongAviablePath())
    {
      whileBlocking(_cellStructuralForm->largestDiameterAlongAViablePathDoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_cellStructuralForm->largestDiameterAlongAViablePathDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}


stdx::optional<double> CellTreeWidgetController::structuralMass()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->structureMass();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

stdx::optional<double> CellTreeWidgetController::structuralDensity()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->structureDensity();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

stdx::optional<double> CellTreeWidgetController::structureHeliumVoidFraction()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->structureHeliumVoidFraction();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}


stdx::optional<double> CellTreeWidgetController::structureSpecificVolume()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->structureSpecificVolume();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

stdx::optional<double> CellTreeWidgetController::structureAccessiblePoreVolume()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->structureAccessiblePoreVolume();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void CellTreeWidgetController::setFrameworkProbeMolecule(int value)
{
  if(value>=0 && value<int(ProbeMolecule::multiple_values))
  {
    for(std::shared_ptr<Structure> structure: _structures)
    {
      structure->setFrameworkProbeMolecule(ProbeMolecule(value));
      structure->recheckRepresentationStyle();
    }

    emit computeNitrogenSurfaceArea(std::vector<std::shared_ptr<RKRenderStructure>>{_structures.begin(), _structures.end()});
    this->reloadStructureProperties();

    reloadFrameworkProbeMoleculePopupBox();
  }
}

stdx::optional<ProbeMolecule> CellTreeWidgetController::frameworkProbeMolecule()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<ProbeMolecule, enum_hash> set = std::unordered_set<ProbeMolecule, enum_hash>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    ProbeMolecule value = structure->frameworkProbeMolecule();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

stdx::optional<double> CellTreeWidgetController::structureVolumetricNitrogenSurfaceArea()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->structureVolumetricNitrogenSurfaceArea();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

stdx::optional<double> CellTreeWidgetController::structureGravimetricNitrogenSurfaceArea()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->structureGravimetricNitrogenSurfaceArea();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

stdx::optional<int> CellTreeWidgetController::structureNumberOfChannelSystems()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<int> set = std::unordered_set<int>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->structureNumberOfChannelSystems();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void CellTreeWidgetController::setStructureNumberOfChannelSystems(int value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setStructureNumberOfChannelSystems(value);
  }
}

stdx::optional<int> CellTreeWidgetController::structureNumberOfInaccessiblePockets()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<int> set = std::unordered_set<int>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    int value = structure->structureNumberOfInaccessiblePockets();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void CellTreeWidgetController::setStructureNumberOfInaccessiblePockets(int value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setStructureNumberOfInaccessiblePockets(value);
  }
}

stdx::optional<int> CellTreeWidgetController::structureDimensionalityOfPoreSystem()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<int> set = std::unordered_set<int>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    int value = structure->structureDimensionalityOfPoreSystem();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void CellTreeWidgetController::setStructureDimensionalityOfPoreSystem(int value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setStructureDimensionalityOfPoreSystem(value);
  }
}

stdx::optional<double> CellTreeWidgetController::structureLargestCavityDiameter()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->structureLargestCavityDiameter();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void CellTreeWidgetController::setStructureLargestCavityDiameter(double value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setStructureLargestCavityDiameter(value);
  }
}

stdx::optional<double> CellTreeWidgetController::structureRestrictingPoreLimitingDiameter()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->structureRestrictingPoreLimitingDiameter();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void CellTreeWidgetController::setStructureRestrictingPoreLimitingDiameter(double value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setStructureRestrictingPoreLimitingDiameter(value);
  }
}

stdx::optional<double> CellTreeWidgetController::structureLargestCavityDiameterAlongAviablePath()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->structureLargestCavityDiameterAlongAviablePath();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void CellTreeWidgetController::setStructureLargestCavityDiameterAlongAviablePath(double value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setStructureLargestCavityDiameterAlongAviablePath(value);
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
  if(!_structures.empty())
  {
    if (stdx::optional<int> type = symmetrySpaceGroupHallNumber())
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
  if(!_structures.empty())
  {
    if (stdx::optional<int> type = symmetrySpaceGroupStamdardNumber())
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
  if(!_structures.empty())
  {
    if (stdx::optional<QString> string = symmetryHolohedryString())
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
  if(!_structures.empty())
  {
    if (stdx::optional<QString> string = symmetryQualifierString())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = symmetryPrecision())
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
  if(!_structures.empty())
  {
    if (stdx::optional<QString> value = symmetryCenteringString())
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
  if(!_structures.empty())
  {
    if (stdx::optional<int> value = symmetrySpaceGroupHallNumber())
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
  if(!_structures.empty())
  {
    if (stdx::optional<bool> type = symmetryInversion())
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
  if(!_structures.empty())
  {
    if (stdx::optional<QString> value = symmetryInversionCenterString())
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
  if(!_structures.empty())
  {
    if (stdx::optional<bool> type = symmetryCentrosymmetric())
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
  if(!_structures.empty())
  {
    if (stdx::optional<bool> type = symmetryEnatiomorphic())
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
  if(!_structures.empty())
  {
    if (stdx::optional<int> type = symmetryPointGroup())
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
  if(!_structures.empty())
  {
    if (stdx::optional<int> type = symmetryPointGroup())
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
  if(!_structures.empty())
  {
    if (stdx::optional<int> type = symmetryPointGroup())
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
  if(!_structures.empty())
  {
    if (stdx::optional<QString> string = symmetrySymmorphicity())
    {
      whileBlocking(_cellSymmetryForm->symmorphicityLineEdit)->setText(*string);
    }
    else
    {
      whileBlocking(_cellSymmetryForm->symmorphicityLineEdit)->setText("Multiple values");
    }
  }
}

stdx::optional<int> CellTreeWidgetController::symmetrySpaceGroupHallNumber()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<int> set = std::unordered_set<int>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    int value = structure->spaceGroup().spaceGroupSetting().HallNumber();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void CellTreeWidgetController::setSymmetrySpaceGroupHallNumber(int value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setSpaceGroupHallNumber(value);
  }

  emit invalidateCachedAmbientOcclusionTexture(std::vector<std::shared_ptr<RKRenderStructure>>{_structures.begin(), _structures.end()});
  emit rendererReloadData();

  reloadSymmetryProperties();
}

stdx::optional<int> CellTreeWidgetController::symmetrySpaceGroupStamdardNumber()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<int> set = std::unordered_set<int>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    int value = structure->spaceGroup().spaceGroupSetting().number();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void CellTreeWidgetController::setSymmetrySpaceGroupStandardNumber(int value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    int HallNumber = SKSpaceGroup::spaceGroupHallData[value].front();
    structure->setSpaceGroupHallNumber(HallNumber);
  }

  emit invalidateCachedAmbientOcclusionTexture(std::vector<std::shared_ptr<RKRenderStructure>>{_structures.begin(), _structures.end()});
  emit rendererReloadData();

  reloadSymmetryProperties();
}

stdx::optional<QString> CellTreeWidgetController::symmetryHolohedryString()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::set<QString> set = std::set<QString>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    int pointGroup = structure->spaceGroup().spaceGroupSetting().pointGroup();
    QString value = SKPointGroup::pointGroupData[pointGroup].holohedryString();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

stdx::optional<QString> CellTreeWidgetController::symmetryQualifierString()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::set<QString> set = std::set<QString>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    QString value = structure->spaceGroup().spaceGroupSetting().qualifier();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

stdx::optional<double> CellTreeWidgetController::symmetryPrecision()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->cell()->precision();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void CellTreeWidgetController::setSymmetryPrecision(double value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->cell()->setPrecision(value);
  }
}

stdx::optional<QString> CellTreeWidgetController::symmetryCenteringString()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::set<QString> set = std::set<QString>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    QString value = structure->spaceGroup().spaceGroupSetting().centringString();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

stdx::optional<bool> CellTreeWidgetController::symmetryInversion()
{
    if(_structures.empty())
    {
      return stdx::nullopt;
    }
    std::unordered_set<bool> set = std::unordered_set<bool>{};
    for(std::shared_ptr<Structure> structure: _structures)
    {
      bool inversion = structure->spaceGroup().spaceGroupSetting().inversionAtOrigin();
      set.insert(inversion);
    }

    if(set.size() == 1)
    {
      return *set.begin();
    }
    return stdx::nullopt;
}


stdx::optional<QString> CellTreeWidgetController::symmetryInversionCenterString()
{
    if(_structures.empty())
    {
      return stdx::nullopt;
    }
    std::set<QString> set = std::set<QString>{};
    for(std::shared_ptr<Structure> structure: _structures)
    {
      int HallNumber = structure->spaceGroup().spaceGroupSetting().HallNumber();
      QString string = SKSpaceGroup::inversionCenterString(HallNumber);
      set.insert(string);
    }

    if(set.size() == 1)
    {
      return *set.begin();
    }
    return stdx::nullopt;
}

stdx::optional<bool> CellTreeWidgetController::symmetryCentrosymmetric()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<bool> set = std::unordered_set<bool>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    int pointGroup = structure->spaceGroup().spaceGroupSetting().pointGroup();
    bool centroSymmetric = SKPointGroup::pointGroupData[pointGroup].centrosymmetric();
    set.insert(pointGroup);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

stdx::optional<bool> CellTreeWidgetController::symmetryEnatiomorphic()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<bool> set = std::unordered_set<bool>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    int pointGroup = structure->spaceGroup().spaceGroupSetting().pointGroup();
    bool centroSymmetric = SKPointGroup::pointGroupData[pointGroup].enantiomorphic();
    set.insert(pointGroup);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}


stdx::optional<int> CellTreeWidgetController::symmetryPointGroup()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<int> set = std::unordered_set<int>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    int pointGroup = structure->spaceGroup().spaceGroupSetting().pointGroup();
    set.insert(pointGroup);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}


stdx::optional<QString> CellTreeWidgetController::symmetrySymmorphicity()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::set<QString> set = std::set<QString>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    QString symmorphicity = structure->spaceGroup().spaceGroupSetting().symmorphicityString();
    set.insert(symmorphicity);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void CellTreeWidgetController::computeHeliumVoidFractionPushButton()
{
  emit computeHeliumVoidFraction(std::vector<std::shared_ptr<RKRenderStructure>>{_structures.begin(), _structures.end()});
  this->reloadStructureProperties();
}

void CellTreeWidgetController::computeGravimetricSurfaceAreaPushButton()
{
  emit computeNitrogenSurfaceArea(std::vector<std::shared_ptr<RKRenderStructure>>{_structures.begin(), _structures.end()});
  this->reloadStructureProperties();
}

