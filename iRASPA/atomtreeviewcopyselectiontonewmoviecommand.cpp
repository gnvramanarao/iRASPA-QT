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

#include "atomtreeviewcopyselectiontonewmoviecommand.h"
#include "scenetreeviewdraganddropmovemoviesubcommand.h"
#include <QDebug>
#include <algorithm>

AtomTreeViewCopySelectionToNewMovieCommand::AtomTreeViewCopySelectionToNewMovieCommand(MainWindow *mainWindow,
                                                                                       AtomTreeViewModel *atomTreeViewModel,
                                                                                       SceneTreeViewModel *sceneTreeViewModel,
                                                                                       std::shared_ptr<SceneList> sceneList,
                                                                                       std::shared_ptr<iRASPAStructure> iraspaStructure,
                                                                                       std::set<std::shared_ptr<SKAtomTreeNode>> atomTreeNodes,
                                                                                       QUndoCommand *undoParent):
  QUndoCommand(undoParent),
  _mainWindow(mainWindow),
  _atomTreeViewModel(atomTreeViewModel),
  _sceneTreeViewModel(sceneTreeViewModel),
  _sceneList(sceneList),
  _iraspaStructure(iraspaStructure),
  _atomTreeNodes(atomTreeNodes),
  _scene(nullptr),
  _newMovie(nullptr),
  _row(0)
{
  setText(QString("Copy atoms to new movie"));
}

void AtomTreeViewCopySelectionToNewMovieCommand::redo()
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

      for(std::shared_ptr<SKAtomTreeNode> atomTreeNode : _atomTreeNodes)
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

      _sceneTreeViewModel->insertRow(_row, _scene, _newMovie);
    }
  }
}

void AtomTreeViewCopySelectionToNewMovieCommand::undo()
{
  _sceneTreeViewModel->removeRow(_row, _scene, _newMovie);
}
