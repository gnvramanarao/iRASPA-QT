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

#include <QMainWindow>
#include <QObject>
#include <QDockWidget>
#include <QMenu>
#include <QAction>
#include <foundationkit.h>
#include <iraspakit.h>
#include <symmetrykit.h>
#include "helpwidget.h"
//#include "atomtreeviewmodel.h"
//#include "bondlistviewmodel.h"

namespace Ui {
class MainWindow;
}

struct ProjectConsumer;

class MainWindow : public QMainWindow  //, public ProjectConsumer
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  void propagateProject(std::shared_ptr<ProjectTreeNode> project, QObject *widget);
  void propagateMainWindow(MainWindow* mainWindow, QObject *widget);
  void propagateLogReporter(LogReporting* logReporting, QObject *widget);
  std::shared_ptr<ProjectTreeController> projectTreeController() {return _documentData.projectTreeController();}
  Ui::MainWindow* mainWindowUI() {return ui;}
  SKColorSets &colorSets() {return _colorSets;}
  ForceFieldSets &forceFieldSets() {return _forceFieldSets;}
  ~MainWindow();
  void setUndoAction(QAction *newUndoAction);
  void setRedoAction(QAction *newRedoAction);
  void reloadDetailViews();
  void reloadAllViews();
  void resetData();
  void reloadSelectionDetailViews();
  void recheckRemovalButtons();
  void updateMenuToProjectTab();
  void updateMenuToSceneTab();
  void updateMenuToFrameTab();
private:
  Ui::MainWindow *ui;
  DocumentData _documentData{};
  //std::shared_ptr<AtomTreeViewModel> _atomModel;
  //std::shared_ptr<BondListViewModel> _bondModel;
  SKColorSets _colorSets{};
  ForceFieldSets _forceFieldSets{};
  int projectInsertionIndex();

  void readLibraryOfStructures();
  QAction *_helpAction;
  void acknowledgements();
  QDir directoryOf(const QString &subdir);
  void createMenus();

  QMenu *_newMenu{nullptr};
  QAction *_newWorkSpaceAction{nullptr};
  QAction *_newStructureProjectAction{nullptr};
  QAction *_newProjectGroupAction{nullptr};
  QAction *_newCrystalAction{nullptr};
  QAction *_newMolecularCrystalAction{nullptr};
  QAction *_newProteinCrystalAction{nullptr};
  QAction *_newMoleculeAction{nullptr};
  QAction *_newProteinAction{nullptr};
  QMenu *_newObjectsMenu{nullptr};
  QAction *_newCrystalEllipsoidAction{nullptr};
  QAction *_newCrystalCylinderAction{nullptr};
  QAction *_newCrystalPolygonalPrismAction{nullptr};
  QAction *_newEllipsoidAction{nullptr};
  QAction *_newCylinderAction{nullptr};
  QAction *_newPolygonalPrismAction{nullptr};


  QMenu *_editMenu{nullptr};
  QAction *_undoAction{nullptr};
  QAction *_redoAction{nullptr};

  QAction *_cutAction{nullptr};
  QAction *_copyAction{nullptr};
  QAction *_pasteAction{nullptr};
public slots:
  void importFile();
  void openFile();
  void saveFile();
  void showAboutDialog();
  void ShowContextAddStructureMenu(const QPoint &pos);
  void newWorkSpace();
  void newStructureProject();
  void newProjectGroup();
  int slideRightPanel(void);
  int slideDownPanel(void);
  int slideLeftPanel(void);
  void actionTriggered();
  void focusChanged(QWidget *old, QWidget *now);
  void updatePasteStatus();
  void focusWidgetSelectionChanged(const QItemSelection &selected, const QItemSelection &/*deselected*/);
};
