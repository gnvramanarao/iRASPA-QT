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

#include "skatomtreecontroller.h"
#include <stack>
#include <algorithm>
#include <QDebug>

SKAtomTreeController::SKAtomTreeController()
{
  _hiddenRootNode = std::make_shared<SKAtomTreeNode>();
}

SKAtomTreeController::~SKAtomTreeController()
{
}

bool SKAtomTreeController::isRootNode(SKAtomTreeNode *node)
{
  return node == _hiddenRootNode.get();
}

void SKAtomTreeController::insertNodeInParent(std::shared_ptr<SKAtomTreeNode> node, std::weak_ptr<SKAtomTreeNode> parent, int index)
{
  if (std::shared_ptr<SKAtomTreeNode> lockedParent = parent.lock())
  {
    lockedParent->_childNodes.insert(lockedParent->_childNodes.begin() + index, node);
    node->_parent = parent;
    lockedParent->updateFilteredChildren(this->_filterPredicate);
  }
  else
  {
    _hiddenRootNode->_childNodes.insert(_hiddenRootNode->_childNodes.begin() + index, node);
    node->_parent = parent;
    _hiddenRootNode->updateFilteredChildren(this->_filterPredicate);
  }
}

void SKAtomTreeController::insertNodeAtIndexPath(std::shared_ptr<SKAtomTreeNode> node, IndexPath path)
{
  int index = path.lastIndex();
  std::shared_ptr<SKAtomTreeNode> parent = _hiddenRootNode->descendantNodeAtIndexPath(path.removingLastIndex());
  node->insertInParent(parent, index);
}

void SKAtomTreeController::removeNode(std::shared_ptr<SKAtomTreeNode> node)
{
  if(std::shared_ptr<SKAtomTreeNode> parentNode = node->_parent.lock())
  {
    node->removeFromParent();
    parentNode->updateFilteredChildren(_filterPredicate);
  }
}

