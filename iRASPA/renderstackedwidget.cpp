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

#include "renderstackedwidget.h"
#include <iostream>
#include <QMenu>
#include <QObject>
#include <QAction>
#include <QtDebug>
#include <iraspakit.h>
#include "moviemaker.h"
#include "fixedaspectratiolayoutitem.h"
#include "renderviewdeleteselectioncommand.h"
#include "atomchangeselectioncommand.h"

RenderStackedWidget::RenderStackedWidget(QWidget* parent ): QStackedWidget(parent ),
    _rubberBand(new QRubberBand(QRubberBand::Rectangle, this))
{
  this->setContextMenuPolicy(Qt::CustomContextMenu);

  setMouseTracking(true);

  QObject::connect(this, &QWidget::customContextMenuRequested, this, &RenderStackedWidget::ShowContextMenu);
}

void RenderStackedWidget::setProject(std::shared_ptr<ProjectTreeNode> projectTreeNode)
{
  this->_projectTreeNode = projectTreeNode;
  this->_project.reset();


  std::vector<std::vector<std::shared_ptr<RKRenderStructure>>> render_structures{};
  if (projectTreeNode)
  {
    if(std::shared_ptr<iRASPAProject> iraspaProject = projectTreeNode->representedObject())
    {
      if(std::shared_ptr<Project> project = iraspaProject->project())
      {
        if (std::shared_ptr<ProjectStructure> projectStructure = std::dynamic_pointer_cast<ProjectStructure>(project))
        {
          QSize size = frameSize();
          double ratio = double(size.width()) / double(size.height());
          projectStructure->setImageAspectRatio(ratio);
          emit rendererWidgetResized();

          this->_project = projectStructure;
          this->_camera = projectStructure->camera();
          _iraspa_structures = projectStructure->iRASPAstructures();
          render_structures = projectStructure->renderStructures();
        }
      }
    }
  }

  if (RKRenderViewController* widget = dynamic_cast<RKRenderViewController*>(currentWidget()))
  {
    widget->setRenderStructures(render_structures);
  }
}

void RenderStackedWidget::setLogReportingWidget(LogReporting *logReporting)
{
  _logReporter = logReporting;
}

void RenderStackedWidget::setSelectedRenderFrames(std::vector<std::vector<std::shared_ptr<iRASPAStructure>>> iraspa_structures)
{
  std::vector<std::vector<std::shared_ptr<RKRenderStructure>>> renderStructures{};
  for(std::vector<std::shared_ptr<iRASPAStructure>> v : iraspa_structures)
  {
    std::vector<std::shared_ptr<RKRenderStructure>> r{};
    std::transform(v.begin(),v.end(),std::back_inserter(r),
                   [](std::shared_ptr<iRASPAStructure> iraspastructure) -> std::shared_ptr<RKRenderStructure> {return iraspastructure->structure();});
    renderStructures.push_back(r);
  }

  _iraspa_structures = iraspa_structures;
  if (RKRenderViewController* widget = dynamic_cast<RKRenderViewController*>(currentWidget()))
  {
    widget->setRenderStructures(renderStructures);
  }

  reloadData();
}

void RenderStackedWidget::setAtomModel(std::shared_ptr<AtomTreeViewModel> atomModel)
{
  _atomModel = atomModel;
}

void RenderStackedWidget::setBondModel(std::shared_ptr<BondListViewModel> bondModel)
{
  _bondModel = bondModel;
}



void RenderStackedWidget::resizeEvent(QResizeEvent *event)
{
  QSize size = event->size();
  double ratio = double(size.width())/double(size.height());
  if(std::shared_ptr<ProjectStructure> project = this->_project.lock())
  {
    project->setImageAspectRatio(ratio);
  }
  emit rendererWidgetResized();
}


