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

#include <QString>
#include <vector>
#include <memory>
#include <iostream>
#include <foundationkit.h>
#include "iraspaproject.h"

class ProjectTreeController;

class ProjectTreeNode: public std::enable_shared_from_this<ProjectTreeNode>
{
public:
    ProjectTreeNode(QString displayName = "Default", bool isEditable=true, bool isDropEnabled=true): _displayName(displayName), _representedObject(std::make_shared<iRASPAProject>()), _isEditable(isEditable), _isDropEnabled(isDropEnabled) {}
    ProjectTreeNode(std::shared_ptr<iRASPAProject> representedObject, bool isEditable=true, bool isDropEnabled=true):  _representedObject(representedObject), _isEditable(isEditable), _isDropEnabled(isDropEnabled) {}
    ProjectTreeNode(QString displayName, std::shared_ptr<iRASPAProject> representedObject, bool isEditable=true, bool isDropEnabled=true): _displayName(displayName), _representedObject(representedObject), _isEditable(isEditable), _isDropEnabled(isDropEnabled) {}
    ~ProjectTreeNode();

    bool removeChild(int row);
    bool insertChild(int row, std::shared_ptr<ProjectTreeNode> child);

    std::shared_ptr<ProjectTreeNode> getChildNode(int index) {return _childNodes[index];}
    int row() const;
    int childCount() {return _childNodes.size();}

    inline  std::vector<std::shared_ptr<ProjectTreeNode>> childNodes() const {return this->_childNodes;}
    inline  std::vector<std::shared_ptr<ProjectTreeNode>> filteredChildNodes() const {return this->_filteredAndSortedNodes;}
    //inline std::weak_ptr<ProjectTreeNode> parent()  {return this->_parent;}
    inline const std::shared_ptr<ProjectTreeNode> parent() const {return _parent.lock();}
    std::shared_ptr<iRASPAProject> representedObject();

    inline const QString displayName() const {return this->_displayName;}
    void setDisplayName(QString name) {_displayName = name;}

    void setRepresentedObject(std::shared_ptr<iRASPAProject> object) {_representedObject = object;}

    inline bool isEditable() {return _isEditable;}
    inline bool isDropEnabled() {return _isDropEnabled;}
    void insertInParent(std::shared_ptr<ProjectTreeNode> parent, int index);
    void appendToParent(std::shared_ptr<ProjectTreeNode> parent);
    const IndexPath indexPath();
    std::shared_ptr<ProjectTreeNode> descendantNodeAtIndexPath(IndexPath indexPath);
    std::vector<std::shared_ptr<ProjectTreeNode>> flattenedNodes();
    std::vector<std::shared_ptr<ProjectTreeNode>> flattenedLeafNodes();
    std::vector<std::shared_ptr<ProjectTreeNode>> flattenedGroupNodes();
    std::vector<std::shared_ptr<ProjectTreeNode>> descendants();
    std::vector<std::shared_ptr<ProjectTreeNode>> descendantNodes();
    std::vector<std::shared_ptr<ProjectTreeNode>> descendantLeafNodes();
    std::vector<std::shared_ptr<ProjectTreeNode>> descendantGroupNodes();
    std::vector<std::shared_ptr<iRASPAProject>> flattenedObjects();
    std::vector<std::shared_ptr<iRASPAProject>> descendantObjects();
    bool isDescendantOfNode(ProjectTreeNode* parent);
    void updateFilteredChildren(std::function<bool(std::shared_ptr<ProjectTreeNode>)> predicate);
    void updateFilteredChildrenRecursively(std::function<bool(std::shared_ptr<ProjectTreeNode>)> predicate);
    void setFilteredNodesAsMatching();
    bool insertChildren(int position, int count, int columns);
    bool setData(const QVariant &value);
private:
    qint64 _versionNumber{1};
    QString _displayName;
    std::weak_ptr<ProjectTreeNode> _parent{};
    std::shared_ptr<iRASPAProject> _representedObject{};

    std::vector<std::shared_ptr<ProjectTreeNode>> _childNodes{};
    std::vector<std::shared_ptr<ProjectTreeNode>> _filteredAndSortedNodes{};
    std::optional<int> findChildIndex(std::shared_ptr<ProjectTreeNode> child);

    bool _matchesFilter = true;
    bool _selected = true;
    bool _isEditable;
    bool _isDropEnabled;

    friend ProjectTreeController;
    friend QDataStream &operator<<(QDataStream &, const std::shared_ptr<ProjectTreeNode> &);
    friend QDataStream &operator>>(QDataStream &, std::shared_ptr<ProjectTreeNode> &);
};

