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

#include "bondlistviewmodel.h"

BondListViewModel::BondListViewModel(): _bondSetController(std::make_shared<SKBondSetController>(std::make_shared<SKAtomTreeController>()))
{
}

QModelIndex BondListViewModel::index(int row, int column, const QModelIndex &parent) const
{
  const std::vector<std::shared_ptr<SKBond>> bonds = _bondSetController->rearrangedObjects();
  if(bonds.empty())
    return QModelIndex();

  //if (!parent.isValid())
  //{
    return createIndex(row, column, bonds[row].get());
  //}

   return QModelIndex();
}

QModelIndex BondListViewModel::parent(const QModelIndex &index) const
{
  return QModelIndex();
}

/*
void BondListViewModel::setBondSetController(std::shared_ptr<SKBondSetController> controller)
{
  if(_bondSetController != controller)
  {
    beginResetModel();
    _bondSetController = controller;
    endResetModel();
  }
}*/

void BondListViewModel::setBondSetStructure(std::shared_ptr<Structure> structure)
{
  _structure = structure;
  if(_structure)
  {
    std::shared_ptr<SKBondSetController>  controller = structure->bondSetController();
    if(_bondSetController != controller)
    {
      beginResetModel();
      _bondSetController = controller;
      endResetModel();
    }
  }
}


int BondListViewModel::columnCount(const QModelIndex &parent) const
{
    return 5;
}

int BondListViewModel::rowCount(const QModelIndex &parent) const
{
   return _bondSetController->rearrangedObjects().size();
}

QVariant BondListViewModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();


 if (role != Qt::DisplayRole)
   return QVariant();

 //DisplayableProtocol *item = static_cast<DisplayableProtocol*>(index.internalPointer());

  if(SKBond *bond = static_cast<SKBond*>(index.internalPointer()))
  {
    switch(index.column())
    {
    case 0:
      return QString::number(index.row());
    case 1:
      return QString::number(bond->atom1()->parent()->tag()) + QString("-") + QString::number(bond->atom2()->parent()->tag());
    case 2:
      return QString(bond->atom1()->parent()->displayName());
    case 3:
      return QString(bond->atom2()->parent()->displayName());
    case 4:
      return _structure->bondLenght(bond->shared_from_this());
    }
  }

  return QVariant();
}

Qt::ItemFlags BondListViewModel::flags(const QModelIndex &index) const
{
  if (!index.isValid()) return Qt::NoItemFlags;

  Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

  return flags;
}

QVariant BondListViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role != Qt::DisplayRole)
    return QVariant();

  switch(section)
  {
    case 0:
      return QString("id");
    case 1:
      return QString("atom ids");
    case 2:
      return QString("type");
    case 3:
      return QString("type");
    case 4:
      return QString("length");
  }

  return QVariant();
}


