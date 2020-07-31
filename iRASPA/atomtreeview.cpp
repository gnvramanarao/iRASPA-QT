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

#include "atomtreeview.h"
#include <QPainter>
#include <QtCore>
#include <QItemSelection>
#include "atomtreeviewinsertatomcommand.h"
#include "atomtreeviewinsertatomgroupcommand.h"
#include "atomtreeviewdeleteselectioncommand.h"
#include "atomchangeselectioncommand.h"
#include "atomtreeviewdecorationstyleditemdelegate.h"

AtomTreeView::AtomTreeView(QWidget* parent): QTreeView(parent ), _atomModel(std::make_shared<AtomTreeViewModel>())
{
  this->setModel(_atomModel.get());



  this->viewport()->setMouseTracking(true);

  this->setHeaderHidden(true);
  this->setAlternatingRowColors(true);
  this->setSelectionBehavior (QAbstractItemView::SelectRows);
  this->setSelectionMode(QAbstractItemView::ExtendedSelection);

  this->setStyleSheet(":disabled { color: dark-grey;  background-color: rgb(255, 255, 255);}");

  setDragEnabled(true);
  setAcceptDrops(true);
  setDropIndicatorShown(true);
  setDragDropMode(DragDropMode::InternalMove);
  setDragDropOverwriteMode(false);
  //setDefaultDropAction(Qt::MoveAction);

  setItemsExpandable(true);
  setRootIsDecorated(true);

  this->setContextMenuPolicy(Qt::CustomContextMenu);
  QObject::connect(this, &QWidget::customContextMenuRequested, this, &AtomTreeView::ShowContextMenu);

  _dropIndicatorRect = QRect();

  pushButtonDelegate = new AtomTreeViewPushButtonStyledItemDelegate(this);
  this->setItemDelegateForColumn(1, pushButtonDelegate);

  this->setItemDelegateForColumn(0, new AtomTreeViewDecorationStyledItemDelegate(this));

  this->header()->setStretchLastSection(true);
  this->setColumnWidth(0,110);
  this->setColumnWidth(1,50);
  this->setColumnWidth(2,40);
  this->setColumnWidth(3,70);
  this->setColumnWidth(4,70);
  this->setColumnWidth(5,70);
  this->setColumnWidth(6,70);

  QObject::connect(model(),&QAbstractItemModel::modelReset, this, &AtomTreeView::reloadSelection);
  QObject::connect(_atomModel.get(),&AtomTreeViewModel::collapse, this, &AtomTreeView::collapse);
  QObject::connect(_atomModel.get(), &AtomTreeViewModel::reloadSelection, this, &AtomTreeView::reloadSelection);
  QObject::connect(_atomModel.get(), &AtomTreeViewModel::rendererReloadData, this, &AtomTreeView::rendererReloadData);
}

void AtomTreeView::setMainWindow(MainWindow* mainWindow)
{
  _mainWindow = mainWindow;
  _atomModel->_mainWindow = mainWindow;
}


void AtomTreeView::setBondModel(std::shared_ptr<BondListViewModel> bondModel)
{
  _bondModel = bondModel;
}


void AtomTreeView::setProject(std::shared_ptr<ProjectTreeNode> projectTreeNode)
{
  _projectTreeNode = projectTreeNode;
  _iRASPAProject.reset();
  _iraspaStructure.reset();

  if (projectTreeNode)
  {
    _atomModel->setProject(projectTreeNode);
    setEnabled(projectTreeNode->isEditable());
    _iRASPAProject = projectTreeNode->representedObject();
    if(_iRASPAProject)
    {
      if(std::shared_ptr<Project> project = _iRASPAProject->project())
      {
        if (std::shared_ptr<ProjectStructure> projectStructure = std::dynamic_pointer_cast<ProjectStructure>(project))
        {
          _iraspaStructure = projectStructure->selectedFrame();
        }

      }
    }
  }



  // reload and resize the columns
  _atomModel->setFrame(_iraspaStructure);
  this->reloadData();
}

void AtomTreeView::setSelectedFrame(std::shared_ptr<iRASPAStructure> iraspastructure)
{
  _iraspaStructure = iraspastructure;

  if(AtomTreeViewModel* atomModel = qobject_cast<AtomTreeViewModel*>(model()))
  {
    atomModel->setFrame(iraspastructure);
  }
}

