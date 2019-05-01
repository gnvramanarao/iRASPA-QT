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

#include "atomtreeviewmodel.h"

char AtomTreeViewModel::mimeType[] = "application/x-qt-iraspa-mime";

AtomTreeViewModel::AtomTreeViewModel(): _atomTreeController(std::make_shared<SKAtomTreeController>())
{
}

void AtomTreeViewModel::setAtomTreeController(std::shared_ptr<SKAtomTreeController> controller)
{
  if(_atomTreeController != controller)
  {
    beginResetModel();
    _atomTreeController = controller;
    endResetModel();
  }
}

QModelIndexList AtomTreeViewModel::selectedIndexes()
{
  QModelIndexList list = QModelIndexList();
  for(std::shared_ptr<SKAtomTreeNode> node : _atomTreeController->selectedTreeNodes())
  {
    int localRow = node->row();
    QModelIndex index = createIndex(localRow,0,node.get());
    list.push_back(index);
  }
  return list;
}


SKAtomTreeNode *AtomTreeViewModel::getItem(const QModelIndex &index) const
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


QModelIndex AtomTreeViewModel::index(int row, int column, const QModelIndex &parent) const
{
  if(!hasIndex(row, column, parent))
     return QModelIndex();

  if(_atomTreeController->rootNodes().empty())
    return QModelIndex();

  SKAtomTreeNode *parentItem;

  if (!parent.isValid())
    parentItem = _atomTreeController->hiddenRootNode().get();
  else
    parentItem = static_cast<SKAtomTreeNode*>(parent.internalPointer());

  std::shared_ptr<SKAtomTreeNode> childItem = parentItem->getChildNode(row);
  if (childItem)
    return createIndex(row, column, childItem.get());
  else
    return QModelIndex();
}

QModelIndex AtomTreeViewModel::parent(const QModelIndex &index) const
{
  if (!index.isValid())
    return QModelIndex();

  SKAtomTreeNode *childItem = static_cast<SKAtomTreeNode*>(index.internalPointer());
  SKAtomTreeNode *parentItem = childItem->parent().get();

  if (parentItem == _atomTreeController->hiddenRootNode().get())
    return QModelIndex();

  return createIndex(parentItem->row(), 0, parentItem);
}

int AtomTreeViewModel::columnCount(const QModelIndex &parent) const
{
  return 7;
}


int AtomTreeViewModel::rowCount(const QModelIndex &parent) const
{
  SKAtomTreeNode *parentItem;

  if (!parent.isValid())
    parentItem = _atomTreeController->hiddenRootNode().get();
  else
    parentItem = static_cast<SKAtomTreeNode*>(parent.internalPointer());

  return parentItem->childCount();
}

QVariant AtomTreeViewModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();
  SKAtomTreeNode *item = static_cast<SKAtomTreeNode*>(index.internalPointer());
  if(std::shared_ptr<SKAsymmetricAtom> atom = item->representedObject())
  {
    int elementIdentifier = atom->elementIdentifier();

    if(item->isGroup())
    {
      if (role == Qt::CheckStateRole)
      {
        return atom->isVisible() ? Qt::Checked : Qt::Unchecked;
      }
    }

    if (role == Qt::CheckStateRole && index.column() == 0)
    {
      return atom->isVisible() ? Qt::Checked : Qt::Unchecked;
    }

   if (role != Qt::DisplayRole)
     return QVariant();

    switch(index.column())
    {
    case 0:
      return item->representedObject()->displayName();
    case 1:
      return QString::number(atom->tag());
    case 2:
      return PredefinedElements::predefinedElements[elementIdentifier]._chemicalSymbol;
    case 3:
      return QString::number(item->representedObject()->position().x, 'f', 5);
    case 4:
      return QString::number(item->representedObject()->position().y, 'f', 5);
    case 5:
      return QString::number(item->representedObject()->position().z, 'f', 5);
    case 6:
      return QString::number(item->representedObject()->charge(), 'f', 5);
    }
  }

  return QVariant();
}

Qt::ItemFlags AtomTreeViewModel::flags(const QModelIndex &index) const
{
  if (!index.isValid()) return Qt::NoItemFlags;

  Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

  if ( index.column() == 0 )
  {
    flags |= Qt::ItemIsUserCheckable;
    flags != Qt::ItemIsTristate;
    flags |= Qt::ItemIsEditable;
  }

  flags |= Qt::ItemIsDragEnabled;

  SKAtomTreeNode *node = getItem(index);
  if(node->isGroup())
  {
    flags |= Qt::ItemIsDropEnabled;
  }

  return flags;
}



QVariant AtomTreeViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  Q_UNUSED(orientation);

  if (role != Qt::DisplayRole)
    return QVariant();

  switch(section)
  {
    case 0:
      return QString("vis  name");
    case 1:
      return QString("atom id.");
    case 2:
      return QString("type");
    case 3:
      return QString("x");
    case 4:
      return QString("y");
    case 5:
      return QString("z");
    case 6:
      return QString("q");
  }

  return QVariant();
}

