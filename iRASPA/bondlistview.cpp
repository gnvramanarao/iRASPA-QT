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

#include "bondlistview.h"
#include "bondlistviewsliderstyleditemdelegate.h"
#include "bondlistviewdeleteselectioncommand.h"
#include "bondchangeselectioncommand.h"


BondListView::BondListView(QWidget* parent): QTreeView(parent ), _bondModel(std::make_shared<BondListViewModel>())
{
  this->setModel(_bondModel.get());

  QObject::connect(model(),&QAbstractItemModel::modelReset, this, &BondListView::reloadSelection);

  this->viewport()->setMouseTracking(true);

  this->setUniformRowHeights(true);
  this->setAlternatingRowColors(true);
  this->setSelectionBehavior (QAbstractItemView::SelectRows);
  this->setSelectionMode(QAbstractItemView::ExtendedSelection);

  this->setStyleSheet(":disabled { color: dark-grey;  background-color: rgb(255, 255, 255);}");

  this->setRootIsDecorated(false);
  this->setItemsExpandable(false);
  this->setIndentation(0);

  pushButtonDelegate = new BondListPushButtonStyledItemDelegate(this);
  this->setItemDelegateForColumn(2, pushButtonDelegate);

  comboBoxDelegate = new BondListViewComboBoxStyledItemDelegate(this);
  this->setItemDelegateForColumn(5, comboBoxDelegate);

  sliderDelegate = new BondListViewSliderStyledItemDelegate(this);
  this->setItemDelegateForColumn(7, sliderDelegate);

  this->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);

  QObject::connect(selectionModel(), &QItemSelectionModel::selectionChanged, this, &BondListView::setSelectedBonds);
}

void BondListView::resetData()
{
  _bondModel->resetData();
}

void BondListView::setProject(std::shared_ptr<ProjectTreeNode> projectTreeNode)
{
  _projectTreeNode = projectTreeNode;
  _iRASPAProject.reset();
  _iraspaStructure.reset();

  if (projectTreeNode)
  {
    setEnabled(projectTreeNode->isEditable());
    _iRASPAProject = projectTreeNode->representedObject();
    if(_iRASPAProject)
    {
      if(std::shared_ptr<Project> project = _iRASPAProject->project())
      {
        if (std::shared_ptr<ProjectStructure> projectStructure = std::dynamic_pointer_cast<ProjectStructure>(project))
        {
          std::shared_ptr<Scene> selectedScene = projectStructure->sceneList()->selectedScene();
          std::shared_ptr<Movie> selectedMovie = selectedScene? selectedScene->selectedMovie() : nullptr;
          std::shared_ptr<iRASPAStructure> iraspaStructure = selectedMovie ? selectedMovie->selectedFrame() : nullptr;
          _iraspaStructure = iraspaStructure;
        }
      }
    }
  }

  // reload and resize the columns
  _bondModel->setFrame(_iraspaStructure.lock());
  this->reloadData();
}

void BondListView::setSelectedFrame(std::shared_ptr<iRASPAStructure> iraspaStructure)
{
  _iraspaStructure = iraspaStructure;

  if(BondListViewModel* bondModel = qobject_cast<BondListViewModel*>(model()))
  {
    bondModel->setFrame(iraspaStructure);
  }
}

void BondListView::reloadSelection()
{

  whileBlocking(selectionModel())->clearSelection();

  if(BondListViewModel* pModel = qobject_cast<BondListViewModel*>(model()))
  {
    for(QModelIndex index : pModel->selectedIndexes())
    {
      whileBlocking(selectionModel())->select(index, QItemSelectionModel::Select|QItemSelectionModel::Rows);
    }
  }
  viewport()->update();
}