bool RenderStackedWidget::eventFilter(QObject *obj, QEvent *event)
{
  Q_UNUSED(obj);
  if(event->type() == QEvent::MouseMove || event->type() == QEvent::Wheel)
  {
    emit updateCameraModelViewMatrix();
    emit updateCameraEulerAngles();
  }

  if( event->type() == QEvent::KeyPress )
  {
    QKeyEvent * keyEvent = dynamic_cast<QKeyEvent*>(event);
    if( keyEvent->key() == Qt::Key_Delete )
    {
      deleteSelection();
    }
  }

  Qt::KeyboardModifiers keyMod = QApplication::keyboardModifiers();
  bool isSHIFT = keyMod.testFlag(Qt::ShiftModifier);
  bool isCTRL = keyMod.testFlag(Qt::ControlModifier);
  bool isALT = keyMod.testFlag(Qt::AltModifier);

  if(event->type() == QEvent::MouseButtonPress)
  {
    const QMouseEvent* const me = static_cast<const QMouseEvent*>( event );
    _origin = me->pos(); //...or ->globalPos();

    if(isSHIFT)   // contains shift
    {
      // Using the shift key means a new selection is chosen. If later a drag occurs it is modified to 'draggedNewSelection'
      _tracking = Tracking::newSelection;
      _rubberBand->setGeometry(QRect(_origin, QSize()));
      _rubberBand->show();
    }
    else if(isCTRL)  // command, not option
    {
      // Using the command key means the selection is extended. If later a drag occurs it is modified to 'draggedAddToSelection'
      _tracking = Tracking::addToSelection;
      _rubberBand->setGeometry(QRect(_origin, QSize()));
      _rubberBand->show();
    }
    else if(isALT && isCTRL)  // option and command
    {
      _tracking = Tracking::translateSelection;
    }
    else if(isALT) // option, not command
    {
      _tracking = Tracking::measurement;
    }
    else
    {
      _tracking = Tracking::backgroundClick;
    }
  }

  if(event->type() == QEvent::MouseMove)
  {
    const QMouseEvent* const me = static_cast<const QMouseEvent*>( event );
    switch(_tracking)
    {
      case Tracking::newSelection:
        // convert to dragged version
        _tracking = Tracking::draggedNewSelection;
        _rubberBand->setGeometry(QRect(_origin, me->pos()).normalized());
        break;
      case Tracking::addToSelection:
        // convert to dragged version
        _tracking = Tracking::draggedAddToSelection;
        _rubberBand->setGeometry(QRect(_origin, me->pos()).normalized());
        break;
      case Tracking::draggedNewSelection:
        _rubberBand->setGeometry(QRect(_origin, me->pos()).normalized());
        break;
      case Tracking::draggedAddToSelection:
        _rubberBand->setGeometry(QRect(_origin, me->pos()).normalized());
        break;
      case Tracking::translateSelection:
        break;
      case Tracking::measurement:
        break;
      default:
        _tracking =Tracking::other;
        break;
    }
  }

  if(event->type() == QEvent::MouseButtonRelease)
  {
    const QMouseEvent* const me = static_cast<const QMouseEvent*>( event );
    std::array<int,4> pixel{0,0,0,0};
    QRect rect = QRect(_origin, me->pos()).normalized();

    _rubberBand->hide();

    switch(_tracking)
    {
      case Tracking::newSelection:
        if (RKRenderViewController* widget = dynamic_cast<RKRenderViewController*>(currentWidget()))
        {
          pixel = widget->pickTexture(me->x(), me->y(), this->width(), this->height());

          int structureIdentifier = pixel[1];
          int movieIdentifier = pixel[2];
          int pickedObject  = pixel[3];

          switch(pixel[0])
          {
          case 1:
            _iraspa_structures[structureIdentifier][movieIdentifier]->structure()->setAtomSelection(pickedObject);
            reloadData();
            emit updateAtomSelection();
            emit updateBondSelection();
            break;
          case 2:
            _iraspa_structures[structureIdentifier][movieIdentifier]->structure()->setBondSelection(pickedObject);
            reloadData();
            emit updateBondSelection();
            break;
          default:
            break;
          }
        }
        break;
      case Tracking::addToSelection:
        if (RKRenderViewController* widget = dynamic_cast<RKRenderViewController*>(currentWidget()))
        {
          pixel = widget->pickTexture(me->x(), me->y(), this->width(), this->height());
          int structureIdentifier = pixel[1];
          int movieIdentifier = pixel[2];
          int pickedObject  = pixel[3];
          switch(pixel[0])
          {
          case 1:
            _iraspa_structures[structureIdentifier][movieIdentifier]->structure()->toggleAtomSelection(pickedObject);
            reloadData();
            emit updateAtomSelection();
            emit updateBondSelection();
            break;
          case 2:
            _iraspa_structures[structureIdentifier][movieIdentifier]->structure()->toggleBondSelection(pickedObject);
            reloadData();
            emit updateBondSelection();
            break;
          default:
            break;
          }
        }
        break;
      case Tracking::draggedNewSelection:
        if (std::shared_ptr<RKCamera> camera = _camera.lock())
        {
          selectAsymetricAtomsInRectangle(rect, false);
        }
        break;
      case Tracking::draggedAddToSelection:
        if (std::shared_ptr<RKCamera> camera = _camera.lock())
        {
          selectAsymetricAtomsInRectangle(rect, true);
        }
        break;
      case Tracking::translateSelection:
        break;
      case Tracking::measurement:
        break;
      case Tracking::backgroundClick:
        for(size_t i=0; i< _iraspa_structures.size();i++)
        {
          for(size_t j=0; j<_iraspa_structures[i].size(); j++)
          {
            _iraspa_structures[i][j]->structure()->clearSelection();
          }
        }
        if (RKRenderViewController* widget = dynamic_cast<RKRenderViewController*>(currentWidget()))
        {
          pixel = widget->pickTexture(me->x(), me->y(), this->width(), this->height());

          int structureIdentifier = pixel[1];
          int movieIdentifier = pixel[2];
          int pickedObject  = pixel[3];
          switch(pixel[0])
          {
          case 0:
            reloadData();
            emit updateAtomSelection();
            emit updateBondSelection();
            break;
          case 1:
            _iraspa_structures[structureIdentifier][movieIdentifier]->structure()->setAtomSelection(pickedObject);
            reloadData();
            emit updateAtomSelection();
            emit updateBondSelection();
            break;
          case 2:
            _iraspa_structures[structureIdentifier][movieIdentifier]->structure()->setBondSelection(pickedObject);
            reloadData();
            emit updateBondSelection();
            break;
          default:
            break;
          }
        }
        break;
      default:
        break;
    }
  }

  return QStackedWidget::eventFilter(obj, event);
}

