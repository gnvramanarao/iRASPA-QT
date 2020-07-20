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
  if(_sceneList)
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
  if(_sceneList)
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
  }
  return QModelIndex();
}



// Returns the index of the item in the model specified by the given row, column and parent index.
QModelIndex SceneTreeViewModel::index(int row, int column, const QModelIndex &parent) const
{
  Q_UNUSED(column);

  if(_sceneList)
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
      if(scene->movies().size() == 0)
        return QModelIndex();
      Movie* movie = scene->movies()[row].get();
      return createIndex(row, 0, movie);
    }
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
  if(DisplayableProtocol* indexItem = static_cast<DisplayableProtocol*>(index.internalPointer()))
  {
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
  }

  return QModelIndex();
}


int SceneTreeViewModel::rowCount(const QModelIndex &parent) const
{
  if(_sceneList)
  {
    if(parent == QModelIndex())
    {
      return static_cast<int>(_sceneList->scenes().size());
    }

    if(DisplayableProtocol *parentItem = static_cast<DisplayableProtocol*>(parent.internalPointer()))
    {
      if(SceneList* parentSceneList = dynamic_cast<SceneList*>(parentItem))
      {
        return static_cast<int>(parentSceneList->scenes().size());
      }
      if(Scene* scene = dynamic_cast<Scene*>(parentItem))
      {
        return static_cast<int>(scene->movies().size());
      }
    }
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

  if(DisplayableProtocol *item = static_cast<DisplayableProtocol*>(index.internalPointer()))
  {
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
  }
  return false;
}


Qt::ItemFlags SceneTreeViewModel::flags(const QModelIndex &index) const
{
  if (!index.isValid()) return Qt::NoItemFlags;

  Qt::ItemFlags flags = Qt::NoItemFlags;

  DisplayableProtocol *item = static_cast<DisplayableProtocol*>(index.internalPointer());
  if(Scene* scene = dynamic_cast<Scene*>(item))
  {
    flags |= Qt::ItemIsDropEnabled;
    return flags;
  }

  if(Movie* movie = dynamic_cast<Movie*>(item))
  {
    flags |= Qt::ItemIsEnabled;
    flags |= Qt::ItemIsSelectable;
    flags |= Qt::ItemIsUserCheckable;
    flags |= Qt::ItemIsDragEnabled;
    return flags;
  }

  return Qt::NoItemFlags;
}

bool SceneTreeViewModel::hasChildren(const QModelIndex &parent) const
{
  if (!parent.isValid()) return true;

  DisplayableProtocol *item = static_cast<DisplayableProtocol*>(parent.internalPointer());
  if(Movie* movie = dynamic_cast<Movie*>(item))
  {
    return false;
  }
  return true;
}

bool SceneTreeViewModel::removeRows(int position, int rows, const QModelIndex &parent)
{
  DisplayableProtocol *item = static_cast<DisplayableProtocol*>(parent.internalPointer());

  if(Scene* scene = dynamic_cast<Scene*>(item))
  {
    if (position < 0 || position > static_cast<int>(scene->movies().size()))
      return false;

    beginRemoveRows(parent, position, position + rows - 1);
    for (int row = 0; row < rows; ++row)
    {
      if (!scene->removeChild(position))
      {
        qDebug() << "error removing " << position;
        break;
      }
    }
    endRemoveRows();

    return true;
  }
  return false;
}


QStringList SceneTreeViewModel::mimeTypes() const
{
  return QAbstractItemModel::mimeTypes() << Movie::mimeType;
}


Qt::DropActions SceneTreeViewModel::supportedDropActions() const
{
  return Qt::CopyAction | Qt::MoveAction;
}

Qt::DropActions SceneTreeViewModel::supportedDragActions() const
{
  return Qt::CopyAction | Qt::MoveAction;
}


QMimeData* SceneTreeViewModel::mimeData(const QModelIndexList &indexes) const
{
  QByteArray encodedData;
  QDataStream stream(&encodedData, QIODevice::WriteOnly);

  QModelIndexList sortedIndexes = indexes;
  std::sort(sortedIndexes.begin(), sortedIndexes.end());

  stream << QCoreApplication::applicationPid();
  stream << sortedIndexes.count();

  for(const QModelIndex &index: sortedIndexes)
  {
    if(index.isValid())
    {
      DisplayableProtocol *item = static_cast<DisplayableProtocol*>(index.internalPointer());
      if(Movie* movie = dynamic_cast<Movie*>(item))
      {
        qulonglong ptrval(reinterpret_cast<qulonglong>(movie));
        stream << ptrval;
      }
    }
  }
  QMimeData *mimeData = new QMimeData();
  mimeData->setData(Movie::mimeType, encodedData);

  return mimeData;
}

bool SceneTreeViewModel::canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const
{
  Q_UNUSED(data);
  Q_UNUSED(action);
  Q_UNUSED(row);
  Q_UNUSED(column);


  return true;
}

// drops onto existing items have row and column set to -1 and parent set to the current item
bool SceneTreeViewModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
  Q_UNUSED(column);

  if(action == Qt::IgnoreAction)
  {
    return true;
  }
  if(!data->hasFormat(Movie::mimeType))
  {
    return false;
  }

  QByteArray ddata = data->data(Movie::mimeType);
  QDataStream stream(&ddata, QIODevice::ReadOnly);

  qint64 senderPid;
  stream >> senderPid;
  if (senderPid != QCoreApplication::applicationPid())
  {
    return false;
  }

  DisplayableProtocol *item = static_cast<DisplayableProtocol*>(parent.internalPointer());
  Scene* scene = dynamic_cast<Scene*>(item);

  qDebug() << "scene: " << scene;

    qDebug() << "parent is: " << item->displayName();


  int count;
  stream >> count;

  qDebug() << "COUNT DRAG: " << count;


  int beginRow = row;
  if (beginRow == -1)
  {
    if (parent.isValid())
      beginRow = 0;
    else
      beginRow = rowCount(parent);
  }

  if(action == Qt::DropAction::CopyAction)
  {
    qDebug() << "Qt::DropAction::CopyAction";
    emit layoutAboutToBeChanged();
    bool oldState = this->blockSignals(true);
    while (!stream.atEnd())
    {
      qlonglong nodePtr;
      stream >> nodePtr;
      std::shared_ptr<ProjectTreeNode> copiedProjectTreeNode = std::make_shared<ProjectTreeNode>();
      stream >>= copiedProjectTreeNode;
      copiedProjectTreeNode->setIsEditable(true);
      copiedProjectTreeNode->setIsDropEnabled(true);

      beginInsertRows(parent, beginRow, beginRow);
      //if (!scene->insertChild(beginRow, copiedProjectTreeNode))
      //  break;
      endInsertRows();
      ++beginRow;
    }
    this->blockSignals(oldState);
  }
  else if(action == Qt::DropAction::MoveAction)
  {
    qDebug() << "Qt::DropAction::MoveAction";
    emit layoutAboutToBeChanged();
    bool oldState = this->blockSignals(true);
    while (!stream.atEnd())
    {
      int localRow;
      qlonglong nodePtr;
      stream >> nodePtr;
      Movie *node = reinterpret_cast<Movie *>(nodePtr);

      if(true)
      {
        std::shared_ptr<Movie> movieNode = node->shared_from_this();

        std::shared_ptr<Scene> parentScene = parentForMovie(movieNode);
        std::optional<int> row = parentScene->findChildIndex(movieNode);
        if(row)
        {
          localRow = *row;
          QModelIndex index = createIndex(localRow,0, node);

          removeRows(localRow,1,index.parent());


          // Adjust destination row for the case of moving an item
          // within the same parent, to a position further down.
          // Its own removal will reduce the final row number by one.
          if (index.row() < beginRow && (parentScene.get() == scene))
          {
             --beginRow;
          }

          beginInsertRows(parent, beginRow, beginRow);
          if (!scene->insertChild(beginRow, movieNode))
          {
            break;
          }
          endInsertRows();
        }
      }
      else
      {
        qDebug() << "YEAH";
        //std::shared_ptr<ProjectTreeNode> copiedTreeNode = node->shallowClone();
        //copiedTreeNode->setIsEditable(true);
        //copiedTreeNode->setIsDropEnabled(true);
        beginInsertRows(parent, beginRow, beginRow);
        //if (!parentNode->insertChild(beginRow, copiedTreeNode))
        //  break;
        endInsertRows();
      }

      ++beginRow;
    }
    this->blockSignals(oldState);
  }

  emit layoutChanged();


  return true;
}