SKAtomTreeNode* AtomTreeView::getItem(const QModelIndex &index) const
{
  if(_iraspaStructure)
  {
    std::shared_ptr<Structure> structure = _iraspaStructure->structure();
    if (index.isValid())
    {
       SKAtomTreeNode *item = static_cast<SKAtomTreeNode*>(index.internalPointer());
       if (item)
       {
         return item;
       }
    }
    return structure->atomsTreeController()->hiddenRootNode().get();
  }
  return nullptr;
}

void AtomTreeView::keyPressEvent(QKeyEvent *event)
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


void AtomTreeView::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
  qDebug() << "Number of selected atoms: " << selected.size();

  if(_iraspaStructure)
  {
    std::shared_ptr<Structure> structure = _iraspaStructure->structure();
    std::shared_ptr<SKAtomTreeController> atomTreeControler = structure->atomsTreeController();
    std::shared_ptr<SKBondSetController> bondListControler = structure->bondSetController();

    std::unordered_set<std::shared_ptr<SKAtomTreeNode>> previousAtomSelection = std::unordered_set<std::shared_ptr<SKAtomTreeNode>>(atomTreeControler->selectedTreeNodes());
    std::set<int> previousBondSelection = bondListControler->selectionIndexSet();

    for(QModelIndex index : deselected.indexes())
    {
      SKAtomTreeNode *node = getItem(index);
      std::shared_ptr<SKAtomTreeNode> atom = node->shared_from_this();
      atomTreeControler->selectedTreeNodes().erase(atom);

      // remove bonds that are connected to this atom from the selection
      int bondIndex=0;
      for(std::shared_ptr<SKAsymmetricBond> bond : bondListControler->arrangedObjects())
      {
        if(bond->atom1() == atom->representedObject() || bond->atom2() == atom->representedObject())
        {
          bondListControler->selectionIndexSet().erase(bondIndex);
        }
        bondIndex++;
      }
    }

    for(QModelIndex index : selected.indexes())
    {
      SKAtomTreeNode *node = getItem(index);
      std::shared_ptr<SKAtomTreeNode> atom = node->shared_from_this();
      atomTreeControler->selectedTreeNodes().insert(atom);
    }

    bondListControler->correctBondSelectionDueToAtomSelection();


    std::unordered_set<std::shared_ptr<SKAtomTreeNode>> atomSelection = std::unordered_set<std::shared_ptr<SKAtomTreeNode>>(atomTreeControler->selectedTreeNodes());
    std::set<int> bondSelection = bondListControler->selectionIndexSet();

    AtomChangeSelectionCommand *changeSelectionCommand = new AtomChangeSelectionCommand(_mainWindow, structure, atomSelection, previousAtomSelection, bondSelection, previousBondSelection);
    _iRASPAProject->undoManager().push(changeSelectionCommand);
  }
}

void AtomTreeView::paintEvent(QPaintEvent *event)
{
  QPainter painter(viewport());
  drawTree(&painter, event->region());
  paintDropIndicator(painter);
}

void AtomTreeView::paintDropIndicator(QPainter& painter)
{
  if(state() == QAbstractItemView::DraggingState)
  {
    QStyleOption opt = QStyleOption();
    opt.init(this);
    opt.rect = _dropIndicatorRect;
    QRect rect = opt.rect;

    QBrush brush = QBrush(QColor(Qt::black));

    if(rect.height() == 0)
    {
      QPen pen = QPen(brush, 2, Qt::SolidLine);
      painter.setPen(pen);
      painter.drawLine(rect.topLeft(), rect.topRight());
    }
    else
    {
      QPen pen = QPen(brush, 2, Qt::SolidLine);
      painter.setPen(pen);
      painter.drawRect(rect);
    }
  }
}

