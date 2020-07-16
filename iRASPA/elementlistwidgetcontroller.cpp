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

#include "elementlistwidgetcontroller.h"
#include <QPaintDevice>
#include <iostream>
#include <vector>
#include <tuple>
#include <set>
#include <numeric>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <foundationkit.h>

ElementListWidgetController::ElementListWidgetController(QWidget* parent): QListWidget(parent ), _elementsForm(new ElementsForm)
{
  this->setFrameStyle(QFrame::NoFrame);
  this->setVerticalScrollMode(QTreeWidget::ScrollPerPixel);
  this->setSelectionMode(QAbstractItemView::SingleSelection);
}

void ElementListWidgetController::setStructures(std::vector<std::shared_ptr<Structure>> structures)
{
  _structures = structures;
  reloadData();
}

void ElementListWidgetController::setColorComboBoxIndex(size_t type)
{
  if(_mainWindow)
  {
    SKColorSets& colorSets = _mainWindow->colorSets();
    SKColorSet& currentColorSet = colorSets[_selectedColorSetIndex];
    if(type>=colorSets.colorSets().size())
    {
      QString newName = _mainWindow->mainWindowUI()->colorComboBox->itemText(static_cast<int>(type));
      SKColorSet colorSetCopy =  SKColorSet(newName, currentColorSet, true);
      colorSets.append(colorSetCopy);
    }
    _selectedColorSetIndex = static_cast<int>(type);
    reloadColorData();
  }
}

void ElementListWidgetController::setForceFieldComboBoxIndex(int type)
{
  if(_mainWindow)
  {
    ForceFieldSets& forceFieldSets = _mainWindow->forceFieldSets();
    ForceFieldSet& currentForceFieldSet = forceFieldSets[_selectedForceFieldSetIndex];
    if(type>=forceFieldSets.forceFieldSets().size())
    {
      QString newName = _mainWindow->mainWindowUI()->forceFieldComboBox->itemText(type);
      ForceFieldSet forceFieldCopy =  ForceFieldSet(newName, currentForceFieldSet, true);
      forceFieldSets.append(forceFieldCopy);

    }
    _selectedForceFieldSetIndex = type;
    reloadData();
  }
}



void ElementListWidgetController::setMainWindow(MainWindow *mainWindow)
{
  _mainWindow = mainWindow;
  reloadData();

  if(_mainWindow)
  {
    QObject::connect(_mainWindow->mainWindowUI()->colorComboBox,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),this,&ElementListWidgetController::setColorComboBoxIndex);
    QObject::connect(_mainWindow->mainWindowUI()->forceFieldComboBox,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),this,&ElementListWidgetController::setForceFieldComboBoxIndex);
    QObject::connect(_mainWindow->mainWindowUI()->addElementToolButton,static_cast<void (QToolButton::*)(void)>(&QToolButton::pressed),this,&ElementListWidgetController::addNewForceFieldAtomType);
    QObject::connect(_mainWindow->mainWindowUI()->removeElementToolButton,static_cast<void (QToolButton::*)(void)>(&QToolButton::pressed),this,&ElementListWidgetController::removeForceFieldAtomType);
  }
}

void ElementListWidgetController::setProject(std::shared_ptr<ProjectTreeNode> projectTreeNode)
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
        }
      }
    }
  }
}