void BondListView::reloadData()
{
  if(std::shared_ptr<iRASPAStructure> iraspaStructure = _iraspaStructure.lock())
  {
    if(iraspaStructure->structure()->bondSetController()->getNumberOfBonds() > 0)
    {
      this->header()->setStretchLastSection(false);
      this->setColumnWidth(0,25);
      this->setColumnWidth(1,50);
      this->setColumnWidth(2,100);
      this->setColumnWidth(3,50);
      this->setColumnWidth(4,50);
      this->setColumnWidth(5,60);
      this->setColumnWidth(6,60);
      this->setColumnWidth(7,85);  // 20 left for scroller
    }
  }
}


QSize BondListView::sizeHint() const
{
  return QSize(500, 800);
}

void BondListView::keyPressEvent(QKeyEvent *event)
{
  if( event->type() == QEvent::KeyPress )
  {
    QKeyEvent * keyEvent = dynamic_cast<QKeyEvent*>(event);
    if( keyEvent->key() == Qt::Key_Delete )
    {
      deleteSelection();
    }
  }
  QTreeView::keyPressEvent(event);
}

void BondListView::deleteSelection()
{
  qDebug() << "deleteSelection";
  if(std::shared_ptr<ProjectTreeNode> projectTreeNode = _projectTreeNode.lock())
  {
    if(projectTreeNode->isEditable())
    {
      if(std::shared_ptr<iRASPAStructure> iraspaStructure = _iraspaStructure.lock())
      {
        std::shared_ptr<Structure> structure = iraspaStructure->structure();
        if(_iRASPAProject)
        {
          std::vector<std::shared_ptr<SKAsymmetricBond>> bonds = structure->bondSetController()->selectedObjects();
          std::set<int> bondSelection = structure->bondSetController()->selectionIndexSet();
          BondListViewDeleteSelectionCommand *deleteSelectionCommand = new BondListViewDeleteSelectionCommand(_bondModel, _mainWindow, structure, bonds, bondSelection);
          _iRASPAProject->undoManager().push(deleteSelectionCommand);
        }
      }
    }
  }
}


SKAsymmetricBond* BondListView::getItem(const QModelIndex &index) const
{
  if (index.isValid())
  {
     SKAsymmetricBond *item = static_cast<SKAsymmetricBond*>(index.internalPointer());
     if (item)
     {
       return item;
     }
  }
  return nullptr;
}
void BondListView::setSelectedBonds(const QItemSelection &selected, const QItemSelection &deselected)
{
  if(std::shared_ptr<iRASPAStructure> iraspaStructure = _iraspaStructure.lock())
  {
    std::shared_ptr<Structure> structure = iraspaStructure->structure();
    std::set<int> previousBondSelection = structure->bondSetController()->selectionIndexSet();

    for(QModelIndex index : deselected.indexes())
    {
      SKAsymmetricBond *item = getItem(index);
      std::shared_ptr<SKAsymmetricBond> asymmetricBond = item->shared_from_this();

      structure->bondSetController()->selectionIndexSet().erase(asymmetricBond->asymmetricIndex());
    }

    for(QModelIndex index : selected.indexes())
    {
      SKAsymmetricBond *item = getItem(index);
      std::shared_ptr<SKAsymmetricBond> asymmetricBond = item->shared_from_this();

      structure->bondSetController()->selectionIndexSet().insert(asymmetricBond->asymmetricIndex());
    }

    // Force bonds to selected (bonds that are automatically selected due to their atoms being selected).
    structure->bondSetController()->correctBondSelectionDueToAtomSelection();
    this->reloadSelection();

    if(std::shared_ptr<ProjectTreeNode> projectTreeNode = _projectTreeNode.lock())
    {
      if(projectTreeNode->isEditable())
      {
        if(_iRASPAProject)
        {
          std::set<int> bondSelection = structure->bondSetController()->selectionIndexSet();

          BondChangeSelectionCommand *changeSelectionCommand = new BondChangeSelectionCommand(structure->bondSetController(), _mainWindow,
                                                                         structure, bondSelection, previousBondSelection);
          _iRASPAProject->undoManager().push(changeSelectionCommand);
        }
      }
    }
  }
}


