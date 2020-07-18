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
#include <iraspatreeview.h>
#include <QModelIndex>
#include "projecttreeviewmodel.h"
#include <optional>
#include "logreporting.h"
#include <iraspakit.h>
#include "iraspamainwindowconsumerprotocol.h"

class ProjectTreeView: public iRASPATreeView, public MainWindowConsumer, public Reloadable, public LogReportingConsumer
{
  Q_OBJECT
public:
  ProjectTreeView(QWidget* parent = nullptr);
  QSize sizeHint() const override;

  void setLogReportingWidget(LogReporting *logReporting);

  bool hasSelection() const override final;
  void paintEvent(QPaintEvent *event) override final;
  void setMainWindow(MainWindow* mainWindow) final override {_mainWindow = mainWindow;}
  void setController(std::shared_ptr<ProjectTreeController> treeController);
  void reloadSelection() override final;
  void reloadData() override final;

	bool insertRows(int position, int rows, const QModelIndex &parent, std::shared_ptr<ProjectTreeNode> item);

  void addGroupProject(QModelIndex index);
  void startDrag(Qt::DropActions supportedActions) override final;
  void dragMoveEvent(QDragMoveEvent *event) override final;
  QUndoStack& undoManager() {return _undoStack;}
  void focusInEvent( QFocusEvent* ) override final;
  void focusOutEvent( QFocusEvent* ) override final;
private:
  QRect _dropIndicatorRect;
  MainWindow* _mainWindow;
  LogReporting *_logReporting;
  std::shared_ptr<ProjectTreeController> _projectTreeController;
  std::shared_ptr<ProjectTreeViewModel> _model;
  QUndoStack _undoStack;
public slots:
  void copy();
  void paste();
  void cut();
  void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) override final;
  void ShowContextMenu(const QPoint &pos);
  void keyPressEvent(QKeyEvent * event) override final;
signals:
  void setSelectedRenderFrames(std::vector<std::vector<std::shared_ptr<iRASPAStructure>>> structures);
};