void ElementListWidgetController::reloadData()
{
  if(_mainWindow)
  {
    ForceFieldSets& forceFieldSets = _mainWindow->forceFieldSets();
    ForceFieldSet& forceFieldSet = forceFieldSets[_selectedForceFieldSetIndex];
    SKColorSets colorSets = _mainWindow->colorSets();
    SKColorSet& colorSet = colorSets[_selectedColorSetIndex];
    std::vector<ForceFieldType>& atomTypeList = forceFieldSet.atomTypeList();

    // fill in color combobox
    whileBlocking(_mainWindow->mainWindowUI()->colorComboBox)->clear();
    for(SKColorSet& colorSet : colorSets.colorSets())
    {
      QString name = colorSet.displayName();
      whileBlocking(_mainWindow->mainWindowUI()->colorComboBox)->addItem(name);
    }
    whileBlocking(_mainWindow->mainWindowUI()->colorComboBox)->setCurrentIndex(_selectedColorSetIndex);

    // fill in force field combobox
    whileBlocking(_mainWindow->mainWindowUI()->forceFieldComboBox)->clear();
    for(ForceFieldSet& forceField : forceFieldSets.forceFieldSets())
    {
      QString name = forceField.displayName();
      whileBlocking(_mainWindow->mainWindowUI()->forceFieldComboBox)->addItem(name);
    }
    whileBlocking(_mainWindow->mainWindowUI()->forceFieldComboBox)->setCurrentIndex(_selectedForceFieldSetIndex);

    // loop over all force field atoms and add them
    this->clear();
    int index=0;
    bool forceFieldEditable = forceFieldSet.editable();
    bool colorSetEditable = colorSet.editable();
    if(forceFieldEditable)
    {
      this->setSelectionMode(QAbstractItemView::SingleSelection);
    }
    else
    {
      this->clearSelection();
      this->setSelectionMode(QAbstractItemView::NoSelection);
    }



    for(ForceFieldType& atomType : atomTypeList)
    {
       QString forceFieldStringIdentifier = atomType.forceFieldStringIdentifier();

       int atomicNumber = atomType.atomicNumber();
       SKElement element = PredefinedElements::predefinedElements[atomicNumber];
       const QString chemicalElement = element._chemicalSymbol;

       ElementsForm* form = new ElementsForm(this);

       QListWidgetItem* mainItem = new QListWidgetItem(this);
       mainItem->setSizeHint(form->sizeHint());


       // set row-number
       whileBlocking(form->rowNumberLabel)->setText(QString::number(index));

       // set force field identifier
       whileBlocking(form->forceFieldStringIdentifierLabel)->setText(forceFieldStringIdentifier);
       if(forceFieldEditable && atomType.editable())
       {
         whileBlocking(form->forceFieldStringIdentifierLabel)->setTextInteractionFlags(Qt::TextEditorInteraction);
       }
       else
       {
         whileBlocking(form->forceFieldStringIdentifierLabel)->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
       }

       // set color
       QColor color = colorSet[forceFieldStringIdentifier];
       whileBlocking(form->colorPushButton)->setColor(color);
       form->colorPushButton->setEnabled(colorSetEditable);
       const QString  style = QString("QPushButton { background-color: rgb(") + QString::number(color.red()) + QString(",") +
               QString::number(color.green()) + QString(",")  + QString::number(color.blue()) + QString(");  border: 1px solid black;}");
       whileBlocking(form->colorPushButton)->setStyleSheet(style);

       QObject::connect(form->colorPushButton,static_cast<void (SelectColorButton::*)(void)>(&SelectColorButton::pressed),this,&ElementListWidgetController::selectColorButton, Qt::UniqueConnection);
       QObject::connect(form->epsilonDoubleSpinBox,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&ElementListWidgetController::setEpsilonParameter, Qt::UniqueConnection);
       QObject::connect(form->sigmaDoubleSpinBox,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&ElementListWidgetController::setSigmaParameter, Qt::UniqueConnection);
       QObject::connect(form->atomicMassDoubleSpinBox,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&ElementListWidgetController::setMass, Qt::UniqueConnection);

       // set chemical element
       whileBlocking(form->elementLineEdit)->setText(chemicalElement);
       form->elementLineEdit->setReadOnly(true);

       // set atomic number
       whileBlocking(form->numberSpinBox)->setValue(element._atomicNumber);
       form->numberSpinBox->setReadOnly(true);

       // set element group
       whileBlocking(form->groupSpinBox)->setValue(element._group);
       form->groupSpinBox->setReadOnly(true);

       // set element period
       whileBlocking(form->periodSpinBox)->setValue(element._period);
       form->periodSpinBox->setReadOnly(true);

       double epsilon = atomType.potentialParameters().x;
       double sigma = atomType.potentialParameters().y;
       whileBlocking(form->epsilonDoubleSpinBox)->setValue(epsilon);
       form->epsilonDoubleSpinBox->setReadOnly(!forceFieldEditable);
       whileBlocking(form->sigmaDoubleSpinBox)->setValue(sigma);
       form->sigmaDoubleSpinBox->setReadOnly(!forceFieldEditable);

       std::vector<int> oxidationStates = element._possibleOxidationStates;
       QString oxidationStatesString = std::accumulate(std::next(oxidationStates.begin()), oxidationStates.end(),
                                          QString::number(oxidationStates[0]), // start with first element
                                          [](QString& a, int& b) {
                                              return a + ',' + QString::number(b);
                                          });

       whileBlocking(form->oxidationLineEdit)->setText(oxidationStatesString);
       form->oxidationLineEdit->setReadOnly(true);

       double mass = atomType.mass();
       whileBlocking(form->atomicMassDoubleSpinBox)->setValue(mass);
       form->atomicMassDoubleSpinBox->setReadOnly(!forceFieldEditable);

       double atomicRadius = element._atomRadius;
       whileBlocking(form->atomicRadiusDoubleSpinBox)->setValue(atomicRadius);
       form->atomicRadiusDoubleSpinBox->setReadOnly(!forceFieldEditable);

       double covalentRadius = element._covalentRadius;
       whileBlocking(form->covalentRadiusDoubleSpinBox)->setValue(covalentRadius);
       form->covalentRadiusDoubleSpinBox->setReadOnly(!forceFieldEditable);

       double VDWRadius = element._VDWRadius;
       whileBlocking(form->vanderWaalsDoubleSpinBox)->setValue(VDWRadius);
       form->vanderWaalsDoubleSpinBox->setReadOnly(!forceFieldEditable);

       double tripleBondCovalentRadius = element._tripleBondCovalentRadius;
       whileBlocking(form->tripleBondCovalentRadiusDoubleSpinBox)->setValue(tripleBondCovalentRadius);
       form->tripleBondCovalentRadiusDoubleSpinBox->setReadOnly(!forceFieldEditable);

       double userDefinedRadius = atomType.userDefinedRadius();
       whileBlocking(form->userDefinedRadiusDoubleSpinBox)->setValue(userDefinedRadius);
       form->userDefinedRadiusDoubleSpinBox->setReadOnly(!forceFieldEditable);



       this->setItemWidget(mainItem, form);
       addItem(mainItem);
       index++;
    }
  }
}