void RenderStackedWidget::selectAsymetricAtomsInRectangle(QRect rect, bool extend)
{
  if (std::shared_ptr<ProjectTreeNode> projectTreeNode = _projectTreeNode.lock())
  {
    if(std::shared_ptr<iRASPAProject> iraspaProject = projectTreeNode->representedObject())
    {
      if (std::shared_ptr<RKCamera> camera = _camera.lock())
      {
        QRect viewPortBounds = QRect(QPoint(0.0,0.0), this->size());
        double3 Points0 = camera->myGluUnProject(double3(double(rect.left()), double(viewPortBounds.size().height() - rect.top()), 0.0), viewPortBounds);
        double3 Points1 = camera->myGluUnProject(double3(double(rect.left()), double(viewPortBounds.size().height() - rect.top()), 1.0), viewPortBounds);

        double3 Points2 = camera->myGluUnProject(double3(double(rect.left()), double(viewPortBounds.size().height() - rect.bottom()), 0.0), viewPortBounds);
        double3 Points3 = camera->myGluUnProject(double3(double(rect.left()), double(viewPortBounds.size().height() - rect.bottom()), 1.0), viewPortBounds);

        double3 Points4 = camera->myGluUnProject(double3(double(rect.right()), double(viewPortBounds.size().height() - rect.bottom()), 0.0), viewPortBounds);
        double3 Points5 = camera->myGluUnProject(double3(double(rect.right()), double(viewPortBounds.size().height() - rect.bottom()), 1.0), viewPortBounds);

        double3 Points6 = camera->myGluUnProject(double3(double(rect.right()), double(viewPortBounds.size().height() - rect.top()), 0.0), viewPortBounds);
        double3 Points7 = camera->myGluUnProject(double3(double(rect.right()), double(viewPortBounds.size().height() - rect.top()), 1.0), viewPortBounds);

        double3 FrustrumPlane0 = double3::cross(Points0 - Points1, Points0 - Points2).normalise();
        double3 FrustrumPlane1 = double3::cross(Points2 - Points3, Points2 - Points4).normalise();
        double3 FrustrumPlane2 = double3::cross(Points4 - Points5, Points4 - Points6).normalise();
        double3 FrustrumPlane3 = double3::cross(Points6 - Points7, Points6 - Points0).normalise();


        std::function<bool(double3)> closure = [=](double3 position) -> bool {
          return (double3::dot(position-Points0,FrustrumPlane0)<0) &&
              (double3::dot(position-Points2,FrustrumPlane1)<0) &&
              (double3::dot(position-Points4,FrustrumPlane2)<0) &&
              (double3::dot(position-Points6,FrustrumPlane3)<0);
        };
        iraspaProject->undoManager().beginMacro("delete selection");
        for(size_t i=0; i< _iraspa_structures.size();i++)
        {
          for(size_t j=0; j<_iraspa_structures[i].size(); j++)
          {
            std::shared_ptr<SKAtomTreeController> atomTreeController = _iraspa_structures[i][j]->structure()->atomsTreeController();
            std::shared_ptr<SKBondSetController> bondSetController = _iraspa_structures[i][j]->structure()->bondSetController();

            std::unordered_set<std::shared_ptr<SKAtomTreeNode>> previousAtomSelection = std::unordered_set<std::shared_ptr<SKAtomTreeNode>>(atomTreeController->selectedTreeNodes());
            std::set<int> previousBondSelection = bondSetController->selectionIndexSet();

            std::set<int> indexSetSelectedAtoms = _iraspa_structures[i][j]->structure()->filterCartesianAtomPositions(closure);
            std::set<int> indexSetSelectedBonds = _iraspa_structures[i][j]->structure()->filterCartesianBondPositions(closure);

            if(extend)
            {
              _iraspa_structures[i][j]->structure()->addToAtomSelection(indexSetSelectedAtoms);
              _iraspa_structures[i][j]->structure()->bondSetController()->addSelectedObjects(indexSetSelectedBonds);
            }
            else
            {
              _iraspa_structures[i][j]->structure()->setAtomSelection(indexSetSelectedAtoms);
              _iraspa_structures[i][j]->structure()->bondSetController()->setSelectedObjects(indexSetSelectedBonds);
            }

            std::unordered_set<std::shared_ptr<SKAtomTreeNode>> atomSelection = std::unordered_set<std::shared_ptr<SKAtomTreeNode>>(atomTreeController->selectedTreeNodes());
            std::set<int> bondSelection = bondSetController->selectionIndexSet();

            AtomChangeSelectionCommand *changeSelectionCommand = new AtomChangeSelectionCommand(_mainWindow, _iraspa_structures[i][j]->structure(), atomSelection, previousAtomSelection, bondSelection, previousBondSelection);
            iraspaProject->undoManager().push(changeSelectionCommand);
          }
        }
        iraspaProject->undoManager().endMacro();
      }
    }
  }
}

