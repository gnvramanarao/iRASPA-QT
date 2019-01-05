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

#include "projecttreecontroller.h"

#include <QColor>
#include <stack>

ProjectTreeController::ProjectTreeController(): _hiddenRootNode(std::make_shared<ProjectTreeNode>(QString("hidden root node"),true,false))
{
  std::shared_ptr<ProjectTreeNode> rootGalleryProjects = std::make_shared<ProjectTreeNode>(QString("GALLERY"), std::make_shared<iRASPAProject>(std::make_shared<ProjectGroup>()), false, false);
  std::shared_ptr<ProjectTreeNode> emptyLocalProjects1 = std::make_shared<ProjectTreeNode>(QString("  "), false, false);
  std::shared_ptr<ProjectTreeNode> rootLocalProjects = std::make_shared<ProjectTreeNode>(QString("LOCAL PROJECTS"), std::make_shared<iRASPAProject>(std::make_shared<ProjectGroup>()), false, false);
  //std::shared_ptr<ProjectTreeNode> emptyLocalProjects2 = std::make_shared<ProjectTreeNode>(QString("  "), false, false);
  //std::shared_ptr<ProjectTreeNode> rootIcloudProjects = std::make_shared<ProjectTreeNode>(QString("ICLOUD PUBLIC"), false, false);

  _hiddenRootNode->insertChild(0, rootGalleryProjects);
  _hiddenRootNode->insertChild(1, emptyLocalProjects1);
  _hiddenRootNode->insertChild(2, rootLocalProjects);
  //_hiddenRootNode->insertChild(3, emptyLocalProjects2);
  //_hiddenRootNode->insertChild(4, rootIcloudProjects);

  _galleryProjects = std::make_shared<ProjectTreeNode>(QString("Gallery"), std::make_shared<iRASPAProject>(std::make_shared<ProjectGroup>()), false, false);
  _localProjects = std::make_shared<ProjectTreeNode>(QString("Local projects"), std::make_shared<iRASPAProject>(std::make_shared<ProjectGroup>()), false, false);
  _icloudProjects = std::make_shared<ProjectTreeNode>(QString("iCloud public"), std::make_shared<iRASPAProject>(std::make_shared<ProjectGroup>()), false, false);

  rootGalleryProjects->insertChild(0,_galleryProjects);
  rootLocalProjects->insertChild(0,_localProjects);
  //rootIcloudProjects->insertChild(0,_icloudProjects);
}

ProjectTreeController::~ProjectTreeController()
{
  _hiddenRootNode = nullptr;
  _selectedTreeNode = nullptr;
  _selectedTreeNodes.clear();
}



void ProjectTreeController::insertNodeInParent(std::shared_ptr<ProjectTreeNode> node, std::shared_ptr<ProjectTreeNode> parent, int index)
{
  if (parent != nullptr)
  {
    parent->_childNodes.insert(parent->_childNodes.begin() + index, node);
    node->_parent = parent;
    parent->updateFilteredChildren(this->_filterPredicate);
  }
  else
  {
    _hiddenRootNode->_childNodes.insert(_hiddenRootNode->_childNodes.begin() + index, node);
    node->_parent = _hiddenRootNode;
    _hiddenRootNode->updateFilteredChildren(this->_filterPredicate);
  }
}

/*
void ProjectTreeController::removeNode(std::shared_ptr<ProjectTreeNode> node)
{
  if(std::shared_ptr<ProjectTreeNode> lockedParent = node->_parent.lock())
  {
    node->removeFromParent();
    lockedParent->updateFilteredChildren(_filterPredicate);
  }
}*/

/*
std::shared_ptr<ProjectTreeNode> ProjectTreeController::parentItem(std::shared_ptr<ProjectTreeNode> node)
{

  if(node->_parent.lock() == _hiddenRootNode)
  {
    return nullptr;
  }
  else
  {
    return node->_parent.lock();
  }
}*/

/*
bool ProjectTreeController::isRootNode(std::shared_ptr<ProjectTreeNode> node)
{
  if (node->_parent == nullptr)
  {
    return true;
  }
  return false;
}*/

void ProjectTreeController::appendToRootnodes(std::shared_ptr<ProjectTreeNode> item)
{
  item->_parent = _hiddenRootNode;
  _hiddenRootNode->_childNodes.push_back(item);
}

/*
bool ProjectTreeController::nodeIsChildOfItem(ProjectTreeNode* node, ProjectTreeNode* item)
{
  ProjectTreeNode* itemNode = item == nullptr ? _hiddenRootNode : item;
  return node->_parent == itemNode;
}


int ProjectTreeController::filteredChildIndexOfItem(ProjectTreeNode* parentItem, int index)
{
  ProjectTreeNode* parentNode = parentItem == nullptr ? _hiddenRootNode : parentItem;

  if(index < parentNode->_filteredAndSortedNodes.size())
  {
    ProjectTreeNode* node = parentNode->_filteredAndSortedNodes[index];
    std::experimental::optional<int> filteredIndex = parentNode->findChildIndex(node);
    assert(filteredIndex);
    return *filteredIndex;
  }
  else
  {
    return parentNode->_childNodes.size();
  }
}

bool ProjectTreeController::isDescendantOfNode(ProjectTreeNode* item, ProjectTreeNode* parentNode)
{
  ProjectTreeNode* treeNode = item == nullptr ? _hiddenRootNode : item;

  while(treeNode != nullptr)
  {
    if (treeNode == parentNode)
    {
      return true;
    }
    else
    {
      treeNode = treeNode->_parent;
    }
    return false;
  }
}




std::vector<ProjectTreeNode*> ProjectTreeController::childrenForItem(ProjectTreeNode* item)
{
  if (item==nullptr)
  {
    return _hiddenRootNode->_childNodes;
  }
  else
  {
    return item->_childNodes;
  }
}

ProjectTreeNode* ProjectTreeController::nodeAtIndexPath(IndexPath indexPath)
{
  return _hiddenRootNode->descendantNodeAtIndexPath(indexPath);
}
*/

