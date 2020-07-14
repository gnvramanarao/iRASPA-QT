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

#include "projecttreeviewmodel.h"

char ProjectTreeViewModel::mimeType[] = "application/x-qt-iraspa-project-mime";

ProjectTreeViewModel::ProjectTreeViewModel(): _projectTreeController(std::make_shared<ProjectTreeController>())
{
}

void ProjectTreeViewModel::setProjectTreeController(std::shared_ptr<ProjectTreeController> controller)
{
  if(_projectTreeController != controller)
  {
    beginResetModel();
    _projectTreeController = controller;
    endResetModel();
  }
}

QModelIndex ProjectTreeViewModel::indexForItem(std::shared_ptr<ProjectTreeNode> item)
{
  if(item)
  {
    std::shared_ptr<ProjectTreeNode> parent = item->parent();
    if(parent)
    {
      std::vector<std::shared_ptr<ProjectTreeNode>>::iterator it = std::find(parent->childNodes().begin(), parent->childNodes().end(), item);
      if (it != parent->childNodes().end())
      {
        int childIndex = std::distance(parent->childNodes().begin(), it);
        return createIndex(childIndex,0,item.get());
      }
    }
  }
  return QModelIndex();
}

ProjectTreeNode *ProjectTreeViewModel::getItem(const QModelIndex &index) const
{
  if (index.isValid())
  {
     ProjectTreeNode *item = static_cast<ProjectTreeNode*>(index.internalPointer());
     if (item)
     {
       return item;
     }
  }
  return _projectTreeController->hiddenRootNode().get();
}

QModelIndexList ProjectTreeViewModel::selectedIndexes()
{
  QModelIndexList list = QModelIndexList();
  for(std::shared_ptr<ProjectTreeNode> node : _projectTreeController->selectedTreeNodes())
  {
    int localRow = node->row();
    QModelIndex index = createIndex(localRow,0,node.get());
    list.push_back(index);
  }
  return list;
}

bool ProjectTreeViewModel::hasChildren(const QModelIndex &parent) const
{
  if (!parent.isValid()) return true;

  ProjectTreeNode *item = getItem(parent);
  return item->representedObject()->isGroup();
}

bool ProjectTreeViewModel::removeRows(int position, int rows, const QModelIndex &parent)
{
  ProjectTreeNode *parentItem = getItem(parent);

  if (position < 0 || position > static_cast<int>(parentItem->childNodes().size()))
    return false;

  beginRemoveRows(parent, position, position + rows - 1);
  for (int row = 0; row < rows; ++row)
  {
    if (!parentItem->removeChild(position))
      break;
  }
  endRemoveRows();

  return true;
}

bool ProjectTreeViewModel::insertRows(int position, int rows, const QModelIndex &parent, std::shared_ptr<ProjectTreeNode> item)
{
	ProjectTreeNode *parentItem = getItem(parent);

	beginInsertRows(parent, position, position + rows - 1);
	for (int row = 0; row < rows; ++row)
	{
		if (!parentItem->insertChild(position, item))
			break;
	}
	endInsertRows();

	return true;
}

bool ProjectTreeViewModel::insertRows(int position, int rows, const QModelIndex &parent)
{
  ProjectTreeNode *parentItem = getItem(parent);

  beginInsertRows(parent, position, position + rows - 1);
  for (int row = 0; row < rows; ++row)
  {
    std::shared_ptr<ProjectStructure> project = std::make_shared<ProjectStructure>();
    std::shared_ptr<iRASPAProject>  iraspaproject = std::make_shared<iRASPAProject>(project);
    std::shared_ptr<ProjectTreeNode> newItem = std::make_shared<ProjectTreeNode>("New project", iraspaproject, true, true);

    if (!parentItem->insertChild(position, newItem))
      break;
  }
  endInsertRows();

  return true;
}

bool ProjectTreeViewModel::insertRows(int position, int rows, const QModelIndex &parent, bool isGroup, std::shared_ptr<iRASPAProject> project)
{
    ProjectTreeNode *parentItem = getItem(parent);

    std::cout << "insert, parent is: " << parentItem->displayName().toStdString() << std::endl;
    std::cout << "position: " << position << ", " << rows << std::endl;

    beginInsertRows(parent, position, position + rows - 1);
    for (int row = 0; row < rows; ++row)
    {
      std::shared_ptr<ProjectTreeNode> newItem = std::make_shared<ProjectTreeNode>("New project", project, true, true);

      if (!parentItem->insertChild(position, newItem))
        break;
    }
    endInsertRows();

    std::cout << "CHILDCOUNT: " << parentItem->childCount() << std::endl;

    return true;
  }