void RenderStackedWidget::deleteSelection()
{
  if(std::shared_ptr<ProjectTreeNode> projectTreeNode = _projectTreeNode.lock())
  {
    if(projectTreeNode->isEditable())
    {
      if(std::shared_ptr<iRASPAProject> iRASPAProject = projectTreeNode->representedObject())
      {
        iRASPAProject->undoManager().beginMacro("delete selection");
        if(std::shared_ptr<Project> project = iRASPAProject->project())
        {
          if (std::shared_ptr<ProjectStructure> projectStructure = std::dynamic_pointer_cast<ProjectStructure>(project))
          {
            for (std::shared_ptr<Structure> structure : projectStructure->flattenedStructures())
            {
              std::vector<std::shared_ptr<SKAtomTreeNode>> atoms = structure->atomsTreeController()->selectedAtomTreeNodes();
              sort(atoms.begin(), atoms.end(), [](std::shared_ptr<SKAtomTreeNode> node1, std::shared_ptr<SKAtomTreeNode> node2) -> bool {
                   return node1->indexPath() > node2->indexPath();});
              std::vector<IndexPath> atomSelection;
              std::transform(atoms.begin(),atoms.end(),std::back_inserter(atomSelection),[](std::shared_ptr<SKAtomTreeNode> node) -> IndexPath
                        {return node->indexPath();});

              std::vector<std::shared_ptr<SKAsymmetricBond>> bonds = structure->bondSetController()->selectedObjects();
              std::set<int> bondSelection = structure->bondSetController()->selectionIndexSet();
              RenderViewDeleteSelectionCommand *deleteSelectionCommand = new RenderViewDeleteSelectionCommand(_atomModel, _bondModel, _mainWindow, structure, atoms, atomSelection, bonds, bondSelection);
              iRASPAProject->undoManager().push(deleteSelectionCommand);
            }
          }
        }
        iRASPAProject->undoManager().endMacro();
      }
    }
  }
}


