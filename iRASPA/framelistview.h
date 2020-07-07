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
#include <optional>
#include <iraspakit.h>
#include "iraspamainwindowconsumerprotocol.h"
#include "framelistviewmodel.h"

// model: std::shared_ptr<Movie>

class FrameListView: public QListView, public MainWindowConsumer, public ProjectConsumer, public Reloadable
{
  Q_OBJECT

public:
  FrameListView(QWidget* parent = 0);
  QSize sizeHint() const override final;
  void setMainWindow(MainWindow* mainWindow) final override {_mainWindow = mainWindow;}
  void reloadSelection() override final;
  void reloadData() override final;
  void setProject(std::shared_ptr<ProjectTreeNode> projectTreeNode) override final;
  void setSelectedMovie(std::shared_ptr<Movie> movie);
private:
  MainWindow* _mainWindow;
  std::shared_ptr<ProjectTreeNode> _projectTreeNode;
  std::shared_ptr<FrameListViewModel> _model;
  std::shared_ptr<Movie> _movie;
  std::shared_ptr<SceneList> _sceneList;
  void keyPressEvent(QKeyEvent *event) override final;
private slots:
  void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) override final;
signals:
  void updateRenderer();
  void setSelectedFrame(std::shared_ptr<iRASPAStructure> structure);
  void setSelectedRenderFrames(std::vector<std::vector<std::shared_ptr<iRASPAStructure>>> structures);
  void setFlattenedSelectedFrames(std::vector<std::shared_ptr<iRASPAStructure>> structures);
};
