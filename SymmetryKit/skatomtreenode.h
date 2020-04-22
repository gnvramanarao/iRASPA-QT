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

#include <vector>
#include <memory>
#include <functional>
#include <QDataStream>
#include <optional>
#include <foundationkit.h>
#include "skasymmetricatom.h"

class SKAtomTreeController;

class SKAtomTreeNode: public std::enable_shared_from_this<SKAtomTreeNode>
{
public:
    SKAtomTreeNode(): _representedObject(std::make_shared<SKAsymmetricAtom>()) {}
    SKAtomTreeNode(std::shared_ptr<SKAsymmetricAtom> representedObject): _representedObject(representedObject) {_displayName = representedObject->displayName();}
    SKAtomTreeNode(std::weak_ptr<SKAtomTreeNode> parent, std::shared_ptr<SKAsymmetricAtom> representedObject): _parent(parent), _representedObject(representedObject) {_displayName = representedObject->displayName();}
    ~SKAtomTreeNode();

    inline  std::vector<std::shared_ptr<SKAtomTreeNode>> childNodes() const {return this->_childNodes;}
    std::shared_ptr<SKAtomTreeNode> getChildNode(int index) {return _childNodes[index];}
    inline  std::vector<std::shared_ptr<SKAtomTreeNode>> filteredChildNodes() const {return this->_filteredAndSortedNodes;}
    inline const std::shared_ptr<SKAtomTreeNode> parent() const {return _parent.lock();}
    inline const std::shared_ptr<SKAsymmetricAtom> representedObject() const {return this->_representedObject;}
    inline const QString displayName() const {return this->_displayName;}
    void setDisplayName(QString name) {_displayName = name;}

    int childCount() {return _childNodes.size();}
    inline bool isLeaf() {return !_isGroup;}
    inline bool isGroup() {return _isGroup;}
    bool insertChild(int row, std::shared_ptr<SKAtomTreeNode> child);
    void insertInParent(std::shared_ptr<SKAtomTreeNode> parent, int index);
    void appendToParent(std::shared_ptr<SKAtomTreeNode> parent);
    bool removeChild(int position);
    void removeFromParent();
    const IndexPath indexPath();
    std::shared_ptr<SKAtomTreeNode> descendantNodeAtIndexPath(IndexPath indexPath);
    std::vector<std::shared_ptr<SKAtomTreeNode>> flattenedNodes();
    std::vector<std::shared_ptr<SKAtomTreeNode>> flattenedLeafNodes();
    std::vector<std::shared_ptr<SKAtomTreeNode>> flattenedGroupNodes();
    std::vector<std::shared_ptr<SKAtomTreeNode>> descendants();
    std::vector<std::shared_ptr<SKAtomTreeNode>> descendantNodes();
    std::vector<std::shared_ptr<SKAtomTreeNode>> descendantLeafNodes();
    std::vector<std::shared_ptr<SKAtomTreeNode>> descendantGroupNodes();
    std::vector<std::shared_ptr<SKAsymmetricAtom>> flattenedObjects();
    std::vector<std::shared_ptr<SKAsymmetricAtom>> descendantObjects();
    bool isDescendantOfNode(std::shared_ptr<SKAtomTreeNode> parent);
    void updateFilteredChildren(std::function<bool(std::shared_ptr<SKAtomTreeNode>)> predicate);
    void updateFilteredChildrenRecursively(std::function<bool(std::shared_ptr<SKAtomTreeNode>)> predicate);
    void setFilteredNodesAsMatching();
    std::optional<int> findChildIndex(std::shared_ptr<SKAtomTreeNode> child);
    int row() const;
    void setParent(std::shared_ptr<SKAtomTreeNode> parent) {_parent = parent;}
    void setGroupItem(bool state) {_isGroup = state;}
private:
    qint64 _versionNumber{1};
    QString _displayName;
    std::weak_ptr<SKAtomTreeNode> _parent;
    std::shared_ptr<SKAsymmetricAtom> _representedObject;

    std::vector<std::shared_ptr<SKAtomTreeNode>> _childNodes;
    std::vector<std::shared_ptr<SKAtomTreeNode>> _filteredAndSortedNodes;

    bool _matchesFilter = true;
    bool _selected = true;
    bool _isGroup = false;
    bool _isEditable = false;

    friend SKAtomTreeController ;
    friend QDataStream &operator<<(QDataStream &, const std::shared_ptr<SKAtomTreeNode> &);
    friend QDataStream &operator>>(QDataStream &, std::shared_ptr<SKAtomTreeNode> &);
};

