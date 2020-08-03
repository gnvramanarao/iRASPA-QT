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

#include "scenetreeviewdeleteselectioncommand.h"
#include "scenetreeviewdeletemoviesubcommand.h"
#include <QDebug>
#include <algorithm>

SceneTreeViewDeleteSelectionCommand::SceneTreeViewDeleteSelectionCommand(MainWindow *main_window, SceneTreeView *sceneTreeView,
                                                                         std::shared_ptr<SceneList> sceneList,
                                                                         std::vector<std::shared_ptr<Movie>> movies,
                                                                         SceneListSelection selection,
                                                                         QUndoCommand *undoParent):
  QUndoCommand(undoParent),
  _main_window(main_window),
  _sceneTreeView(sceneTreeView),
  _sceneList(sceneList),
  _movies(movies),
  _selection(selection)
{
  Q_UNUSED(undoParent);

  setText(QString("Delete movie selection"));

  for(std::shared_ptr<Movie> movie : _movies)
  {
    new SceneTreeViewDeleteMovieSubCommand(_main_window, _sceneTreeView, _sceneList, movie, this);
  }
}

void SceneTreeViewDeleteSelectionCommand::redo()
{
   QUndoCommand::redo();

   if(_main_window)
   {
     // after removing the selection, choose the first scene/movie as the new selection
     std::shared_ptr<Scene> newSelectedScene = _sceneList->scenes().empty() ? nullptr : _sceneList->scenes().front();
     std::shared_ptr<Movie> newSelectedMovie = newSelectedScene ? newSelectedScene->movies().front() : nullptr;
     _sceneList->setSelection({newSelectedScene, {newSelectedScene}, {{newSelectedScene, newSelectedMovie}}, {{newSelectedScene, {newSelectedMovie}}}});

     _sceneTreeView->reloadSelection();
     _main_window->reloadDetailViews();
   }
}

void SceneTreeViewDeleteSelectionCommand::undo()
{
  // automatically peform the undo's
  QUndoCommand::undo();

  if(_main_window)
  {
    _sceneList->setSelection(_selection);
    _sceneTreeView->reloadSelection();
    _main_window->reloadDetailViews();
  }
}
