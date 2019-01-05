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
#include <QAction>
#include <QtDebug>
#include <iraspakit.h>
#include "fixedaspectratiolayoutitem.h"

RenderStackedWidget::RenderStackedWidget(QWidget* parent ): QStackedWidget(parent ),
    _rubberBand(new QRubberBand(QRubberBand::Rectangle, this))
{
  this->setContextMenuPolicy(Qt::CustomContextMenu);

  setMouseTracking(true);

  QObject::connect(this, &QWidget::customContextMenuRequested, this, &RenderStackedWidget::ShowContextMenu);
}


void RenderStackedWidget::resizeEvent(QResizeEvent *event)
{
  QSize size = event->size();
  double ratio = double(size.width())/double(size.height());
  if(std::shared_ptr<ProjectStructure> project = this->project.lock())
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

          if(pixel[0]>0)
          {
            int structureIdentifier = pixel[1];
            int movieIdentifier = pixel[2];
            int pickedAtom  = pixel[3];
            _structures[structureIdentifier][movieIdentifier]->setAtomSelection(pickedAtom);
            reloadData();
            emit updateAtomSelection();
          }
        }
        break;
      case Tracking::addToSelection:
        if (RKRenderViewController* widget = dynamic_cast<RKRenderViewController*>(currentWidget()))
        {
          pixel = widget->pickTexture(me->x(), me->y(), this->width(), this->height());
          if(pixel[0]>0)
          {
            int structureIdentifier = pixel[1];
            int movieIdentifier = pixel[2];
            int pickedAtom  = pixel[3];
            _structures[structureIdentifier][movieIdentifier]->toggleAtomSelection(pickedAtom);
            reloadData();
            emit updateAtomSelection();
          }
        }
        break;
      case Tracking::draggedNewSelection:
        if (std::shared_ptr<RKCamera> camera = _camera.lock())
        {
          for(int i=0; i< _structures.size();i++)
          {
            for(int j=0; j<_structures[i].size(); j++)
            {
              std::vector<double3> atomPositions = _structures[i][j]->atomPositions();
              std::vector<int> atomIds = camera->selectPositionsInRectangle(atomPositions, rect, _structures[i][j]->origin(), QRect(QPoint(0.0,0.0), this->size()));
              _structures[i][j]->setAtomSelection(atomIds);
            }
          }
          reloadData();
          emit updateAtomSelection();
        }
        break;
      case Tracking::draggedAddToSelection:
        if (std::shared_ptr<RKCamera> camera = _camera.lock())
        {
          for(int i=0; i< _structures.size();i++)
          {
            for(int j=0; j<_structures[i].size(); j++)
            {
              std::vector<double3> atomPositions = _structures[i][j]->atomPositions();
              std::vector<int> atomIds = camera->selectPositionsInRectangle(atomPositions, rect, _structures[i][j]->origin(), QRect(QPoint(0.0,0.0), this->size()));
              _structures[i][j]->addToAtomSelection(atomIds);
            }
          }
          reloadData();
          emit updateAtomSelection();
        }
        break;
      case Tracking::translateSelection:
        break;
      case Tracking::measurement:
        break;
      case Tracking::backgroundClick:
        for(int i=0; i< _structures.size();i++)
        {
          for(int j=0; j<_structures[i].size(); j++)
          {
            _structures[i][j]->clearSelection();
          }
        }
        if (RKRenderViewController* widget = dynamic_cast<RKRenderViewController*>(currentWidget()))
        {
          pixel = widget->pickTexture(me->x(), me->y(), this->width(), this->height());
          if(pixel[0]>0)
          {
            int structureIdentifier = pixel[1];
            int movieIdentifier = pixel[2];
            int pickedAtom  = pixel[3];
            _structures[structureIdentifier][movieIdentifier]->setAtomSelection(pickedAtom);
          }
          reloadData();
          emit updateAtomSelection();
        }
        break;
      default:
        break;
    }
  }

  return false;
}


void RenderStackedWidget::setProject(std::shared_ptr<ProjectTreeNode> projectTreeNode)
{
  this->projectTreeNode = projectTreeNode;

  this->project.reset();
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

          this->project = projectStructure;
          this->_camera = projectStructure->camera();
          _structures = projectStructure->structures();

          qDebug() << "RenderStackedWidget: setProject, number of structures" << _structures.size();
        }
      }
    }
  }
}




void RenderStackedWidget::ShowContextMenu(const QPoint &pos)
{
  if(std::shared_ptr<ProjectStructure> project = this->project.lock())
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
    widget->reloadSelectionData();
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
  if(std::shared_ptr<ProjectStructure> project = this->project.lock())
  {
     project->camera()->resetCameraDistance();
     emit updateCameraModelViewMatrix();
  }
}

void RenderStackedWidget::resetCameraToZ()
{
  if(std::shared_ptr<ProjectStructure> project = this->project.lock())
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
  if(std::shared_ptr<ProjectStructure> project = this->project.lock())
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
  if(std::shared_ptr<ProjectStructure> project = this->project.lock())
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
  if(std::shared_ptr<ProjectStructure> project = this->project.lock())
  {
    project->camera()->setCameraToOrthographic();
    emit updateCameraProjection();
  }
}

void RenderStackedWidget::setCameraToPerspective()
{
  if(std::shared_ptr<ProjectStructure> project = this->project.lock())
  {
    project->camera()->setCameraToPerspective();
    emit updateCameraProjection();
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

void RenderStackedWidget::redraw()
{
  currentWidget()->update();

  //foreach(QObject *child, currentWidget()->children())
  //{
  //  if (RKRenderViewController* widget = dynamic_cast<RKRenderViewController*>(child))
  //  {
  //    widget->redraw();
  //  }
  //}
}


void RenderStackedWidget::redrawWithLowQuality()
{
  std::cout << "redrawWithLowQuality" << std::endl;
  if (RKRenderViewController* widget = dynamic_cast<RKRenderViewController*>(currentWidget()))
  {
    widget->redrawWithLowQuality();
  }
}

void RenderStackedWidget::redrawWithMediumQuality()
{
  std::cout << "redrawWithMediumQuality" << std::endl;
  if (RKRenderViewController* widget = dynamic_cast<RKRenderViewController*>(currentWidget()))
  {
    widget->redrawWithMediumQuality();
  }
}

void RenderStackedWidget::redrawWithHighQuality()
{
  std::cout << "redrawWithHighQuality" << std::endl;
  if (RKRenderViewController* widget = dynamic_cast<RKRenderViewController*>(currentWidget()))
  {
    widget->redrawWithHighQuality();
  }
  std::cout << "end redrawWithHighQuality" << std::endl;
}

void RenderStackedWidget::redrawWithPictureQuality()
{
  std::cout << "redrawWithPictureQuality" << std::endl;
  if (RKRenderViewController* widget = dynamic_cast<RKRenderViewController*>(currentWidget()))
  {
    widget->redrawWithPictureQuality();
  }
}

void RenderStackedWidget::invalidateCachedAmbientOcclusionTexture(std::vector<std::shared_ptr<RKRenderStructure>> structures)
{
  if (RKRenderViewController* widget = dynamic_cast<RKRenderViewController*>(currentWidget()))
  {
    std::cout << "invalidateCachedAmbientOcclusionTexture: " << structures.size() << std::endl;
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
