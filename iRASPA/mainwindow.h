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

namespace Ui {
class MainWindow;
}

struct ProjectConsumer;

class MainWindow : public QMainWindow, public ProjectConsumer
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  void propagateProject(std::shared_ptr<ProjectTreeNode> project, QObject *widget);
  void propagateMainWindow(MainWindow* mainWindow, QObject *widget);
  void propagateLogReporter(LogReporting* logReporting, QObject *widget);
  std::shared_ptr<ProjectTreeController> projectTreeController() {return _documentData.projectTreeController();}
  Ui::MainWindow* mainWindowUI() {return ui;}
  void setProject(std::shared_ptr<ProjectTreeNode> projectTreeNode) override final;
  SKColorSets &colorSets() {return _colorSets;}
  ForceFieldSets &forceFieldSets() {return _forceFieldSets;}
  ~MainWindow();
  void setUndoAction(QAction *newUndoAction);
  void setRedoAction(QAction *newRedoAction);
private:
  Ui::MainWindow *ui;
  DocumentData _documentData{};
  SKColorSets _colorSets{};
  ForceFieldSets _forceFieldSets{};
  int projectInsertionIndex();

	void readLibraryOfStructures();
  // QMenu *helpMenu;
   QAction *helpAction;
  // QAction *aboutAction;
   //HelpWidget* helpWindow;
   void acknowledgements();
   QDir directoryOf(const QString &subdir);
   void createMenus();
   QMenu *editMenu = nullptr;
   QAction *undoAction = nullptr;
   QAction *redoAction = nullptr;
public slots:
   void importFile();
   void openFile();
   void saveFile();
   void showAboutDialog();
   int slideRightPanel(void);
   int slideDownPanel(void);
   int slideLeftPanel(void);


};
