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

#include "atomtreeviewdeleteselectioncommand.h"
#include <QDebug>
#include <algorithm>

AtomTreeViewDeleteSelectionCommand::AtomTreeViewDeleteSelectionCommand(std::shared_ptr<AtomTreeViewModel> atomTreeModel, std::shared_ptr<BondListViewModel> bondTreeModel,
                                               MainWindow *main_window, std::shared_ptr<Structure> structure,
                                               std::vector<std::shared_ptr<SKAtomTreeNode>> atoms, std::vector<IndexPath> indexPaths,
                                               std::vector<std::shared_ptr<SKAsymmetricBond>> bonds, std::set<int> bondSelection, QUndoCommand *parent):
  _atomTreeModel(atomTreeModel),
  _bondTreeModel(bondTreeModel),
  _main_window(main_window),
  _structure(structure),
  _atoms(atoms),
  _indexPaths(indexPaths),
  _bonds(bonds),
  _bondSelection(bondSelection)
{
  Q_UNUSED(parent);

  setText(QString("Delete atom selection"));
}

void AtomTreeViewDeleteSelectionCommand::redo()
{
  qDebug() << "Redoing";
  qDebug() << "Deleting bonds: " << _bonds.size();
  qDebug() << "Deleting atoms: " << _atoms.size();

  if(std::shared_ptr<BondListViewModel> bondListModel = _bondTreeModel.lock())
  {
    bondListModel->deleteSelection(_structure, _bondSelection);
  }

  if(std::shared_ptr<AtomTreeViewModel> atomTreeModel = _atomTreeModel.lock())
  {
    atomTreeModel->deleteSelection(_structure, _atoms);
  }

  if(_main_window)
  {
    _main_window->reloadDetailViews();
  }
}

void AtomTreeViewDeleteSelectionCommand::undo()
{
  qDebug() << "Undoing";

  if(std::shared_ptr<AtomTreeViewModel> atomTreeModel = _atomTreeModel.lock())
  {
    atomTreeModel->insertSelection(_structure, _atoms, _indexPaths);
  }

  if(std::shared_ptr<BondListViewModel> bondListModel = _bondTreeModel.lock())
  {
    bondListModel->insertSelection(_structure, _bonds, _bondSelection);
  }

  if(_main_window)
  {
    _main_window->reloadDetailViews();
  }
}
