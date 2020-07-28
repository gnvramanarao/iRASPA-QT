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
#include "projecttreeviewinsertprojectstructurecommand.h"
#include <QApplication>
#include <QClipboard>
#include <QMimeData>
#include <QFont>
#include <QFontMetrics>

ProjectTreeView::ProjectTreeView(QWidget* parent): iRASPATreeView(parent),
  _model(std::make_shared<ProjectTreeViewModel>()),
  _undoStack()
{
  this->setModel(_model.get());
  this->setHeaderHidden(true);

  this->setAttribute(Qt::WA_MacShowFocusRect, false);
  this->setStyleSheet("background-color:rgb(240, 240, 240);");
  this->setStyle(new ProjectTreeViewProxyStyle());

  this->setSelectionBehavior (QAbstractItemView::SelectRows);
  this->setSelectionMode(QAbstractItemView::ExtendedSelection);

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

void ProjectTreeView::setLogReportingWidget(LogReporting *logReporting)
{
  _logReporting = logReporting;
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

bool ProjectTreeView::hasSelection() const
{
  QModelIndexList selection = selectionModel()->selectedIndexes();
  for(QModelIndex index : selection)
  {
    if(ProjectTreeNode *selectedTreeNode = static_cast<ProjectTreeNode*>(index.internalPointer()))
    {
      if(std::shared_ptr<iRASPAProject> iraspa_project = selectedTreeNode->representedObject())
      {
        if(std::shared_ptr<ProjectStructure> structureProject = std::dynamic_pointer_cast<ProjectStructure>(iraspa_project->project()))
        {
          return true;
        }
      }
    }
  }
  return false;
}

void ProjectTreeView::paintEvent(QPaintEvent *event)
{
  QTreeView:: paintEvent(event);
}


QPixmap ProjectTreeView::selectionToPixmap()
{
  QModelIndexList selectionIndexes = selectionModel()->selectedRows();
  QMimeData* mimeData = model()->mimeData(selectionIndexes);

  QDrag* drag = new QDrag(this);
  drag->setMimeData(mimeData);

  QFontMetrics fontMetrics = QFontMetrics(this->font());

  if(ProjectTreeViewModel* pModel = qobject_cast<ProjectTreeViewModel*>(model()))
  {
    int maxWidth=0;
    int height=4;

    for(QModelIndex index: selectionIndexes)
    {
      if(ProjectTreeNode* item = pModel->getItem(index))
      {
        QString text = item->displayName();
        QRect textBoundingRect = fontMetrics.boundingRect(text);
        if(textBoundingRect.width() > maxWidth) maxWidth = textBoundingRect.size().width();
        height += textBoundingRect.size().height();
      }
    }

    QRect rect = QRect(QPoint(0,0), QSize(maxWidth, height));
    QPixmap pix = QPixmap(maxWidth, height);
    pix.fill(Qt::transparent);

    QPainter painter( &pix );
    painter.setFont(this->font());
    painter.setOpacity(1.0);
    painter.setPen(Qt::black);

    int currentHeight=0;
    for(QModelIndex index: selectionIndexes)
    {
      if(ProjectTreeNode* item = pModel->getItem(index))
      {
        QString text = item->displayName();
        QRect rect = fontMetrics.boundingRect(text);
        currentHeight += rect.size().height();

        painter.save();
        painter.translate(QPointF(0,currentHeight));
        painter.drawText(rect,Qt::AlignLeft|Qt::AlignCenter, text);
        painter.restore();
      }
    }
    return pix;
  }

  return QPixmap();
}

void ProjectTreeView::startDrag(Qt::DropActions supportedActions)
{
  setDropIndicatorShown(true);
  QModelIndex index = currentIndex();

  ProjectTreeViewModel* pModel = qobject_cast<ProjectTreeViewModel*>(model());
  if(ProjectTreeNode* item = pModel->getItem(index))
  {
    QModelIndexList selectionIndexes = selectionModel()->selectedRows();
    QMimeData* mimeData = pModel->mimeDataLazy(selectionIndexes);

    QDrag* drag = new QDrag(this);
    drag->setMimeData(mimeData);

    drag->setPixmap(selectionToPixmap());

    if(drag->exec(supportedActions))
    {
      reloadSelection();
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
      std::set<std::shared_ptr<ProjectTreeNode>> selectedProjects = _projectTreeController->selectedTreeNodes();
      whileBlocking(selectionModel())->clearSelection();
      if(selectedProjects.size()>0)
      {
        for(std::shared_ptr<ProjectTreeNode> project: selectedProjects)
        {
          QModelIndex index = pModel->indexForItem(project);
          selectionModel()->select(index, QItemSelectionModel::Select);
        }
      }
      if(std::shared_ptr<ProjectTreeNode> project = _projectTreeController->selectedTreeNode())
      {
        QModelIndex index = pModel->indexForItem(project);
        selectionModel()->select(index, QItemSelectionModel::Select);
        selectionModel()->setCurrentIndex(index, QItemSelectionModel::Select);
      }
    }
    update();
  }
}

void ProjectTreeView::reloadData()
{
  _mainWindow->updateMenuToProjectTab();
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
  // avoid empty selection
  if (selectedIndexes().isEmpty())
  {
    selectionModel()->select(selected, QItemSelectionModel::QItemSelectionModel::Deselect);
    selectionModel()->select(deselected, QItemSelectionModel::QItemSelectionModel::SelectCurrent);
    return;
  }

  QAbstractItemView::selectionChanged(selected, deselected);

  if(_projectTreeController)
  {
    _projectTreeController->selectedTreeNodes().clear();

    if(selectedIndexes().size() == 1)
    {
      QModelIndex current = selectedIndexes().front();
      if(ProjectTreeNode *selectedTreeNode = static_cast<ProjectTreeNode*>(current.internalPointer()))
      {
        _projectTreeController->setSelectedTreeNode(selectedTreeNode->shared_from_this());
        _projectTreeController->selectedTreeNodes().insert(selectedTreeNode->shared_from_this());
        selectedTreeNode->representedObject()->unwrapIfNeeded();

        if(std::shared_ptr<iRASPAProject> iraspa_project = selectedTreeNode->representedObject())
        {
          if(std::shared_ptr<Project> project = iraspa_project->project())
          {
            project->setInitialSelectionIfNeeded();
            _mainWindow->propagateProject(selectedTreeNode->shared_from_this(), _mainWindow);
          }
        }
      }
    }

    // loop over all selected indexes
    for(QModelIndex index : selectedIndexes())
    {
      if(ProjectTreeNode *selectedTreeNode = static_cast<ProjectTreeNode*>(index.internalPointer()))
      {
        _projectTreeController->selectedTreeNodes().insert(selectedTreeNode->shared_from_this());
      }
    }

    ProjectTreeViewModel* pModel = qobject_cast<ProjectTreeViewModel*>(model());
    QModelIndex selectedIndex = pModel->indexForItem(_projectTreeController->selectedTreeNode());
    if(selectedIndexes().empty())
    {
      _mainWindow->propagateProject(nullptr, _mainWindow);
    }
    else
    {
      // if the main selected project is unselected, pick another as the main
      if(!selectedIndexes().contains(selectedIndex))
      {
        selectedIndex = selectedIndexes().front();
        if(ProjectTreeNode *selectedTreeNode = static_cast<ProjectTreeNode*>(selectedIndex.internalPointer()))
        {
          _projectTreeController->setSelectedTreeNode(selectedTreeNode->shared_from_this());

          selectedTreeNode->representedObject()->unwrapIfNeeded();

          if(std::shared_ptr<iRASPAProject> iraspa_project = selectedTreeNode->representedObject())
          {
            if(std::shared_ptr<Project> project = iraspa_project->project())
            {
              project->setInitialSelectionIfNeeded();
              _mainWindow->propagateProject(selectedTreeNode->shared_from_this(), _mainWindow);
            }
          }
        }
      }
    }

    // set currentIndex for keyboard navigation
    if (selectedIndex.isValid())
    {
      selectionModel()->setCurrentIndex(selectedIndex, QItemSelectionModel::SelectionFlag::Current);
    }

    update();
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

    //pModel->insertRows(index.row(), 1, index.parent(), true, iraspaproject);
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

void ProjectTreeView::keyPressEvent(QKeyEvent * event)
{
  if(event->matches(QKeySequence::Copy) )
  {
    copy();
  }
  else if(event->matches(QKeySequence::Paste) )
  {
    paste();
  }
  else if (event->matches(QKeySequence::Cut))
  {
    cut();
  }
  else
  {
    QTreeView::keyPressEvent(event);
  }
}

void ProjectTreeView::copy()
{
  if(ProjectTreeViewModel* pModel = qobject_cast<ProjectTreeViewModel*>(model()))
  {
    if(QClipboard *clipboard = QApplication::clipboard())
    {
      QModelIndexList indexes = this->selectionModel()->selectedIndexes();

      if(indexes.isEmpty())
        return;

      std::sort(indexes.begin(), indexes.end());

      QMimeData *mimeData = pModel->mimeData(indexes);
      clipboard->setMimeData(mimeData);
    }
  }
}

void ProjectTreeView::paste()
{
  if(ProjectTreeViewModel* pModel = qobject_cast<ProjectTreeViewModel*>(model()))
  {
    if(qApp->clipboard()->mimeData()->hasFormat(ProjectTreeNode::mimeType))
    {
      const QMimeData *mimeData = qApp->clipboard()->mimeData();

      QModelIndex currentIndex = selectionModel()->currentIndex();
      if(currentIndex.isValid())
      {
        if(pModel->canDropMimeData(mimeData,Qt::CopyAction, 0, 0, currentIndex))
        {
          pModel->pasteMimeData(mimeData, 0, 0, currentIndex);
        }
        else
        {
          if(ProjectTreeNode *projectTreeNode = pModel->getItem(currentIndex.parent()))
          {
            _logReporting->logMessage(LogReporting::ErrorLevel::warning, "Not allowed to paste into container " + projectTreeNode->displayName());
          }
        }
      }
    }
  }
}

void ProjectTreeView::cut()
{

}

void ProjectTreeView::insertProjectStructure()
{
  qDebug() << "ProjectTreeView::insertProjectStructure";
  if(_projectTreeController)
  {
    if(ProjectTreeViewModel* pModel = qobject_cast<ProjectTreeViewModel*>(model()))
    {
      std::shared_ptr<ProjectTreeNode> parent = _projectTreeController->localProjects();
      QModelIndex parentIndex = pModel->indexForItem(parent);
      if(parentIndex.isValid())
      {
        ProjectTreeViewInsertProjectStructureCommand *insertProjectCommand = new ProjectTreeViewInsertProjectStructureCommand(_mainWindow, this, parent, 0, nullptr);
        undoManager().push(insertProjectCommand);
      }
    }
  }
}

