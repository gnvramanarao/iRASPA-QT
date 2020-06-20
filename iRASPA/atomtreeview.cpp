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
#include "atomtreeviewdeleteselectioncommand.h"
#include "atomchangeselectioncommand.h"

AtomTreeView::AtomTreeView(QWidget* parent): QTreeView(parent ), _atomModel(std::make_shared<AtomTreeViewModel>())
{
  this->setModel(_atomModel.get());

  QObject::connect(model(),&QAbstractItemModel::modelReset, this, &AtomTreeView::reloadSelection);

  //_atomTreeController = std::make_shared<SKAtomTreeController>();

  this->viewport()->setMouseTracking(true);

  this->setHeaderHidden(true);
  this->setAlternatingRowColors(true);
  //this->setSelectionMode(QAbstractItemView::MultiSelection);
  this->setSelectionBehavior (QAbstractItemView::SelectRows);
  this->setSelectionMode(QAbstractItemView::ExtendedSelection);
  //this->setStyleSheet("background-color:rgb(240, 240, 240);");

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

  QObject::connect(selectionModel(), &QItemSelectionModel::selectionChanged, this, &AtomTreeView::setSelectedAtoms);
}

void AtomTreeView::setBondModel(std::shared_ptr<BondListViewModel> bondModel)
{
  _bondModel = bondModel;
}

