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

#include "bondlistview.h"
#include "bondlistviewsliderstyleditemdelegate.h"

BondListView::BondListView(QWidget* parent): QTreeView(parent ), _model(std::make_shared<BondListViewModel>())
{
  _structure = std::make_shared<ProjectStructure>();
  this->setModel(_model.get());

 // this->setItemDelegateForColumn(5, new BondListViewSliderStyledItemDelegate(this));

  this->viewport()->setMouseTracking(true);

  this->setAlternatingRowColors(true);
  //this->setSelectionMode(QAbstractItemView::MultiSelection);
  this->setSelectionBehavior (QAbstractItemView::SelectRows);
  this->setSelectionMode(QAbstractItemView::ExtendedSelection);
  //this->setStyleSheet("background-color:rgb(240, 240, 240);");

  this->setRootIsDecorated(false);
  this->setItemsExpandable(false);
  this->setIndentation(0);
}

void BondListView::reloadSelection()
{

}

void BondListView::reloadData()
{
  if(_model->bondSetController() != _controller)
  {
    if(_structure)
    {
      _model->setBondSetStructure(_structure->selectedStructure());
    }
  }
}



void BondListView::setRootNode(std::shared_ptr<ProjectStructure> structure)
{
  _structure = structure;
  if(_structure)
  {
    _controller = structure->getBondListModel();
  }
}


void BondListView::setBondListModel(const QModelIndex &current, const QModelIndex &previous)
{
  DisplayableProtocol *item = static_cast<DisplayableProtocol*>(current.internalPointer());

  if(iRASPAStructure* structure = dynamic_cast<iRASPAStructure*>(item))
  {
    _model->setBondSetStructure(structure->structure());
  }

}


QSize BondListView::sizeHint() const
{
  return QSize(500, 800);
}