QModelIndex ProjectTreeViewModel::index(int row, int column, const QModelIndex &parent) const
{
  if(!hasIndex(row, column, parent))
     return QModelIndex();

  if(_projectTreeController->rootNodes().empty())
    return QModelIndex();

  ProjectTreeNode *parentItem;

  if (!parent.isValid())
    parentItem = _projectTreeController->hiddenRootNode().get();
  else
    parentItem = static_cast<ProjectTreeNode*>(parent.internalPointer());

  std::shared_ptr<ProjectTreeNode> childItem = parentItem->getChildNode(row);
  if (childItem)
    return createIndex(row, column, childItem.get());
  else
    return QModelIndex();
}

QModelIndex ProjectTreeViewModel::parent(const QModelIndex &index) const
{
  if (!index.isValid())
    return QModelIndex();

  ProjectTreeNode *childItem = static_cast<ProjectTreeNode*>(index.internalPointer());
  ProjectTreeNode *parentItem = childItem->parent().get();

  if(!parentItem) return QModelIndex();

  if (parentItem == _projectTreeController->hiddenRootNode().get())
    return QModelIndex();

  return createIndex(parentItem->row(), 0, parentItem);
}

int ProjectTreeViewModel::columnCount(const QModelIndex &parent) const
{
  return 1;
}


int ProjectTreeViewModel::rowCount(const QModelIndex &parent) const
{
  ProjectTreeNode *parentItem;

  if (!parent.isValid())
    parentItem = _projectTreeController->hiddenRootNode().get();
  else
    parentItem = static_cast<ProjectTreeNode*>(parent.internalPointer());

  return static_cast<int>(parentItem->childCount());
}

QVariant ProjectTreeViewModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  if (role == Qt::TextColorRole)
  {
    if(index.parent() == QModelIndex())
    {
      return QColor(114,114,114,255);
    }
  }

  if (role == Qt::FontRole)
  {
    if(index.parent() == QModelIndex())
    {
      QFont font;// = QFont("Lucida Grande", 12, QFont::Bold, false);
      font.setFamily("Lucida Grande");
      font.setBold(true);
      return font;
    }
  }

  if (role != Qt::DisplayRole)
    return QVariant();

  ProjectTreeNode *item = static_cast<ProjectTreeNode*>(index.internalPointer());
  return item->displayName();
}


Qt::ItemFlags ProjectTreeViewModel::flags(const QModelIndex &index) const
{
  if (!index.isValid()) return Qt::NoItemFlags;

  Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

  if ( index.column() == 0 )
  {
    flags |= Qt::ItemIsEditable;
  }

  if(ProjectTreeNode *node = getItem(index))
  {
    if(node->representedObject()->isLeaf())
    {
      flags |= Qt::ItemIsDragEnabled;
    }
    if(node->representedObject()->isGroup() && node->isDropEnabled())
    {
      flags |= Qt::ItemIsDropEnabled;
    }
  }

  return flags;
}


bool ProjectTreeViewModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  if (role != Qt::EditRole)
    return false;

  ProjectTreeNode *item = getItem(index);
  bool result = item->setData(value);

  if (result)
    emit dataChanged(index, index);

  return result;
}

bool ProjectTreeViewModel::setData(const QModelIndex &index, std::shared_ptr<iRASPAProject> value, bool isGroup)
{
  ProjectTreeNode *item = getItem(index);
  item->representedObject()->setIsGroup(isGroup);
  item->setRepresentedObject(value);

  emit dataChanged(index, index);

  return true;
}

Qt::DropActions ProjectTreeViewModel::supportedDropActions() const
{
  return Qt::CopyAction | Qt::MoveAction;
}

Qt::DropActions ProjectTreeViewModel::supportedDragActions() const
{
   return Qt::CopyAction | Qt::MoveAction;
}

