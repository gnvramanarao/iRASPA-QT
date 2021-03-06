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

#include "projecttreenode.h"
#include <assert.h>
#include <vector>
#include <foundationkit.h>

ProjectTreeNode::~ProjectTreeNode()
{
}

char ProjectTreeNode::mimeType[] = "application/x-qt-iraspa-project-mime";

std::shared_ptr<iRASPAProject> ProjectTreeNode::representedObject()
{
  return _representedObject;
}

size_t ProjectTreeNode::row() const
{
  if(!_parent.expired())
  {
    if (std::shared_ptr<ProjectTreeNode> lockedParent = _parent.lock())
    {
      std::vector<std::shared_ptr<ProjectTreeNode>>::iterator it = std::find(lockedParent->_childNodes.begin(), lockedParent->_childNodes.end(), shared_from_this());
      if (it != lockedParent->_childNodes.end())
      {
        return  std::distance(lockedParent->_childNodes.begin(), it);
      }
    }
  }
  return 0;
}


std::optional<size_t> ProjectTreeNode::findChildIndex(std::shared_ptr<ProjectTreeNode> child)
{
  if (std::shared_ptr<ProjectTreeNode> lockedParent = _parent.lock())
  {
    std::vector<std::shared_ptr<ProjectTreeNode>>::iterator it = std::find(lockedParent->_childNodes.begin(), lockedParent->_childNodes.end(), shared_from_this());
    if (it != lockedParent->_childNodes.end())
    {
      return  std::distance(lockedParent->_childNodes.begin(), it);
    }
  }

  return std::nullopt;
}

bool ProjectTreeNode::removeChild(size_t row)
{
  if (row < 0 || row >= _childNodes.size())
     return false;

  _childNodes.erase(_childNodes.begin() + row);
  return true;
}

bool ProjectTreeNode::insertChild(size_t row, std::shared_ptr<ProjectTreeNode> child)
{
  if (row < 0 || row > _childNodes.size())
    return false;

  child->_parent = shared_from_this();
  _childNodes.insert(_childNodes.begin() + row, child);
  return true;
}

const IndexPath ProjectTreeNode::indexPath()
{
  if (std::shared_ptr<ProjectTreeNode> lockedParent =  _parent.lock())
  {
    IndexPath indexpath = lockedParent->indexPath();
    std::optional<int> index = lockedParent->findChildIndex(shared_from_this());

    assert(index);
    if (indexpath.count() > 0)
    {
      return indexpath.appending(*index);
    }
    else
    {
      return IndexPath(*index);
    }
  }
  else
  {
   return IndexPath();
  }

  return IndexPath();
}

std::shared_ptr<ProjectTreeNode> ProjectTreeNode::descendantNodeAtIndexPath(IndexPath indexPath)
{
  size_t length = indexPath.count();
  std::shared_ptr<ProjectTreeNode> node = shared_from_this();

  for(size_t i=0; i < length; i++)
  {
    size_t index = indexPath[i];
    if(index<node->_childNodes.size())
    {
      return nullptr;
    }
    else
    {
      node = node->_childNodes[index];
    }
  }

  return node;
}


void ProjectTreeNode::insertInParent(std::shared_ptr<ProjectTreeNode> parent, size_t index)
{
  this->_parent = parent;
  parent->_childNodes.insert(parent->_childNodes.begin() + index, shared_from_this());
}


void ProjectTreeNode::appendToParent(std::shared_ptr<ProjectTreeNode> parent1)
{
  this->_parent = parent1;
  parent1->_childNodes.push_back(shared_from_this());
}



std::vector<std::shared_ptr<ProjectTreeNode>> ProjectTreeNode::flattenedNodes()
{
  std::vector<std::shared_ptr<ProjectTreeNode>> nodes{shared_from_this()};

  std::vector<std::shared_ptr<ProjectTreeNode>> otherNodes = this->descendantNodes();
  for(std::shared_ptr<ProjectTreeNode> child : otherNodes)
  {
    nodes.push_back(child);
  }
  return nodes;
}

std::vector<std::shared_ptr<ProjectTreeNode>> ProjectTreeNode::flattenedLeafNodes()
{
  std::vector<std::shared_ptr<ProjectTreeNode>> nodes{};

  if(this->representedObject()->isLeaf())
  {
    nodes.push_back(shared_from_this());
  }

  std::vector<std::shared_ptr<ProjectTreeNode>> otherNodes = this->descendantLeafNodes();
  for(std::shared_ptr<ProjectTreeNode> child : otherNodes)
  {
    nodes.push_back(child);
  }
  return nodes;
}

