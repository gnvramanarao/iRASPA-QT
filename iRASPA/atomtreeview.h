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
#include "atomtreeviewmodel.h"
#include "bondlistviewmodel.h"
#include "iraspamainwindowconsumerprotocol.h"
#include "atomtreeviewpushbuttonstyleditemdelegate.h"

class QAbstractItemViewPrivate;
class AtomTreeView: public QTreeView, public MainWindowConsumer, public ProjectConsumer, public Reloadable
{
  Q_OBJECT

public:
  AtomTreeView(QWidget* parent = nullptr);
  QSize sizeHint() const override final;
  void setMainWindow(MainWindow* mainWindow) override final {_mainWindow = mainWindow;}
  void setProject(std::shared_ptr<ProjectTreeNode> projectTreeNode) override final;
  void setSelectedFrame(std::shared_ptr<iRASPAStructure> structure);

  void setRootNode(std::shared_ptr<ProjectStructure> _iraspaStructure);
  void setAtomController(std::shared_ptr<SKAtomTreeController> atomController);
  void setBondController(std::shared_ptr<SKBondSetController> bondController);
  AtomTreeViewModel* atomTreeModel() {return _atomModel.get();}
  void reloadData() override final;
  virtual void dropEvent(QDropEvent * event) final override;
  void paintEvent(QPaintEvent *event) override final;
  void startDrag(Qt::DropActions supportedActions) override final;
  void dragMoveEvent(QDragMoveEvent *event) override final;
  void setBondModel(std::shared_ptr<BondListViewModel> bondModel);
  std::shared_ptr<AtomTreeViewModel> atomModel() {return _atomModel;}
private:
  QRect _dropIndicatorRect;
  MainWindow* _mainWindow;
  std::shared_ptr<AtomTreeViewModel> _atomModel;
  std::shared_ptr<BondListViewModel> _bondModel;
  std::weak_ptr<iRASPAStructure> _iraspaStructure;
  std::weak_ptr<ProjectTreeNode> _projectTreeNode;
  std::shared_ptr<iRASPAProject> _iRASPAProject;
  void paintDropIndicator(QPainter& painter);
  QAbstractItemView::DropIndicatorPosition position(QPoint pos, QRect rect, QModelIndex index);
  void addAtomGroup(QModelIndex index);
  void flattenHierachy();
  void makeSuperCell();
  void setSelectedAtoms(const QItemSelection &selected, const QItemSelection &deselected);
  AtomTreeViewPushButtonStyledItemDelegate *pushButtonDelegate;
  SKAtomTreeNode* getItem(const QModelIndex &index) const;
  void exportToPDB() const;
  void exportToMMCIF() const;
  void exportToCIF() const;
  void exportToXYZ() const;
  void exportToPOSCAR() const;
  void deleteSelection();
protected:
  void keyPressEvent(QKeyEvent *event) override final;
public slots:
  void setAtomTreeModel(const QModelIndex &current, const QModelIndex &previous);
  void ShowContextMenu(const QPoint &pos);
  void reloadSelection() override final;
signals:
  void rendererReloadData();
  void invalidateCachedAmbientOcclusionTexture(std::vector<std::shared_ptr<RKRenderStructure>> structures);
};
