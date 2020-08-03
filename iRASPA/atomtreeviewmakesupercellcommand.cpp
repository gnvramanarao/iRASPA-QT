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

#include "atomtreeviewmakesupercellcommand.h"
#include <QDebug>
#include <algorithm>

// Note: The iRASPAStructure is not modified, but the structure it contains is replaced.

AtomTreeViewMakeSuperCellCommand::AtomTreeViewMakeSuperCellCommand(MainWindow *mainWindow, AtomTreeViewModel *model, std::shared_ptr<iRASPAStructure> iraspa_structure,
                                     QUndoCommand *undoParent):
  QUndoCommand(undoParent),
  _mainWindow(mainWindow),
  _model(model),
  _iraspa_structure(iraspa_structure),
  _structure(iraspa_structure->structure())
{
  setText(QString("Make super cell"));
}

void AtomTreeViewMakeSuperCellCommand::redo()
{
  qDebug() << "AtomTreeViewMakeSuperCellCommand::redo()";

  std::shared_ptr<Structure> structure = _iraspa_structure->structure()->superCell();

  _iraspa_structure->setStructure(structure);

  _mainWindow->resetData();
}

void AtomTreeViewMakeSuperCellCommand::undo()
{
  _iraspa_structure->setStructure(_structure);

  _mainWindow->resetData();
}

