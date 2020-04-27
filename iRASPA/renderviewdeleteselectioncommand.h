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
#include "symmetrykit.h"
#include "mathkit.h"
#include "mainwindow.h"
#include "atomtreeviewmodel.h"
#include "bondlistviewmodel.h"
#include "renderstackedwidget.h"

class RenderViewDeleteSelectionCommand : public QUndoCommand
{
public:
  RenderViewDeleteSelectionCommand(std::shared_ptr<AtomTreeViewModel> atomTreeModel, std::shared_ptr<BondListViewModel> bondTreeModel,
                         MainWindow *main_window, std::shared_ptr<Structure> structure, std::vector<std::shared_ptr<SKAtomTreeNode>> atoms, std::vector<IndexPath> indexPaths,
                         std::vector<std::shared_ptr<SKAsymmetricBond>> bonds, std::set<int> bondSelection, QUndoCommand *parent = nullptr);
  void undo() override final;
  void redo() override final;
private:
  std::weak_ptr<AtomTreeViewModel> _atomTreeModel;
  std::weak_ptr<BondListViewModel> _bondTreeModel;
  MainWindow *_main_window;
  std::shared_ptr<Structure> _structure;
  std::vector<std::shared_ptr<SKAtomTreeNode>> _atoms;
  std::vector<IndexPath> _indexPaths;
  std::vector<std::shared_ptr<SKAsymmetricBond>> _bonds;
  std::set<int> _bondSelection;
};
