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

#include "scenetreeview.h"
#include "scenetreeviewstyleditemdelegate.h"
#include "scenetreeviewproxystyle.h"
#include <QModelIndexList>

SceneTreeView::SceneTreeView(QWidget* parent): QTreeView(parent ), _model(std::make_shared<SceneTreeViewModel>())
{
  this->setModel(_model.get());

  this->setSelectionBehavior (QAbstractItemView::SelectRows);
  this->setSelectionMode(QAbstractItemView::ExtendedSelection);
  //this->viewport()->setMouseTracking(true);

  this->setRootIsDecorated(false);
  this->setItemsExpandable(false);
  this->setHeaderHidden(true);
  this->setIndentation(4);

  this->setAttribute(Qt::WA_MacShowFocusRect, false);
  this->setStyleSheet("background-color:rgb(240, 240, 240);");
  this->setStyle(new SceneTreeViewProxyStyle());

  this->setItemDelegateForColumn(0,new SceneTreeViewStyledItemDelegate(this));
}

void SceneTreeView::setProject(std::shared_ptr<ProjectTreeNode> projectTreeNode)
{
  _projectTreeNode = projectTreeNode;
  if (projectTreeNode)
  {
    if(std::shared_ptr<iRASPAProject> iraspaProject = projectTreeNode->representedObject())
    {
      if(std::shared_ptr<Project> project = iraspaProject->project())
      {
        if (std::shared_ptr<ProjectStructure> projectStructure = std::dynamic_pointer_cast<ProjectStructure>(project))
        {
          _sceneList = projectStructure->getSceneTreeModel();
          _model->setSceneList(_sceneList);
          expandAll();
        }
      }
     }
  }
}


QSize SceneTreeView::sizeHint() const
{
  return QSize(200, 800);
}

void SceneTreeView::reloadSelection()
{
  if(_sceneList)
  {
    std::optional<int> sceneIndex = _sceneList->selectedSceneIndex();
    if(sceneIndex)
    {
      QModelIndex parentItem = model()->index(*sceneIndex,0,QModelIndex());

      std::optional<int> movieIndex = _sceneList->selectedScene()->selectMovieIndex();

      if(movieIndex)
      {
        QModelIndex selectedMovie = model()->index(*movieIndex,0,parentItem);

        whileBlocking(selectionModel())->clearSelection();
        whileBlocking(selectionModel())->select(selectedMovie, QItemSelectionModel::Select);
      }
    }

    // set currentIndex for keyboard navigation
    SceneTreeViewModel* pModel = qobject_cast<SceneTreeViewModel*>(model());
    QModelIndex selectedIndex = pModel->indexOfMainSelected();
    if (selectedIndex.isValid())
    {
      whileBlocking(selectionModel())->setCurrentIndex(selectedIndex, QItemSelectionModel::SelectionFlag::Current);
    }

    update();
  }
}

void SceneTreeView::reloadData()
{

}

void SceneTreeView::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
  if (selectedIndexes().isEmpty())
  {
    selectionModel()->select(selected, QItemSelectionModel::QItemSelectionModel::Deselect);
    selectionModel()->select(deselected, QItemSelectionModel::QItemSelectionModel::SelectCurrent);
    return;
  }

  QAbstractItemView::selectionChanged(selected, deselected);

  // clear scene-movie selection
  _sceneList->selectedScenes().clear();
  for(std::shared_ptr<Scene> scene : _sceneList->scenes())
  {
    scene->selectedMovies().clear();
  }

  if(selectedIndexes().size() == 1)
  {
    QModelIndex current = selectedIndexes().front();
    DisplayableProtocol *item = static_cast<DisplayableProtocol*>(current.internalPointer());

    if(Movie* movie = dynamic_cast<Movie*>(item))
    {
      QModelIndex parentItem = current.parent();
      DisplayableProtocol *item = static_cast<DisplayableProtocol*>(parentItem.internalPointer());

      if(Scene* scene = dynamic_cast<Scene*>(item))
      {
        _sceneList->setSelectedScene(scene->shared_from_this());
        scene->setSelectedMovie(movie->shared_from_this());
      }

      emit setSelectedMovie(movie->shared_from_this());
      emit setSelectedFrame(movie->selectedFrame());
    }
  }

  // loop over all selected indexes
  for(QModelIndex index : selectedIndexes())
  {
    DisplayableProtocol *item = static_cast<DisplayableProtocol*>(index.internalPointer());

    if(Movie* movie = dynamic_cast<Movie*>(item))
    {
      QModelIndex parentItem = index.parent();
      DisplayableProtocol *item = static_cast<DisplayableProtocol*>(parentItem.internalPointer());

      if(Scene* scene = dynamic_cast<Scene*>(item))
      {
        _sceneList->selectedScenes().insert(scene->shared_from_this());
        scene->selectedMovies().insert(movie->shared_from_this());
      }
    }
  }

  // set currentIndex for keyboard navigation
  SceneTreeViewModel* pModel = qobject_cast<SceneTreeViewModel*>(model());
  QModelIndex selectedIndex = pModel->indexOfMainSelected();
  if (selectedIndex.isValid())
  {
    selectionModel()->setCurrentIndex(selectedIndex, QItemSelectionModel::SelectionFlag::Current);
  }

  emit setSelectedRenderFrames(_sceneList->selectediRASPARenderStructures());
  emit setFlattenedSelectedFrames(_sceneList->selectedMoviesiRASPAStructures());
  emit updateRenderer();

  update();
}




QString SceneTreeView::nameOfItem(const QModelIndex &current)
{
  DisplayableProtocol *item = static_cast<DisplayableProtocol*>(current.internalPointer());

  if(Movie* movie = dynamic_cast<Movie*>(item))
  {
    QModelIndex parentItem = current.parent();
    DisplayableProtocol *item = static_cast<DisplayableProtocol*>(parentItem.internalPointer());

    if(Scene* scene = dynamic_cast<Scene*>(item))
    {
      _sceneList->setSelectedScene(scene->shared_from_this());
      scene->setSelectedMovie(movie->shared_from_this());
      return movie->displayName();
    }
  }
  return QString("unknown");
}
