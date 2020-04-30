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
#include "bondlistpushbuttonstyleditemdelegate.h"

BondListViewModel::BondListViewModel(): _bondSetController(std::make_shared<SKBondSetController>(std::make_shared<SKAtomTreeController>()))
{
  //this->setItemDelegateForColumn(2, new BondListPushButtonStyledItemDelegate() );
}

QModelIndex BondListViewModel::index(int row, int column, const QModelIndex &parent) const
{
  if (!hasIndex(row, column, parent))
    return QModelIndex();

  const std::vector<std::shared_ptr<SKAsymmetricBond>> bonds = _bondSetController->arrangedObjects();
  if(bonds.empty())
    return QModelIndex();

  return createIndex(row, column, bonds[row].get());
}

QModelIndex BondListViewModel::parent(const QModelIndex &index) const
{
  return QModelIndex();
}

void BondListViewModel::setBondSetController(std::shared_ptr<SKBondSetController> controller)
{
  if(_bondSetController != controller)
  {
    beginResetModel();
    _bondSetController = controller;
    endResetModel();
  }
}

void BondListViewModel::setStructure(std::shared_ptr<Structure> structure)
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
    return 8;
}

int BondListViewModel::rowCount(const QModelIndex &parent) const
{
   return _bondSetController->arrangedObjects().size();
}

QVariant BondListViewModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  if (role == Qt::CheckStateRole && index.column() == 0)
  {
    if(SKAsymmetricBond *bond = static_cast<SKAsymmetricBond*>(index.internalPointer()))
    {
      return bond->isVisible() ? Qt::Checked : Qt::Unchecked;
    }
  }

  if (role != Qt::DisplayRole)
    return QVariant();

 //DisplayableProtocol *item = static_cast<DisplayableProtocol*>(index.internalPointer());

  if(SKAsymmetricBond *asymmetricBond = static_cast<SKAsymmetricBond*>(index.internalPointer()))
  {
    switch(index.column())
    {
    case 0:
      return QVariant();
    case 1:
      return QString::number(index.row());
    case 2:
      return QVariant();
    case 3:
    {
      int atomicNumber = asymmetricBond->atom1()->elementIdentifier();
      SKElement element = PredefinedElements::predefinedElements[atomicNumber];
      return QString(element._chemicalSymbol);
    }
    case 4:
    {
      int atomicNumber = asymmetricBond->atom2()->elementIdentifier();
      SKElement element = PredefinedElements::predefinedElements[atomicNumber];
      return QString(element._chemicalSymbol);
    }
    case 5:
      return QVariant();
    case 6:
      if(asymmetricBond->copies().size()>0)
      {
        std::shared_ptr<SKBond> bond = asymmetricBond->copies().front();
        double bondLength = _structure->bondLength(bond);
        return QString::asprintf("%5.4f", bondLength);
      }
      return  0.0;
    case 7:
      return QVariant();
    }
  }

  return QVariant();
}

Qt::ItemFlags BondListViewModel::flags(const QModelIndex &index) const
{
  if (!index.isValid()) return Qt::NoItemFlags;

  Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

  if ( index.column() == 0)
  {
    flags |= Qt::ItemIsUserCheckable;
    flags |= Qt::ItemIsTristate;
    flags |= Qt::ItemIsEditable;
  }


  if ( index.column() != 2)
  {
    flags |= Qt::ItemIsEditable;
  }

  return flags;
}

QVariant BondListViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role != Qt::DisplayRole)
    return QVariant();

  switch(section)
  {
    case 0:
      return QString("vis");
    case 1:
      return QString("id");
    case 2:
      return QString("atom ids");
    case 3:
      return QString("el(A)");
    case 4:
      return QString("el(B)");
    case 5:
      return QString("type");
    case 6:
      return QString("length");
    case 7:
      return QString("length");
  }

  return QVariant();
}

bool BondListViewModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  if (!index.isValid() /*|| role != Qt::EditRole*/)
    return false;

  if(SKAsymmetricBond *bond = static_cast<SKAsymmetricBond*>(index.internalPointer()))
  {
    switch(index.column())
    {
    case 0:
      if (role == Qt::CheckStateRole)
      {
        if ((Qt::CheckState)value.toInt() == Qt::Checked)
        {
          qDebug() << "checked visibility";
          bond->setIsVisible(true);
          emit rendererReloadData();
          return true;
        }
        else
        {
          bond->setIsVisible(false);
          emit rendererReloadData();
          return true;
        }
        break;
      case 2:
          if ((Qt::CheckState)value.toInt() == Qt::Checked)
          {
            qDebug() << "checked";
          }
        case 5:
          if (role == Qt::EditRole)
          {
            bond->setBondType(SKAsymmetricBond::SKBondType(value.toInt()));
            qDebug() << " Edit: " << value.toInt();
            emit rendererReloadData();
            return true;
          }
      default:
        break;
      }
    }
  }

  return false;
}

QModelIndexList BondListViewModel::selectedIndexes()
{
  QModelIndexList list = QModelIndexList();
  for(int localRow : _bondSetController->selectionIndexSet())
  {
    std::shared_ptr<SKAsymmetricBond> node = _bondSetController->arrangedObjects()[localRow];
    QModelIndex index = createIndex(localRow,0,node.get());
    list.push_back(index);
  }
  return list;
}

void BondListViewModel::deleteSelection(std::shared_ptr<Structure> structure, std::set<int> indexSet)
{
  this->beginResetModel();

  structure->bondSetController()->deleteBonds(indexSet);
  structure->bondSetController()->selectionIndexSet().clear();
  structure->bondSetController()->setTags();

  this->endResetModel();
}

void BondListViewModel::insertSelection(std::shared_ptr<Structure> structure, std::vector<std::shared_ptr<SKAsymmetricBond>> bonds, std::set<int> indexSet)
{
  this->beginResetModel();

  structure->bondSetController()->insertBonds(bonds, indexSet);
  structure->bondSetController()->selectionIndexSet().insert(indexSet.begin(), indexSet.end());
  structure->bondSetController()->setTags();

  this->endResetModel();
}
