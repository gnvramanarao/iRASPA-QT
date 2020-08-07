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

#include <QUndoCommand>
#include <set>
#include <vector>
#include "iraspakit.h"
#include "indexpath.h"
#include "symmetrykit.h"
#include "mathkit.h"
#include "atomtreeviewmodel.h"
#include "bondlistviewmodel.h"
#include "scenetreeviewmodel.h"

class AtomTreeViewMoveSelectionToNewMovieCommand : public QUndoCommand
{
public:
  AtomTreeViewMoveSelectionToNewMovieCommand(MainWindow *mainWindow, AtomTreeViewModel *atomTreeViewModel, BondListViewModel *bondListViewModel,
                                             SceneTreeViewModel *sceneTreeViewModel, std::shared_ptr<SceneList> sceneList,
                                             std::shared_ptr<iRASPAStructure> iraspaStructure, AtomSelection atomSelection, BondSelection bondSelection,
                                             QUndoCommand *undoParent = nullptr);
  void redo() override final;
  void undo() override final;
private:
  MainWindow *_mainWindow;
  AtomTreeViewModel *_atomTreeViewModel;
  BondListViewModel *_bondListViewModel;
  SceneTreeViewModel *_sceneTreeViewModel;
  std::shared_ptr<SceneList> _sceneList;
  std::shared_ptr<iRASPAStructure> _iraspaStructure;
  AtomSelection _atomSelection;
  AtomSelection _reverseAtomSelection;
  BondSelection _bondSelection;
  BondSelection _reverseBondSelection;
  SceneListSelection _sceneSelection;
  std::shared_ptr<Scene> _scene;
  std::shared_ptr<Movie> _newMovie;
  int _row;
};

