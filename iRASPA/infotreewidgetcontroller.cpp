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

#include "infotreewidgetcontroller.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <set>
#include <QLabel>
#include <QComboBox>
#include <QHash>
#include <mathkit.h>
#include <renderkit.h>
#include <foundationkit.h>
#include "textfield.h"

InfoTreeWidgetController::InfoTreeWidgetController(QWidget* parent): QTreeWidget(parent),
    _infoCreatorForm(new InfoCreatorForm),
    _infoCreationForm(new InfoCreationForm),
    _infoChemicalForm(new InfoChemicalForm),
    _infoCitationForm(new InfoCitationForm)
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



  // Creator
  //==========================================================================================================
  QTreeWidgetItem* creatorItem = new QTreeWidgetItem(this);
  this->addTopLevelItem(creatorItem);

  pushButtonCreator = new QPushButton(QString("Creator"),this);
  pushButtonCreator->setIcon(QIcon(":/iRASPA/collapsed.png"));
  pushButtonCreator->setStyleSheet("text-align:left;");
  setItemWidget(creatorItem,0,pushButtonCreator);

  QTreeWidgetItem *childCreatorItem = new QTreeWidgetItem(creatorItem);
  this->setItemWidget(childCreatorItem,0, _infoCreatorForm);

  QObject::connect(pushButtonCreator, &QPushButton::clicked, this, &InfoTreeWidgetController::expandCreatorItem);

  pushButtonCreator->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
  QFontMetrics fm(font());
  pushButtonCreator->resize(size().width(), fm.height());

  // Creation
  //==========================================================================================================
  QTreeWidgetItem* creationItem = new QTreeWidgetItem(this);
  this->addTopLevelItem(creationItem);

  pushButtonCreation = new QPushButton(QString("Creation"),this);
  pushButtonCreation->setIcon(QIcon(":/iRASPA/collapsed.png"));
  pushButtonCreation->setStyleSheet("text-align:left;");
  setItemWidget(creationItem,0,pushButtonCreation);

  QTreeWidgetItem *childCreationItem = new QTreeWidgetItem(creationItem);
  this->setItemWidget(childCreationItem,0, _infoCreationForm);

  QObject::connect(pushButtonCreation, &QPushButton::clicked, this, &InfoTreeWidgetController::expandCreationItem);

  QObject::connect(_infoCreationForm->methodComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [this](int index) {
      switch(index)
      {
      case 0:
      case 1:
        _infoCreationForm->stackedWidget->setCurrentIndex(0);
        break;
      case 2:
        _infoCreationForm->stackedWidget->setCurrentIndex(1);
        break;
      }
  });

  pushButtonCreation->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
  pushButtonCreation->resize(size().width(), fm.height());

  _infoCreationForm->temperatureComboBox->insertItem(0,"Kelvin");
  _infoCreationForm->temperatureComboBox->insertItem(1,"Celsius");

  _infoCreationForm->pressureComboBox->insertItem(0,"Pascal");
  _infoCreationForm->pressureComboBox->insertItem(1,"Bar");

  _infoCreationForm->methodComboBox->insertItem(0,"Unknown");
  _infoCreationForm->methodComboBox->insertItem(1,"Simulation");
  _infoCreationForm->methodComboBox->insertItem(2,"Experiment");


  _infoCreationForm->relaxUnitCellComboBox->insertItem(0,"Unknown");
  _infoCreationForm->relaxUnitCellComboBox->insertItem(1,"Cell and edge-lengths free");
  _infoCreationForm->relaxUnitCellComboBox->insertItem(2,"Fixed angles; isotropic edge-length free");
  _infoCreationForm->relaxUnitCellComboBox->insertItem(3,"fixed angles; anisotropic edge-lengths free");
  _infoCreationForm->relaxUnitCellComboBox->insertItem(4,"Fixed β; α and γ-angles and edge-lengths free");
  _infoCreationForm->relaxUnitCellComboBox->insertItem(5,"Fixed volume; shape free");
  _infoCreationForm->relaxUnitCellComboBox->insertItem(6,"Fixed cell");

  _infoCreationForm->positionsSoftwarePackageComboBox->insertItem(0,"Unknown");
  _infoCreationForm->positionsSoftwarePackageComboBox->insertItem(1,"Experimental");
  _infoCreationForm->positionsSoftwarePackageComboBox->insertItem(2,"Gaussian");
  _infoCreationForm->positionsSoftwarePackageComboBox->insertItem(3,"Turbomol");
  _infoCreationForm->positionsSoftwarePackageComboBox->insertItem(4,"VASP");
  _infoCreationForm->positionsSoftwarePackageComboBox->insertItem(5,"Spartan");
  _infoCreationForm->positionsSoftwarePackageComboBox->insertItem(6,"CP2K");
  _infoCreationForm->positionsSoftwarePackageComboBox->insertItem(7,"Classical");

  _infoCreationForm->positionsAlgorithmComboBox->insertItem(0,"Unknown");
  _infoCreationForm->positionsAlgorithmComboBox->insertItem(1,"None");
  _infoCreationForm->positionsAlgorithmComboBox->insertItem(2,"Simplex");
  _infoCreationForm->positionsAlgorithmComboBox->insertItem(3,"Simulated annealing");
  _infoCreationForm->positionsAlgorithmComboBox->insertItem(4,"Gentic algorithm");
  _infoCreationForm->positionsAlgorithmComboBox->insertItem(5,"Steepest descent");
  _infoCreationForm->positionsAlgorithmComboBox->insertItem(6,"Conjugate gradient");
  _infoCreationForm->positionsAlgorithmComboBox->insertItem(7,"Quasi-Newton");
  _infoCreationForm->positionsAlgorithmComboBox->insertItem(8,"Newton-Rhapson");
  _infoCreationForm->positionsAlgorithmComboBox->insertItem(9,"Mode following");


  _infoCreationForm->positionsEigenvaluesComboBox->insertItem(0,"Unknown");
  _infoCreationForm->positionsEigenvaluesComboBox->insertItem(1,"None");
  _infoCreationForm->positionsEigenvaluesComboBox->insertItem(2,"All positive");
  _infoCreationForm->positionsEigenvaluesComboBox->insertItem(3,"First order sadle-point");
  _infoCreationForm->positionsEigenvaluesComboBox->insertItem(4,"Second order sadle-point");
  _infoCreationForm->positionsEigenvaluesComboBox->insertItem(5,"Some small negative values");
  _infoCreationForm->positionsEigenvaluesComboBox->insertItem(6,"Some significant negative values");
  _infoCreationForm->positionsEigenvaluesComboBox->insertItem(7,"Many negative eigenvalues");


  _infoCreationForm->positionsForceFieldComboBox->insertItem(0,"Unknown");
  _infoCreationForm->positionsForceFieldComboBox->insertItem(1,"Ab initio");
  _infoCreationForm->positionsForceFieldComboBox->insertItem(2,"UFF");
  _infoCreationForm->positionsForceFieldComboBox->insertItem(3,"DREIDING");
  _infoCreationForm->positionsForceFieldComboBox->insertItem(4,"MM3");
  _infoCreationForm->positionsForceFieldComboBox->insertItem(5,"MM4");


  _infoCreationForm->chargeSoftwarePackageComboBox->insertItem(0,"Unknown");
  _infoCreationForm->chargeSoftwarePackageComboBox->insertItem(1,"Experimental");
  _infoCreationForm->chargeSoftwarePackageComboBox->insertItem(2,"Gaussian");
  _infoCreationForm->chargeSoftwarePackageComboBox->insertItem(3,"Turbomol");
  _infoCreationForm->chargeSoftwarePackageComboBox->insertItem(4,"VASP");
  _infoCreationForm->chargeSoftwarePackageComboBox->insertItem(5,"Spartan");
  _infoCreationForm->chargeSoftwarePackageComboBox->insertItem(6,"CP2K");
  _infoCreationForm->chargeSoftwarePackageComboBox->insertItem(7,"Classical");

  _infoCreationForm->chargeAlgorithmComboBox->insertItem(0,"Unknown");
  _infoCreationForm->chargeAlgorithmComboBox->insertItem(1,"REPEAT");
  _infoCreationForm->chargeAlgorithmComboBox->insertItem(2,"CHELPG");
  _infoCreationForm->chargeAlgorithmComboBox->insertItem(3,"CHELP");
  _infoCreationForm->chargeAlgorithmComboBox->insertItem(4,"RESP");
  _infoCreationForm->chargeAlgorithmComboBox->insertItem(5,"Mulliken");
  _infoCreationForm->chargeAlgorithmComboBox->insertItem(6,"Merz-Kollman");
  _infoCreationForm->chargeAlgorithmComboBox->insertItem(7,"Hirshfeld");
  _infoCreationForm->chargeAlgorithmComboBox->insertItem(8,"Mulliken");
  _infoCreationForm->chargeAlgorithmComboBox->insertItem(9,"Natural Bond Orbital");
  _infoCreationForm->chargeAlgorithmComboBox->insertItem(10,"Qeq - Rappe and Goddard 1991");
  _infoCreationForm->chargeAlgorithmComboBox->insertItem(11,"Qeq - Rick, Stuart abd Berne 1994");
  _infoCreationForm->chargeAlgorithmComboBox->insertItem(12,"Qeq - York and Yang 1996");
  _infoCreationForm->chargeAlgorithmComboBox->insertItem(13,"Qeq - Itskoqitz and Berkowitz 1997");
  _infoCreationForm->chargeAlgorithmComboBox->insertItem(14,"Qeq - Wilmer, Kim, Snurr 2012");

  _infoCreationForm->chargeForceFieldComboBox->insertItem(0,"Unknown");
  _infoCreationForm->chargeForceFieldComboBox->insertItem(1,"Ab initio");
  _infoCreationForm->chargeForceFieldComboBox->insertItem(2,"UFF");
  _infoCreationForm->chargeForceFieldComboBox->insertItem(3,"DREIDING");
  _infoCreationForm->chargeForceFieldComboBox->insertItem(4,"MM3");
  _infoCreationForm->chargeForceFieldComboBox->insertItem(5,"MM4");

  // Chemical
  //==========================================================================================================
  QTreeWidgetItem* chemicalItem = new QTreeWidgetItem(this);
  this->addTopLevelItem(chemicalItem);

  pushButtonChemical = new QPushButton(QString("Chemical"),this);
  pushButtonChemical->setIcon(QIcon(":/iRASPA/collapsed.png"));
  pushButtonChemical->setStyleSheet("text-align:left;");
  setItemWidget(chemicalItem,0,pushButtonChemical);

  QTreeWidgetItem *childChemicalItem = new QTreeWidgetItem(chemicalItem);
  this->setItemWidget(childChemicalItem,0, _infoChemicalForm);

  QObject::connect(pushButtonChemical, &QPushButton::clicked, this, &InfoTreeWidgetController::expandChemicalItem);

  pushButtonChemical->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
  pushButtonChemical->resize(size().width(), fm.height());

  // Citation
  //==========================================================================================================
  QTreeWidgetItem* citationItem = new QTreeWidgetItem(this);
  this->addTopLevelItem(citationItem);

  pushButtonCitation = new QPushButton(QString("Citation"),this);
  pushButtonCitation->setIcon(QIcon(":/iRASPA/collapsed.png"));
  pushButtonCitation->setStyleSheet("text-align:left;");
  setItemWidget(citationItem,0,pushButtonCitation);

  QTreeWidgetItem *childCitationItem = new QTreeWidgetItem(citationItem);
  this->setItemWidget(childCitationItem,0, _infoCitationForm);

  QObject::connect(pushButtonCitation, &QPushButton::clicked, this, &InfoTreeWidgetController::expandCitationItem);

  pushButtonCitation->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
  pushButtonCitation->resize(size().width(), fm.height());
}

