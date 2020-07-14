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

#include <QUuid>
#include <QByteArray>
#include "project.h"
#include "projectstructure.h"
#include "projectgroup.h"
#include <foundationkit.h>



class iRASPAProject
{
public:
  iRASPAProject();
  iRASPAProject(std::shared_ptr<Project> project);
  iRASPAProject(std::shared_ptr<ProjectStructure> project);
  iRASPAProject(std::shared_ptr<ProjectGroup> project);

  enum class ProjectType: qint64 {none=0, generic=1, group=2, structure=3, VASP=4, RASPA=5, GULP=6, CP2K=7};
  enum class NodeType: qint64 {group=0, leaf=1};
  enum class StorageType: qint64 {local = 0, publicCloud = 1, privateCloud = 2, sharedCloud};
  enum class LazyStatus: qint64 { lazy = 0, loaded = 1, error = 2};

  void readData(ZipReader& reader);
  void saveData(ZipWriter& writer);
  void unwrapIfNeeded();
  bool isLeaf() {return _nodeType == NodeType::leaf;}
  bool isGroup() {return _nodeType == NodeType::group;}
  void setIsGroup(bool value) {_nodeType = value ? NodeType::group : NodeType::leaf;}
  inline const std::shared_ptr<Project> project() const {return _project;}
  std::shared_ptr<Project> project() {return _project;}
  ~iRASPAProject() {}
  QUndoStack& undoManager() {return _undoStack;}
private:
  qint64 _versionNumber{1};
  ProjectType _projectType;
  QString _fileNameUUID;
  std::shared_ptr<Project> _project;
  NodeType _nodeType;
  StorageType _storageType;
  LazyStatus _lazyStatus;
  QByteArray _data;

  QUndoStack _undoStack;

  friend QDataStream &operator<<(QDataStream &, const std::shared_ptr<iRASPAProject> &);
  friend QDataStream &operator>>(QDataStream &, std::shared_ptr<iRASPAProject> &);

  friend QDataStream &operator<<=(QDataStream &, const std::shared_ptr<iRASPAProject> &);
  friend QDataStream &operator>>=(QDataStream &, std::shared_ptr<iRASPAProject> &);
};


