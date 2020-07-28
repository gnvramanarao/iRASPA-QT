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

#include "scenetreeviewdropcopycommand.h"
#include "scenetreeviewdraganddropinsertscenesubcommand.h"
#include "scenetreeviewdraganddropinsertmoviesubcommand.h"
#include <QDebug>
#include <algorithm>

SceneTreeViewDropCopyCommand::SceneTreeViewDropCopyCommand(SceneTreeViewModel *sceneTreeViewModel, std::shared_ptr<SceneList> sceneList, std::shared_ptr<Scene> parent, int row,
                                                           std::vector<std::shared_ptr<Movie>> movies, SceneListSelection selection, QUndoCommand *undoParent):
  QUndoCommand(undoParent),
  _sceneTreeViewModel(sceneTreeViewModel),
  _sceneList(sceneList),
  _parent(parent),
  _row(row),
  _movies(movies),
  _selection(selection)
{
  Q_UNUSED(undoParent);

  setText(QString("Copy movies"));

  int beginRow = row;
  std::shared_ptr<Scene> newParent = _parent;

  if(!newParent)
  {
    std::shared_ptr<Scene> scene = std::make_shared<Scene>("NEW SCENE");
    int insertionRow = _sceneList->scenes().size();
    new SceneTreeViewDragAndDropInsertSceneSubCommand(_sceneTreeViewModel, scene, insertionRow, this);

    newParent = scene;
    beginRow = 0;
  }

  for(std::shared_ptr<Movie> movie : _movies)
  {
    new SceneTreeViewDragAndDropInsertMovieSubCommand(_sceneTreeViewModel, newParent, movie, beginRow, this);
    beginRow += 1;
  }
}

void SceneTreeViewDropCopyCommand::redo()
{
  QUndoCommand::redo();

  _sceneList->setSelection(_selection);
  emit _sceneTreeViewModel->updateSelection();
}

void SceneTreeViewDropCopyCommand::undo()
{
  QUndoCommand::undo();

  _sceneList->setSelection(_selection);
  emit _sceneTreeViewModel->updateSelection();
}