bool AtomTreeViewModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  if (!index.isValid() /*|| role != Qt::EditRole*/)
    return false;

  if (role == Qt::CheckStateRole)
  {
    SKAtomTreeNode *item = static_cast<SKAtomTreeNode*>(index.internalPointer());
    if ((Qt::CheckState)value.toInt() == Qt::Checked)
    {
      item->representedObject()->setVisibility(true);
      emit rendererReloadData();
      return true;
    }
    else
    {
      item->representedObject()->setVisibility(false);
      emit rendererReloadData();
      return true;
    }
  }

  return false;
}

Qt::DropActions AtomTreeViewModel::supportedDropActions() const
{
  return Qt::CopyAction | Qt::MoveAction;
}

Qt::DropActions AtomTreeViewModel::supportedDragActions() const
{
   return Qt::CopyAction | Qt::MoveAction;
}


bool AtomTreeViewModel::removeRows(int position, int rows, const QModelIndex &parent)
{

  SKAtomTreeNode *parentItem = getItem(parent);

  if (position < 0 || position > parentItem->childNodes().size())
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

bool AtomTreeViewModel::insertRows(int position, int rows, const QModelIndex &parent)
{
  SKAtomTreeNode *parentItem = getItem(parent);

  beginInsertRows(parent, position, position + rows - 1);
  for (int row = 0; row < rows; ++row)
  {
    std::shared_ptr<SKAsymmetricAtom> atom = std::make_shared<SKAsymmetricAtom>();
    std::shared_ptr<SKAtomTreeNode> newItem = std::make_shared<SKAtomTreeNode>(parentItem->shared_from_this(), atom);

    if (!parentItem->insertChild(position, newItem))
      break;
  }
  endInsertRows();

  return true;
}

bool AtomTreeViewModel::moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild)
{
  std::cout << "MOVE ROWS!!!!!" << std::endl;
  return true;

  SKAtomTreeNode *sourceParentNode = static_cast<SKAtomTreeNode*>(sourceParent.internalPointer());
  SKAtomTreeNode *destinationParentNode = static_cast<SKAtomTreeNode*>(destinationParent.internalPointer());
  std::shared_ptr<SKAtomTreeNode> childNode = sourceParentNode->getChildNode(sourceRow);

  /*
      // if source and destination parents are the same, move elements locally
      if(sourceParentNode == destinationParentNode)
      {
          // only process if a local move is possible
          if(sourceParentNode->IsMovePossible(sourceRow, count, destinationChild))
          {
              beginMoveRows(sourceParent, sourceRow, sourceRow+count-1, destinationParent, destinationChild);
              sourceParentNode->MoveChildren(sourceRow, count, destinationChild);
              endMoveRows();
          }
      } else {
          // otherwise, move the node under the parent
          beginMoveRows(sourceParent, sourceRow, sourceRow+count-1, destinationParent, destinationChild);
          childNode->SetParent(destinationParentNode);
          endMoveRows();
      }
      Scene::GetInstance()->GetRoot()->PrintChildren();
      */
  return true;
}


QStringList AtomTreeViewModel::mimeTypes() const
{
  return QAbstractItemModel::mimeTypes() << mimeType;
}

QMimeData* AtomTreeViewModel::mimeData(const QModelIndexList &indexes) const
{
  QByteArray encodedData;
  QDataStream stream(&encodedData, QIODevice::WriteOnly);

  QModelIndexList indexes2 = indexes;
  qSort(indexes2.begin(), indexes2.end(), qGreater<QModelIndex>());

  stream << QCoreApplication::applicationPid();
  stream << indexes2.count();

  for(auto iter = indexes2.constBegin(); iter != indexes2.constEnd(); ++iter)
  {
    QModelIndex index = *iter;
    if(index.isValid())
    {
      SKAtomTreeNode *atom = getItem(index);
      qulonglong ptrval(reinterpret_cast<qulonglong>(atom));
      stream << ptrval;
    }
  }
  QMimeData *mimeData = new QMimeData();
  mimeData->setData(mimeType, encodedData);

  return mimeData;
}

bool AtomTreeViewModel::hasChildren(const QModelIndex &parent) const
{
  if (!parent.isValid()) return true;

  SKAtomTreeNode *item = static_cast<SKAtomTreeNode*>(parent.internalPointer());
  return item->isGroup();
}

bool AtomTreeViewModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
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

  SKAtomTreeNode *parentNode = getItem(parent);

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
     insertRows(beginRow,count,parent);
  }
  else
  {
    std::vector<std::shared_ptr<SKAtomTreeNode>> newItems{};

    emit layoutAboutToBeChanged();
    bool oldState = this->blockSignals(true);
    while (!stream.atEnd())
    {
      int localRow;
      qlonglong nodePtr;
      stream >> nodePtr;
      SKAtomTreeNode *node = reinterpret_cast<SKAtomTreeNode *>(nodePtr);
      std::shared_ptr<SKAtomTreeNode> atom = node->shared_from_this();

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
