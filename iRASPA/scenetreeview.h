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
#include <optional>
#include <iraspakit.h>
#include "scenetreeviewmodel.h"
#include "iraspamainwindowconsumerprotocol.h"

class SceneTreeView: public QTreeView, public MainWindowConsumer, public ProjectConsumer, public Reloadable
{
  Q_OBJECT

public:
  SceneTreeView(QWidget* parent = nullptr);
  QSize sizeHint() const override final;
  SceneTreeViewModel* sceneTreeModel() {return _model.get();}
  void setMainWindow(MainWindow* mainWindow) final override {_mainWindow = mainWindow;}
  void setProject(std::shared_ptr<ProjectTreeNode> projectTreeNode) override final;
  void reloadSelection() override final;
  void reloadData() override final;

  void startDrag(Qt::DropActions supportedActions) override final;
  void dragMoveEvent(QDragMoveEvent* event) override final;

  void keyPressEvent(QKeyEvent *event) override final;
  void restoreSelection(SceneListSelection selection);

  std::shared_ptr<SceneTreeViewModel> sceneModel() {return _model;}
private:
  MainWindow* _mainWindow;
  std::shared_ptr<SceneTreeViewModel> _model;
  std::shared_ptr<ProjectTreeNode> _projectTreeNode;
  std::shared_ptr<SceneList> _sceneList;
  QString nameOfItem(const QModelIndex &current);
  QPixmap selectionToPixmap();
public slots:
  void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) override final;
  void newCrystal();
  void newMolecularCrystal();
  void newProteinCrystal();
  void newMolecule();
  void newProtein();
  void newCrystalEllipsoid();
  void newCrystalCylinder();
  void newCrystalPolygonalPrism();
  void newEllipsoid();
  void newCylinder();
  void newPolygonalPrism();
  void insertMovie(std::shared_ptr<iRASPAStructure> iraspaStructure);
  void deleteSelection();
signals:
  void updateRenderer();
  void setSelectedFrame(std::shared_ptr<iRASPAStructure> structure);
  void setSelectedMovie(std::shared_ptr<Movie> movie);
  void setFlattenedSelectedFrames(std::vector<std::shared_ptr<iRASPAStructure>> structures);
  void setSelectedRenderFrames(std::vector<std::vector<std::shared_ptr<iRASPAStructure>>> structures);
};
