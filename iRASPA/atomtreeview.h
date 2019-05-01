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
#ifdef _WIN32
  #include <optional>
#else
  #include <experimental/optional>
#endif
#include <iraspakit.h>
#include "atomtreeviewmodel.h"
#include "iraspamainwindowconsumerprotocol.h"

class QAbstractItemViewPrivate;
class AtomTreeView: public QTreeView, public MainWindowConsumer, public ProjectConsumer, public Reloadable
{
  Q_OBJECT

public:
  AtomTreeView(QWidget* parent = nullptr);
  QSize sizeHint() const override final;
  void setMainWindow(MainWindow* mainWindow) override final {_mainWindow = mainWindow;}
  void setProject(std::shared_ptr<ProjectTreeNode> projectTreeNode) override final;
  void setRootNode(std::shared_ptr<SKAtomTreeController> treeController);
  AtomTreeViewModel* atomTreeModel() {return _model.get();}
  void reloadData() override final;
  virtual void dropEvent(QDropEvent * event) final override;
  void paintEvent(QPaintEvent *event) override final;
  void startDrag(Qt::DropActions supportedActions) override final;
  void dragMoveEvent(QDragMoveEvent *event) override final;
private:
  QRect _dropIndicatorRect;
  MainWindow* _mainWindow;
  std::shared_ptr<AtomTreeViewModel> _model;
  std::shared_ptr<SKAtomTreeController> _atomTreeController;
  std::shared_ptr<ProjectStructure> _projectStructure;
  std::vector<std::shared_ptr<Structure>> _structures{};
  void paintDropIndicator(QPainter& painter);
  QAbstractItemView::DropIndicatorPosition position(QPoint pos, QRect rect, QModelIndex index);
  void addAtomGroup(QModelIndex index);
  void flattenHierachy();
  void makeSuperCell();
  void setSelectedAtoms(const QItemSelection &selected, const QItemSelection &deselected);
  SKAtomTreeNode* getItem(const QModelIndex &index) const;
public slots:
  void setAtomTreeModel(const QModelIndex &current, const QModelIndex &previous);
  void ShowContextMenu(const QPoint &pos);
  void reloadSelection() override final;
signals:
  void rendererReloadData();
  void invalidateCachedAmbientOcclusionTexture(std::vector<std::shared_ptr<RKRenderStructure>> structures);
};