/*
std::shared_ptr<SKAtomTreeNode> SKAtomTreeController::parentItem(std::shared_ptr<SKAtomTreeNode> node)
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


void SKAtomTreeController::appendToRootnodes(std::shared_ptr<SKAtomTreeNode> item)
{
  item->_parent = _hiddenRootNode;
  _hiddenRootNode->_childNodes.push_back(item);
}

bool SKAtomTreeController::nodeIsChildOfItem(std::shared_ptr<SKAtomTreeNode> node, std::shared_ptr<SKAtomTreeNode> item)
{
  std::shared_ptr<SKAtomTreeNode> itemNode = item == nullptr ? _hiddenRootNode : item;
  return node->_parent.lock() == itemNode;
}


int SKAtomTreeController::filteredChildIndexOfItem(std::shared_ptr<SKAtomTreeNode> parentItem, int index)
{
  std::shared_ptr<SKAtomTreeNode> parentNode = parentItem == nullptr ? _hiddenRootNode : parentItem;

  if(index < parentNode->_filteredAndSortedNodes.size())
  {
    std::shared_ptr<SKAtomTreeNode> node = parentNode->_filteredAndSortedNodes[index];

    std::optional<int> filteredIndex = parentNode->findChildIndex(node);

    return *filteredIndex;
  }
  else
  {
    return parentNode->_childNodes.size();
  }
}

bool SKAtomTreeController::isDescendantOfNode(std::shared_ptr<SKAtomTreeNode> item, std::shared_ptr<SKAtomTreeNode> parentNode)
{
  std::shared_ptr<SKAtomTreeNode> treeNode = item == nullptr ? _hiddenRootNode : item;

  while(treeNode != nullptr)
  {
    if (treeNode == parentNode)
    {
      return true;
    }
    else
    {
      treeNode = treeNode->_parent.lock();
    }
    return false;
  }
}




std::vector<std::shared_ptr<SKAtomTreeNode>> SKAtomTreeController::childrenForItem(std::shared_ptr<SKAtomTreeNode> item)
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

std::shared_ptr<SKAtomTreeNode> SKAtomTreeController::nodeAtIndexPath(IndexPath indexPath)
{
  return _hiddenRootNode->descendantNodeAtIndexPath(indexPath);
}

std::vector<std::shared_ptr<SKAtomTreeNode> > SKAtomTreeController::flattenedLeafNodes() const
{
  return _hiddenRootNode->flattenedLeafNodes();
}

std::vector<std::shared_ptr<SKAsymmetricAtom> > SKAtomTreeController::flattenedObjects() const
{
   return _hiddenRootNode->flattenedObjects();
}


// MARK: Selection
// =========================

// Deletion is done bottom-up (post-order).

void SKAtomTreeController::deleteSelection()
{
  std::vector<std::shared_ptr<SKAtomTreeNode>> atomNodes;
  std::copy(_selectedTreeNodes.begin(), _selectedTreeNodes.end(), std::back_inserter(atomNodes));

  std::sort(atomNodes.begin(), atomNodes.end(), [](std::shared_ptr<SKAtomTreeNode> a, std::shared_ptr<SKAtomTreeNode> b) -> bool {
      return a->indexPath() > b->indexPath();
  });

  for(std::shared_ptr<SKAtomTreeNode> atom : atomNodes)
  {
    IndexPath indexPath = atom->indexPath();
    int lastIndex = indexPath.lastIndex();
    const std::shared_ptr<SKAtomTreeNode> parentItem = atom->parent();
    parentItem->childNodes().erase(parentItem->childNodes().begin() + lastIndex);
  };

  _selectedTreeNode = nullptr;
  _selectedTreeNodes.clear();
}

std::vector<std::shared_ptr<SKAtomCopy>> SKAtomTreeController::allAtomCopies() const
{
  std::vector<std::shared_ptr<SKAtomCopy>> atomCopies = std::vector<std::shared_ptr<SKAtomCopy>>();

  std::vector<std::shared_ptr<SKAtomTreeNode>> asymmetricAtomNodes = flattenedLeafNodes();

  for(std::shared_ptr<SKAtomTreeNode> node: asymmetricAtomNodes)
  {
    if(std::shared_ptr<SKAsymmetricAtom> atom = node->representedObject())
    {
      for(std::shared_ptr<SKAtomCopy> copy: atom->copies())
      {
        atomCopies.push_back(copy);
      }
    }
  }
  return atomCopies;
}

std::vector<std::shared_ptr<SKAtomCopy>> SKAtomTreeController::atomCopies() const
{
  std::vector<std::shared_ptr<SKAtomCopy>> atomCopies = std::vector<std::shared_ptr<SKAtomCopy>>();

  std::vector<std::shared_ptr<SKAtomTreeNode>> asymmetricAtomNodes = flattenedLeafNodes();

  for(std::shared_ptr<SKAtomTreeNode> node: asymmetricAtomNodes)
  {
    if(std::shared_ptr<SKAsymmetricAtom> atom = node->representedObject())
    {
      for(std::shared_ptr<SKAtomCopy> copy: atom->copies())
      {
        if(copy->type() == SKAtomCopy::AtomCopyType::copy)
        {
          atomCopies.push_back(copy);
        }
      }
    }
  }
  return atomCopies;
}

void SKAtomTreeController::setTags()
{
  std::vector<std::shared_ptr<SKAtomTreeNode>> asymmetricAtomNodes = flattenedLeafNodes();

  int atomIndex=0;
  int asymmetricAtomIndex=0;
  for(std::shared_ptr<SKAtomTreeNode> node: asymmetricAtomNodes)
  {
    if(std::shared_ptr<SKAsymmetricAtom> asymmetricAtom = node->representedObject())
    {
      for(std::shared_ptr<SKAtomCopy> atom: asymmetricAtom->copies())
      {
        atom->setTag(atomIndex);
        atom->setAsymmetricIndex(asymmetricAtomIndex);
        atomIndex++;
      }
      asymmetricAtom->setTag(asymmetricAtomIndex);
      asymmetricAtom->setAsymmetricIndex(asymmetricAtomIndex);
      asymmetricAtomIndex++;
    }
  }
}

std::vector<std::shared_ptr<SKAtomTreeNode>> SKAtomTreeController::selectedAtomTreeNodes()
{
  return std::vector<std::shared_ptr<SKAtomTreeNode>>(_selectedTreeNodes.begin(),_selectedTreeNodes.end());
}


QDataStream &operator<<(QDataStream& stream, const std::shared_ptr<SKAtomTreeController>& controller)
{
  stream << controller->_versionNumber;
  stream << controller->_hiddenRootNode;
  return stream;
}

QDataStream &operator>>(QDataStream& stream, std::shared_ptr<SKAtomTreeController>& controller)
{
  qint64 versionNumber;
  stream >> versionNumber;
  if(versionNumber > controller->_versionNumber)
  {
    throw InvalidArchiveVersionException(__FILE__, __LINE__, "SKAtomTreeController");
  }

  std::cout << "starte decoding from hiddenRootNode" << std::endl;
  stream >> controller->_hiddenRootNode;
  return stream;
}
