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
#include <QModelIndexList>

SceneTreeView::SceneTreeView(QWidget* parent): QTreeView(parent ), _model(std::make_shared<SceneTreeViewModel>())
{
  this->setModel(_model.get());

  this->setSelectionBehavior (QAbstractItemView::SelectRows);
  this->setSelectionMode(QAbstractItemView::ExtendedSelection);
  //this->viewport()->setMouseTracking(true);

  this->setAttribute(Qt::WA_MacShowFocusRect, false);

  this->setRootIsDecorated(false);
  this->setItemsExpandable(false);
  this->setHeaderHidden(true);
  this->setIndentation(4);

  this->setStyleSheet("background-color:rgb(240, 240, 240);");

  QObject::connect(selectionModel(),&QItemSelectionModel::currentRowChanged,this, &SceneTreeView::currentMovieChanged);

  //QObject::connect(selectionModel(),&QItemSelectionModel::selectionChanged,this, &SceneTreeView::synchronizeSelection);

}


void SceneTreeView::setRootNode(std::shared_ptr<SceneList> sceneList)
{
  _sceneList = sceneList;
  _model->setSceneList(sceneList);
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

        selectionModel()->clearSelection();
        selectionModel()->select(selectedMovie, QItemSelectionModel::Select);
        emit setCellTreeController(_sceneList->selectedScene()->selectedMovie()->structures());
        update();
      }
    }
  }
}

void SceneTreeView::reloadData()
{

}

void SceneTreeView::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
   if (selectedIndexes().isEmpty())
   {
     std::cout << "AVOID EMPTY SELECTION" << std::endl;
     selectionModel()->select(selected, QItemSelectionModel::QItemSelectionModel::Deselect);
     selectionModel()->select(deselected, QItemSelectionModel::QItemSelectionModel::SelectCurrent);
     //QTreeView::selectionChanged(deselected, selected);
     //update();
     return;
   }

   QAbstractItemView::selectionChanged(selected, deselected);

   // synchronize selection
   _sceneList->clearSelection();
   for(QModelIndex &selectedIndex: selectedIndexes())
   {
     DisplayableProtocol *item = static_cast<DisplayableProtocol*>(selectedIndex.internalPointer());

     if(Movie* movie = dynamic_cast<Movie*>(item))
     {
       QModelIndex parentItem = selectedIndex.parent();
       DisplayableProtocol *item = static_cast<DisplayableProtocol*>(parentItem.internalPointer());

       if(Scene* scene = dynamic_cast<Scene*>(item))
       {
         _sceneList->selectedScenes().insert(scene->shared_from_this());
         scene->selectedMovies().insert(movie->shared_from_this());
       }
     }
   }
   std::vector<std::shared_ptr<Structure>> structures = _sceneList->selectedStructures();
   std::cout << "selected structures under movies: " << structures.size() << std::endl;
   emit setCellTreeController(structures);
   emit setAppearanceTreeController(structures);
}

/*
void SceneTreeView::synchronizeSelection(const QItemSelection &selected, const QItemSelection &deselected)
{
  std::cout << "Selection changed!! total selected items: " << this->selectedIndexes().size() <<  std::endl;

  DisplayableProtocol *item = static_cast<DisplayableProtocol*>(this->selectionModel()->currentIndex().internalPointer());

  if(Movie* movie = dynamic_cast<Movie*>(item))
  {
    QModelIndex parentItem = this->selectionModel()->currentIndex().parent();
    DisplayableProtocol *item = static_cast<DisplayableProtocol*>(parentItem.internalPointer());

    if(Scene* scene = dynamic_cast<Scene*>(item))
    {
      //_sceneList->setSelectedScene(scene->shared_from_this());
      //scene->setSelectedMovie(movie->shared_from_this());

      // std::cout << "current changed to: " << movie->displayName().toStdString() << std::endl;
    }
  }

  for (int i = 0; i < selected.indexes().size(); i++)
  {
      QModelIndex index = selected.indexes()[i];
      if(index.isValid())
      {
        std::cout << "selected rows: " << index.row() << " name: " <<  this->nameOfItem(index).toStdString() << std::endl;
      }
      else
      {
        std::cout << "selected Invalid index" << std::endl;
      }
  }
  for (int i = 0; i < deselected.indexes().size(); i++)
  {
    QModelIndex index = deselected.indexes()[i];
    if(index.isValid())
    {
      std::cout << "deselected rows: " << index.row() << " name: " <<  this->nameOfItem(index).toStdString() << std::endl;
    }
    else
    {
      std::cout << "deselected Invalid index" << std::endl;
    }
  }

  std::cout << std::endl << std::endl;


}
*/

void SceneTreeView::currentMovieChanged(const QModelIndex &current)
{
  //return;

  DisplayableProtocol *item = static_cast<DisplayableProtocol*>(current.internalPointer());

  if(Movie* movie = dynamic_cast<Movie*>(item))
  {
    QModelIndex parentItem = current.parent();
    DisplayableProtocol *item = static_cast<DisplayableProtocol*>(parentItem.internalPointer());

    if(Scene* scene = dynamic_cast<Scene*>(item))
    {
      _sceneList->setSelectedScene(scene->shared_from_this());
      scene->setSelectedMovie(movie->shared_from_this());

      emit setSelectedMovie(movie->shared_from_this());
      emit setCellTreeController(movie->structures());
      emit setAtomTreeController(movie->selectedFrame()->structure()->atomsTreeController());
    }
  }
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
       // std::cout << "current changed to: " << movie->displayName().toStdString() << std::endl;
    }
  }
  return QString("unknown");
}