void AtomTreeView::startDrag(Qt::DropActions supportedActions)
{
  QModelIndexList selectionIndexes = selectionModel()->selectedRows();

  std::cout << "Start drag: " << selectionIndexes.count() << std::endl;

  if(!selectionIndexes.isEmpty())
  {
    QMimeData* mimeData = model()->mimeData(selectionIndexes);
    QDrag* drag = new QDrag(this);
    drag->setMimeData(mimeData);
    Qt::DropAction dropAction = Qt::IgnoreAction;
    if((supportedActions & Qt::CopyAction) && (dragDropMode() != QAbstractItemView::InternalMove))
    {
      dropAction = Qt::CopyAction;
    }
    if(drag->exec(supportedActions, dropAction) == Qt::MoveAction)
    {

     // emit invalidateCachedAmbientOcclusionTexture(std::vector<std::shared_ptr<RKRenderStructure>>{_structures.begin(), _structures.end()});
      emit rendererReloadData();

      reloadSelection();
    }
  }
}


void AtomTreeView::dragMoveEvent(QDragMoveEvent *event)
{
  QPoint pos = event->pos();
  QModelIndex index = indexAt(mapFromParent(pos));

  int columnCount = model()->columnCount();

  QRect rect = visualRect(index);
  QRect rect_left = visualRect(index.sibling(index.row(), 0));
  QRect rect_right = visualRect(index.sibling(index.row(), header()->logicalIndex(columnCount - 1)));

  QAbstractItemView::DropIndicatorPosition dropIndicatorPosition = position(event->pos(), rect, index);

  if (dropIndicatorPosition == AboveItem)
  {
    _dropIndicatorRect = QRect(rect_left.left(), rect_left.top(), rect_right.right() - rect_left.left(), 0);
    event->accept();
  }
  else if(dropIndicatorPosition == BelowItem)
  {
    _dropIndicatorRect = QRect(rect_left.left(), rect_left.bottom(), rect_right.right() - rect_left.left(), 0);
    event->accept();
  }
  else if(dropIndicatorPosition == OnItem && index.flags() & Qt::ItemIsDropEnabled)
  {
    _dropIndicatorRect = QRect(rect_left.left(), rect_left.top(), rect_right.right() - rect_left.left(), rect.height());
    event->accept();
  }
  else
  {
    _dropIndicatorRect = QRect();
  }

  model()->setData(index, dropIndicatorPosition, Qt::UserRole);

  viewport()->update();
}

QAbstractItemView::DropIndicatorPosition AtomTreeView::position(QPoint pos, QRect rect, QModelIndex index)
{
  QAbstractItemView::DropIndicatorPosition r = OnViewport;

  //  margin*2 must be smaller than row height, or the drop onItem rect won't show
  double margin = 6;
  if (pos.y() - rect.top() < margin)
    r = QAbstractItemView::AboveItem;
  else if(rect.bottom() - pos.y() < margin)
    r = QAbstractItemView::BelowItem;
  else if((pos.y() - rect.top() > margin) && (rect.bottom() - pos.y() > margin))
    r = QAbstractItemView::OnItem;

  return r;
}

void AtomTreeView::reloadSelection()
{
  qDebug() << "AtomTreeView::reloadSelection()";
  if(AtomTreeViewModel* pModel = qobject_cast<AtomTreeViewModel*>(model()))
  {
    whileBlocking(selectionModel())->clearSelection();

    for(QModelIndex index : pModel->selectedIndexes())
    {
      whileBlocking(selectionModel())->select(index, QItemSelectionModel::Select|QItemSelectionModel::Rows);
    }
  }
  viewport()->update();
}

void AtomTreeView::reloadData()
{
  if(_iraspaStructure)
  {
    std::shared_ptr<Structure> structure = _iraspaStructure->structure();
    std::shared_ptr<SKAtomTreeController> atomTreeController = structure->atomsTreeController();

    if(atomTreeController->rootNodes().size() > 0)
    {
      this->header()->setStretchLastSection(true);
      this->setColumnWidth(0,120);
      this->resizeColumnToContents(1);
      this->resizeColumnToContents(2);
      this->resizeColumnToContents(3);
      this->resizeColumnToContents(4);
      this->resizeColumnToContents(5);
      this->resizeColumnToContents(6);
    }
  }
}

void AtomTreeView::dropEvent(QDropEvent * event)
{
  QModelIndex droppedIndex = indexAt( event->pos() );
  if( !droppedIndex.isValid() )
    return;

  QTreeView::dropEvent(event);

  DropIndicatorPosition dp = dropIndicatorPosition();
  if (dp == QAbstractItemView::BelowItem)
  {
     droppedIndex = droppedIndex.sibling(droppedIndex.row() + 1, droppedIndex.column());
  }
}



