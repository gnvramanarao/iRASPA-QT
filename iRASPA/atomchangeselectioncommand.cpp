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

#include "atomchangeselectioncommand.h"
#include <QDebug>
#include <algorithm>

AtomChangeSelectionCommand::AtomChangeSelectionCommand(MainWindow *main_window, std::shared_ptr<Structure> structure,
                                     std::unordered_set<std::shared_ptr<SKAtomTreeNode> > atomSelection, std::unordered_set<std::shared_ptr<SKAtomTreeNode> > previousAtomSelection,
                                     std::set<int> bondSelection, std::set<int> previousBondSelection, QUndoCommand *parent):
  _main_window(main_window),
  _structure(structure),
  _atomSelection(atomSelection),
  _previousAtomSelection(previousAtomSelection),
  _bondSelection(bondSelection),
  _previousBondSelection(previousBondSelection)
{
  Q_UNUSED(parent);

  setText(QString("Change atom/bond selection"));
}

void AtomChangeSelectionCommand::redo()
{
  qDebug() << "Redoing AtomTreeViewChangeSelectionCommand";

  if(std::shared_ptr<Structure> structure = _structure)
  {
    structure->atomsTreeController()->setSelectedTreeNodes(_atomSelection);
    structure->bondSetController()->setSelectedObjects(_bondSelection);
  }

  if(_main_window)
  {
    _main_window->reloadSelectionDetailViews();
  }
}

void AtomChangeSelectionCommand::undo()
{
  qDebug() << "Undoing AtomTreeViewChangeSelectionCommand";

  if(std::shared_ptr<Structure> structure = _structure)
  {
    structure->atomsTreeController()->setSelectedTreeNodes(_previousAtomSelection);
    structure->bondSetController()->setSelectedObjects(_previousBondSelection);
  }

  if(_main_window)
  {
    _main_window->reloadSelectionDetailViews();
  }
}

