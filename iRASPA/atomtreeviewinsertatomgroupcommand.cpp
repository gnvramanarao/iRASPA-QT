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

#include "atomtreeviewinsertatomgroupcommand.h"
#include <QDebug>
#include <algorithm>

AtomTreeViewInsertAtomGroupCommand::AtomTreeViewInsertAtomGroupCommand(MainWindow *main_window, AtomTreeView *atomTreeView, std::shared_ptr<Structure> structure, std::shared_ptr<SKAtomTreeNode> parentTreeNode, int row, QUndoCommand *parent):
  _main_window(main_window),
  _atomTreeView(atomTreeView),
  _structure(structure),
  _parentTreeNode(parentTreeNode),
  _row(row)
{
  Q_UNUSED(parent);

  setText(QString("Insert group atom"));

  std::shared_ptr<SKAsymmetricAtom> atom = std::make_shared<SKAsymmetricAtom>("N", 8);
  _newAtomtreeNode = std::make_shared<SKAtomTreeNode>(atom);
  _newAtomtreeNode->setGroupItem(true);
  _newAtomtreeNode->setDisplayName("New atom group");
}

void AtomTreeViewInsertAtomGroupCommand::redo()
{
  if(_atomTreeView)
  {
    if(AtomTreeViewModel* pModel = qobject_cast<AtomTreeViewModel*>(_atomTreeView->model()))
    {
      pModel->insertRow(_row, _parentTreeNode, _newAtomtreeNode);
      QModelIndex index = pModel->indexForNode(_newAtomtreeNode.get());
      _atomTreeView->setFirstColumnSpanned(index.row(),index.parent(), true);
    }
    _structure->atomsTreeController()->setTags();
  }
}

void AtomTreeViewInsertAtomGroupCommand::undo()
{
  if(_atomTreeView)
  {
    if(AtomTreeViewModel* pModel = qobject_cast<AtomTreeViewModel*>(_atomTreeView->model()))
    {
      pModel->removeRow(_row, _parentTreeNode);
    }
    _structure->atomsTreeController()->setTags();
  }
}
