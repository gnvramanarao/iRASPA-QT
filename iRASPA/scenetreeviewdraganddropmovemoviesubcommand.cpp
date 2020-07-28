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

#include "scenetreeviewdraganddropmovemoviesubcommand.h"
#include <QDebug>
#include <algorithm>

SceneTreeViewDragAndDropMoveMovieSubCommand::SceneTreeViewDragAndDropMoveMovieSubCommand(SceneTreeViewModel *sceneTreeViewModel, std::shared_ptr<SceneList> sceneList, std::shared_ptr<Scene> parent,
                                                                                         std::shared_ptr<Movie> movie, int row, QUndoCommand *undoParent):
  QUndoCommand(undoParent),
  _sceneTreeViewModel(sceneTreeViewModel),
  _sceneList(sceneList),
  _parent(parent),
  _movie(movie),
  _row(row),
  _movieRow(std::nullopt),
  _sceneRow(std::nullopt)
{
  Q_UNUSED(undoParent);

  setText(QString("Move movie(s)"));
}

void SceneTreeViewDragAndDropMoveMovieSubCommand::redo()
{
  _movieParent = _sceneTreeViewModel->parentForMovie(_movie);
  _movieRow = _movieParent->findChildIndex(_movie);
  if(_movieRow)
  {
    _sceneTreeViewModel->removeRow(*_movieRow, _movieParent, _movie);
    if(_movieParent->movies().empty())
    {
      // remove _movieParent if it would become empty
      if((_sceneRow = _sceneList->findChildIndex(_movieParent)))
      {
        _sceneTreeViewModel->removeRow(*_sceneRow);
      }
    }
  }
  _sceneTreeViewModel->insertRow(_row, _parent, _movie);
}

void SceneTreeViewDragAndDropMoveMovieSubCommand::undo()
{
  _sceneTreeViewModel->removeRow(_row, _parent, _movie);

  if(_sceneRow)
  {
    _sceneTreeViewModel->insertRow(*_sceneRow, _movieParent);
  }

  if(_movieRow)
  {
    _sceneTreeViewModel->insertRow(*_movieRow, _movieParent, _movie);
  }
}