QSize AtomTreeView::sizeHint() const
{
  return QSize(500, 800);
}

void AtomTreeView::deleteSelection()
{
  if(_iraspaStructure)
  {
    std::shared_ptr<Structure> structure = _iraspaStructure->structure();
    std::shared_ptr<SKAtomTreeController> atomTreeController = structure->atomsTreeController();
    std::shared_ptr<SKBondSetController> bondSetController = structure->bondSetController();

    std::vector<std::shared_ptr<SKAtomTreeNode>> atoms = atomTreeController->selectedAtomTreeNodes();
    sort(atoms.begin(), atoms.end(), [](std::shared_ptr<SKAtomTreeNode> node1, std::shared_ptr<SKAtomTreeNode> node2) -> bool {
                 return node1->indexPath() > node2->indexPath();});
    std::vector<IndexPath> atomSelection;
    std::transform(atoms.begin(),atoms.end(),std::back_inserter(atomSelection),[](std::shared_ptr<SKAtomTreeNode> node) -> IndexPath
                      {return node->indexPath();});

    std::vector<std::shared_ptr<SKAsymmetricBond>> bonds = bondSetController->selectedObjects();
    std::set<int> bondSelection = bondSetController->selectionIndexSet();
    AtomTreeViewDeleteSelectionCommand *deleteSelectionCommand = new AtomTreeViewDeleteSelectionCommand(_atomModel, _bondModel, _mainWindow, structure, atoms, atomSelection, bonds, bondSelection);
    _iRASPAProject->undoManager().push(deleteSelectionCommand);
  }
}

void AtomTreeView::addAtom()
{
  if(_iraspaStructure)
  {
    if(AtomTreeViewModel* pModel = qobject_cast<AtomTreeViewModel*>(model()))
    {
      QModelIndex index = currentIndex();
      QModelIndex parentIndex = index.isValid() ? index.parent() : QModelIndex();
      int row = index.isValid() ? index.row() + 1 : pModel->rowCount(index);
      SKAtomTreeNode *parentNode = pModel->nodeForIndex(parentIndex);

      AtomTreeViewInsertAtomCommand *insertAtomCommand = new AtomTreeViewInsertAtomCommand(_mainWindow, this, _iraspaStructure->structure(), parentNode->shared_from_this(), row, nullptr);
      _iRASPAProject->undoManager().push(insertAtomCommand);
    }
  }
}

void AtomTreeView::addAtomGroup()
{
  if(_iraspaStructure)
  {
    if(AtomTreeViewModel* pModel = qobject_cast<AtomTreeViewModel*>(model()))
    {
      QModelIndex index = currentIndex();
      QModelIndex parentIndex = index.isValid() ? index.parent() : QModelIndex();
      int row = index.isValid() ? index.row() + 1 : pModel->rowCount(index);
      SKAtomTreeNode *parentNode = pModel->nodeForIndex(parentIndex);

      AtomTreeViewInsertAtomGroupCommand *insertAtomCommand = new AtomTreeViewInsertAtomGroupCommand(_mainWindow, this, _iraspaStructure->structure(), parentNode->shared_from_this(), row, nullptr);
      _iRASPAProject->undoManager().push(insertAtomCommand);
    }
  }
}

void AtomTreeView::addAtomGroup(QModelIndex index)
{
  if(_iraspaStructure)
  {
    if(AtomTreeViewModel* pModel = qobject_cast<AtomTreeViewModel*>(model()))
    {
      QModelIndex parentIndex = index.isValid() ? index.parent() : QModelIndex();
      int row = index.isValid() ? index.row() + 1 : pModel->rowCount(index);
      SKAtomTreeNode *parentNode = pModel->nodeForIndex(parentIndex);

      AtomTreeViewInsertAtomGroupCommand *insertAtomCommand = new AtomTreeViewInsertAtomGroupCommand(_mainWindow, this, _iraspaStructure->structure(), parentNode->shared_from_this(), row, nullptr);
      _iRASPAProject->undoManager().push(insertAtomCommand);
    }
  }
}