// MARK: Selection
// =========================

// Deletion is done bottom-up (post-order).

void ProjectTreeController::deleteSelection()
{
    /*
  std::vector<ProjectTreeNode*> atomNodes;
  std::copy(_selectedTreeNodes.begin(), _selectedTreeNodes.end(), std::back_inserter(atomNodes));

  std::sort(atomNodes.begin(), atomNodes.end(), [](ProjectTreeNode* a, ProjectTreeNode* b) -> bool {
      return a->indexPath() > b->indexPath();
  });

  for(ProjectTreeNode* atom : atomNodes)
  {
    IndexPath indexPath = atom->indexPath();
    int lastIndex = indexPath.lastIndex();
    const ProjectTreeNode* parentItem = atom->parent();
    parentItem->childNodes().erase(parentItem->childNodes().begin() + lastIndex);
  };

  _selectedTreeNode = nullptr;
  _selectedTreeNodes.clear();
  */
}

std::vector<std::shared_ptr<ProjectTreeNode>> ProjectTreeController::flattenedLeafNodes()
{
  return _hiddenRootNode->flattenedLeafNodes();
}

/*
 QModelIndex ProjectTreeController::index(int row, int column, const QModelIndex &parent) const
 {
   if (parent.isValid() && parent.column() != 0)
     return QModelIndex();




   ProjectTreeNode* parentItem;

   if (!parent.isValid())
     parentItem = _hiddenRootNode.get();
   else
     parentItem = static_cast<ProjectTreeNode*>(parent.internalPointer());

   if(parentItem->_childNodes.empty())
     return QModelIndex();

   ProjectTreeNode* childItem = parentItem->_childNodes[row].get();
   if (childItem)
       return createIndex(row, column, childItem);
   else
     return QModelIndex();
 }

 QModelIndex ProjectTreeController::parent(const QModelIndex &index) const
 {
   if (!index.isValid())
       return QModelIndex();

   ProjectTreeNode* childItem = static_cast<ProjectTreeNode*>(index.internalPointer());
   ProjectTreeNode* parentItem = childItem->_parent.lock().get();

   if (parentItem == _hiddenRootNode.get())
       return QModelIndex();

   std::experimental::optional<int> row = parentItem->_parent.lock()->findChildIndex(childItem->_parent.lock());
   if(row)
   {
     return createIndex(*row, 0, parentItem);
   }
   else
   {
     return QModelIndex();
   }
 }

 int ProjectTreeController::columnCount(const QModelIndex &parent) const
 {
     return 1;
 }

 int ProjectTreeController::rowCount(const QModelIndex &parent) const
 {
     ProjectTreeNode *parentItem;
     if (parent.column() > 0)
         return 0;

     if (!parent.isValid())
         parentItem = _hiddenRootNode.get();
     else
         parentItem = static_cast<ProjectTreeNode*>(parent.internalPointer());

     return parentItem->childNodes().size();
 }

 QVariant ProjectTreeController::data(const QModelIndex &index, int role) const
 {
     //if (role == Qt::BackgroundRole)
     //{
     //return QColor(255, 0, 0);
     //}



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

 Qt::ItemFlags ProjectTreeController::flags(const QModelIndex &index) const
 {
   if (!index.isValid()) return Qt::NoItemFlags;

   return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
 }

bool ProjectTreeController::hasChildren(const QModelIndex &parent) const
{
  if (!parent.isValid()) return true;

  ProjectTreeNode *item = static_cast<ProjectTreeNode*>(parent.internalPointer());
  return item->isGroup();
}

ProjectTreeNode* ProjectTreeController::getItem(const QModelIndex &index) const
{
  if (index.isValid())
  {
    ProjectTreeNode *item = static_cast<ProjectTreeNode*>(index.internalPointer());
    if(item)
      return item;
  }
  return _hiddenRootNode.get();
}

bool ProjectTreeController::insertRows(int row, int count, const QModelIndex &parent)
{
  bool success = true;

  ProjectTreeNode* treeNodeParent = getItem(parent);
  beginInsertRows(parent, row, row + count - 1);
  success = treeNodeParent->insertChildren(row, count, 0);
  endInsertRows();

  return success;
}

bool ProjectTreeController::setData(const QModelIndex &index, const QVariant &value, int role)
{
  if (role != Qt::EditRole)
    return false;

  ProjectTreeNode *item = getItem(index);
  bool result = item->setData(value);

  if (result)
    emit dataChanged(index, index);

  return result;
}

bool ProjectTreeController::setData(const QModelIndex &index, std::shared_ptr<iRASPAProjectStructure> value)
{
  ProjectTreeNode *item = getItem(index);
  item->_representedObject = value;

  emit dataChanged(index, index);

  return true;
}
*/

QDataStream &operator<<(QDataStream& stream, const std::shared_ptr<ProjectTreeController>& controller)
{
  stream << controller->_versionNumber;
  stream << controller->_localProjects;
  return stream;
}

QDataStream &operator>>(QDataStream& stream, std::shared_ptr<ProjectTreeController>& controller)
{
  qint64 versionNumber;
  stream >> versionNumber;
  if(versionNumber > controller->_versionNumber)
  {
    throw InvalidArchiveVersionException(__FILE__, __LINE__, "Project");
  }
  stream >> controller->_localProjects;
  return stream;
}
