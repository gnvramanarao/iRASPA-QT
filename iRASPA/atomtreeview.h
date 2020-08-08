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
#include "scenetreeviewmodel.h"
#include "iraspamainwindowconsumerprotocol.h"
#include "atomtreeviewpushbuttonstyleditemdelegate.h"

class QAbstractItemViewPrivate;
class AtomTreeView: public QTreeView, public MainWindowConsumer, public ProjectConsumer, public Reloadable
{
  Q_OBJECT

public:
  AtomTreeView(QWidget* parent = nullptr);
  QSize sizeHint() const override final;

  void setMainWindow(MainWindow* mainWindow) override final;
  void setBondModel(std::shared_ptr<BondListViewModel> bondModel);
  void setSceneModel(std::shared_ptr<SceneTreeViewModel> sceneModel);

  // update the atomTreeView when a new project is selected in the ProjectTreeView
  void setProject(std::shared_ptr<ProjectTreeNode> projectTreeNode) override final;

  // update the atomTreeView when (implicitely or explicitly) a new frame is selected in the Scene/Movie-ListView or frameListView
  void setSelectedFrame(std::shared_ptr<iRASPAStructure> structure);

  void resetData();
  void reloadData() override final;
  void reloadSelection() override final;

  virtual void dropEvent(QDropEvent * event) final override;
  void paintEvent(QPaintEvent *event) override final;
  void startDrag(Qt::DropActions supportedActions) override final;
  void dragMoveEvent(QDragMoveEvent *event) override final;

  std::shared_ptr<AtomTreeViewModel> atomModel() {return _atomModel;}
  void deleteSelection();
private:
  QRect _dropIndicatorRect;
  MainWindow* _mainWindow;
  std::shared_ptr<AtomTreeViewModel> _atomModel;
  std::shared_ptr<BondListViewModel> _bondModel;
  std::shared_ptr<SceneTreeViewModel> _sceneModel;
  std::shared_ptr<iRASPAStructure> _iraspaStructure;
  std::shared_ptr<ProjectTreeNode> _projectTreeNode;
  std::shared_ptr<iRASPAProject> _iRASPAProject;
  void paintDropIndicator(QPainter& painter);
  QAbstractItemView::DropIndicatorPosition position(QPoint pos, QRect rect, QModelIndex index);
  void addAtomGroup(QModelIndex index);
  void flattenHierachy();
  void makeSuperCell();
  void copyToNewMovie();
  void moveToNewMovie();
  void invertSelection();
  void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) override final;
  AtomTreeViewPushButtonStyledItemDelegate *pushButtonDelegate;
  SKAtomTreeNode* getItem(const QModelIndex &index) const;
  void exportToPDB() const;
  void exportToMMCIF() const;
  void exportToCIF() const;
  void exportToXYZ() const;
  void exportToPOSCAR() const;

  void scrollToFirstSelected();
  void scrollToLastSelected();
  void visibilityMatchSelection();
  void visibilityInvert();
protected:
  void keyPressEvent(QKeyEvent *event) override final;
  void mousePressEvent(QMouseEvent *e) override final;
public slots:
  void ShowContextMenu(const QPoint &pos);
  void addAtom();
  void addAtomGroup();
signals:
  void rendererReloadData();
  void invalidateCachedAmbientOcclusionTexture(std::vector<std::vector<std::shared_ptr<iRASPAStructure>>> structures);
};