void ElementListWidgetController::reloadColorData()
{
  if(_mainWindow)
  {
    ForceFieldSets& forceFieldSets = _mainWindow->forceFieldSets();
    ForceFieldSet& forceFieldSet = forceFieldSets[_selectedForceFieldSetIndex];
    SKColorSets colorSets = _mainWindow->colorSets();
    SKColorSet& colorSet = colorSets[_selectedColorSetIndex];
    std::vector<ForceFieldType>& atomTypeList = forceFieldSet.atomTypeList();

    int row = 0;
    bool colorSetEditable = colorSet.editable();
    for(ForceFieldType& atomType : atomTypeList)
    {
       QString forceFieldStringIdentifier = atomType.forceFieldStringIdentifier();

       QListWidgetItem* mainItem = this->item(row);
       ElementsForm* form = static_cast<ElementsForm*>( this->itemWidget(mainItem));
       mainItem->setSizeHint(form->sizeHint());

       QColor color = colorSet[forceFieldStringIdentifier];


       const QString  style = QString("QPushButton { background-color: rgb(") + QString::number(color.red()) + QString(",") +
               QString::number(color.green()) + QString(",")  + QString::number(color.blue()) + QString(");  border: 1px solid black;}");
       form->colorPushButton->setStyleSheet(style);
       form->colorPushButton->setEnabled(colorSetEditable);
       QObject::connect(form->colorPushButton,static_cast<void (SelectColorButton::*)(void)>(&SelectColorButton::pressed),this,&ElementListWidgetController::selectColorButton, Qt::UniqueConnection);

       row++;
    }
  }
}

 std::optional<size_t> ElementListWidgetController::rowForItem(QObject* item)
{
  ElementsForm* elementForm = qobject_cast<ElementsForm*>(item);

  if(elementForm)
  {
    for (int row=0; row<this->count(); row++)
    {
      QListWidgetItem* listitem = this->item(row);
      if (itemWidget(listitem) == elementForm)
      {
        return size_t(row);
      }
    }
  }

  return  std::nullopt;
}

