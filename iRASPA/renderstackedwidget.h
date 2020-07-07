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
#include <QEvent>
#include <QPoint>
#include <QUrl>
#include <QStackedWidget>
#include <QRubberBand>
#include <iraspaproject.h>
#include <iraspamainwindowconsumerprotocol.h>
#include "atomtreeviewmodel.h"
#include "bondlistviewmodel.h"
#include "symmetrykit.h"

class RenderStackedWidget : public QStackedWidget, public ProjectConsumer, public MainWindowConsumer
{
  Q_OBJECT

public:
  RenderStackedWidget(QWidget* parent = nullptr );
  void setProject(std::shared_ptr<ProjectTreeNode> _projectTreeNode) override final;
  void setMainWindow(MainWindow *mainWindow) override final {_mainWindow = mainWindow;}
  void setAtomModel(std::shared_ptr<AtomTreeViewModel> atomModel);
  void setBondModel(std::shared_ptr<BondListViewModel> bondModel);
private:
  MainWindow *_mainWindow;
  std::shared_ptr<AtomTreeViewModel> _atomModel;
  std::shared_ptr<BondListViewModel> _bondModel;
  std::weak_ptr<ProjectTreeNode> _projectTreeNode;
  std::weak_ptr<ProjectStructure> _project;
  std::weak_ptr<iRASPAProject> _iRASPAProject;
  std::weak_ptr<RKCamera> _camera;
  std::vector<std::vector<std::shared_ptr<iRASPAStructure>>> _iraspa_structures;
  QRubberBand* _rubberBand;
  Tracking _tracking;
  QPoint _origin;
  void selectAsymetricAtomsInRectangle(QRect rect, bool extend);
  void exportToPDB() const;
  void exportToMMCIF() const;
  void exportToCIF() const;
  void exportToXYZ() const;
  void exportToPOSCAR() const;
  void deleteSelection();
protected:
  bool eventFilter(QObject *obj, QEvent *event) override final;
public slots:
  void setSelectedRenderFrames(std::vector<std::vector<std::shared_ptr<iRASPAStructure>>> structures);
  void redraw();
  void reloadRenderData();
  void reloadData();
  void reloadAmbientOcclusionData();
  void reloadSelectionData();
  void reloadBackgroundImage();
  void createPicture(QUrl fileURL, int width, int height);
  void resizeEvent(QResizeEvent *event) override final;
  void redrawWithLowQuality();
  void redrawWithMediumQuality();
  void redrawWithHighQuality();
  void redrawWithPictureQuality();
  void invalidateCachedAmbientOcclusionTexture(std::vector<std::shared_ptr<RKRenderStructure>> structures);
  void invalidateIsosurface(std::vector<std::shared_ptr<RKRenderStructure>> structures);
  void computeHeliumVoidFraction(std::vector<std::shared_ptr<RKRenderStructure>> structures);
  void computeNitrogenSurfaceArea(std::vector<std::shared_ptr<RKRenderStructure>> structures);
private slots:
  void ShowContextMenu(const QPoint &pos);
  void resetCameraDistance();
  void resetCameraToZ();
  void resetCameraToY();
  void resetCameraToX();
  void setCameraToOrthographic();
  void setCameraToPerspective();
  void showBoundingBox(bool checked);
signals:
  void updateCameraModelViewMatrix();
  void updateCameraEulerAngles();
  void updateCameraResetDirection();
  void updateCameraProjection();
  void rendererWidgetResized();
  void updateAtomSelection();
  void updateBondSelection();
};
