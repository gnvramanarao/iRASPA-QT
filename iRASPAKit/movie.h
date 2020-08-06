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
#include <unordered_set>
#include "iraspastructure.h"
#include "iraspakitprotocols.h"

class Scene;

class Movie: public std::enable_shared_from_this<Movie>, public DisplayableProtocol
{
public:

  static char mimeType[];

  Movie();
  Movie(QString displayName);

  static std::shared_ptr<Movie> create(std::shared_ptr<iRASPAStructure> structure);
  virtual ~Movie() {}
  std::shared_ptr<Movie> shallowClone();

  void append(std::shared_ptr<iRASPAStructure> structure);
  const std::vector<std::shared_ptr<iRASPAStructure>> &frames() const {return _frames;}
  QString displayName() const override final;
  void setDisplayName(QString name) {_displayName = name;}
  bool isVisible() {return _isVisible;}
  void setVisibility(bool visibility);

  std::shared_ptr<iRASPAStructure> selectedFrame();
  std::optional<int> selectedFrameIndex();
  void setSelectedFrameIndex(int frameIndex);
  void setSelectedFrame(std::shared_ptr<iRASPAStructure> selectedFrame);
  std::set<std::shared_ptr<iRASPAStructure>> &selectedFramesSet() {return _selectedFramesSet;}
  std::set<int> selectedFramesIndexSet();

  std::vector<std::shared_ptr<iRASPAStructure>> selectedFramesiRASPAStructures() const;

  bool removeChildren(size_t position, size_t count);
  void setParent(std::weak_ptr<Scene> parent) {_parent = parent;}
  std::weak_ptr<Scene> parent() {return _parent;}
private:
  Movie(std::shared_ptr<iRASPAStructure> structure);
  qint64 _versionNumber{1};
  bool _isVisible = true;
  bool _isLoading = false;
  QString _displayName = QString("Movie");

  std::weak_ptr<Scene> _parent{};
  std::vector<std::shared_ptr<iRASPAStructure>> _frames{};
  std::shared_ptr<iRASPAStructure> _selectedFrame;
  std::set<std::shared_ptr<iRASPAStructure>> _selectedFramesSet;

  friend QDataStream &operator<<(QDataStream &, const std::shared_ptr<Movie> &);
  friend QDataStream &operator>>(QDataStream &, std::shared_ptr<Movie> &);
};