void InfoTreeWidgetController::setProject(std::shared_ptr<ProjectTreeNode> projectTreeNode)
{
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
          _structures = projectStructure->flattenedStructures();
          reloadData();
        }
      }
    }
  }

}

void InfoTreeWidgetController::setStructures(std::vector<std::shared_ptr<Structure>> structures)
{
  std::cout << "setStructures : " << structures.size() << std::endl;
  _structures = structures;
  reloadData();
}

void InfoTreeWidgetController::expandCreatorItem()
{
  QModelIndex index = indexFromItem(topLevelItem(0),0);
  if(this->isExpanded(index))
  {
    this->collapse(index);
    pushButtonCreator->setIcon(QIcon(":/iRASPA/collapsed.png"));
  }
  else
  {
    this->expand(index);
    pushButtonCreator->setIcon(QIcon(":/iRASPA/expanded.png"));
  }
}

void InfoTreeWidgetController::expandCreationItem()
{
  QModelIndex index = indexFromItem(topLevelItem(1),0);
  if(this->isExpanded(index))
  {
    this->collapse(index);
    pushButtonCreation->setIcon(QIcon(":/iRASPA/collapsed.png"));
  }
  else
  {
    this->expand(index);
    pushButtonCreation->setIcon(QIcon(":/iRASPA/expanded.png"));
    reloadCreationData();
    update();
  }
}

