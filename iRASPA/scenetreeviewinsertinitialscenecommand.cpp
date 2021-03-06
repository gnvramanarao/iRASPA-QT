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

#include "scenetreeviewinsertinitialscenecommand.h"
#include <QDebug>
#include <algorithm>

SceneTreeViewInsertInitialSceneCommand::SceneTreeViewInsertInitialSceneCommand(MainWindow *main_window, SceneTreeView *sceneTreeView, std::shared_ptr<SceneList> sceneList,
                                                                               std::shared_ptr<Scene> scene, std::shared_ptr<Movie> movie,
                                                                               int row, QUndoCommand *undoParent):
  _main_window(main_window),
  _sceneTreeView(sceneTreeView),
  _sceneList(sceneList),
  _scene(scene),
  _movie(movie),
  _row(row)
{
  Q_UNUSED(undoParent);

  setText(QString("Insert scene/movie"));
}

void SceneTreeViewInsertInitialSceneCommand::redo()
{
  if(_sceneTreeView)
  {
    if(SceneTreeViewModel* pModel = qobject_cast<SceneTreeViewModel*>(_sceneTreeView->model()))
    {
      pModel->insertRow(0, _scene);
      _sceneTreeView->expandAll();
      _sceneList->setSelection(SceneListSelection(_scene, {_scene}, {{_scene, _movie}}, {{_scene, {_movie}}}));
    }
  }

  if(_main_window)
  {
    _sceneTreeView->reloadSelection();
    _main_window->recheckRemovalButtons();
  }
}

void SceneTreeViewInsertInitialSceneCommand::undo()
{
  if(_sceneTreeView)
  {
    if(SceneTreeViewModel* pModel = qobject_cast<SceneTreeViewModel*>(_sceneTreeView->model()))
    {
      pModel->removeRow(0);
      _sceneList->setSelection(SceneListSelection(nullptr, {}, {}, {}));
    }
  }

  if(_main_window)
  {
    _sceneTreeView->reloadSelection();
    _main_window->recheckRemovalButtons();
  }
}
