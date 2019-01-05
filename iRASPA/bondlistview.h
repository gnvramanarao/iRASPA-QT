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
#include <QListView>
#include <QModelIndex>
#ifdef _WIN32
  #include <optional>
#else
  #include <experimental/optional>
#endif
#include <iraspakit.h>
#include "bondlistviewmodel.h"
#include "iraspamainwindowconsumerprotocol.h"

class BondListView: public QTreeView, public MainWindowConsumer, public Reloadable
{
  Q_OBJECT

public:
  BondListView(QWidget* parent = nullptr);
  QSize sizeHint() const override final;
  void setMainWindow(MainWindow* mainWindow) override final {_mainWindow = mainWindow;}
  void setRootNode(std::shared_ptr<ProjectStructure> structure);
  BondListViewModel* bondListModel() {return _model.get();}
  void reloadSelection() override final;
  void reloadData() override final;
private:
  MainWindow* _mainWindow;
  std::shared_ptr<BondListViewModel> _model;
  std::shared_ptr<ProjectStructure> _structure;
  std::shared_ptr<SKBondSetController> _controller;
public slots:
  void setBondListModel(const QModelIndex &current, const QModelIndex &previous);
};
