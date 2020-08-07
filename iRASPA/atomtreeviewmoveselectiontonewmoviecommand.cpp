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

#include "atomtreeviewmoveselectiontonewmoviecommand.h"
#include <QDebug>
#include <algorithm>

AtomTreeViewMoveSelectionToNewMovieCommand::AtomTreeViewMoveSelectionToNewMovieCommand(MainWindow *mainWindow,
                                                                                       AtomTreeViewModel *atomTreeViewModel,
                                                                                       BondListViewModel *bondListViewModel,
                                                                                       SceneTreeViewModel *sceneTreeViewModel,
                                                                                       std::shared_ptr<SceneList> sceneList,
                                                                                       std::shared_ptr<iRASPAStructure> iraspaStructure,
                                                                                       AtomSelection atomSelection,
                                                                                       BondSelection bondSelection,
                                                                                       QUndoCommand *undoParent):
  QUndoCommand(undoParent),
  _mainWindow(mainWindow),
  _atomTreeViewModel(atomTreeViewModel),
  _bondListViewModel(bondListViewModel),
  _sceneTreeViewModel(sceneTreeViewModel),
  _sceneList(sceneList),
  _iraspaStructure(iraspaStructure),
  _atomSelection(atomSelection),
  _reverseAtomSelection({}),
  _bondSelection(bondSelection),
  _sceneSelection(sceneList->allSelection()),
  _scene(nullptr),
  _newMovie(nullptr),
  _row(0)
{
  setText(QString("Move atoms to new movie"));

  std::reverse_copy(std::begin(atomSelection), std::end(atomSelection), std::back_inserter(_reverseAtomSelection));
  std::reverse_copy(std::begin(bondSelection), std::end(bondSelection), std::back_inserter(_reverseBondSelection));
}

void AtomTreeViewMoveSelectionToNewMovieCommand::redo()
{
  if(std::shared_ptr<Movie> movie = _iraspaStructure->parent().lock())
  {
    if((_scene = movie->parent().lock()))
    {
      _row = _scene->movies().size();
      std::shared_ptr<Structure> newStructure = _iraspaStructure->structure()->clone();
      newStructure->setSpaceGroupHallNumber(_iraspaStructure->structure()->spaceGroup().spaceGroupSetting().HallNumber());
      std::shared_ptr<iRASPAStructure> newiRASPAStructure = std::make_shared<iRASPAStructure>(newStructure);
      std::shared_ptr<Movie> _newMovie = Movie::create(newiRASPAStructure);
      _newMovie->setSelectedFrame(newiRASPAStructure);

      for(const auto [atomTreeNode, indexPath] : _atomSelection)
      {
        if(const std::shared_ptr<SKAsymmetricAtom> asymmetricAtom = atomTreeNode->representedObject())
        {
          std::shared_ptr<SKAsymmetricAtom> newAsymmetricAtom = std::make_shared<SKAsymmetricAtom>(*asymmetricAtom);
          std::shared_ptr<SKAtomTreeNode> newAtomTreeNode = std::make_shared<SKAtomTreeNode>(newAsymmetricAtom);
          newStructure->atomsTreeController()->appendToRootnodes(newAtomTreeNode);
        }

      }
      newStructure->expandSymmetry();
      newStructure->reComputeBoundingBox();
      newStructure->computeBonds();

      _bondListViewModel->deleteSelection(_iraspaStructure->structure(), _reverseBondSelection);
      _atomTreeViewModel->deleteSelection(_iraspaStructure->structure(), _reverseAtomSelection);

      _sceneTreeViewModel->insertRow(_row, _scene, _newMovie);
      _sceneList->setSelection(_sceneSelection);

      emit _atomTreeViewModel->rendererReloadData();
      emit _sceneTreeViewModel->updateSelection();
    }
  }
}

void AtomTreeViewMoveSelectionToNewMovieCommand::undo()
{
  _sceneTreeViewModel->removeRow(_row, _scene, _newMovie);

  _atomTreeViewModel->insertSelection(_iraspaStructure->structure(), _atomSelection);
  _bondListViewModel->insertSelection(_iraspaStructure->structure(), _bondSelection);

  emit _atomTreeViewModel->rendererReloadData();
  _sceneList->setSelection(_sceneSelection);
  emit _sceneTreeViewModel->updateSelection();
}