void AtomTreeView::flattenHierachy()
{

}

void AtomTreeView::makeSuperCell()
{
  if(_projectTreeNode)
  {
    if(_projectTreeNode->isEditable())
    {
      if(std::shared_ptr<iRASPAProject> iRASPAProject = _projectTreeNode->representedObject())
      {
        if(std::shared_ptr<Project> project = iRASPAProject->project())
        {
          if (std::shared_ptr<ProjectStructure> projectStructure = std::dynamic_pointer_cast<ProjectStructure>(project))
          {
            std::set<SKAsymmetricAtom> asymmetricAtoms = std::set<SKAsymmetricAtom>();
            std::vector<IndexPath> atomSelection = std::vector<IndexPath>();
            std::vector<SKAsymmetricBond> asymmetricBonds = std::vector<SKAsymmetricBond>();
            std::set<int> bondSelection = std::set<int>();
            //DeleteSelectionCommand *deleteSelectionCommand = new DeleteSelectionCommand(structure,asymmetricAtoms,atomSelection,asymmetricBonds,bondSelection);
            //iRASPAProject->undoManager().push(deleteSelectionCommand);
          }
        }
      }
    }
  }
}

void AtomTreeView::ShowContextMenu(const QPoint &pos)
{
  QModelIndex index = indexAt(pos);

  QMenu contextMenu(tr("Context menu"), nullptr);

  bool isEnabled = false;
  if(_iraspaStructure)
  {
    isEnabled = _projectTreeNode->isEditable()  && (model()->rowCount(QModelIndex()) > 0);
  }

  QAction actionAddAtomGroup("Add Atom Group", this);
  actionAddAtomGroup.setEnabled(isEnabled);
  connect(&actionAddAtomGroup, &QAction::triggered, [this, index](void) {
     this->addAtomGroup(index);
  });
  contextMenu.addAction(&actionAddAtomGroup);

  QAction actionFlattenHierarchy("Flatten hierarchy", this);
  actionFlattenHierarchy.setEnabled(false);
  connect(&actionFlattenHierarchy, &QAction::triggered, this, &AtomTreeView::flattenHierachy);
  contextMenu.addAction(&actionFlattenHierarchy);

  QAction actionMakeSuperCell("Make super-cell", this);
  actionMakeSuperCell.setEnabled(false);
  connect(&actionMakeSuperCell, &QAction::triggered, this, &AtomTreeView::makeSuperCell);
  contextMenu.addAction(&actionMakeSuperCell);

  QMenu* subMenuSelection = contextMenu.addMenu( "Selection" );
  QActionGroup* selectionGroup = new QActionGroup(this);
  QAction actionSelectionInvert("Invert", this);
  actionSelectionInvert.setEnabled(false);
  selectionGroup->addAction(&actionSelectionInvert);
  subMenuSelection->addAction(&actionSelectionInvert);
  //connect(&actionSelectionInvert, &QAction::triggered, this, &AtomTreeView::invertSelection);
  QAction actionCopyToNewMovie("CopyToNewMovie", this);
  actionCopyToNewMovie.setEnabled(false);
  selectionGroup->addAction(&actionCopyToNewMovie);
  subMenuSelection->addAction(&actionCopyToNewMovie);
  //connect(&actionSelectionInvert, &QAction::triggered, this, &AtomTreeView::invertSelection);
  QAction actionMoveToNewMovie("CopyToNewMovie", this);
  actionMoveToNewMovie.setEnabled(false);
  selectionGroup->addAction(&actionMoveToNewMovie);
  subMenuSelection->addAction(&actionMoveToNewMovie);
  //connect(&actionSelectionInvert, &QAction::triggered, this, &AtomTreeView::invertSelection);

  QMenu* subMenuVisibility = contextMenu.addMenu( "Visibility" );
  QActionGroup* visibilityGroup = new QActionGroup(this);
  QAction actionVisibilityMatchSelection("Match selection", this);
  actionVisibilityMatchSelection.setEnabled(false);
  visibilityGroup->addAction(&actionVisibilityMatchSelection);
  subMenuVisibility->addAction(&actionVisibilityMatchSelection);
  //connect(&actionSelectionInvert, &QAction::triggered, this, &AtomTreeView::invertSelection);
  QAction actionVisibilityInvert("Invert", this);
  actionVisibilityInvert.setEnabled(false);
  visibilityGroup->addAction(&actionVisibilityInvert);
  subMenuVisibility->addAction(&actionVisibilityInvert);
  //connect(&actionSelectionInvert, &QAction::triggered, this, &AtomTreeView::invertSelection);

  QMenu* subMenuScrollTo = contextMenu.addMenu( "Visibility" );
  QActionGroup* scrollToGroup = new QActionGroup(this);
  QAction actionScrollToTop("Top", this);
  actionScrollToTop.setEnabled(false);
  scrollToGroup->addAction(&actionScrollToTop);
  subMenuScrollTo->addAction(&actionScrollToTop);
  //connect(&actionSelectionInvert, &QAction::triggered, this, &AtomTreeView::invertSelection);
  QAction actionScrollToBottom("Bottom", this);
  actionScrollToBottom.setEnabled(false);
  scrollToGroup->addAction(&actionScrollToBottom);
  subMenuScrollTo->addAction(&actionScrollToBottom);
  //connect(&actionSelectionInvert, &QAction::triggered, this, &AtomTreeView::invertSelection);
  QAction actionScrollToFirstSelected("First selected", this);
  actionScrollToFirstSelected.setEnabled(false);
  scrollToGroup->addAction(&actionScrollToFirstSelected);
  subMenuScrollTo->addAction(&actionScrollToFirstSelected);
  //connect(&actionSelectionInvert, &QAction::triggered, this, &AtomTreeView::invertSelection);
  QAction actionScrollToLastSelected("Last selected", this);
  actionScrollToLastSelected.setEnabled(false);
  scrollToGroup->addAction(&actionScrollToLastSelected);
  subMenuScrollTo->addAction(&actionScrollToLastSelected);
  //connect(&actionSelectionInvert, &QAction::triggered, this, &AtomTreeView::invertSelection);


  QMenu* subMenuExport = contextMenu.addMenu( "Export to" );
  QActionGroup* exportToGroup = new QActionGroup(this);
  QAction actionExportToPDB("PDB", this);
  actionExportToPDB.setEnabled(false);
  exportToGroup->addAction(&actionExportToPDB);
  subMenuExport->addAction(&actionExportToPDB);
  connect(&actionExportToPDB, &QAction::triggered, this, &AtomTreeView::exportToPDB);
  QAction actionExportToMMCIF("mmCIF", this);
  actionExportToMMCIF.setEnabled(false);
  exportToGroup->addAction(&actionExportToMMCIF);
  subMenuExport->addAction(&actionExportToMMCIF);
  connect(&actionExportToMMCIF, &QAction::triggered, this, &AtomTreeView::exportToMMCIF);
  QAction actionExportToCIF("CIF", this);
  actionExportToCIF.setEnabled(false);
  exportToGroup->addAction(&actionExportToCIF);
  subMenuExport->addAction(&actionExportToCIF);
  connect(&actionExportToCIF, &QAction::triggered, this, &AtomTreeView::exportToCIF);
  QAction actionExportToXYZ("XYZ", this);
  actionExportToXYZ.setEnabled(false);
  exportToGroup->addAction(&actionExportToXYZ);
  subMenuExport->addAction(&actionExportToXYZ);
  connect(&actionExportToXYZ, &QAction::triggered, this, &AtomTreeView::exportToXYZ);
  QAction actionExportToPOSCAR("VASP POSCAR", this);
  actionExportToPOSCAR.setEnabled(false);
  exportToGroup->addAction(&actionExportToPOSCAR);
  subMenuExport->addAction(&actionExportToPOSCAR);
  connect(&actionExportToPOSCAR, &QAction::triggered, this, &AtomTreeView::exportToPOSCAR);

  contextMenu.exec(viewport()->mapToGlobal(pos));
}

void AtomTreeView::exportToPDB() const
{

}

void AtomTreeView::exportToMMCIF() const
{

}

void AtomTreeView::exportToCIF() const
{

}

void AtomTreeView::exportToXYZ() const
{

}

void AtomTreeView::exportToPOSCAR() const
{

}


