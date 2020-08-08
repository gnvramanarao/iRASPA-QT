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

#include "atomtreeviewinvertselectioncommand.h"
#include <QDebug>
#include <algorithm>
#include <set>

AtomTreeViewInvertSelectionCommand::AtomTreeViewInvertSelectionCommand(MainWindow *main_window, std::shared_ptr<Structure> structure,
                                     AtomSelection atomSelection, BondSelection bondSelection, QUndoCommand *parent):
  _main_window(main_window),
  _structure(structure),
  _atomSelection(atomSelection),
  _bondSelection(bondSelection)
{
  Q_UNUSED(parent);

  setText(QString("Invert atom/bond selection"));

  std::set<std::shared_ptr<SKAtomTreeNode>> allAtomTreeNodes{};
  std::vector<std::shared_ptr<SKAtomTreeNode>> flattenedNodes = structure->atomsTreeController()->flattenedLeafNodes();
  std::copy(flattenedNodes.begin(),flattenedNodes.end(), std::inserter(allAtomTreeNodes, allAtomTreeNodes.begin()));
  std::set<std::shared_ptr<SKAtomTreeNode>> selectedAtoms = structure->atomsTreeController()->selectedTreeNodes();

  std::set<std::shared_ptr<SKAtomTreeNode>> invertedTreeNodeAtoms{};
  std::set_difference(allAtomTreeNodes.begin(), allAtomTreeNodes.end(), selectedAtoms.begin(), selectedAtoms.end(),
      std::inserter(invertedTreeNodeAtoms, invertedTreeNodeAtoms.end()));

  std::set<std::shared_ptr<SKAsymmetricAtom>> invertedAsymmetricAtoms{};
  std::transform(invertedTreeNodeAtoms.begin(),invertedTreeNodeAtoms.end(), std::inserter(invertedAsymmetricAtoms, invertedAsymmetricAtoms.begin()),
                 [](std::shared_ptr<SKAtomTreeNode> treeNode) -> std::shared_ptr<SKAsymmetricAtom>
                 {return treeNode->representedObject();});

  _invertedAtomSelection.clear();
  std::transform(invertedTreeNodeAtoms.begin(),invertedTreeNodeAtoms.end(), std::back_inserter(_invertedAtomSelection),
                 [](std::shared_ptr<SKAtomTreeNode> treeNode) -> std::pair< std::shared_ptr<SKAtomTreeNode>, IndexPath>
                 {return std::make_pair(treeNode, treeNode->indexPath());});

  std::set<int> selectedBonds = structure->bondSetController()->selectionIndexSet();
  std::set<int> allBondIndices{};
  std::generate_n( inserter( allBondIndices, allBondIndices.begin() ), selectedBonds.size(), [ i=0 ]() mutable { return i++; });

  std::set<int> invertedBonds{};
  std::set_difference(allBondIndices.begin(), allBondIndices.end(), selectedBonds.begin(), selectedBonds.end(),
      std::inserter(invertedBonds, invertedBonds.end()));


  const std::vector<std::shared_ptr<SKAsymmetricBond>> asymmetricBonds = structure->bondSetController()->arrangedObjects();
  int index=0;
  for(std::shared_ptr<SKAsymmetricBond> asymmetricBond: asymmetricBonds)
  {
    const bool containsAtom1 = invertedAsymmetricAtoms.find(asymmetricBond->atom1()) != invertedAsymmetricAtoms.end();
    const bool containsAtom2 = invertedAsymmetricAtoms.find(asymmetricBond->atom2()) != invertedAsymmetricAtoms.end();
    if(containsAtom1 || containsAtom2)
    {
       invertedBonds.insert(index);
    }
    index++;
  }

  _invertedBondSelection.clear();
  std::transform(invertedBonds.begin(),invertedBonds.end(), std::back_inserter(_invertedBondSelection),
                 [asymmetricBonds](int index) -> std::pair<std::shared_ptr<SKAsymmetricBond>, int>
                 {return std::make_pair(asymmetricBonds[index], index);});
}

void AtomTreeViewInvertSelectionCommand::redo()
{
  if(std::shared_ptr<Structure> structure = _structure)
  {
    structure->atomsTreeController()->setSelection(_invertedAtomSelection);
    structure->bondSetController()->setSelection(_invertedBondSelection);
  }

  if(_main_window)
  {
    _main_window->reloadSelectionDetailViews();
  }
}

void AtomTreeViewInvertSelectionCommand::undo()
{
  if(std::shared_ptr<Structure> structure = _structure)
  {
    structure->atomsTreeController()->setSelection(_atomSelection);
    structure->bondSetController()->setSelection(_bondSelection);
  }

  if(_main_window)
  {
    _main_window->reloadSelectionDetailViews();
  }
}

