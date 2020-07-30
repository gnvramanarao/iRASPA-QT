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

#include "atomtreeviewdropcopycommand.h"
#include <QDebug>
#include <algorithm>

AtomTreeViewDropCopyCommand::AtomTreeViewDropCopyCommand(AtomTreeViewModel *model, std::shared_ptr<Structure> structure, std::vector<std::tuple<std::shared_ptr<SKAtomTreeNode>, std::shared_ptr<SKAtomTreeNode>, int>> moves, QUndoCommand *undoParent):
  QUndoCommand(undoParent),
  _model(model),
  _structure(structure),
  _moves(moves),
  _reverseMoves({})
{
  setText(QString("Copy atoms"));
}

void AtomTreeViewDropCopyCommand::redo()
{
  _reverseMoves.clear();

  _model->layoutAboutToBeChanged();
  for(const auto [atom, parentNode, insertionRow] : _moves)
  {
    _reverseMoves.insert(_reverseMoves.begin(), std::make_tuple(atom, atom->parent(), atom->row()));

    whileBlocking(_model)->insertRow(insertionRow, parentNode,atom);
  }
  _structure->atomsTreeController()->setTags();
  _model->layoutChanged();
}

void AtomTreeViewDropCopyCommand::undo()
{
  _model->layoutAboutToBeChanged();
  for(const auto [atom, parentNode, insertionRow] : _reverseMoves)
  {
    whileBlocking(_model)->removeRow(atom->row(), atom->parent());
  }
  _structure->atomsTreeController()->setTags();
  _model->layoutChanged();
}