void RenderStackedWidget::ShowContextMenu(const QPoint &pos)
{
  if(std::shared_ptr<ProjectStructure> project = this->_project.lock())
  {
    QMenu contextMenu(tr("Context menu"), this);

    QAction actionResetCameraDistance("Reset camera distance", this);
    connect(&actionResetCameraDistance, &QAction::triggered, this, &RenderStackedWidget::resetCameraDistance);
    contextMenu.addAction(&actionResetCameraDistance);

    QMenu* subMenuResetCamera = contextMenu.addMenu( "Reset camera to" );
    QActionGroup* cameraResetDirectionGroup = new QActionGroup(this);
    QAction actionResetToZ("Z-direction", this);
    cameraResetDirectionGroup->addAction(&actionResetToZ);
    subMenuResetCamera->addAction(&actionResetToZ);
    connect(&actionResetToZ, &QAction::triggered, this, &RenderStackedWidget::resetCameraToZ);
    QAction actionResetToY("Y-direction", this);
    subMenuResetCamera->addAction(&actionResetToY);
    cameraResetDirectionGroup->addAction(&actionResetToY);
    connect(&actionResetToY, &QAction::triggered, this, &RenderStackedWidget::resetCameraToY);
    QAction actionResetToX("X-direction", this);
    cameraResetDirectionGroup->addAction(&actionResetToX);
    connect(&actionResetToX, &QAction::triggered, this, &RenderStackedWidget::resetCameraToX);
    subMenuResetCamera->addAction(&actionResetToX);


    QMenu* subMenuCameraProjection = contextMenu.addMenu( "Camera projection" );
    QActionGroup* cameraProjectionGroup = new QActionGroup(this);
    QAction actionOrthographic("Orthographic", this);
    actionOrthographic.setCheckable(true);
    cameraProjectionGroup->addAction(&actionOrthographic);
    subMenuCameraProjection->addAction(&actionOrthographic);
    actionOrthographic.setChecked(project->camera()->isOrthographic());
    connect(&actionOrthographic, &QAction::triggered, this, &RenderStackedWidget::setCameraToOrthographic);
    QAction actionPerspective("Perspective", this);
    actionPerspective.setCheckable(true);
    cameraProjectionGroup->addAction(&actionPerspective);
    subMenuCameraProjection->addAction(&actionPerspective);
    connect(&actionPerspective, &QAction::triggered, this, &RenderStackedWidget::setCameraToPerspective);
    actionPerspective.setChecked(project->camera()->isPerspective());

    QAction _actionShowBoundingBox("Show bounding box", this);
    _actionShowBoundingBox.setCheckable(true);
    _actionShowBoundingBox.setChecked(project->showBoundingBox());
    connect(&_actionShowBoundingBox, &QAction::toggled, this, &RenderStackedWidget::showBoundingBox);
    contextMenu.addAction(&_actionShowBoundingBox);

    QAction _actionComputeAOHighQuality("Compute AO high quality", this);
    //connect(&_actionComputeAOHighQuality, &QAction::toggled, this, &RenderStackedWidget::showBoundingBox);
    contextMenu.addAction(&_actionComputeAOHighQuality);

    QMenu* subMenuExport = contextMenu.addMenu( "Export to" );
    QActionGroup* exportToGroup = new QActionGroup(this);
    QAction actionExportToPDB("PDB", this);
    exportToGroup->addAction(&actionExportToPDB);
    subMenuExport->addAction(&actionExportToPDB);
    connect(&actionOrthographic, &QAction::triggered, this, &RenderStackedWidget::exportToPDB);
    QAction actionExportToMMCIF("mmCIF", this);
    exportToGroup->addAction(&actionExportToMMCIF);
    subMenuExport->addAction(&actionExportToMMCIF);
    connect(&actionOrthographic, &QAction::triggered, this, &RenderStackedWidget::exportToMMCIF);
    QAction actionExportToCIF("CIF", this);
    exportToGroup->addAction(&actionExportToCIF);
    subMenuExport->addAction(&actionExportToCIF);
    connect(&actionOrthographic, &QAction::triggered, this, &RenderStackedWidget::exportToCIF);
    QAction actionExportToXYZ("XYZ", this);
    exportToGroup->addAction(&actionExportToXYZ);
    subMenuExport->addAction(&actionExportToXYZ);
    connect(&actionOrthographic, &QAction::triggered, this, &RenderStackedWidget::exportToXYZ);
    QAction actionExportToPOSCAR("VASP POSCAR", this);
    exportToGroup->addAction(&actionExportToPOSCAR);
    subMenuExport->addAction(&actionExportToPOSCAR);
    connect(&actionOrthographic, &QAction::triggered, this, &RenderStackedWidget::exportToPOSCAR);


    contextMenu.exec(mapToGlobal(pos));
  }
}