void InfoTreeWidgetController::expandChemicalItem()
{
  QModelIndex index = indexFromItem(topLevelItem(2),0);
  if(this->isExpanded(index))
  {
    this->collapse(index);
    pushButtonChemical->setIcon(QIcon(":/iRASPA/collapsed.png"));
  }
  else
  {
    this->expand(index);
    pushButtonChemical->setIcon(QIcon(":/iRASPA/expanded.png"));
  }
}

void InfoTreeWidgetController::expandCitationItem()
{
  QModelIndex index = indexFromItem(topLevelItem(3),0);
  if(this->isExpanded(index))
  {
    this->collapse(index);
    pushButtonCitation->setIcon(QIcon(":/iRASPA/collapsed.png"));
  }
  else
  {
    this->expand(index);
    pushButtonCitation->setIcon(QIcon(":/iRASPA/expanded.png"));
  }
}

void InfoTreeWidgetController::reloadData()
{
  reloadCreatorData();
  reloadCreationData();
  reloadChemicalData();
  reloadCitationData();

  update();
}

void InfoTreeWidgetController::reloadCreatorData()
{
  reloadAuthorFirstName();
  reloadAuthorMiddleName();
  reloadAuthorLastName();
  reloadAuthorOrchidID();
  reloadAuthorResearcherID();
  reloadAuthorAffiliationUniversityName();
  reloadAuthorAffiliationFacultyName();
  reloadAuthorAffiliationInstituteName();
  reloadAuthorAffiliationCityName();
  reloadAuthorAffiliationCountryName();
}

