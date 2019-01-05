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

AtomTreeView::AtomTreeView(QWidget* parent): QTreeView(parent ), _model(std::make_shared<AtomTreeViewModel>())
{
  _atomTreeController = std::make_shared<SKAtomTreeController>();
  this->setModel(_model.get());

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

  QObject::connect(selectionModel(), &QItemSelectionModel::selectionChanged, this, &AtomTreeView::setSelectedAtoms);


  _dropIndicatorRect = QRect();
}

void AtomTreeView::setProject(std::shared_ptr<ProjectTreeNode> projectTreeNode)
{
  if (projectTreeNode)
  {
    if(std::shared_ptr<iRASPAProject> iraspaProject = projectTreeNode->representedObject())
    {
      if(std::shared_ptr<Project> project = iraspaProject->project())
      {
        if (std::shared_ptr<ProjectStructure> projectStructure = std::dynamic_pointer_cast<ProjectStructure>(project))
        {
           std::cout << "setProject : " << projectTreeNode->displayName().toStdString() << std::endl;
          _projectStructure = projectStructure;
          _structures = projectStructure->flattenedStructures();
          //reloadData();
          return;
        }
      }
    }
  }
  _projectStructure = nullptr;
}

SKAtomTreeNode* AtomTreeView::getItem(const QModelIndex &index) const
{
  if (index.isValid())
  {
     SKAtomTreeNode *item = static_cast<SKAtomTreeNode*>(index.internalPointer());
     if (item)
     {
       return item;
     }
  }
  return _atomTreeController->hiddenRootNode().get();
}

void AtomTreeView::setSelectedAtoms(const QItemSelection &selected, const QItemSelection &deselected)
{
  for(QModelIndex index : deselected.indexes())
  {
    SKAtomTreeNode *node = getItem(index);
    std::shared_ptr<SKAtomTreeNode> atom = node->shared_from_this();
    _atomTreeController->selectedTreeNodes().erase(atom);
  }

  for(QModelIndex index : selected.indexes())
  {
    SKAtomTreeNode *node = getItem(index);
    std::shared_ptr<SKAtomTreeNode> atom = node->shared_from_this();
    _atomTreeController->selectedTreeNodes().insert(atom);
  }

  emit rendererReloadData();
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

  std::cout << "SELECTED NODEs: " << _atomTreeController->selectedTreeNodes().size() << std::endl;
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

      std::cout << "DRAG: " << _structures.size() << std::endl;
      emit invalidateCachedAmbientOcclusionTexture(std::vector<std::shared_ptr<RKRenderStructure>>{_structures.begin(), _structures.end()});
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
  AtomTreeViewModel* pModel = qobject_cast<AtomTreeViewModel*>(model());
  whileBlocking(selectionModel())->clearSelection();

  for(QModelIndex index : pModel->selectedIndexes())
  {
    whileBlocking(selectionModel())->select(index, QItemSelectionModel::Select|QItemSelectionModel::Rows);
  }
  viewport()->update();
}

void AtomTreeView::reloadData()
{
  if(_model->atomTreeController() != _atomTreeController)
  {
    _model->setAtomTreeController(_atomTreeController);

    if(_atomTreeController->rootNodes().size() > 0)
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



void AtomTreeView::setRootNode(std::shared_ptr<SKAtomTreeController> treeController)
{
  _atomTreeController = treeController;
}


void AtomTreeView::setAtomTreeModel(const QModelIndex &current, const QModelIndex &previous)
{
  DisplayableProtocol *item = static_cast<DisplayableProtocol*>(current.internalPointer());

  if(iRASPAStructure* structure = dynamic_cast<iRASPAStructure*>(item))
  {
    _model->setAtomTreeController(structure->structure()->atomsTreeController());
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

void AtomTreeView::addAtomGroup(QModelIndex index)
{
  model()->insertRows(index.row(), 1, index.parent());

  QModelIndex index2 = model()->index(index.row(),0,index.parent());
  SKAtomTreeNode *item = static_cast<SKAtomTreeNode*>(index2.internalPointer());
  item->setGroupItem(true);
  item->setDisplayName("New atom group");
  setFirstColumnSpanned(index2.row(),index2.parent(), true);

  emit rendererReloadData();
}

void AtomTreeView::flattenHierachy()
{

}

void AtomTreeView::makeSuperCell()
{

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

  contextMenu.exec(viewport()->mapToGlobal(pos));
}