void AtomTreeView::setProject(std::shared_ptr<ProjectTreeNode> projectTreeNode)
{
  _projectTreeNode = projectTreeNode;
  _iRASPAProject = nullptr;
  _iraspaStructure.reset();

  if (projectTreeNode)
  {
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
  this->reloadData();
}

void AtomTreeView::setSelectedFrame(std::shared_ptr<iRASPAStructure> iraspastructure)
{
  _iraspaStructure = iraspastructure;

  if(AtomTreeViewModel* atomModel = qobject_cast<AtomTreeViewModel*>(model()))
  {

    if(std::shared_ptr<iRASPAStructure> iraspaStructure = _iraspaStructure.lock())
    {
      std::shared_ptr<Structure> structure = iraspaStructure->structure();
      std::shared_ptr<SKAtomTreeController> atomTreeControler = structure->atomsTreeController();
      atomModel->setAtomTreeController(atomTreeControler);
    }
  }
}

SKAtomTreeNode* AtomTreeView::getItem(const QModelIndex &index) const
{
  if(std::shared_ptr<iRASPAStructure> iraspaStructure = _iraspaStructure.lock())
  {
    std::shared_ptr<Structure> structure = iraspaStructure->structure();
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


void AtomTreeView::setSelectedAtoms(const QItemSelection &selected, const QItemSelection &deselected)
{
  if(std::shared_ptr<iRASPAStructure> iraspaStructure = _iraspaStructure.lock())
  {
    std::shared_ptr<Structure> structure = iraspaStructure->structure();
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

  std::cout << "row:" << index.row() << ", " << index.column() << ", " << index.parent().internalPointer() <<  std::endl;

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
  if(AtomTreeViewModel* pModel = qobject_cast<AtomTreeViewModel*>(model()))
  {
    whileBlocking(selectionModel())->clearSelection();

    for(QModelIndex index : pModel->selectedIndexes())
    {
      whileBlocking(selectionModel())->select(index, QItemSelectionModel::Select|QItemSelectionModel::Rows);
    }
    viewport()->update();
  }
}

void AtomTreeView::reloadData()
{
  if(std::shared_ptr<iRASPAStructure> iraspaStructure = _iraspaStructure.lock())
  {
    std::shared_ptr<Structure> structure = iraspaStructure->structure();
    std::shared_ptr<SKAtomTreeController> atomTreeController = structure->atomsTreeController();

    if(_atomModel->atomTreeController() != atomTreeController)
    {
      _atomModel->setAtomTreeController(atomTreeController);

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
}


void AtomTreeView::setAtomTreeModel(const QModelIndex &current, const QModelIndex &previous)
{
  DisplayableProtocol *item = static_cast<DisplayableProtocol*>(current.internalPointer());

  if(iRASPAStructure* structure = dynamic_cast<iRASPAStructure*>(item))
  {
    _atomModel->setAtomTreeController(structure->structure()->atomsTreeController());
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
  //selectionModel()->select(droppedIndex, QItemSelectionModel::Select|QItemSelectionModel::Rows);

 // viewport()->update();

}



QSize AtomTreeView::sizeHint() const
{
  return QSize(500, 800);
}

void AtomTreeView::deleteSelection()
{
  if(std::shared_ptr<iRASPAStructure> iraspaStructure = _iraspaStructure.lock())
  {
    std::shared_ptr<Structure> structure = iraspaStructure->structure();
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

void AtomTreeView::addAtomGroup(QModelIndex index)
{
  if(std::shared_ptr<iRASPAStructure> iraspaStructure = _iraspaStructure.lock())
  {
    std::shared_ptr<Structure> structure = iraspaStructure->structure();

    model()->insertRows(index.row(), 1, index.parent());

    QModelIndex index2 = model()->index(index.row(),0,index.parent());
    SKAtomTreeNode *item = static_cast<SKAtomTreeNode*>(index2.internalPointer());
    item->setGroupItem(true);
    item->setDisplayName("New atom group");
    setFirstColumnSpanned(index2.row(),index2.parent(), true);

    emit rendererReloadData();
  }
}

void AtomTreeView::flattenHierachy()
{

}

void AtomTreeView::makeSuperCell()
{
  if(std::shared_ptr<ProjectTreeNode> projectTreeNode = _projectTreeNode.lock())
  {
    if(projectTreeNode->isEditable())
    {
      if(std::shared_ptr<iRASPAProject> iRASPAProject = projectTreeNode->representedObject())
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

  QMenu contextMenu(tr("Context menu"), this);

  QAction actionAddAtomGroup("Add Atom Group", this);
  connect(&actionAddAtomGroup, &QAction::triggered, [this, index](void) {
     this->addAtomGroup(index);
  });
  contextMenu.addAction(&actionAddAtomGroup);

  QAction actionFlattenHierarchy("Flatten hierarchy", this);
  connect(&actionFlattenHierarchy, &QAction::triggered, this, &AtomTreeView::flattenHierachy);
  contextMenu.addAction(&actionFlattenHierarchy);

  QAction actionMakeSuperCell("Make super-cell", this);
  connect(&actionMakeSuperCell, &QAction::triggered, this, &AtomTreeView::makeSuperCell);
  contextMenu.addAction(&actionMakeSuperCell);

  QMenu* subMenuSelection = contextMenu.addMenu( "Selection" );
  QActionGroup* selectionGroup = new QActionGroup(this);
  QAction actionSelectionInvert("Invert", this);
  selectionGroup->addAction(&actionSelectionInvert);
  subMenuSelection->addAction(&actionSelectionInvert);
  //connect(&actionSelectionInvert, &QAction::triggered, this, &AtomTreeView::invertSelection);
  QAction actionCopyToNewMovie("CopyToNewMovie", this);
  selectionGroup->addAction(&actionCopyToNewMovie);
  subMenuSelection->addAction(&actionCopyToNewMovie);
  //connect(&actionSelectionInvert, &QAction::triggered, this, &AtomTreeView::invertSelection);
  QAction actionMoveToNewMovie("CopyToNewMovie", this);
  selectionGroup->addAction(&actionMoveToNewMovie);
  subMenuSelection->addAction(&actionMoveToNewMovie);
  //connect(&actionSelectionInvert, &QAction::triggered, this, &AtomTreeView::invertSelection);

  QMenu* subMenuVisibility = contextMenu.addMenu( "Visibility" );
  QActionGroup* visibilityGroup = new QActionGroup(this);
  QAction actionVisibilityMatchSelection("Match selection", this);
  visibilityGroup->addAction(&actionVisibilityMatchSelection);
  subMenuVisibility->addAction(&actionVisibilityMatchSelection);
  //connect(&actionSelectionInvert, &QAction::triggered, this, &AtomTreeView::invertSelection);
  QAction actionVisibilityInvert("Invert", this);
  visibilityGroup->addAction(&actionVisibilityInvert);
  subMenuVisibility->addAction(&actionVisibilityInvert);
  //connect(&actionSelectionInvert, &QAction::triggered, this, &AtomTreeView::invertSelection);

  QMenu* subMenuScrollTo = contextMenu.addMenu( "Visibility" );
  QActionGroup* scrollToGroup = new QActionGroup(this);
  QAction actionScrollToTop("Top", this);
  scrollToGroup->addAction(&actionScrollToTop);
  subMenuScrollTo->addAction(&actionScrollToTop);
  //connect(&actionSelectionInvert, &QAction::triggered, this, &AtomTreeView::invertSelection);
  QAction actionScrollToBottom("Bottom", this);
  scrollToGroup->addAction(&actionScrollToBottom);
  subMenuScrollTo->addAction(&actionScrollToBottom);
  //connect(&actionSelectionInvert, &QAction::triggered, this, &AtomTreeView::invertSelection);
  QAction actionScrollToFirstSelected("First selected", this);
  scrollToGroup->addAction(&actionScrollToFirstSelected);
  subMenuScrollTo->addAction(&actionScrollToFirstSelected);
  //connect(&actionSelectionInvert, &QAction::triggered, this, &AtomTreeView::invertSelection);
  QAction actionScrollToLastSelected("Last selected", this);
  scrollToGroup->addAction(&actionScrollToLastSelected);
  subMenuScrollTo->addAction(&actionScrollToLastSelected);
  //connect(&actionSelectionInvert, &QAction::triggered, this, &AtomTreeView::invertSelection);


  QMenu* subMenuExport = contextMenu.addMenu( "Export to" );
  QActionGroup* exportToGroup = new QActionGroup(this);
  QAction actionExportToPDB("PDB", this);
  exportToGroup->addAction(&actionExportToPDB);
  subMenuExport->addAction(&actionExportToPDB);
  connect(&actionExportToPDB, &QAction::triggered, this, &AtomTreeView::exportToPDB);
  QAction actionExportToMMCIF("mmCIF", this);
  exportToGroup->addAction(&actionExportToMMCIF);
  subMenuExport->addAction(&actionExportToMMCIF);
  connect(&actionExportToMMCIF, &QAction::triggered, this, &AtomTreeView::exportToMMCIF);
  QAction actionExportToCIF("CIF", this);
  exportToGroup->addAction(&actionExportToCIF);
  subMenuExport->addAction(&actionExportToCIF);
  connect(&actionExportToCIF, &QAction::triggered, this, &AtomTreeView::exportToCIF);
  QAction actionExportToXYZ("XYZ", this);
  exportToGroup->addAction(&actionExportToXYZ);
  subMenuExport->addAction(&actionExportToXYZ);
  connect(&actionExportToXYZ, &QAction::triggered, this, &AtomTreeView::exportToXYZ);
  QAction actionExportToPOSCAR("VASP POSCAR", this);
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