void InfoTreeWidgetController::reloadCreationData()
{
  std::cout << "reloadCreationData" << std::endl;
  reloadCreationDate();
  reloadCreationTemperature();
  reloadCreationPressure();
  reloadCreationMethod();
  reloadCreationRelaxUnitCell();
  reloadCreationAtomicPositionsSoftwarePackage();
  reloadCreationAtomicPositionsAlgorithm();
  reloadCreationAtomicPositionsEigenvalues();
  reloadCreationAtomicPositionsForceField();
  reloadCreationAtomicPositionsForceFieldDetails();
  reloadCreationAtomicChargesSoftwarePackage();
  reloadCreationAtomicChargesAlgorithm();
  reloadCreationAtomicChargesEigenvalues();
  reloadCreationAtomicChargesForceField();
  reloadCreationAtomicChargesForceFieldDetails();
}

void InfoTreeWidgetController::reloadChemicalData()
{
  reloadChemicalFormulaMoiety();
  reloadChemicalFormulaSum();
  reloadChemicalNameSystematic();
}

void InfoTreeWidgetController::reloadCitationData()
{
  reloadCitationArticleTitle();
  reloadCitationArticleAuthors();
  reloadCitationJournalVolume();
  reloadCitationJournalNumber();
  reloadCitationPublicationDate();
  reloadCitationPublicationDOI();
  reloadCitationPublicationDatabaseCodes();
}

// reload Creator properties
//========================================================================================================================================


void InfoTreeWidgetController::reloadAuthorFirstName()
{
  if(!_structures.empty())
  {
    if ( std::optional<QString> value = authorFirstName())
    {
      whileBlocking(_infoCreatorForm->firstNameLineEdit)->setText(*value);
    }
    else
    {
      whileBlocking(_infoCreatorForm->firstNameLineEdit)->setText("Mult. Val.");
    }
  }
}

void InfoTreeWidgetController::reloadAuthorMiddleName()
{
  if(!_structures.empty())
  {
    if ( std::optional<QString> value = authorMiddleName())
    {
      whileBlocking(_infoCreatorForm->middleNameLineEdit)->setText(*value);
    }
    else
    {
      whileBlocking(_infoCreatorForm->middleNameLineEdit)->setText("Mult. Val.");
    }
  }
}

void InfoTreeWidgetController::reloadAuthorLastName()
{
  if(!_structures.empty())
  {
    if ( std::optional<QString> value = authorLastName())
    {
      whileBlocking(_infoCreatorForm->lastNameLineEdit)->setText(*value);
    }
    else
    {
      whileBlocking(_infoCreatorForm->lastNameLineEdit)->setText("Mult. Val.");
    }
  }
}

void InfoTreeWidgetController::reloadAuthorOrchidID()
{
  if(!_structures.empty())
  {
    if ( std::optional<QString> value = authorOrchidID())
    {
      whileBlocking(_infoCreatorForm->orchidLineEdit)->setText(*value);
    }
    else
    {
      whileBlocking(_infoCreatorForm->orchidLineEdit)->setText("Mult. Val.");
    }
  }
}

void InfoTreeWidgetController::reloadAuthorResearcherID()
{
  if(!_structures.empty())
  {
    if ( std::optional<QString> value = authorResearcherID())
    {
      whileBlocking(_infoCreatorForm->researcherIDLineEdit)->setText(*value);
    }
    else
    {
      whileBlocking(_infoCreatorForm->researcherIDLineEdit)->setText("Mult. Val.");
    }
  }
}

void InfoTreeWidgetController::reloadAuthorAffiliationUniversityName()
{
  if(!_structures.empty())
  {
    if ( std::optional<QString> value = authorAffiliationUniversityName())
    {
      whileBlocking(_infoCreatorForm->universityNameLineEdit)->setText(*value);
    }
    else
    {
      whileBlocking(_infoCreatorForm->universityNameLineEdit)->setText("Mult. Val.");
    }
  }
}

void InfoTreeWidgetController::reloadAuthorAffiliationFacultyName()
{
  if(!_structures.empty())
  {
    if ( std::optional<QString> value = authorAffiliationFacultyName())
    {
      whileBlocking(_infoCreatorForm->facultyNameLineEdit)->setText(*value);
    }
    else
    {
      whileBlocking(_infoCreatorForm->facultyNameLineEdit)->setText("Mult. Val.");
    }
  }
}

