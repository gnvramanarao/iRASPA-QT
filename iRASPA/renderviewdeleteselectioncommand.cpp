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

#include "renderviewdeleteselectioncommand.h"
#include <QDebug>
#include <algorithm>

RenderViewDeleteSelectionCommand::RenderViewDeleteSelectionCommand(std::shared_ptr<AtomTreeViewModel> atomTreeModel, std::shared_ptr<BondListViewModel> bondTreeModel,
                                               MainWindow *main_window, std::shared_ptr<Structure> structure,
                                               AtomSelection atomSelection, BondSelection bondSelection, QUndoCommand *parent):
  _atomTreeModel(atomTreeModel),
  _bondTreeModel(bondTreeModel),
  _main_window(main_window),
  _structure(structure),
  _atomSelection(atomSelection),
  _bondSelection(bondSelection),
  _reverseAtomSelection(),
  _reverseBondSelection()
{
  Q_UNUSED(parent);

  setText(QString("Delete selection"));
  std::reverse_copy(std::begin(atomSelection), std::end(atomSelection), std::back_inserter(_reverseAtomSelection));
  std::reverse_copy(std::begin(bondSelection), std::end(bondSelection), std::back_inserter(_reverseBondSelection));
}

void RenderViewDeleteSelectionCommand::redo()
{

  if(std::shared_ptr<BondListViewModel> bondListModel = _bondTreeModel.lock())
  {
    bondListModel->deleteSelection(_structure, _reverseBondSelection);
  }

  if(std::shared_ptr<AtomTreeViewModel> atomTreeModel = _atomTreeModel.lock())
  {
    atomTreeModel->deleteSelection(_structure, _reverseAtomSelection);
  }

  if(_main_window)
  {
    _main_window->reloadDetailViews();
  }
}

void RenderViewDeleteSelectionCommand::undo()
{
  qDebug() << "Undoing";

  if(std::shared_ptr<AtomTreeViewModel> atomTreeModel = _atomTreeModel.lock())
  {
    atomTreeModel->insertSelection(_structure, _atomSelection);
  }

  if(std::shared_ptr<BondListViewModel> bondListModel = _bondTreeModel.lock())
  {
    bondListModel->insertSelection(_structure, _bondSelection);
  }

  if(_main_window)
  {
    _main_window->reloadDetailViews();
  }
}