void ElementListWidgetController::addNewForceFieldAtomType()
{
  if(!selectedItems().empty())
  {
    int selectedRow = row(selectedItems().first());

    ElementsForm* form = new ElementsForm(this);
    QListWidgetItem* mainItem = new QListWidgetItem();
    mainItem->setSizeHint(QSize(0,202));
    insertItem(selectedRow + 1, mainItem);
    this->setItemWidget(mainItem, form);

    ForceFieldSets& forceFieldSets = _mainWindow->forceFieldSets();
    ForceFieldSet& forceFieldSet = forceFieldSets[_selectedForceFieldSetIndex];
    bool forceFieldEditable = forceFieldSet.editable();
    std::vector<ForceFieldType>& atomTypeList = forceFieldSet.atomTypeList();
    ForceFieldType& atomType = atomTypeList[selectedRow];

    SKColorSets& colorSets = _mainWindow->colorSets();
    SKColorSet& colorSet = colorSets[_selectedColorSetIndex];
    QString forceFieldStringIdentifier = atomType.forceFieldStringIdentifier();
    QString newForceFieldStringIdentifier = forceFieldSet.uniqueName(atomType.atomicNumber());
    QColor color = colorSet[forceFieldStringIdentifier];
    colorSet[newForceFieldStringIdentifier] = color;

    ForceFieldType copiedAtomType = ForceFieldType(atomType);
    copiedAtomType.setForceFieldStringIdentifier(newForceFieldStringIdentifier);
    copiedAtomType.setEditable(true);
    forceFieldSet.insert(selectedRow+1, copiedAtomType);

    const QString  style = QString("QPushButton { background-color: rgb(") + QString::number(color.red()) + QString(",") +
            QString::number(color.green()) + QString(",")  + QString::number(color.blue()) + QString(");  border: 1px solid black;}");
    form->colorPushButton->setStyleSheet(style);
    form->colorPushButton->setEnabled(true);
    QObject::connect(form->colorPushButton,static_cast<void (SelectColorButton::*)(void)>(&SelectColorButton::pressed),this,&ElementListWidgetController::selectColorButton, Qt::UniqueConnection);



    for(int row=0; row<count(); row++)
    {
      ElementsForm* form = static_cast<ElementsForm*>( this->itemWidget(mainItem));

      QListWidgetItem* mainItem = this->item(row);
      mainItem->setSizeHint(form->sizeHint());

      whileBlocking(form->rowNumberLabel)->setText(QString::number(row));
    }

    int atomicNumber = copiedAtomType.atomicNumber();
    SKElement element = PredefinedElements::predefinedElements[atomicNumber];
    const QString chemicalElement = element._chemicalSymbol;

    whileBlocking(form->forceFieldStringIdentifierLabel)->setText(newForceFieldStringIdentifier);
    if(forceFieldEditable && copiedAtomType.editable())
    {
      whileBlocking(form->forceFieldStringIdentifierLabel)->setTextInteractionFlags(Qt::TextEditorInteraction);
    }
    else
    {
      whileBlocking(form->forceFieldStringIdentifierLabel)->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    }

    whileBlocking(form->elementLineEdit)->setText(chemicalElement);
    form->elementLineEdit->setReadOnly(true);

    whileBlocking(form->numberSpinBox)->setValue(element._atomicNumber);
    form->numberSpinBox->setReadOnly(true);
    whileBlocking(form->groupSpinBox)->setValue(element._group);
    form->groupSpinBox->setReadOnly(true);
    whileBlocking(form->periodSpinBox)->setValue(element._period);
    form->periodSpinBox->setReadOnly(true);


    std::vector<int> oxidationStates = element._possibleOxidationStates;
    QString oxidationStatesString = std::accumulate(std::next(oxidationStates.begin()), oxidationStates.end(),
                                       QString::number(oxidationStates[0]), // start with first element
                                       [](QString& a, int& b) {
                                           return a + ',' + QString::number(b);
                                       });

    whileBlocking(form->oxidationLineEdit)->setText(oxidationStatesString);
    form->oxidationLineEdit->setReadOnly(true);



    double epsilon = copiedAtomType.potentialParameters().x;
    double sigma = copiedAtomType.potentialParameters().y;
    whileBlocking(form->epsilonDoubleSpinBox)->setValue(epsilon);
    form->epsilonDoubleSpinBox->setReadOnly(false);
    whileBlocking(form->sigmaDoubleSpinBox)->setValue(sigma);
    form->sigmaDoubleSpinBox->setReadOnly(false);



    double mass = copiedAtomType.mass();
    whileBlocking(form->atomicMassDoubleSpinBox)->setValue(mass);
    form->atomicMassDoubleSpinBox->setReadOnly(false);

    double atomicRadius = element._atomRadius;
    whileBlocking(form->atomicRadiusDoubleSpinBox)->setValue(atomicRadius);
    form->atomicRadiusDoubleSpinBox->setReadOnly(false);

    double covalentRadius = element._covalentRadius;
    whileBlocking(form->covalentRadiusDoubleSpinBox)->setValue(covalentRadius);
    form->covalentRadiusDoubleSpinBox->setReadOnly(false);

    double VDWRadius = element._VDWRadius;
    whileBlocking(form->vanderWaalsDoubleSpinBox)->setValue(VDWRadius);
    form->vanderWaalsDoubleSpinBox->setReadOnly(false);

    double tripleBondCovalentRadius = element._tripleBondCovalentRadius;
    whileBlocking(form->tripleBondCovalentRadiusDoubleSpinBox)->setValue(tripleBondCovalentRadius);
    form->tripleBondCovalentRadiusDoubleSpinBox->setReadOnly(false);

    double userDefinedRadius = atomType.userDefinedRadius();
    whileBlocking(form->userDefinedRadiusDoubleSpinBox)->setValue(userDefinedRadius);
    form->userDefinedRadiusDoubleSpinBox->setReadOnly(false);



    std::cout << "insert item" << std::endl;
  }
}