void InfoTreeWidgetController::reloadAuthorAffiliationInstituteName()
{
  if(!_structures.empty())
  {
    if ( std::optional<QString> value = authorAffiliationInstituteName())
    {
      whileBlocking(_infoCreatorForm->instituteNameLineEdit)->setText(*value);
    }
    else
    {
      whileBlocking(_infoCreatorForm->instituteNameLineEdit)->setText("Mult. Val.");
    }
  }
}

void InfoTreeWidgetController::reloadAuthorAffiliationCityName()
{
  if(!_structures.empty())
  {
    if ( std::optional<QString> value = authorAffiliationCityName())
    {
      whileBlocking(_infoCreatorForm->cityNameLineEdit)->setText(*value);
    }
    else
    {
      whileBlocking(_infoCreatorForm->cityNameLineEdit)->setText("Mult. Val.");
    }
  }
}

void InfoTreeWidgetController::reloadAuthorAffiliationCountryName()
{
  if(!_structures.empty())
  {

  }
}

 std::optional<QString> InfoTreeWidgetController::authorFirstName()
{
  if(_structures.empty())
  {
    return std::nullopt;
  }
  std::set<QString> set = std::set<QString>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    QString value = structure->authorFirstName();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void InfoTreeWidgetController::setAuthorFirstName(QString name)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setAuthorFirstName(name);
  }
}

 std::optional<QString> InfoTreeWidgetController::authorMiddleName()
{
  if(_structures.empty())
  {
    return std::nullopt;
  }
  std::set<QString> set = std::set<QString>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    QString value = structure->authorMiddleName();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void InfoTreeWidgetController::setAuthorMiddleName(QString name)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setAuthorMiddleName(name);
  }
}

 std::optional<QString> InfoTreeWidgetController::authorLastName()
{
  if(_structures.empty())
  {
    return std::nullopt;
  }
  std::set<QString> set = std::set<QString>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    QString value = structure->authorLastName();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void InfoTreeWidgetController::setAuthorLastName(QString name)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setAuthorLastName(name);
  }
}

 std::optional<QString> InfoTreeWidgetController::authorOrchidID()
{
  if(_structures.empty())
  {
    return std::nullopt;
  }
  std::set<QString> set = std::set<QString>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    QString value = structure->authorOrchidID();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void InfoTreeWidgetController::setAuthorOrchidID(QString name)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setAuthorOrchidID(name);
  }
}

 std::optional<QString> InfoTreeWidgetController::authorResearcherID()
{
  if(_structures.empty())
  {
    return std::nullopt;
  }
  std::set<QString> set = std::set<QString>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    QString value = structure->authorResearcherID();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void InfoTreeWidgetController::setAuthorResearcherID(QString name)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setAuthorResearcherID(name);
  }
}

 std::optional<QString> InfoTreeWidgetController::authorAffiliationUniversityName()
{
  if(_structures.empty())
  {
    return std::nullopt;
  }
  std::set<QString> set = std::set<QString>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    QString value = structure->authorAffiliationUniversityName();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void InfoTreeWidgetController::setAuthorAffiliationUniversityName(QString name)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setAuthorAffiliationUniversityName(name);
  }
}

 std::optional<QString> InfoTreeWidgetController::authorAffiliationFacultyName()
{
  if(_structures.empty())
  {
    return std::nullopt;
  }
  std::set<QString> set = std::set<QString>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    QString value = structure->authorAffiliationFacultyName();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void InfoTreeWidgetController::setAuthorAffiliationFacultyName(QString name)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setAuthorAffiliationFacultyName(name);
  }
}

 std::optional<QString> InfoTreeWidgetController::authorAffiliationInstituteName()
{
  if(_structures.empty())
  {
    return std::nullopt;
  }
  std::set<QString> set = std::set<QString>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    QString value = structure->authorAffiliationInstituteName();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void InfoTreeWidgetController::setAuthorAffiliationInstituteName(QString name)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setAuthorAffiliationInstituteName(name);
  }
}

 std::optional<QString> InfoTreeWidgetController::authorAffiliationCityName()
{
  if(_structures.empty())
  {
    return std::nullopt;
  }
  std::set<QString> set = std::set<QString>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    QString value = structure->authorAffiliationCityName();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void InfoTreeWidgetController::setAuthorAffiliationCityName(QString name)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setAuthorAffiliationCityName(name);
  }
}

// reload Creation properties
//========================================================================================================================================

void InfoTreeWidgetController::reloadCreationDate()
{
  if(!_structures.empty())
  {
    if ( std::optional<QDate> value = creationDate())
    {
      whileBlocking(_infoCreationForm->creationDateEdit)->setDate(*value);
    }
    else
    {
      whileBlocking(_infoCreationForm->creationDateEdit)->setDate(QDate());
    }
  }
}