std::vector<std::shared_ptr<ProjectTreeNode>> ProjectTreeNode::flattenedGroupNodes()
{
  std::vector<std::shared_ptr<ProjectTreeNode>> nodes{};

  if(this->_representedObject->isGroup())
  {
    nodes.push_back(shared_from_this());
  }

  std::vector<std::shared_ptr<ProjectTreeNode>> otherNodes = this->descendantGroupNodes();
  for(std::shared_ptr<ProjectTreeNode> child : otherNodes)
  {
    nodes.push_back(child);
  }
  return nodes;
}


bool ProjectTreeNode::isDescendantOfNode(ProjectTreeNode* parent)
{
  ProjectTreeNode* treeNode = this;

  while(treeNode != nullptr)
  {
    if(treeNode == parent)
    {
      return true;
    }
    else
    {
      treeNode = treeNode->_parent.lock().get();
    }
  }

  return false;
}

std::vector<std::shared_ptr<ProjectTreeNode>> ProjectTreeNode::descendants()
{
  std::vector<std::shared_ptr<ProjectTreeNode>> descendants{};
  for(std::shared_ptr<ProjectTreeNode> child : this->_childNodes)
  {
    if(child->representedObject()->isLeaf())
    {
      descendants.push_back(child);
    }
    else
    {
      std::vector<std::shared_ptr<ProjectTreeNode>> otherNodes = child->descendants();
      for(std::shared_ptr<ProjectTreeNode> node : otherNodes)
      {
        descendants.push_back(node);
      }
    }
  }

  return descendants;
}

std::vector<std::shared_ptr<ProjectTreeNode>> ProjectTreeNode::descendantNodes()
{
    std::vector<std::shared_ptr<ProjectTreeNode>> descendants{};
    for(std::shared_ptr<ProjectTreeNode> child : this->_childNodes)
    {
      descendants.push_back(child);
      if (child->representedObject()->isGroup())
      {
        std::vector<std::shared_ptr<ProjectTreeNode>> otherNodes = child->descendantNodes();
        for(std::shared_ptr<ProjectTreeNode> node : otherNodes)
        {
          descendants.push_back(node);
        }
      }
    }

    return descendants;
}

std::vector<std::shared_ptr<ProjectTreeNode>> ProjectTreeNode::descendantLeafNodes()
{
  std::vector<std::shared_ptr<ProjectTreeNode>> descendants{};
  for(std::shared_ptr<ProjectTreeNode> child : this->_childNodes)
  {
    if(child->representedObject()->isLeaf())
    {
      descendants.push_back(child);
    }
    else
    {
      std::vector<std::shared_ptr<ProjectTreeNode>> otherNodes = child->descendantLeafNodes();
      for(std::shared_ptr<ProjectTreeNode> node : otherNodes)
      {
        descendants.push_back(node);
      }
    }
  }

  return descendants;
}

std::vector<std::shared_ptr<ProjectTreeNode>> ProjectTreeNode::descendantGroupNodes()
{
  std::vector<std::shared_ptr<ProjectTreeNode>> descendants{};
  for(std::shared_ptr<ProjectTreeNode> child : this->_childNodes)
  {
    if(child->representedObject()->isGroup())
    {
      descendants.push_back(child);
      std::vector<std::shared_ptr<ProjectTreeNode>> otherNodes = child->descendantGroupNodes();
      for(std::shared_ptr<ProjectTreeNode> node : otherNodes)
      {
        descendants.push_back(node);
      }
    }
  }

  return descendants;
}

std::vector<std::shared_ptr<iRASPAProject>> ProjectTreeNode::flattenedObjects()
{
  std::vector<std::shared_ptr<iRASPAProject>> objects{this->_representedObject};

  std::vector<std::shared_ptr<iRASPAProject>> groupObjects = this->descendantObjects();
  for(std::shared_ptr<iRASPAProject> object : groupObjects)
  {
    objects.push_back(object);
  }
  return objects;
}


std::vector<std::shared_ptr<iRASPAProject>> ProjectTreeNode::descendantObjects()
{
  std::vector<std::shared_ptr<iRASPAProject>> descendants{};
  for(std::shared_ptr<ProjectTreeNode> child : this->_childNodes)
  {
    descendants.push_back(child->_representedObject);
    if(child->representedObject()->isGroup())
    {
      std::vector<std::shared_ptr<iRASPAProject>> groupObjects = child->descendantObjects();
      for(std::shared_ptr<iRASPAProject> object : groupObjects)
      {
        descendants.push_back(object);
      }
    }
  }
  return descendants;
}


// MARK: -
// MARK: Filtering support