void ElementListWidgetController::removeForceFieldAtomType()
{
  if(!selectedItems().empty())
  {
    int selectedRow = row(selectedItems().first());

    ForceFieldSets& forceFieldSets = _mainWindow->forceFieldSets();
    ForceFieldSet& forceFieldSet = forceFieldSets[_selectedForceFieldSetIndex];

    if(forceFieldSet.editable())
    {
      std::vector<ForceFieldType>& atomTypeList = forceFieldSet.atomTypeList();
      ForceFieldType& atomType = atomTypeList[selectedRow];

      if(atomType.editable())
      {
        atomTypeList.erase(atomTypeList.begin() + selectedRow);
        QListWidgetItem* item = takeItem(selectedRow);
        delete item;

        for(int row=0; row<count(); row++)
        {
          QListWidgetItem* mainItem = this->item(row);
          mainItem->setSizeHint(QSize(0,202));
          ElementsForm* form = static_cast<ElementsForm*>( this->itemWidget(mainItem));

          whileBlocking(form->rowNumberLabel)->setText(QString::number(row));
        }
      }
    }
  }
}

void ElementListWidgetController::selectColorButton()
{
  SelectColorButton *button = static_cast<SelectColorButton *>(sender());
   std::optional<size_t> row = rowForItem(button->parent());

  if(_mainWindow)
  {
    if(row)
    {
      ForceFieldSets& forceFieldSets = _mainWindow->forceFieldSets();
      ForceFieldSet& forceFieldSet = forceFieldSets[_selectedForceFieldSetIndex];
      SKColorSets& colorSets = _mainWindow->colorSets();
      SKColorSet& colorSet = colorSets[_selectedColorSetIndex];
      std::vector<ForceFieldType>& atomTypeList = forceFieldSet.atomTypeList();
      ForceFieldType& forceFieldType = atomTypeList[*row];
      QString forceFieldStringIdentifier = forceFieldType.forceFieldStringIdentifier();

      QColor color = QColorDialog::getColor(Qt::white,this,"Choose Color");
      if(color.isValid())
      {
        colorSet[forceFieldStringIdentifier] = color;
        const QString  style = QString("QPushButton { background-color: rgb(") + QString::number(color.red()) + QString(",") +
                    QString::number(color.green()) + QString(",")  + QString::number(color.blue()) + QString(");  border: 1px solid black;}");
        button->setStyleSheet(style);

        for(std::shared_ptr<Structure> structure : _structures)
        {
          QString colorScheme = structure->atomColorSchemeIdentifier();
          structure->setRepresentationColorSchemeIdentifier(colorScheme, _mainWindow->colorSets());
        }

        emit rendererReloadData();
      }
    }
  }
}