void InfoTreeWidgetController::reloadCreationTemperature()
{
  if(!_structures.empty())
  {
    if ( std::optional<QString> value = creationTemperature())
    {
       whileBlocking(_infoCreationForm->temperatureLineEdit)->setText(*value);
    }
    else
    {
      std::cout << "Setting text temperature to: Mult. Val." << std::endl;
      whileBlocking(_infoCreationForm->temperatureLineEdit)->setText("Mult. Val.");
    }
  }
}

void InfoTreeWidgetController::reloadCreationPressure()
{
  if(!_structures.empty())
  {
    if ( std::optional<QString> value = creationPressure())
    {
       whileBlocking(_infoCreationForm->pressureLineEdit)->setText(*value);
    }
    else
    {
      whileBlocking(_infoCreationForm->pressureLineEdit)->setText("Mult. Val.");
    }
  }
}

void InfoTreeWidgetController::reloadCreationMethod()
{
  if(!_structures.empty())
  {

  }
}

void InfoTreeWidgetController::reloadCreationRelaxUnitCell()
{
  if(!_structures.empty())
  {

  }
}

void InfoTreeWidgetController::reloadCreationAtomicPositionsSoftwarePackage()
{
  if(!_structures.empty())
  {

  }
}

void InfoTreeWidgetController::reloadCreationAtomicPositionsAlgorithm()
{
  if(!_structures.empty())
  {

  }
}

void InfoTreeWidgetController::reloadCreationAtomicPositionsEigenvalues()
{
  if(!_structures.empty())
  {

  }
}

void InfoTreeWidgetController::reloadCreationAtomicPositionsForceField()
{
  if(!_structures.empty())
  {

  }
}

void InfoTreeWidgetController::reloadCreationAtomicPositionsForceFieldDetails()
{
  if(!_structures.empty())
  {

  }
}

void InfoTreeWidgetController::reloadCreationAtomicChargesSoftwarePackage()
{
  if(!_structures.empty())
  {

  }
}

void InfoTreeWidgetController::reloadCreationAtomicChargesAlgorithm()
{
  if(!_structures.empty())
  {

  }
}

void InfoTreeWidgetController::reloadCreationAtomicChargesEigenvalues()
{
  if(!_structures.empty())
  {

  }
}

void InfoTreeWidgetController::reloadCreationAtomicChargesForceField()
{
  if(!_structures.empty())
  {

  }
}

void InfoTreeWidgetController::reloadCreationAtomicChargesForceFieldDetails()
{
  if(!_structures.empty())
  {

  }
}

std::optional<QDate> InfoTreeWidgetController::creationDate()
{
  return std::nullopt;
}