void RenderStackedWidget::reloadRenderData()
{
  if (RKRenderViewController* widget = dynamic_cast<RKRenderViewController*>(currentWidget()))
  {
    widget->reloadRenderData();
  }
}

void RenderStackedWidget::reloadData()
{
  if (RKRenderViewController* widget = dynamic_cast<RKRenderViewController*>(currentWidget()))
  {
    widget->reloadData();
  }
  update();
}

void RenderStackedWidget::reloadAmbientOcclusionData()
{
  if (RKRenderViewController* widget = dynamic_cast<RKRenderViewController*>(currentWidget()))
  {
    widget->reloadAmbientOcclusionData();
  }
  update();
}

void RenderStackedWidget::reloadSelectionData()
{
  if (RKRenderViewController* widget = dynamic_cast<RKRenderViewController*>(currentWidget()))
  {
    widget->reloadData();
  }
  update();
}

void RenderStackedWidget::reloadBackgroundImage()
{
  if (RKRenderViewController* widget = dynamic_cast<RKRenderViewController*>(currentWidget()))
  {
    widget->reloadBackgroundImage();
  }
  update();
}


void RenderStackedWidget::resetCameraDistance()
{
  if(std::shared_ptr<ProjectStructure> project = this->_project.lock())
  {
     project->camera()->resetCameraDistance();
     emit updateCameraModelViewMatrix();
  }
}

