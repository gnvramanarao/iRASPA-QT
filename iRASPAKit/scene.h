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
#include <QUrl>
#include <QSharedPointer>
#include <vector>
#include <tuple>
#include <unordered_set>
#include <optional>
#include <ostream>
#include <sstream>
#include <fstream>
#include <simulationkit.h>
#include "movie.h"
#include "iraspakitprotocols.h"


class Scene: public std::enable_shared_from_this<Scene>, public DisplayableProtocol
{
public:
  Scene();
  Scene(QString displayName);
  Scene(std::shared_ptr<Movie> movie);
  Scene(const QUrl url, const SKColorSets& colorSets, ForceFieldSets &forcefieldSets, LogReporting *log, bool asSeparateProject, bool onlyAsymmetricUnit, bool asMolecule);
  const std::vector<std::shared_ptr<Movie>> &movies() const {return _movies;}
  std::optional<int> findChildIndex(std::shared_ptr<Movie> movie);
  bool removeChild(size_t row);
  bool removeChildren(size_t position, size_t count);
  bool insertChild(size_t row, std::shared_ptr<Movie> child);
  void setSelectedMovie(std::shared_ptr<Movie> movie);
  void setSelectedMovies(std::unordered_set<std::shared_ptr<Movie>> movies);
  void setSelectedFrameIndices(int frameIndex);
  std::unordered_set<std::shared_ptr<Movie>>& selectedMovies() {return _selectedMovies;}
  std::shared_ptr<Movie> selectedMovie();
  QString displayName() const override final;
  std::optional<int> selectMovieIndex();
private:
  qint64 _versionNumber{1};
  QString _displayName = QString("Scene");
  std::vector<std::shared_ptr<Movie>> _movies{};
  std::shared_ptr<Movie> _selectedMovie{nullptr};
  std::unordered_set<std::shared_ptr<Movie>> _selectedMovies;

  friend QDataStream &operator<<(QDataStream &, const std::shared_ptr<Scene> &);
  friend QDataStream &operator>>(QDataStream &, std::shared_ptr<Scene> &);
};