void InfoTreeWidgetController::setCreationDate(QDate date)
{

}

 std::optional<QString> InfoTreeWidgetController::creationTemperature()
{
  if(_structures.empty())
  {
    return std::nullopt;
  }
  std::set<QString> set = std::set<QString>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    QString value = structure->creationTemperature();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void InfoTreeWidgetController::setCreationTemperature(QString name)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setCreationTemperature(name);
  }
}

 std::optional<QString> InfoTreeWidgetController::creationPressure()
{
  if(_structures.empty())
  {
    return std::nullopt;
  }
  std::set<QString> set = std::set<QString>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    QString value = structure->creationPressure();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void InfoTreeWidgetController::setCreationPressure(QString name)
{

}

 std::optional<QString> InfoTreeWidgetController::creationMethod()
{
 return std::nullopt;
}

void InfoTreeWidgetController::setCreationMethod(QString name)
{

}

 std::optional<QString> InfoTreeWidgetController::creationRelaxUnitCell()
{
 return std::nullopt;
}

void InfoTreeWidgetController::setCreationRelaxUnitCell(QString name)
{

}

 std::optional<QString> InfoTreeWidgetController::creationAtomicPositionsSoftwarePackage()
{
 return std::nullopt;
}

void InfoTreeWidgetController::setCreationAtomicPositionsSoftwarePackage(QString name)
{

}

 std::optional<QString> InfoTreeWidgetController::creationAtomicPositionsAlgorithm()
{
 return std::nullopt;
}

void InfoTreeWidgetController::setCreationAtomicPositionsAlgorithm(QString name)
{

}

 std::optional<QString> InfoTreeWidgetController::creationAtomicPositionsEigenvalues()
{
 return std::nullopt;
}

void InfoTreeWidgetController::setCreationAtomicPositionsEigenvalues(QString name)
{

}

 std::optional<QString> InfoTreeWidgetController::creationAtomicPositionsForceField()
{
 return std::nullopt;
}

void InfoTreeWidgetController::setCreationAtomicPositionsForceField(QString name)
{

}

 std::optional<QString> InfoTreeWidgetController::creationAtomicPositionsForceFieldDetails()
{
 return std::nullopt;
}

void InfoTreeWidgetController::setCreationAtomicPositionsForceFieldDetails(QString name)
{

}

 std::optional<QString> InfoTreeWidgetController::creationAtomicChargesSoftwarePackage()
{
 return std::nullopt;
}

void InfoTreeWidgetController::setCreationAtomicChargesSoftwarePackage(QString name)
{

}

 std::optional<QString> InfoTreeWidgetController::creationAtomicChargesAlgorithm()
{
 return std::nullopt;
}

void InfoTreeWidgetController::setCreationAtomicChargesAlgorithm(QString name)
{

}

 std::optional<QString> InfoTreeWidgetController::creationAtomicChargesEigenvalues()
{
 return std::nullopt;
}

void InfoTreeWidgetController::setCreationAtomicChargesEigenvalues(QString name)
{

}

 std::optional<QString> InfoTreeWidgetController::creationAtomicChargesForceField()
{
 return std::nullopt;
}

void InfoTreeWidgetController::setCreationAtomicChargesForceField(QString name)
{

}

 std::optional<QString> InfoTreeWidgetController::creationAtomicChargesForceFieldDetails()
{
 return std::nullopt;
}

void InfoTreeWidgetController::setCreationAtomicChargesForceFieldDetails(QString name)
{

}

// reload Chemical properties
//========================================================================================================================================

void InfoTreeWidgetController::reloadChemicalFormulaMoiety()
{
  if(!_structures.empty())
  {
    if ( std::optional<QString> value = chemicalFormulaMoiety())
    {
      whileBlocking(_infoChemicalForm->moietyLineEdit)->setText(*value);
    }
    else
    {
      whileBlocking(_infoChemicalForm->moietyLineEdit)->setText("Mult. Val.");
    }
  }
}

void InfoTreeWidgetController::reloadChemicalFormulaSum()
{
  if(!_structures.empty())
  {
    if ( std::optional<QString> value = chemicalFormulaSum())
    {
      whileBlocking(_infoChemicalForm->sumLineEdit)->setText(*value);
    }
    else
    {
      whileBlocking(_infoChemicalForm->sumLineEdit)->setText("Mult. Val.");
    }
  }
}

void InfoTreeWidgetController::reloadChemicalNameSystematic()
{
  if(!_structures.empty())
  {
    if ( std::optional<QString> value = chemicalNameSystematic())
    {
      whileBlocking(_infoChemicalForm->sytematicLineEdit)->setText(*value);
    }
    else
    {
      whileBlocking(_infoChemicalForm->sytematicLineEdit)->setText("Mult. Val.");
    }
  }
}

 std::optional<QString> InfoTreeWidgetController::chemicalFormulaMoiety()
{
  if(_structures.empty())
  {
    return std::nullopt;
  }
  std::set<QString> set = std::set<QString>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    QString value = structure->chemicalFormulaMoiety();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void InfoTreeWidgetController::setChemicalFormulaMoiety(QString name)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setChemicalFormulaMoiety(name);
  }
}

 std::optional<QString> InfoTreeWidgetController::chemicalFormulaSum()
{
  if(_structures.empty())
  {
    return std::nullopt;
  }
  std::set<QString> set = std::set<QString>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    QString value = structure->chemicalFormulaSum();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void InfoTreeWidgetController::setChemicalFormulaSum(QString name)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setChemicalFormulaSum(name);
  }
}

 std::optional<QString> InfoTreeWidgetController::chemicalNameSystematic()
{
  if(_structures.empty())
  {
    return std::nullopt;
  }
  std::set<QString> set = std::set<QString>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    QString value = structure->chemicalNameSystematic();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void InfoTreeWidgetController::setChemicalNameSystematic(QString name)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setChemicalNameSystematic(name);
  }
}

// reload Citation properties
//========================================================================================================================================


void InfoTreeWidgetController::reloadCitationArticleTitle()
{
  if(!_structures.empty())
  {
    if ( std::optional<QString> value = citationArticleTitle())
    {
      whileBlocking(_infoCitationForm->articleTitleTextEdit)->setText(*value);
    }
    else
    {
      whileBlocking(_infoCitationForm->articleTitleTextEdit)->setText("Mult. Val.");
    }
  }
}

void InfoTreeWidgetController::reloadCitationArticleAuthors()
{
  if(!_structures.empty())
  {
    if ( std::optional<QString> value = citationArticleAuthors())
    {
      whileBlocking(_infoCitationForm->articleAuthorsTextEdit)->setText(*value);
    }
    else
    {
      whileBlocking(_infoCitationForm->articleAuthorsTextEdit)->setText("Mult. Val.");
    }
  }
}

void InfoTreeWidgetController::reloadCitationJournalVolume()
{
  if(!_structures.empty())
  {
    if ( std::optional<QString> value = citationJournalVolume())
    {
      whileBlocking(_infoCitationForm->journalVolumeLineEdit)->setText(*value);
    }
    else
    {
      whileBlocking(_infoCitationForm->journalVolumeLineEdit)->setText("Mult. Val.");
    }
  }
}