void ProjectTreeNode::updateFilteredChildren(std::function<bool(std::shared_ptr<ProjectTreeNode>)> predicate)
{
  for(std::shared_ptr<ProjectTreeNode> child : this->_childNodes)
  {
    child->_matchesFilter = true;
    child->_matchesFilter = predicate(shared_from_this());
  }

  this->_filteredAndSortedNodes.clear();
  for(std::shared_ptr<ProjectTreeNode> child : this->_childNodes)
  {
    if (child->_matchesFilter) this->_filteredAndSortedNodes.push_back(child);
  }

  // if we have filtered nodes, then all parents of this node needs to be included
  if(this->_filteredAndSortedNodes.size() > 0)
  {
    this->_matchesFilter = true;
  }
}

void ProjectTreeNode::updateFilteredChildrenRecursively(std::function<bool(std::shared_ptr<ProjectTreeNode>)> predicate)
{
  this->_matchesFilter = false;
  this->_matchesFilter = predicate(shared_from_this());

  for(std::shared_ptr<ProjectTreeNode> child : this->_childNodes)
  {
    child->updateFilteredChildrenRecursively(predicate);
  }

  if(this->_matchesFilter)
  {
      /*
    if(std::shared_ptr<ProjectTreeNode> lockedParent = _parent.lock())
    {
      lockedParent->_matchesFilter = true;
    }*/
  }

  this->_filteredAndSortedNodes.clear();
  for(std::shared_ptr<ProjectTreeNode> child : this->_childNodes)
  {
    if (child->_matchesFilter) this->_filteredAndSortedNodes.push_back(child);
  }
}

void ProjectTreeNode::setFilteredNodesAsMatching()
{
  this->_matchesFilter = false;

  this->_filteredAndSortedNodes.clear();
  for(std::shared_ptr<ProjectTreeNode> child : this->_childNodes)
  {
    this->_filteredAndSortedNodes.push_back(child);
  }
}




bool ProjectTreeNode::insertChildren(size_t position, size_t count, size_t columns)
{
  if (position < 0 || position > _childNodes.size())
    return false;

  for (size_t row = 0; row < count; ++row)
  {
    std::shared_ptr<ProjectTreeNode> item = std::make_shared<ProjectTreeNode>(QString("tree node"));
    item->_parent = shared_from_this();
    _childNodes.insert(_childNodes.begin() + position, item);
  }

  return true;
}

bool ProjectTreeNode::setData(const QVariant &value)
{
  _displayName = value.toString();
  return true;
}

std::shared_ptr<ProjectTreeNode> ProjectTreeNode::shallowClone()
{
  QByteArray byteArray = QByteArray();
  QDataStream stream(&byteArray, QIODevice::WriteOnly);
  stream <<= this->shared_from_this();

  QDataStream streamRead(&byteArray, QIODevice::ReadOnly);
  std::shared_ptr<ProjectTreeNode> projectTreeNode = std::make_shared<ProjectTreeNode>();
  streamRead >>= projectTreeNode;

  return projectTreeNode;
}

QDataStream &operator<<(QDataStream& stream, const std::shared_ptr<ProjectTreeNode>& node)
{
  stream << node->_versionNumber;
  stream << node->_displayName;

  stream << node->_isEditable;
  stream << node->_isDropEnabled;

  stream << node->_representedObject;

  stream << node->_childNodes;

  return stream;
}

QDataStream &operator>>(QDataStream& stream, std::shared_ptr<ProjectTreeNode>& node)
{
  qint64 versionNumber;
  stream >> versionNumber;
  if(versionNumber > node->_versionNumber)
  {
    throw InvalidArchiveVersionException(__FILE__, __LINE__, "ProjectTreeNode");
  }
  stream >> node->_displayName;
  stream >> node->_isEditable;
  stream >> node->_isDropEnabled;

  stream >> node->_representedObject;

  stream >> node->_childNodes;

  for(std::shared_ptr<ProjectTreeNode> child : node->_childNodes)
  {
    child->_parent = node;
  }

  return stream;
}

QDataStream &operator<<=(QDataStream& stream, const std::shared_ptr<ProjectTreeNode>& node)
{
  stream << node->_versionNumber;
  stream << node->_displayName;

  stream << node->_isEditable;
  stream << node->_isDropEnabled;

  stream <<= node->_representedObject;

  return stream;
}

QDataStream &operator>>=(QDataStream& stream, std::shared_ptr<ProjectTreeNode>& node)
{
  qint64 versionNumber;
  stream >> versionNumber;
  if(versionNumber > node->_versionNumber)
  {
    throw InvalidArchiveVersionException(__FILE__, __LINE__, "ProjectTreeNode");
  }
  stream >> node->_displayName;
  stream >> node->_isEditable;
  stream >> node->_isDropEnabled;

  stream >>= node->_representedObject;

  return stream;
}

