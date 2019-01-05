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
#include <QTabWidget>
#include <QSize>
#include <iraspakit.h>
#include "cameratreewidgetcontroller.h"
#include "elementlistwidgetcontroller.h"
#include "infotreewidgetcontroller.h"
#include "celltreewidgetcontroller.h"
#include "atomtreeview.h"
#include "bondlistview.h"

class DetailTabViewController: public QTabWidget
{
  Q_OBJECT

public:
  DetailTabViewController(QWidget* parent = nullptr );
  QSize sizeHint() const override final;
  void setCameraTreeView(CameraTreeWidgetController* view) {_cameraTreeView = view;}
  void setElementListView(ElementListWidgetController* view) {_elementListview = view;}
  void setInfoTreeView(InfoTreeWidgetController* view) {_infoTreeView = view;}
  void setCellTreeView(CellTreeWidgetController* view) {_cellTreeView = view;}
  void setAtomTreeView(AtomTreeView* view) {_atomTreeView = view;}
  void setBondListView(BondListView* view) {_bondListView = view;}
private:
  CameraTreeWidgetController* _cameraTreeView;
  ElementListWidgetController* _elementListview;
  InfoTreeWidgetController* _infoTreeView;
  CellTreeWidgetController* _cellTreeView;
  AtomTreeView* _atomTreeView;
  BondListView* _bondListView;
public slots:
  void reloadTab(int tab);
  void rendererWidgetResized();
  void setAtomTreeController(std::shared_ptr<SKAtomTreeController> treeController);
  void setBondSetController(std::shared_ptr<ProjectStructure> structure);
};