void InfoTreeWidgetController::reloadCitationJournalNumber()
{
  if(!_structures.empty())
  {
    if ( std::optional<QString> value = citationJournalNumber())
    {
      whileBlocking(_infoCitationForm->journalNumberLineEdit)->setText(*value);
    }
    else
    {
      whileBlocking(_infoCitationForm->journalNumberLineEdit)->setText("Mult. Val.");
    }
  }
}

void InfoTreeWidgetController::reloadCitationPublicationDate()
{
  if(!_structures.empty())
  {
    if ( std::optional<QDate> value = citationPublicationDate())
    {
      whileBlocking(_infoCitationForm->publicationDateEdit)->setDate(*value);
    }
    else
    {
      whileBlocking(_infoCitationForm->publicationDateEdit)->setDate(QDate());
    }
  }
}

void InfoTreeWidgetController::reloadCitationPublicationDOI()
{
  if(!_structures.empty())
  {
    if ( std::optional<QString> value = citationDOI())
    {
      whileBlocking(_infoCitationForm->publicationDOILineEdit)->setText(*value);
    }
    else
    {
      whileBlocking(_infoCitationForm->publicationDOILineEdit)->setText("Mult. Val.");
    }
  }
}

void InfoTreeWidgetController::reloadCitationPublicationDatabaseCodes()
{
  if(!_structures.empty())
  {
    if ( std::optional<QString> value = citationDatabaseCodes())
    {
      whileBlocking(_infoCitationForm->publicationDatabaseCodeLineEdit)->setText(*value);
    }
    else
    {
      whileBlocking(_infoCitationForm->publicationDatabaseCodeLineEdit)->setText("Mult. Val.");
    }
  }
}

 std::optional<QString> InfoTreeWidgetController::citationArticleTitle()
{
  if(_structures.empty())
  {
    return std::nullopt;
  }
  std::set<QString> set = std::set<QString>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    QString value = structure->citationArticleTitle();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void InfoTreeWidgetController::setCitationArticleTitle(QString name)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setCitationArticleTitle(name);
  }
}

 std::optional<QString> InfoTreeWidgetController::citationJournalTitle()
{
  if(_structures.empty())
  {
    return std::nullopt;
  }
  std::set<QString> set = std::set<QString>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    QString value = structure->citationJournalTitle();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void InfoTreeWidgetController::setCitationJournalTitle(QString name)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setCitationJournalTitle(name);
  }
}

 std::optional<QString> InfoTreeWidgetController::citationArticleAuthors()
{
  if(_structures.empty())
  {
    return std::nullopt;
  }
  std::set<QString> set = std::set<QString>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    QString value = structure->citationAuthors();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void InfoTreeWidgetController::setCitationArticleAuthors(QString name)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setCitationAuthors(name);
  }
}

 std::optional<QString> InfoTreeWidgetController::InfoTreeWidgetController::citationJournalVolume()
{
  if(_structures.empty())
  {
    return std::nullopt;
  }
  std::set<QString> set = std::set<QString>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    QString value = structure->citationJournalVolume();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void InfoTreeWidgetController::setCitationJournalVolume(QString name)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setCitationJournalVolume(name);
  }
}

 std::optional<QString> InfoTreeWidgetController::citationJournalNumber()
{
  if(_structures.empty())
  {
    return std::nullopt;
  }
  std::set<QString> set = std::set<QString>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    QString value = structure->citationJournalNumber();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void InfoTreeWidgetController::setCitationJournalNumber(QString name)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setCitationJournalNumber(name);
  }
}

 std::optional<QDate> InfoTreeWidgetController::citationPublicationDate()
{
  if(_structures.empty())
  {
    return std::nullopt;
   }
  std::set<QDate> set = std::set<QDate>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    QDate value = structure->citationPublicationDate();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void InfoTreeWidgetController::setCitationPublicationDate(QDate date)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setCitationPublicationDate(date);
  }
}

 std::optional<QString> InfoTreeWidgetController::citationDOI()
{
  if(_structures.empty())
  {
    return std::nullopt;
  }
  std::set<QString> set = std::set<QString>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    QString value = structure->citationDOI();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void InfoTreeWidgetController::setCitationDOI(QString name)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setCitationDOI(name);
  }
}

 std::optional<QString> InfoTreeWidgetController::citationDatabaseCodes()
{
  if(_structures.empty())
  {
    return std::nullopt;
  }
  std::set<QString> set = std::set<QString>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    QString value = structure->citationDatebaseCodes();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void InfoTreeWidgetController::setCitationDatabaseCodes(QString name)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setCitationDatebaseCodes(name);
  }
}
