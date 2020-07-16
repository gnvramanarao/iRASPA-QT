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

#pragma once

#include <unordered_set>
#include <vector>
#include <optional>
#include <foundationkit.h>
#include "skelement.h"
#include "skatomtreenode.h"


// inserting with nil as parent means inserting in the rootNodes
// hiddenRooNode convenient for resursion of all nodes

class SKAtomTreeController
{
public:
    SKAtomTreeController();
    ~SKAtomTreeController();

    const std::vector<std::shared_ptr<SKAtomTreeNode>> rootNodes() const {return _hiddenRootNode->_childNodes;}
    const std::vector<std::shared_ptr<SKAtomTreeNode>> filteredRootNodes() const {return _hiddenRootNode->_filteredAndSortedNodes;}


    std::shared_ptr<SKAtomTreeNode> hiddenRootNode() {return _hiddenRootNode;}
    void insertNodeInParent(std::shared_ptr<SKAtomTreeNode> node, std::weak_ptr<SKAtomTreeNode> parent, int index);
    void insertNodeAtIndexPath(std::shared_ptr<SKAtomTreeNode> node, IndexPath path);
    void removeNode(std::shared_ptr<SKAtomTreeNode> node);
    //std::shared_ptr<SKAtomTreeNode> parentItem(std::shared_ptr<SKAtomTreeNode> node);
    //bool isRootNode(std::shared_ptr<SKAtomTreeNode> node);
    bool isRootNode(SKAtomTreeNode *node);
    bool nodeIsChildOfItem(std::shared_ptr<SKAtomTreeNode> node, std::shared_ptr<SKAtomTreeNode> item);
    bool isDescendantOfNode(std::shared_ptr<SKAtomTreeNode> item, std::shared_ptr<SKAtomTreeNode> parentNode);
    size_t filteredChildIndexOfItem(std::shared_ptr<SKAtomTreeNode> parentItem, size_t index);
    std::vector<std::shared_ptr<SKAtomTreeNode>> childrenForItem(std::shared_ptr<SKAtomTreeNode> item);
    std::shared_ptr<SKAtomTreeNode> nodeAtIndexPath(IndexPath indexpath);
    void appendToRootnodes(std::shared_ptr<SKAtomTreeNode> item);
    void deleteSelection();

    std::vector<std::shared_ptr<SKAtomTreeNode>> flattenedLeafNodes() const;
    std::vector<std::shared_ptr<SKAsymmetricAtom>> flattenedObjects() const;
    std::vector<std::shared_ptr<SKAtomCopy>> allAtomCopies() const;
    std::vector<std::shared_ptr<SKAtomCopy>> atomCopies() const;

    std::unordered_set<std::shared_ptr<SKAtomTreeNode>>& selectedTreeNodes() {return _selectedTreeNodes;}
    void setSelectedTreeNodes(std::unordered_set<std::shared_ptr<SKAtomTreeNode>> selectedNodes) {_selectedTreeNodes = selectedNodes;}
    void setTags();

    std::vector<std::shared_ptr<SKAtomTreeNode>> selectedAtomTreeNodes();
private:
    qint64 _versionNumber{1};
    std::shared_ptr<SKAtomTreeNode> _hiddenRootNode{nullptr};
    std::shared_ptr<SKAtomTreeNode> _selectedTreeNode{nullptr};
    std::unordered_set<std::shared_ptr<SKAtomTreeNode>> _selectedTreeNodes{};
    std::function<bool(std::shared_ptr<SKAtomTreeNode>)> _filterPredicate = [](std::shared_ptr<SKAtomTreeNode>) { return true; };

    friend QDataStream &operator<<(QDataStream &, const std::shared_ptr<SKAtomTreeController> &);
    friend QDataStream &operator>>(QDataStream &, std::shared_ptr<SKAtomTreeController> &);
};



