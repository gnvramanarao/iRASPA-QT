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
#include <QSharedPointer>
#include <vector>
#include <tuple>
#include <memory>
#include <ostream>
#include "scene.h"
#include "iraspakitprotocols.h"

using SceneListSelection = std::tuple<std::shared_ptr<Scene>,
                                      std::set<std::shared_ptr<Scene>>,
                                      std::map<std::shared_ptr<Scene>, std::shared_ptr<Movie>>,
                                      std::map<std::shared_ptr<Scene>, std::set<std::shared_ptr<Movie>>> >;


class SceneList: public std::enable_shared_from_this<SceneList>, public DisplayableProtocol
{
public:
  SceneList();
  void appendScene(std::shared_ptr<Scene> scene);
  const std::vector<std::shared_ptr<Scene>> &scenes() const {return _scenes;}
  std::shared_ptr<Scene> selectedScene();
  std::set<std::shared_ptr<Scene>> &selectedScenes() {return _selectedScenes;}
  void setSelectedScene(std::shared_ptr<Scene> scene);
  void setSelectedFrameIndices(int frameIndex);
  QString displayName() const override final;
  std::optional<int> selectedSceneIndex();
  std::optional<int> findChildIndex(std::shared_ptr<Scene> scene);
  bool removeChild(size_t row);
  bool removeChildren(size_t position, size_t count);
  bool insertChild(size_t row, std::shared_ptr<Scene> child);

  std::vector<std::shared_ptr<iRASPAStructure>> allIRASPAStructures();
  std::vector<std::shared_ptr<iRASPAStructure>> selectedMoviesIRASPAStructures();
  std::vector<std::vector<std::shared_ptr<iRASPAStructure>>> selectediRASPARenderStructures() const;

  SceneListSelection allSelection();
  void setSelection(SceneListSelection selection);
private:
  qint64 _versionNumber{1};
  QString _displayName = QString("");
  std::vector<std::shared_ptr<Scene>> _scenes{};
  std::shared_ptr<Scene> _selectedScene;
  std::set<std::shared_ptr<Scene>> _selectedScenes;

  friend QDataStream &operator<<(QDataStream &, const std::shared_ptr<SceneList> &);
  friend QDataStream &operator>>(QDataStream &, std::shared_ptr<SceneList> &);
};
