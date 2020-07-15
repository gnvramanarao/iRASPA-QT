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

#include "scenetreeviewmodel.h"
#include <optional>

SceneTreeViewModel::SceneTreeViewModel(): _sceneList(std::make_shared<SceneList>())
{

}

void SceneTreeViewModel::setSceneList(std::shared_ptr<SceneList> sceneList)
{
  beginResetModel();
  _sceneList = sceneList;
  endResetModel();
}


std::shared_ptr<Scene> SceneTreeViewModel::parentForMovie(const std::shared_ptr<Movie> movie) const
{
  for(std::shared_ptr<Scene> currentScene: _sceneList->scenes())
  {
    for(std::shared_ptr<Movie> currentMovie: currentScene->movies())
    {
      if (currentMovie == movie)
      {
        return currentScene;
      }
    }
  }
  return nullptr;
}

bool SceneTreeViewModel::isMainSelectedItem(std::shared_ptr<Movie> movie)
{
  if(_sceneList)
  {
    if(std::shared_ptr<Scene> selectedScene = _sceneList->selectedScene())
    {
      return selectedScene->selectedMovie() == movie;
    }
  }
  return false;
}

QModelIndex SceneTreeViewModel::indexOfMainSelected() const
{
  if(std::shared_ptr<Scene> selectedScene = _sceneList->selectedScene())
  {
    if(std::shared_ptr<Movie> selectedMovie = selectedScene->selectedMovie())
    {
      if (std::optional<int> row = selectedScene->findChildIndex(selectedMovie))
      {
        return createIndex(*row, 0, selectedMovie.get());
      }
    }
  }
  return QModelIndex();
}



// Returns the index of the item in the model specified by the given row, column and parent index.
QModelIndex SceneTreeViewModel::index(int row, int column, const QModelIndex &parent) const
{
  if(_sceneList->scenes().empty())
    return QModelIndex();

  if (!parent.isValid())
  {
    return createIndex(row, 0, _sceneList->scenes()[row].get());
  }

  DisplayableProtocol* parentItem = static_cast<DisplayableProtocol*>(parent.internalPointer());
  if(dynamic_cast<SceneList*>(parentItem))
  {
    Scene* scene = _sceneList->scenes()[row].get();
    return createIndex(row, 0, scene);
  }

  if(Scene* scene = dynamic_cast<Scene*>(parentItem))
  {
    Movie* movie = scene->movies()[row].get();
    return createIndex(row, 0, movie);
  }

  return QModelIndex();
}


QModelIndex SceneTreeViewModel::parent(const QModelIndex &index) const
{
  if (!index.isValid())
    return QModelIndex();

  if(!index.internalPointer())
  {
    return QModelIndex();
  }

  // pointer to the sceneList is the root
  DisplayableProtocol* indexItem = static_cast<DisplayableProtocol*>(index.internalPointer());
  if(SceneList* sceneListItem = dynamic_cast<SceneList*>(indexItem))
  {
    return QModelIndex();
  }

  if(Scene* scene = dynamic_cast<Scene*>(indexItem))
  {
    return QModelIndex();
  }

  if(Movie* movie = dynamic_cast<Movie*>(indexItem))
  {
    std::shared_ptr<Scene> scene = parentForMovie(movie->shared_from_this());


    std::optional<int> row = scene->findChildIndex(movie->shared_from_this());

    if(row)
    {
      return createIndex(*row,0,scene.get());
    }
  }

  // should not reach this
  return QModelIndex();
}


int SceneTreeViewModel::rowCount(const QModelIndex &parent) const
{
  if(parent == QModelIndex())
  {
    return static_cast<int>(_sceneList->scenes().size());
  }

  DisplayableProtocol *parentItem = static_cast<DisplayableProtocol*>(parent.internalPointer());
  if(SceneList* parentSceneList = dynamic_cast<SceneList*>(parentItem))
  {
    return static_cast<int>(parentSceneList->scenes().size());
  }
  if(Scene* scene = dynamic_cast<Scene*>(parentItem))
  {
    return static_cast<int>(scene->movies().size());
  }
  return 0;
}

int SceneTreeViewModel::columnCount(const QModelIndex &parent) const
{
  return 1;
}

QVariant SceneTreeViewModel::data(const QModelIndex &index, int role) const
{

  if (!index.isValid())
   return QVariant();

  DisplayableProtocol *item = static_cast<DisplayableProtocol*>(index.internalPointer());

  if (role == Qt::CheckStateRole)
  {
    if(Movie* movie = dynamic_cast<Movie*>(item))
    {
      return movie->isVisible() ? Qt::Checked : Qt::Unchecked;
    }
    return QVariant();
  }

  if (role == Qt::TextColorRole)
  {
    DisplayableProtocol *item = static_cast<DisplayableProtocol*>(index.internalPointer());
    if(Scene* scene = dynamic_cast<Scene*>(item))
    {
      return QColor(114,114,114,255);
    }
  }

  if (role == Qt::FontRole)
  {
    if(Scene* scene = dynamic_cast<Scene*>(item))
    {
      QFont font = QFont();
      font.setFamily("Lucida Grande");
      font.setBold(true);
      return font;
    }
  }

  if (role != Qt::DisplayRole)
   return QVariant();


  if(Movie* movie = dynamic_cast<Movie*>(item))
  {
    //return QString("movie");
    return movie->displayName();
  }
  if(Scene* scene = dynamic_cast<Scene*>(item))
  {
    //  return QString("test scene");
    return scene->displayName().toUpper();
  }
  return QString(item->displayName());
}

bool SceneTreeViewModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  if (!index.isValid() /*|| role != Qt::EditRole*/)
    return false;

  DisplayableProtocol *item = static_cast<DisplayableProtocol*>(index.internalPointer());
  if(Movie* movie = dynamic_cast<Movie*>(item))
  {
    if (role == Qt::CheckStateRole)
    {
      std::shared_ptr<Scene> scene = parentForMovie(movie->shared_from_this());
      std::vector<std::shared_ptr<Structure>> structures = scene->structures();
      if ((Qt::CheckState)value.toInt() == Qt::Checked)
      {
        movie->setVisibility(true);
        emit invalidateCachedAmbientOcclusionTexture(std::vector<std::shared_ptr<RKRenderStructure>>{structures.begin(), structures.end()});
        emit rendererReloadData();
        return true;
      }
      else
      {
        movie->setVisibility(false);
        emit invalidateCachedAmbientOcclusionTexture(std::vector<std::shared_ptr<RKRenderStructure>>{structures.begin(), structures.end()});
        emit rendererReloadData();
        return true;
      }
    }
  }
  return false;
}


Qt::ItemFlags SceneTreeViewModel::flags(const QModelIndex &index) const
{
  if (!index.isValid()) return Qt::NoItemFlags;

  DisplayableProtocol *item = static_cast<DisplayableProtocol*>(index.internalPointer());
  if(Scene* scene = dynamic_cast<Scene*>(item))
  {
    return Qt::NoItemFlags;
  }

  if(Movie* movie = dynamic_cast<Movie*>(item))
  {
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
  }

  return Qt::NoItemFlags;
}

bool SceneTreeViewModel::hasChildren(const QModelIndex &parent) const
{
  if (!parent.isValid()) return true;

  DisplayableProtocol *item = static_cast<DisplayableProtocol*>(parent.internalPointer());
  if(Movie* scene = dynamic_cast<Movie*>(item))
  {
    return false;
  }
  return true;
}
