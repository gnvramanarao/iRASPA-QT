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


ProjectTreeViewModel::ProjectTreeViewModel(): _projectTreeController(std::make_shared<ProjectTreeController>())
{
  _folderIcon = QIcon(":/iRASPA/GenericFolderIcon.png");
  _font.setFamily("Lucida Grande");
  _font.setBold(true);
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

bool ProjectTreeViewModel::isMainSelectedItem(std::shared_ptr<ProjectTreeNode> treeNode)
{
  return (_projectTreeController->selectedTreeNode() == treeNode);
}

QModelIndex ProjectTreeViewModel::indexForItem(std::shared_ptr<ProjectTreeNode> item)
{
  if(item)
  {
    std::shared_ptr<ProjectTreeNode> parent = item->parent();
    if(parent)
    {
      std::vector<std::shared_ptr<ProjectTreeNode>> childNodes =  parent->childNodes();
      std::vector<std::shared_ptr<ProjectTreeNode>>::iterator it = std::find(childNodes.begin(), childNodes.end(), item);
      if (it != childNodes.end())
      {
        int childIndex = std::distance(childNodes.begin(), it);
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
       return item;
     else
       return nullptr;
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

bool ProjectTreeViewModel::insertRow(int position, std::shared_ptr<ProjectTreeNode> parent, std::shared_ptr<ProjectTreeNode> project)
{
  if(parent)
  {
    QModelIndex parentIndex = indexForItem(parent);
    if(parentIndex.isValid())
    {
      beginInsertRows(parentIndex, position, position);
      parent->insertChild(position, project);
      endInsertRows();

      return true;
    }
  }
  return false;
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
  Q_UNUSED(parent);
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

  if (role == Qt::ForegroundRole)
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
      return _font;
    }
  }

  if( role == Qt::DecorationRole )
  {
    if(index.parent() != QModelIndex())
    {
      if(ProjectTreeNode *item = static_cast<ProjectTreeNode*>(index.internalPointer()))
      {
        if(std::shared_ptr<iRASPAProject> iraspa_project = std::dynamic_pointer_cast<iRASPAProject>(item->representedObject()))
        {
          if (iraspa_project->isGroup())
          {
            return _folderIcon;
          }
        }
      }
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

  Qt::ItemFlags flags = Qt::ItemFlag::NoItemFlags;

  if(index.parent().isValid())
  {
    flags |= Qt::ItemIsSelectable;
    flags |= Qt::ItemIsEnabled;
  }

  if(ProjectTreeNode *node = getItem(index))
  {
    if(node->isEditable() && ( index.column() == 0 ))
    {
      flags |= Qt::ItemIsEditable;
    }

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
  return QAbstractItemModel::mimeTypes() << ProjectTreeNode::mimeType;
}

QMimeData* ProjectTreeViewModel::mimeDataLazy(const QModelIndexList &indexes) const
{
  QByteArray encodedData;
  QDataStream stream(&encodedData, QIODevice::WriteOnly);

  QModelIndexList sortedIndexes = indexes;
  std::sort(sortedIndexes.begin(), sortedIndexes.end());

  // write application id
  stream << QCoreApplication::applicationPid();

  // write application projectTreeView id
  qulonglong ptrval(reinterpret_cast<qulonglong>(this));
  stream << ptrval;

  // write the number of objects
  stream << sortedIndexes.count();

  for(const QModelIndex &index: sortedIndexes)
  {
    if(index.isValid())
    {
      if(ProjectTreeNode *projectTreeNode = getItem(index))
      {
        qulonglong ptrval(reinterpret_cast<qulonglong>(projectTreeNode));
        stream << ptrval;
      }
    }
  }
  QMimeData *mimeData = new QMimeData();
  mimeData->setData(ProjectTreeNode::mimeType, encodedData);

  return mimeData;
}

QMimeData* ProjectTreeViewModel::mimeData(const QModelIndexList &indexes) const
{
  QByteArray encodedData;
  QDataStream stream(&encodedData, QIODevice::WriteOnly);

  QModelIndexList sortedIndexes = indexes;
  std::sort(sortedIndexes.begin(), sortedIndexes.end());

  // write application id
  stream << QCoreApplication::applicationPid();

  for(const QModelIndex &index: sortedIndexes)
  {
    if(index.isValid())
    {
      if(ProjectTreeNode *projectTreeNode = getItem(index))
      {
        stream <<= projectTreeNode->shared_from_this();
      }
    }
  }
  QMimeData *mimeData = new QMimeData();
  mimeData->setData(ProjectTreeNode::mimeType, encodedData);

  return mimeData;
}

bool ProjectTreeViewModel::canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const
{
  Q_UNUSED(data);
  Q_UNUSED(action);
  Q_UNUSED(row);
  Q_UNUSED(column);

  if(ProjectTreeNode *parentNode = getItem(parent))
  {
    if(parentNode->isDropEnabled())
    {
      return true;
    }
  }
  return false;
}

// drops onto existing items have row and column set to -1 and parent set to the current item
bool ProjectTreeViewModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
  Q_UNUSED(column);


  if(action == Qt::IgnoreAction)
  {
    return true;
  }
  if(!data->hasFormat(ProjectTreeNode::mimeType))
  {
    return false;
  }

  QByteArray ddata = data->data(ProjectTreeNode::mimeType);
  QDataStream stream(&ddata, QIODevice::ReadOnly);

  ProjectTreeNode *parentNode = getItem(parent);

  // read application id
  qint64 senderPid;
  stream >> senderPid;
  if (senderPid != QCoreApplication::applicationPid())
  {
    return false;
  }

  // read application projectTreeView id
  qulonglong sourceProjectTreeViewId(reinterpret_cast<qulonglong>(this));
  qulonglong senderProjectTreeViewId;
  stream >> senderProjectTreeViewId;

  // read the number of objects
  int count;
  stream >> count;

  int beginRow = row;
  if (beginRow == -1)
  {
    if (parent.isValid())
      beginRow = 0;
    else
      beginRow = rowCount(parent);
  }

  if(action == Qt::DropAction::CopyAction || (senderProjectTreeViewId != sourceProjectTreeViewId))
  {
    emit layoutAboutToBeChanged();
    bool oldState = this->blockSignals(true);
    while (!stream.atEnd())
    {
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
  else if(action == Qt::DropAction::MoveAction && (senderProjectTreeViewId == sourceProjectTreeViewId))
  {
    emit layoutAboutToBeChanged();
    bool oldState = this->blockSignals(true);
    while (!stream.atEnd())
    {
      int localRow;
      qlonglong nodePtr;
      stream >> nodePtr;
      ProjectTreeNode *node = reinterpret_cast<ProjectTreeNode *>(nodePtr);

      if(node->isEditable())
      {
        // source project is editable, so move it
        std::shared_ptr<ProjectTreeNode> projectTreeNode = node->shared_from_this();
        localRow = node->row();
        QModelIndex index = createIndex(localRow,0,projectTreeNode.get());

        removeRows(localRow,1,index.parent());


        // Adjust destination row for the case of moving an item
        // within the same parent, to a position further down.
        // Its own removal will reduce the final row number by one.
        if (index.row() < beginRow && (parentNode == projectTreeNode->parent().get()))
        {
           --beginRow;
        }

        beginInsertRows(parent, beginRow, beginRow);
        if (!parentNode->insertChild(beginRow, projectTreeNode))
          break;
        endInsertRows();
      }
      else
      {
        // source project not editable, so copy the project and do NOT delete the source
        std::shared_ptr<ProjectTreeNode> copiedTreeNode = node->shallowClone();
        copiedTreeNode->setIsEditable(true);
        copiedTreeNode->setIsDropEnabled(true);
        beginInsertRows(parent, beginRow, beginRow);
        if (!parentNode->insertChild(beginRow, copiedTreeNode))
          break;
        endInsertRows();
      }

      ++beginRow;
    }
    this->blockSignals(oldState);
  }

  emit layoutChanged();

  return true;
}

bool ProjectTreeViewModel::pasteMimeData(const QMimeData *data, int row, int column, const QModelIndex &parent)
{
  Q_UNUSED(column);

  if(!data->hasFormat(ProjectTreeNode::mimeType))
  {
    return false;
  }

  QByteArray ddata = data->data(ProjectTreeNode::mimeType);
  QDataStream stream(&ddata, QIODevice::ReadOnly);



  // read application id
  qint64 senderPid;
  stream >> senderPid;
  if (senderPid != QCoreApplication::applicationPid())
  {
    return false;
  }

  // determine insertion point
  QModelIndex beginParent = parent;
  int beginRow = row;
  if(ProjectTreeNode *parentNode = getItem(parent))
  {
    if(std::shared_ptr<iRASPAProject> iraspa_project = parentNode->representedObject())
    {
      if(std::shared_ptr<ProjectStructure> project = std::dynamic_pointer_cast<ProjectStructure>(iraspa_project->project()))
      {
        beginParent = parent.parent();
        beginRow = parent.row() + 1;
      }
    }
  }

  if (row == -1)
  {
      if (parent.isValid())
          beginRow = 0;
      else
          beginRow = rowCount(parent);
  }

  if(ProjectTreeNode *parentNode = getItem(beginParent))
  {
    emit layoutAboutToBeChanged();
    bool oldState = this->blockSignals(true);
    while (!stream.atEnd())
    {
      std::shared_ptr<ProjectTreeNode> copiedProjectTreeNode = std::make_shared<ProjectTreeNode>();
      stream >>= copiedProjectTreeNode;
      copiedProjectTreeNode->setIsEditable(true);
      copiedProjectTreeNode->setIsDropEnabled(true);

      beginInsertRows(beginParent, beginRow, beginRow);
      if (!parentNode->insertChild(beginRow, copiedProjectTreeNode))
        break;
      endInsertRows();
      beginRow += 1;
    }
    this->blockSignals(oldState);
  }

  emit layoutChanged();

  return true;
}

