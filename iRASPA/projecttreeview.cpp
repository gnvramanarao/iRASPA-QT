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
#include "projecttreeviewstyleditemdelegate.h"
#include "projecttreeviewproxystyle.h"

ProjectTreeView::ProjectTreeView(QWidget* parent): QTreeView(parent ),
  _model(std::make_shared<ProjectTreeViewModel>()),
  _undoStack()
{
  this->setModel(_model.get());
  this->setHeaderHidden(true);

  this->setAttribute(Qt::WA_MacShowFocusRect, false);
  this->setStyleSheet("background-color:rgb(240, 240, 240);");
  this->setStyle(new ProjectTreeViewProxyStyle());

  this->setSelectionBehavior (QAbstractItemView::SelectRows);
  this->setSelectionMode(QAbstractItemView::SingleSelection);

  setDragEnabled(true);
  setAcceptDrops(true);
  setDropIndicatorShown(true);
  setDragDropMode(DragDropMode::DragDrop);
  setDragDropOverwriteMode(false);

  setItemsExpandable(true);
  setRootIsDecorated(false);
  setExpandsOnDoubleClick(false);

  this->setContextMenuPolicy(Qt::CustomContextMenu);
  QObject::connect(this, &QWidget::customContextMenuRequested, this, &ProjectTreeView::ShowContextMenu);

  _dropIndicatorRect = QRect();

  this->setItemDelegateForColumn(0,new ProjectTreeViewStyledItemDelegate(this));
}

// Use the general undoManager for changes to the project-treeView.
void ProjectTreeView::focusInEvent( QFocusEvent* )
{
  QAction *newUndoAction = this->undoManager().createUndoAction(this, tr("&Undo"));
  _mainWindow->setUndoAction(newUndoAction);

  QAction *newRedoAction = this->undoManager().createRedoAction(this, tr("&Redo"));
  _mainWindow->setRedoAction(newRedoAction);
}

// Use the undoManger of the project for all changes inside a project. Each project has its own undoManager.
void ProjectTreeView::focusOutEvent( QFocusEvent* )
{
  QModelIndex index = this->selectionModel()->currentIndex();

  if(index.isValid())
  {
    if(ProjectTreeNode* item = static_cast<ProjectTreeNode*>(index.internalPointer()))
    {
       if(std::shared_ptr<iRASPAProject> iraspaProject =  item->representedObject())
       {
         QAction *newUndoAction = iraspaProject->undoManager().createUndoAction(this, tr("&Undo"));
         _mainWindow->setUndoAction(newUndoAction);

         QAction *newRedoAction = iraspaProject->undoManager().createRedoAction(this, tr("&Redo"));
         _mainWindow->setRedoAction(newRedoAction);
       }
    }
  }
}

void ProjectTreeView::paintEvent(QPaintEvent *event)
{
  QTreeView:: paintEvent(event);
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
  setDropIndicatorShown(true);
  QModelIndex index = currentIndex();

  ProjectTreeViewModel* pModel = qobject_cast<ProjectTreeViewModel*>(model());
  if(ProjectTreeNode* item = pModel->getItem(index))
  {
    QModelIndexList selectionIndexes = selectionModel()->selectedRows();
    QMimeData* mimeData = model()->mimeData(selectionIndexes);

    QDrag* drag = new QDrag(this);
    drag->setMimeData(mimeData);

    switch(item->type())
    {
    case ProjectTreeNode::Type::user:
      if(drag->exec(Qt::MoveAction))
      {
        reloadSelection();
      }
      break;
    default:
      if(drag->exec(Qt::CopyAction))
      {
        reloadSelection();
      }
      break;
    }
  }
}

void ProjectTreeView::dragMoveEvent(QDragMoveEvent* event)
{
  QModelIndex index = indexAt(event->pos());

  // use the visualRect of the index to avoid dropping on the background left to items
  if (!index.isValid() || !visualRect(index).contains(event->pos()))
  {
    setDropIndicatorShown(false);
    event->ignore();
    viewport()->update();
  }
  else
  {
    setDropIndicatorShown(true);
    viewport()->update();
    event->accept();
  }

   QTreeView::dragMoveEvent(event);
}


void ProjectTreeView::reloadSelection()
{
  ProjectTreeViewModel* pModel = qobject_cast<ProjectTreeViewModel*>(model());
  if (pModel)
  {
    if(_projectTreeController)
    {
      std::shared_ptr<ProjectTreeNode> project = _projectTreeController->selectedTreeNode();
      QModelIndex index = pModel->indexForItem(project);
      if(index.isValid())
      {
        selectionModel()->clearSelection();
        selectionModel()->select(index, QItemSelectionModel::Select);
        update();
      }
    }
  }
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


void ProjectTreeView::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
  if (selectedIndexes().isEmpty())
  {
    selectionModel()->select(selected, QItemSelectionModel::QItemSelectionModel::Deselect);
    selectionModel()->select(deselected, QItemSelectionModel::QItemSelectionModel::SelectCurrent);
    return;
  }

  QAbstractItemView::selectionChanged(selected, deselected);

  if(selectedIndexes().size() == 1)
  {
    QModelIndex current = selectedIndexes().front();
    if(ProjectTreeNode *selectedTreeNode = static_cast<ProjectTreeNode*>(current.internalPointer()))
    {
      _projectTreeController->selectedTreeNodes().clear();
      _projectTreeController->setSelectedTreeNode(selectedTreeNode->shared_from_this());
      _projectTreeController->selectedTreeNodes().insert(selectedTreeNode->shared_from_this());
      selectedTreeNode->representedObject()->unwrapIfNeeded();

      if(std::shared_ptr<iRASPAProject> iraspa_project = selectedTreeNode->representedObject())
      {
        if(std::shared_ptr<Project> project = iraspa_project->project())
        {
          project->setInitialSelectionIfNeeded();

          // set currentIndex for keyboard navigation
          ProjectTreeViewModel* pModel = qobject_cast<ProjectTreeViewModel*>(model());
          if (pModel)
          {
            QModelIndex index = pModel->indexForItem(selectedTreeNode->shared_from_this());
            selectionModel()->setCurrentIndex(index, QItemSelectionModel::SelectionFlag::Current);
          }
        }
      }


      if(std::shared_ptr<iRASPAProject> iraspa_project = std::dynamic_pointer_cast<iRASPAProject>(selectedTreeNode->representedObject()))
      {
        _mainWindow->propagateProject(selectedTreeNode->shared_from_this(), _mainWindow);
        return;
      }
    }
  }
  _mainWindow->propagateProject(nullptr, _mainWindow);
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
  actionAddStructureProject.setEnabled(false);
  connect(&actionAddStructureProject, &QAction::triggered, [this, index](void) {

  });
  contextMenu.addAction(&actionAddStructureProject);


  QAction actionGroupProject("Add group project", this);
  actionGroupProject.setEnabled(false);
  connect(&actionGroupProject, &QAction::triggered, [this, index](void) {
    this->addGroupProject(index);
  });
  contextMenu.addAction(&actionGroupProject);

  contextMenu.exec(viewport()->mapToGlobal(pos));
}