void ElementListWidgetController::setEpsilonParameter(double parameter)
{
  SelectColorButton *button = static_cast<SelectColorButton *>(sender());
   std::optional<size_t> row = rowForItem(button->parent());

  if(_mainWindow)
  {
    if(row)
    {
      ForceFieldSets& forceFieldSets = _mainWindow->forceFieldSets();
      ForceFieldSet& forceFieldSet = forceFieldSets[_selectedForceFieldSetIndex];
      std::vector<ForceFieldType>& atomTypeList = forceFieldSet.atomTypeList();
      //ForceFieldType& forceFieldType = atomTypeList[*row];

      atomTypeList[*row].setEpsilonPotentialParameter(parameter);

      for(std::shared_ptr<Structure> structure : _structures)
      {
         structure->updateForceField(forceFieldSets);
      }

      emit invalidateIsosurface(std::vector<std::shared_ptr<RKRenderStructure>>{_structures.begin(), _structures.end()});
      emit rendererReloadData();
    }
  }
}


void ElementListWidgetController::setSigmaParameter(double parameter)
{
  SelectColorButton *button = static_cast<SelectColorButton *>(sender());
   std::optional<size_t> row = rowForItem(button->parent());

  if(_mainWindow)
  {
    if(row)
    {
      ForceFieldSets& forceFieldSets = _mainWindow->forceFieldSets();
      ForceFieldSet& forceFieldSet = forceFieldSets[_selectedForceFieldSetIndex];
      std::vector<ForceFieldType>& atomTypeList = forceFieldSet.atomTypeList();
      //ForceFieldType& forceFieldType = atomTypeList[*row];

      atomTypeList[*row].setSigmaPotentialParameter(parameter);

      for(std::shared_ptr<Structure> structure : _structures)
      {
         structure->updateForceField(forceFieldSets);
      }

      emit invalidateIsosurface(std::vector<std::shared_ptr<RKRenderStructure>>{_structures.begin(), _structures.end()});
      emit rendererReloadData();
    }
  }
}

void ElementListWidgetController::setMass(double mass)
{
  QAbstractButton *button = static_cast<QAbstractButton *>(sender());
   std::optional<size_t> row = rowForItem(button->parent());

  if(_mainWindow)
  {
    if(row)
    {
      ForceFieldSets& forceFieldSets = _mainWindow->forceFieldSets();
      ForceFieldSet& forceFieldSet = forceFieldSets[_selectedForceFieldSetIndex];
      std::vector<ForceFieldType>& atomTypeList = forceFieldSet.atomTypeList();
      //ForceFieldType& forceFieldType = atomTypeList[*row];

      atomTypeList[*row].setMass(mass);

      for(std::shared_ptr<Structure> structure : _structures)
      {
         structure->recomputeDensityProperties();
      }
    }
  }
}


