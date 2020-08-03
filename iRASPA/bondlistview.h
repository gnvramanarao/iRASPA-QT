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
#include <QTreeView>
#include <QModelIndex>
#include <QAbstractItemView>
#include <optional>
#include <iraspakit.h>
#include "bondlistviewmodel.h"
#include "atomtreeviewmodel.h"
#include "iraspamainwindowconsumerprotocol.h"
#include "bondlistpushbuttonstyleditemdelegate.h"
#include "bondlistviewcomboboxstyleditemdelegate.h"
#include "bondlistviewsliderstyleditemdelegate.h"

class BondListView: public QTreeView, public MainWindowConsumer, public ProjectConsumer, public Reloadable
{
  Q_OBJECT

public:
  BondListView(QWidget* parent = nullptr);
  QSize sizeHint() const override final;
  void setMainWindow(MainWindow* mainWindow) override final {_mainWindow = mainWindow;}

  // update the bondListView when a new project is selected in the ProjectTreeView
  void setProject(std::shared_ptr<ProjectTreeNode> projectTreeNode) override final;

  // update the bondListView when (implicitely or explicitly) a new frame is selected in the Scene/Movie-ListView or frameListView
  void setSelectedFrame(std::shared_ptr<iRASPAStructure> iraspastructure);

  void resetData();
  void reloadSelection() override final;
  void reloadData() override final;

  std::shared_ptr<BondListViewModel> bondModel() {return _bondModel;}
private:
  MainWindow* _mainWindow;
  std::shared_ptr<BondListViewModel> _bondModel;
  std::weak_ptr<iRASPAStructure> _iraspaStructure;
  std::weak_ptr<ProjectTreeNode> _projectTreeNode;
  std::shared_ptr<iRASPAProject> _iRASPAProject;

  BondListPushButtonStyledItemDelegate *pushButtonDelegate;
  BondListViewComboBoxStyledItemDelegate *comboBoxDelegate;
  BondListViewSliderStyledItemDelegate *sliderDelegate;
  SKAsymmetricBond* getItem(const QModelIndex &index) const;
  void setSelectedBonds(const QItemSelection &selected, const QItemSelection &deselected);
  void deleteSelection();
protected:
  void keyPressEvent(QKeyEvent *event) override final;
signals:
  void rendererReloadData();
};