void RenderStackedWidget::resetCameraToZ()
{
  if(std::shared_ptr<ProjectStructure> project = this->_project.lock())
  {
    project->camera()->setBoundingBox(project->renderBoundingBox());
    project->camera()->setResetDirectionType( ResetDirectionType::plus_Z);
    project->camera()->resetCameraToDirection();
    project->camera()->resetCameraDistance();
    emit updateCameraResetDirection();
  }
}

void RenderStackedWidget::resetCameraToY()
{
  if(std::shared_ptr<ProjectStructure> project = this->_project.lock())
  {
    project->camera()->setBoundingBox(project->renderBoundingBox());
    project->camera()->setResetDirectionType( ResetDirectionType::plus_Y);
    project->camera()->resetCameraToDirection();
    project->camera()->resetCameraDistance();
    emit updateCameraResetDirection();
  }
}

void RenderStackedWidget::resetCameraToX()
{
  if(std::shared_ptr<ProjectStructure> project = this->_project.lock())
  {
    project->camera()->setBoundingBox(project->renderBoundingBox());
    project->camera()->setResetDirectionType( ResetDirectionType::plus_X);
    project->camera()->resetCameraToDirection();
    project->camera()->resetCameraDistance();
    emit updateCameraResetDirection();
  }
}

void RenderStackedWidget::setCameraToOrthographic()
{
  if(std::shared_ptr<ProjectStructure> project = this->_project.lock())
  {
    project->camera()->setCameraToOrthographic();
    emit updateCameraProjection();
  }
}

void RenderStackedWidget::setCameraToPerspective()
{
  if(std::shared_ptr<ProjectStructure> project = this->_project.lock())
  {
    project->camera()->setCameraToPerspective();
    emit updateCameraProjection();
  }
}

void RenderStackedWidget::showBoundingBox(bool checked)
{
  if(std::shared_ptr<ProjectStructure> project = this->_project.lock())
  {
    project->setShowBoundingBox(checked);
    emit reloadData();
  }
}


void RenderStackedWidget::createPicture(QUrl fileURL, int width, int height)
{
  if (RKRenderViewController* widget = dynamic_cast<RKRenderViewController*>(currentWidget()))
  {
    std::cout << "Saving file: " << fileURL.path().toStdString() << std::endl;
    QImage image = widget->renderSceneToImage(width, height);
    image.save(fileURL.toLocalFile());
  }
}

int nearestEvenInt(int to)
{
  return (to % 2 == 0) ? to : (to + 1);
}

void RenderStackedWidget::createMovie(QUrl fileURL, int width, int height, MovieWriter::Type type)
{
  if (RKRenderViewController* widget = dynamic_cast<RKRenderViewController*>(currentWidget()))
  {
    if (std::shared_ptr<ProjectStructure> project = _project.lock())
    {
      MovieWriter movie(nearestEvenInt(width), nearestEvenInt(height), project->movieFramesPerSecond(), _logReporter, type);
      int ret = movie.initialize(fileURL.toLocalFile().toStdString());
      if (ret < 0)
      {
        _logReporter->logMessage(LogReporting::ErrorLevel::error, "failed to create movie");
        return;
      }

      int numberOfFrames = project->maxNumberOfMoviesFrames();
      for (int iframe = 0; iframe < numberOfFrames; iframe++)
      {
        project->setMovieFrameIndex(iframe);
        _mainWindow->propagateProject(_projectTreeNode.lock(), _mainWindow);
        widget->reloadData();
        QImage image = widget->renderSceneToImage(nearestEvenInt(width), nearestEvenInt(height));

        movie.addFrame(image.bits(), iframe);
      }
      movie.finalize();
    }
  }
}


