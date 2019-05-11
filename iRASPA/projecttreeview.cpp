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

#include "projecttreeview.h"

ProjectTreeView::ProjectTreeView(QWidget* parent): QTreeView(parent ), _model(std::make_shared<ProjectTreeViewModel>())
{
  this->setModel(_model.get());
  this->setHeaderHidden(true);
  this->setStyleSheet("background-color:rgb(240, 240, 240);");

  this->setSelectionBehavior (QAbstractItemView::SelectRows);
  this->setSelectionMode(QAbstractItemView::SingleSelection);

  setDragEnabled(true);
  setAcceptDrops(true);
  setDropIndicatorShown(true);
  setDragDropMode(DragDropMode::InternalMove);
  setDragDropOverwriteMode(false);
  //setDefaultDropAction(Qt::MoveAction);

  setItemsExpandable(true);
  setRootIsDecorated(false);

  this->setContextMenuPolicy(Qt::CustomContextMenu);
  QObject::connect(this, &QWidget::customContextMenuRequested, this, &ProjectTreeView::ShowContextMenu);

  ProjectTreeViewModel* pModel = qobject_cast<ProjectTreeViewModel*>(model());
  //QObject::connect(selectionModel(), &QItemSelectionModel::selectionChanged, this, &ProjectTreeView::setSelectedProjects);

  _dropIndicatorRect = QRect();
}

void ProjectTreeView::paintEvent(QPaintEvent *event)
{
  QPainter painter(viewport());
  drawTree(&painter, event->region());
  paintDropIndicator(painter);
}


void ProjectTreeView::paintDropIndicator(QPainter& painter)
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

QAbstractItemView::DropIndicatorPosition ProjectTreeView::position(QPoint pos, QRect rect, QModelIndex index)
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

void ProjectTreeView::startDrag(Qt::DropActions supportedActions)
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
      reloadSelection();
    }
  }
}


void ProjectTreeView::dragMoveEvent(QDragMoveEvent *event)
{
  QPoint pos = event->pos();
  QModelIndex index = indexAt(mapFromParent(pos));

  //std::cout << "row:" << index.row() << ", " << index.column() << ", " << index.parent().internalPointer() << ", " << index.parent().isValid()  << std::endl;

  ProjectTreeViewModel* pModel = qobject_cast<ProjectTreeViewModel*>(model());
  ProjectTreeNode* item = pModel->getItem(index);
  if(!item->isDropEnabled())
  {
    _dropIndicatorRect = QRect();
    event->ignore();
    viewport()->update();
    return;
  }

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



void ProjectTreeView::reloadSelection()
{
    ProjectTreeViewModel* pModel = qobject_cast<ProjectTreeViewModel*>(model());
    whileBlocking(selectionModel())->clearSelection();

    std::cout << "porject selection: " << pModel->selectedIndexes().size() << std::endl;

    for(QModelIndex index : pModel->selectedIndexes())
    {
      whileBlocking(selectionModel())->select(index, QItemSelectionModel::Select|QItemSelectionModel::Rows);
    }
    viewport()->update();
/*
  QModelIndex index = this->selectionModel()->currentIndex();
  if(ProjectTreeNode* item = static_cast<ProjectTreeNode*>(index.internalPointer()))
  {
    _mainWindow->propagateProject(item->shared_from_this(),_mainWindow);
  }*/
}

void ProjectTreeView::reloadData()
{

}

void ProjectTreeView::setController(std::shared_ptr<ProjectTreeController> treeController)
{
  if(_projectTreeController != treeController)
  {
    _projectTreeController = treeController;
    _model->setProjectTreeController(treeController);
  }
}

bool ProjectTreeView::insertRows(int position, int rows, const QModelIndex &parent, std::shared_ptr<ProjectTreeNode> item)
{
	ProjectTreeViewModel* pModel = qobject_cast<ProjectTreeViewModel*>(model());
	if (pModel)
	{
		return pModel->insertRows(position, rows, parent, item);
	}
	return false;
}


void ProjectTreeView::setSelectedProject(const QModelIndex& current, const QModelIndex& previous)
{

  QModelIndex index = this->selectionModel()->currentIndex();

  if(index.isValid())
  {
    qDebug() << "Select project" << index.internalPointer();
    if(ProjectTreeNode* item = static_cast<ProjectTreeNode*>(index.internalPointer()))
    {
      qDebug() << "Inside " << item->displayName();
      _projectTreeController->selectedTreeNodes().clear();
      _projectTreeController->selectedTreeNodes().insert(item->shared_from_this());
      item->representedObject()->unwrapIfNeeded();
      _mainWindow->propagateProject(item->shared_from_this(),_mainWindow);
    }
  }
}

QSize ProjectTreeView::sizeHint() const
{
  return QSize(200, 800);
}

void ProjectTreeView::addGroupProject(QModelIndex index)
{
  ProjectTreeViewModel* pModel = qobject_cast<ProjectTreeViewModel*>(model());

  ProjectTreeNode *item = pModel->getItem(index);

  if(item->isDropEnabled())
  {
    std::shared_ptr<ProjectGroup> project = std::make_shared<ProjectGroup>();
    std::shared_ptr<iRASPAProject>  iraspaproject = std::make_shared<iRASPAProject>(project);

    pModel->insertRows(index.row(), 1, index.parent(), true, iraspaproject);
  }
}

void ProjectTreeView::ShowContextMenu(const QPoint &pos)
{
  QModelIndex index = indexAt(pos);

  QMenu contextMenu(tr("Context menu"), this);

  QAction actionAddStructureProject("Add structure project", this);
  connect(&actionAddStructureProject, &QAction::triggered, [this, index](void) {

  });
  contextMenu.addAction(&actionAddStructureProject);


  QAction actionGroupProject("Add group project", this);
  connect(&actionGroupProject, &QAction::triggered, [this, index](void) {
    this->addGroupProject(index);
  });
  contextMenu.addAction(&actionGroupProject);

  contextMenu.exec(viewport()->mapToGlobal(pos));
}