QStringList ProjectTreeViewModel::mimeTypes() const
{
  return QAbstractItemModel::mimeTypes() << mimeType;
}

QMimeData* ProjectTreeViewModel::mimeData(const QModelIndexList &indexes) const
{
  QByteArray encodedData;
  QDataStream stream(&encodedData, QIODevice::WriteOnly);

  QModelIndexList indexes2 = indexes;
  qSort(indexes2.begin(), indexes2.end(), qGreater<QModelIndex>());

  stream << QCoreApplication::applicationPid();
  stream << indexes2.count();

  for(const QModelIndex &index: indexes)
  {
    if(index.isValid())
    {
      if(ProjectTreeNode *projectTreeNode = getItem(index))
      {
        qulonglong ptrval(reinterpret_cast<qulonglong>(projectTreeNode));
        stream << ptrval;
        //stream << projectTreeNode;
      }
    }
  }
  QMimeData *mimeData = new QMimeData();
  mimeData->setData(mimeType, encodedData);

  return mimeData;
}


// drops onto existing items have row and column set to -1 and parent set to the current item
bool ProjectTreeViewModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
  Q_UNUSED(column);

  if(action == Qt::IgnoreAction)
  {
    return true;
  }
  if(!data->hasFormat(mimeType))
  {
    return false;
  }

  QByteArray ddata = data->data(mimeType);
  QDataStream stream(&ddata, QIODevice::ReadOnly);

  ProjectTreeNode *parentNode = getItem(parent);

  qint64 senderPid;
  stream >> senderPid;
  if (senderPid != QCoreApplication::applicationPid())
  {
    // Let's not cast pointers that come from another process...
    return false;
  }

  int count;
  stream >> count;

  int beginRow = row;
  if (beginRow == -1)
  {
    // valid index means: drop onto item. I chose that this should insert
    // a child item, because this is the only way to create the first child of an item...
    // This explains why Qt calls it parent: unless you just support replacing, this
    // is really the future parent of the dropped items.
    if (parent.isValid())
      beginRow = 0;
    else
      // invalid index means: append at bottom, after last toplevel
      beginRow = rowCount(parent);
  }

  if(action == Qt::DropAction::CopyAction)
  {
    qDebug() << "Qt::DropAction::CopyAction";
    // insertRows(beginRow,count,parent);
    emit layoutAboutToBeChanged();
    bool oldState = this->blockSignals(true);
    while (!stream.atEnd())
    {
      int localRow;
      qlonglong nodePtr;
      stream >> nodePtr;
      ProjectTreeNode *node = reinterpret_cast<ProjectTreeNode *>(nodePtr);
      std::shared_ptr<ProjectTreeNode> copiedProjectTreeNode = node->shallowClone();
      copiedProjectTreeNode->setIsEditable(true);
      copiedProjectTreeNode->setIsDropEnabled(true);

      beginInsertRows(parent, beginRow, beginRow);
      if (!parentNode->insertChild(beginRow, copiedProjectTreeNode))
        break;
      endInsertRows();
      ++beginRow;
    }
    this->blockSignals(oldState);
  }
  else
  {
    qDebug() << "Qt::DropAction::MoveAction";
    emit layoutAboutToBeChanged();
    bool oldState = this->blockSignals(true);
    while (!stream.atEnd())
    {
      int localRow;
      qlonglong nodePtr;
      stream >> nodePtr;
      ProjectTreeNode *node = reinterpret_cast<ProjectTreeNode *>(nodePtr);
      std::shared_ptr<ProjectTreeNode> atom = node->shared_from_this();

      localRow = node->row();
      QModelIndex index = createIndex(localRow,0,atom.get());

      removeRows(localRow,1,index.parent());


      // Adjust destination row for the case of moving an item
      // within the same parent, to a position further down.
      // Its own removal will reduce the final row number by one.
      if (index.row() < beginRow && (parentNode == atom->parent().get()))
      {
        std::cout << "Decrease beginRow" << std::endl;
         --beginRow;
      }

      beginInsertRows(parent, beginRow, beginRow);
      if (!parentNode->insertChild(beginRow, atom))
        break;
      endInsertRows();
      ++beginRow;
    }
    this->blockSignals(oldState);
  }

  emit layoutChanged();

  return true;
}