void RenderStackedWidget::redraw()
{
  currentWidget()->update();
}


void RenderStackedWidget::redrawWithLowQuality()
{
  if (RKRenderViewController* widget = dynamic_cast<RKRenderViewController*>(currentWidget()))
  {
    widget->redrawWithLowQuality();
  }
}

void RenderStackedWidget::redrawWithMediumQuality()
{
  if (RKRenderViewController* widget = dynamic_cast<RKRenderViewController*>(currentWidget()))
  {
    widget->redrawWithMediumQuality();
  }
}

void RenderStackedWidget::redrawWithHighQuality()
{
  if (RKRenderViewController* widget = dynamic_cast<RKRenderViewController*>(currentWidget()))
  {
    widget->redrawWithHighQuality();
  }
}

void RenderStackedWidget::redrawWithPictureQuality()
{
  if (RKRenderViewController* widget = dynamic_cast<RKRenderViewController*>(currentWidget()))
  {
    widget->redrawWithPictureQuality();
  }
}

void RenderStackedWidget::invalidateCachedAmbientOcclusionTexture(std::vector<std::shared_ptr<RKRenderStructure>> structures)
{
  qDebug() << "invalidateCachedAmbientOcclusionTexture: " << structures.size();
  if (RKRenderViewController* widget = dynamic_cast<RKRenderViewController*>(currentWidget()))
  {
    widget->invalidateCachedAmbientOcclusionTexture(structures);
  }
}

void RenderStackedWidget::invalidateIsosurface(std::vector<std::shared_ptr<RKRenderStructure>> structures)
{
  if (RKRenderViewController* widget = dynamic_cast<RKRenderViewController*>(currentWidget()))
  {
    std::cout << "invalidateIsosurface: " << structures.size() << std::endl;
    widget->invalidateIsosurface(structures);
  }
}

void RenderStackedWidget::computeHeliumVoidFraction(std::vector<std::shared_ptr<RKRenderStructure>> structures)
{
  if (RKRenderViewController* widget = dynamic_cast<RKRenderViewController*>(currentWidget()))
  {
    widget->computeHeliumVoidFraction(structures);
  }
}

void RenderStackedWidget::computeNitrogenSurfaceArea(std::vector<std::shared_ptr<RKRenderStructure>> structures)
{
  if (RKRenderViewController* widget = dynamic_cast<RKRenderViewController*>(currentWidget()))
  {
    widget->computeNitrogenSurfaceArea(structures);
  }
}

void RenderStackedWidget::exportToPDB() const
{
  if(std::shared_ptr<ProjectStructure> project = this->_project.lock())
  {
    //project->camera()->setCameraToPerspective();
    //emit updateCameraProjection();
  }
}

void RenderStackedWidget::exportToMMCIF() const
{
  if(std::shared_ptr<ProjectStructure> project = this->_project.lock())
  {
    //project->camera()->setCameraToPerspective();
    //emit updateCameraProjection();
  }
}

void RenderStackedWidget::exportToCIF() const
{
  if(std::shared_ptr<ProjectStructure> project = this->_project.lock())
  {
    //project->camera()->setCameraToPerspective();
    //emit updateCameraProjection();
  }
}

void RenderStackedWidget::exportToXYZ() const
{
  if(std::shared_ptr<ProjectStructure> project = this->_project.lock())
  {
    //project->camera()->setCameraToPerspective();
    //emit updateCameraProjection();
  }
}

void RenderStackedWidget::exportToPOSCAR() const
{
  if(std::shared_ptr<ProjectStructure> project = this->_project.lock())
  {
    //project->camera()->setCameraToPerspective();
    //emit updateCameraProjection();
  }
}
