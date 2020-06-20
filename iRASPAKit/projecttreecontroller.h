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

#include <QAbstractItemModel>
#include <QModelIndex>
#include <unordered_set>
#include <vector>
#include <optional>
#include <foundationkit.h>
#include "projecttreenode.h"


// inserting with nil as parent means inserting in the rootNodes
// hiddenRooNode convenient for resursion of all nodes

class ProjectTreeController
{
public:
    ProjectTreeController();
    ~ProjectTreeController();

    const std::shared_ptr<ProjectTreeNode> hiddenRootNode() const {return _hiddenRootNode;}
    void setHiddenRootNode(std::shared_ptr<ProjectTreeNode> rootNode) {_hiddenRootNode = rootNode;}

    const std::vector<std::shared_ptr<ProjectTreeNode>> rootNodes() const {return _hiddenRootNode->_childNodes;}
    const std::vector<std::shared_ptr<ProjectTreeNode>> filteredRootNodes() const {return _hiddenRootNode->_filteredAndSortedNodes;}

    void insertNodeInParent(std::shared_ptr<ProjectTreeNode> node, std::shared_ptr<ProjectTreeNode> parent, int index);
    //void removeNode(std::shared_ptr<ProjectTreeNode> node);
    //std::shared_ptr<ProjectTreeNode> parentItem(std::shared_ptr<ProjectTreeNode> node);
    bool isRootNode(std::shared_ptr<ProjectTreeNode> node);
    bool nodeIsChildOfItem(std::shared_ptr<ProjectTreeNode> node, std::weak_ptr<ProjectTreeNode> item);
    bool isDescendantOfNode(std::shared_ptr<ProjectTreeNode> item, std::weak_ptr<ProjectTreeNode> parentNode);
    int filteredChildIndexOfItem(std::shared_ptr<ProjectTreeNode> parentItem, int index);
    std::vector<std::shared_ptr<ProjectTreeNode>> childrenForItem(std::shared_ptr<ProjectTreeNode> item);
    std::shared_ptr<ProjectTreeNode> nodeAtIndexPath(IndexPath indexpath);
    void appendToRootnodes(std::shared_ptr<ProjectTreeNode> item);
    void deleteSelection();

    std::vector<std::shared_ptr<ProjectTreeNode>> flattenedLeafNodes();

    std::shared_ptr<ProjectTreeNode> galleryProjects() {return _galleryProjects;}
    std::shared_ptr<ProjectTreeNode> localProjects() {return _localProjects;}
    std::shared_ptr<ProjectTreeNode> icloudProjects() {return _icloudProjects;}

    std::unordered_set<std::shared_ptr<ProjectTreeNode>>& selectedTreeNodes() {return _selectedTreeNodes;}
    std::shared_ptr<ProjectTreeNode>& selectedTreeNode() {return _selectedTreeNode;}
    void setSelectedTreeNode(std::shared_ptr<ProjectTreeNode> selectedNode) {_selectedTreeNode = selectedNode;}
private:
    qint64 _versionNumber{1};

    std::shared_ptr<ProjectTreeNode> _galleryProjects;
    std::shared_ptr<ProjectTreeNode> _localProjects;
    std::shared_ptr<ProjectTreeNode> _icloudProjects;

    std::shared_ptr<ProjectTreeNode> _hiddenRootNode;
    std::shared_ptr<ProjectTreeNode> _selectedTreeNode;
    std::unordered_set<std::shared_ptr<ProjectTreeNode>> _selectedTreeNodes{};
    std::function<bool(std::shared_ptr<ProjectTreeNode>)> _filterPredicate = [](std::shared_ptr<ProjectTreeNode>) { return true; };

    friend QDataStream &operator<<(QDataStream &, const std::shared_ptr<ProjectTreeController> &);
    friend QDataStream &operator>>(QDataStream &, std::shared_ptr<ProjectTreeController> &);
};



