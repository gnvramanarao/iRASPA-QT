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

#include <QObject>
#include <QMainWindow>
#include <QAbstractItemModel>
#include <QTreeView>
#include <QModelIndex>
#include <optional>
#include <iraspakit.h>

class BondListViewModel: public QAbstractItemModel
{
  Q_OBJECT

public:
  BondListViewModel();

  void resetData();
  void setFrame(std::shared_ptr<iRASPAStructure> frame);
  std::shared_ptr<iRASPAStructure> frame() {return _iraspaStructure;}

  //QT
  QModelIndex index(int row, int column, const QModelIndex &parent) const override final;
  QModelIndex parent(const QModelIndex &index) const override final;
  int rowCount(const QModelIndex &parent = QModelIndex()) const override final;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override final;
  QVariant data(const QModelIndex &index, int role) const override final;
  Qt::ItemFlags flags(const QModelIndex &index) const override final;

  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override final;
  bool setData(const QModelIndex &index, const QVariant &value, int role) override final;

  QModelIndexList selectedIndexes();

  void deleteSelection(std::shared_ptr<Structure> structure, std::set<int> indexSet);
  void deleteSelection(std::shared_ptr<Structure> structure, BondSelection selection);
  void insertSelection(std::shared_ptr<Structure> structure, std::vector<std::shared_ptr<SKAsymmetricBond>> bonds, std::set<int> indexSet);
  void insertSelection(std::shared_ptr<Structure> structure, BondSelection selection);
private:
  std::shared_ptr<iRASPAStructure> _iraspaStructure;
signals:
    void reloadRenderDataRenderer();
    void rendererReloadData();
};
