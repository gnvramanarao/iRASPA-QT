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

#include "cameratreewidgetcontroller.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <QLabel>
#include <QPixmap>
#include <mathkit.h>
#include <renderkit.h>
#include <foundationkit.h>
#include "textfield.h"

CameraTreeWidgetController::CameraTreeWidgetController(QWidget* parent): QTreeWidget(parent ),
    _cameraCameraForm(new CameraCameraForm),
    _cameraSelectionForm(new CameraSelectionForm),
    _cameraLightsForm(new CameraLightsForm),
    _cameraPicturesForm(new CameraPicturesForm),
    _cameraBackgroundForm(new CameraBackgroundForm)
{
  this->setHeaderHidden(true);
  this->setRootIsDecorated(true);
  this->setFrameStyle(QFrame::NoFrame);
  this->setAnimated(true);
  this->setVerticalScrollMode(QTreeWidget::ScrollPerPixel);
  this->setExpandsOnDoubleClick(false);
  this->setIndentation(0);
  this->setSelectionMode(QAbstractItemView::NoSelection);



  // Camera
  //==========================================================================================================
  QTreeWidgetItem* cameraItem = new QTreeWidgetItem(this);
  this->addTopLevelItem(cameraItem);

  pushButtonCamera = new QPushButton(QString("Camera"),this);
  pushButtonCamera->setIcon(QIcon(":/iRASPA/collapsed.png"));
  pushButtonCamera->setStyleSheet("text-align:left;");
  setItemWidget(cameraItem,0,pushButtonCamera);

  QTreeWidgetItem *childCameraItem = new QTreeWidgetItem(cameraItem);
  this->setItemWidget(childCameraItem,0, _cameraCameraForm);

  QObject::connect(pushButtonCamera, &QPushButton::clicked, this, &CameraTreeWidgetController::expandCameraItem);

  pushButtonCamera->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
  QFontMetrics fm(font());
  pushButtonCamera->resize(size().width(), fm.height());

  QObject::connect(_cameraCameraForm->resetPercentageLineEdit,&TextField::textEditedOnEnter,this,&CameraTreeWidgetController::setResetPercentage);
  QObject::connect(_cameraCameraForm->resetCameraToDirectionPushButton, &QPushButton::clicked,this,&CameraTreeWidgetController::resetCameraToDefaultDirection);
  QObject::connect(_cameraCameraForm->resetCameraToMinusXRadioButton, &QRadioButton::clicked,this,&CameraTreeWidgetController::setResetDirectionToMinusX);
  QObject::connect(_cameraCameraForm->resetCameraToPlusXRadioButton, &QRadioButton::clicked,this,&CameraTreeWidgetController::setResetDirectionToPlusX);
  QObject::connect(_cameraCameraForm->resetCameraToMinusYRadioButton, &QRadioButton::clicked,this,&CameraTreeWidgetController::setResetDirectionToMinusY);
  QObject::connect(_cameraCameraForm->resetCameraToPlusYRadioButton, &QRadioButton::clicked,this,&CameraTreeWidgetController::setResetDirectionToPlusY);
  QObject::connect(_cameraCameraForm->resetCameraToMinusZRadioButton, &QRadioButton::clicked,this,&CameraTreeWidgetController::setResetDirectionToMinusZ);
  QObject::connect(_cameraCameraForm->resetCameraToPlusZRadioButton, &QRadioButton::clicked,this,&CameraTreeWidgetController::setResetDirectionToPlusZ);


  QObject::connect(_cameraCameraForm->orthographicCameraRadioButton, &QRadioButton::clicked,this,&CameraTreeWidgetController::setCameraToOrthographic);
  QObject::connect(_cameraCameraForm->perspectiveCameraRadioButton, &QRadioButton::clicked,this,&CameraTreeWidgetController::setCameraToPerspective);

  QObject::connect(_cameraCameraForm->angleOfViewSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&CameraTreeWidgetController::setAngleOfView);

  QObject::connect(_cameraCameraForm->rotateAngleLineEdit, &TextField::textEditedOnEnter,this,&CameraTreeWidgetController::setRotationAngle);
  QObject::connect(_cameraCameraForm->rotateMinusXPushButton,&QPushButton::clicked,this,&CameraTreeWidgetController::rotateYawPlus);
  QObject::connect(_cameraCameraForm->rotatePlusXPushButton,&QPushButton::clicked,this,&CameraTreeWidgetController::rotateYawMinus);
  QObject::connect(_cameraCameraForm->rotateMinusYPushButton,&QPushButton::clicked,this,&CameraTreeWidgetController::rotatePitchPlus);
  QObject::connect(_cameraCameraForm->rotatePlusYPushButton,&QPushButton::clicked,this,&CameraTreeWidgetController::rotatePitchMinus);
  QObject::connect(_cameraCameraForm->rotateMinusZPushButton,&QPushButton::clicked,this,&CameraTreeWidgetController::rotateRollPlus);
  QObject::connect(_cameraCameraForm->rotatePlusZPushButton,&QPushButton::clicked,this,&CameraTreeWidgetController::rotateRollMinus);

  QObject::connect(_cameraCameraForm->EulerAngleXDial, &QDial::sliderMoved,this,static_cast<void (CameraTreeWidgetController::*)(int)>(&CameraTreeWidgetController::setEulerAngleX));
  QObject::connect(_cameraCameraForm->EulerAngleXLineEdit, &TextField::textEditedOnEnter,this,static_cast<void (CameraTreeWidgetController::*)(const QString &)>(&CameraTreeWidgetController::setEulerAngleX));
  QObject::connect(_cameraCameraForm->EulerAngleZDial, &QDial::sliderMoved,this,static_cast<void (CameraTreeWidgetController::*)(int)>(&CameraTreeWidgetController::setEulerAngleZ));
  QObject::connect(_cameraCameraForm->EulerAngleZLineEdit, &TextField::textEditedOnEnter,this,static_cast<void (CameraTreeWidgetController::*)(const QString &)>(&CameraTreeWidgetController::setEulerAngleZ));
  QObject::connect(_cameraCameraForm->EulerAngleYSlider, &QDial::sliderMoved,this,static_cast<void (CameraTreeWidgetController::*)(int)>(&CameraTreeWidgetController::setEulerAngleY));
  QObject::connect(_cameraCameraForm->EulerAngleYLineEdit, &TextField::textEditedOnEnter,this,static_cast<void (CameraTreeWidgetController::*)(const QString &)>(&CameraTreeWidgetController::setEulerAngleY));

  // Selection
  //==========================================================================================================
  QTreeWidgetItem* selectionItem = new QTreeWidgetItem(this);
  this->addTopLevelItem(selectionItem);

  pushButtonSelection = new QPushButton(QString("Selection"),this);
  pushButtonSelection->setIcon(QIcon(":/iRASPA/collapsed.png"));
  pushButtonSelection->setStyleSheet("text-align:left;");
  setItemWidget(selectionItem,0,pushButtonSelection);

  QTreeWidgetItem *childSelectionItem = new QTreeWidgetItem(selectionItem);
  this->setItemWidget(childSelectionItem,0, _cameraSelectionForm);

  QObject::connect(pushButtonSelection, &QPushButton::clicked, this, &CameraTreeWidgetController::expandSelectionItem);

  QObject::connect(_cameraSelectionForm->selectionIntensityDoubleSpinBox,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&CameraTreeWidgetController::setSelectionIntensity);
  QObject::connect(_cameraSelectionForm->selectionIntensityDoubleSlider,static_cast<void (QDoubleSlider::*)(double)>(&QDoubleSlider::sliderMoved),this,&CameraTreeWidgetController::setSelectionIntensity);


  pushButtonSelection->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
  pushButtonSelection->resize(size().width(), fm.height());

  _cameraSelectionForm->selectionIntensityDoubleSlider->setDoubleMinimum(0.0);
  _cameraSelectionForm->selectionIntensityDoubleSlider->setDoubleMaximum(2.0);

  // Lights
  //==========================================================================================================
  QTreeWidgetItem* lightsItem = new QTreeWidgetItem(this);
  this->addTopLevelItem(lightsItem);

  pushButtonLights = new QPushButton(QString("Lights"),this);
  pushButtonLights->setIcon(QIcon(":/iRASPA/collapsed.png"));
  pushButtonLights->setStyleSheet("text-align:left;");
  setItemWidget(lightsItem,0,pushButtonLights);

  QTreeWidgetItem *childLightsItem = new QTreeWidgetItem(lightsItem);
  this->setItemWidget(childLightsItem,0, _cameraLightsForm);

  QObject::connect(pushButtonLights, &QPushButton::clicked, this, &CameraTreeWidgetController::expandLightsItem);

  QObject::connect(_cameraLightsForm->ambientLightIntensityDoubleSpinBox,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&CameraTreeWidgetController::setAmbientLightIntensity);
  QObject::connect(_cameraLightsForm->ambientLightIntensityDoubleSlider,static_cast<void (QDoubleSlider::*)(double)>(&QDoubleSlider::sliderMoved),this,&CameraTreeWidgetController::setAmbientLightIntensity);
  QObject::connect(_cameraLightsForm->ambientLightPushButton,&QPushButton::clicked,this,&CameraTreeWidgetController::setAmbientColor);

  QObject::connect(_cameraLightsForm->diffuseLightIntensityDoubleSpinBox,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&CameraTreeWidgetController::setDiffuseLightIntensity);
  QObject::connect(_cameraLightsForm->diffuseLightIntensityDoubleSlider,static_cast<void (QDoubleSlider::*)(double)>(&QDoubleSlider::sliderMoved),this,&CameraTreeWidgetController::setDiffuseLightIntensity);
  QObject::connect(_cameraLightsForm->diffuseLightPushButton,&QPushButton::clicked,this,&CameraTreeWidgetController::setDiffuseColor);

  QObject::connect(_cameraLightsForm->specularLightIntensityDoubleSpinBox,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&CameraTreeWidgetController::setSpecularLightIntensity);
  QObject::connect(_cameraLightsForm->specularLightIntensityDoubleSlider,static_cast<void (QDoubleSlider::*)(double)>(&QDoubleSlider::sliderMoved),this,&CameraTreeWidgetController::setSpecularLightIntensity);
  QObject::connect(_cameraLightsForm->specularLightPushButton,&QPushButton::clicked,this,&CameraTreeWidgetController::setSpecularColor);


  pushButtonLights->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
  pushButtonLights->resize(size().width(), fm.height());

  // Pictures
  //==========================================================================================================
  QTreeWidgetItem* picturesItem = new QTreeWidgetItem(this);
  this->addTopLevelItem(picturesItem);

  pushButtonPictures = new QPushButton(QString("Pictures"),this);
  pushButtonPictures->setIcon(QIcon(":/iRASPA/collapsed.png"));
  pushButtonPictures->setStyleSheet("text-align:left;");
  setItemWidget(picturesItem,0,pushButtonPictures);

  QTreeWidgetItem *childPicturesItem = new QTreeWidgetItem(picturesItem);
  this->setItemWidget(childPicturesItem,0, _cameraPicturesForm);

  QObject::connect(pushButtonPictures, &QPushButton::clicked, this, &CameraTreeWidgetController::expandPicturesItem);

  pushButtonPictures->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
  pushButtonPictures->resize(size().width(), fm.height());

  _cameraPicturesForm->dotsPerInchComboBox->insertItem(0,"72 dpi");
  _cameraPicturesForm->dotsPerInchComboBox->insertItem(1,"75 dpi");
  _cameraPicturesForm->dotsPerInchComboBox->insertItem(2,"150 dpi");
  _cameraPicturesForm->dotsPerInchComboBox->insertItem(3,"300 dpi");
  _cameraPicturesForm->dotsPerInchComboBox->insertItem(4,"600 dpi");
  _cameraPicturesForm->dotsPerInchComboBox->insertItem(5,"1200 dpi");

  _cameraPicturesForm->qualityComboBox->insertItem(0,"8-bits, RGBA");
  _cameraPicturesForm->qualityComboBox->insertItem(1,"16-bits, RGBA");
  _cameraPicturesForm->qualityComboBox->insertItem(2,"8-bits, CMYK");
  _cameraPicturesForm->qualityComboBox->insertItem(3,"16-bits, CMYK");

  QObject::connect(_cameraPicturesForm->dotsPerInchComboBox,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),this,&CameraTreeWidgetController::setPictureDotsPerInch);
  QObject::connect(_cameraPicturesForm->qualityComboBox,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),this,&CameraTreeWidgetController::setPictureQuality);

  QObject::connect(_cameraPicturesForm->physicalWidthDoubleSpinBox,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&CameraTreeWidgetController::setPicturePhysicalSize);
  QObject::connect(_cameraPicturesForm->pixelWidthSpinBox,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),this,&CameraTreeWidgetController::setPictureNumberOfPixels);

  QObject::connect(_cameraPicturesForm->physicalDimensionsRadioButton, &QRadioButton::clicked, this,&CameraTreeWidgetController::setPictureDimensionToPhysical);
  QObject::connect(_cameraPicturesForm->pixelDimensionsRadioButton, &QRadioButton::clicked, this,&CameraTreeWidgetController::setPictureDimensionToPixels);
  QObject::connect(_cameraPicturesForm->inchRadioButton, &QRadioButton::clicked, this,&CameraTreeWidgetController::setPictureUnitsToInch);
  QObject::connect(_cameraPicturesForm->cmRadioButton, &QRadioButton::clicked, this,&CameraTreeWidgetController::setPictureUnitsToCentimeters);

  QObject::connect(_cameraPicturesForm->createPicturePushButton,&QPushButton::clicked,this,&CameraTreeWidgetController::savePicture);
  QObject::connect(_cameraPicturesForm->createMoviePushButton,&QPushButton::clicked,this,&CameraTreeWidgetController::saveMovie);


  // Background
  //==========================================================================================================
  QTreeWidgetItem* backgroundItem = new QTreeWidgetItem(this);
  this->addTopLevelItem(backgroundItem);

  pushButtonBackground = new QPushButton(QString("Background"),this);
  pushButtonBackground->setIcon(QIcon(":/iRASPA/collapsed.png"));
  pushButtonBackground->setStyleSheet("text-align:left;");
  setItemWidget(backgroundItem,0,pushButtonBackground);

  QTreeWidgetItem *childBackgroundItem = new QTreeWidgetItem(backgroundItem);
  this->setItemWidget(childBackgroundItem,0, _cameraBackgroundForm);

  QObject::connect(pushButtonBackground, &QPushButton::clicked, this, &CameraTreeWidgetController::expandBackgroundItem);

  pushButtonBackground->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
  pushButtonBackground->resize(size().width(), fm.height());


  QObject::connect(_cameraBackgroundForm->colorRadioButton, &QRadioButton::clicked, this, &CameraTreeWidgetController::setBackgroundToColor);
  QObject::connect(_cameraBackgroundForm->linearGradientRadioButton, &QRadioButton::clicked, this, &CameraTreeWidgetController::setBackgroundToLinearGradient);
  QObject::connect(_cameraBackgroundForm->radialGradientRadioButton, &QRadioButton::clicked, this, &CameraTreeWidgetController::setBackgroundToRadialGradient);
  QObject::connect(_cameraBackgroundForm->imageRadioButton, &QRadioButton::clicked, this, &CameraTreeWidgetController::setBackgroundToImage);

  QObject::connect(_cameraBackgroundForm->backgroundColorPushButton,&QPushButton::clicked,this,&CameraTreeWidgetController::setBackgroundColor);
  QObject::connect(_cameraBackgroundForm->linearGradientFromColorPushButton,&QPushButton::clicked,this,&CameraTreeWidgetController::setLinearGradientFromColor);
  QObject::connect(_cameraBackgroundForm->linearGradientToColorPushButton,&QPushButton::clicked,this,&CameraTreeWidgetController::setLinearGradientToColor);
  QObject::connect(_cameraBackgroundForm->radialGradientFromColorPushButton,&QPushButton::clicked,this,&CameraTreeWidgetController::setRadialGradientFromColor);
  QObject::connect(_cameraBackgroundForm->radialGradientToColorPushButton,&QPushButton::clicked,this,&CameraTreeWidgetController::setRadialGradientToColor);

  QObject::connect(_cameraBackgroundForm->selectImagePushButton,&QPushButton::clicked,this,&CameraTreeWidgetController::selectBackgroundImage);

  QObject::connect(_cameraBackgroundForm->linearGradientAngleDoubleSpinBox,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&CameraTreeWidgetController::setLinearGradientAngle);
  QObject::connect(_cameraBackgroundForm->linearGradientAngleDoubleSlider,static_cast<void (QDoubleSlider::*)(double)>(&QDoubleSlider::sliderMoved),this,&CameraTreeWidgetController::setLinearGradientAngle);

  _cameraBackgroundForm->linearGradientAngleDoubleSlider->setDoubleMinimum(0.0);
  _cameraBackgroundForm->linearGradientAngleDoubleSlider->setDoubleMaximum(360.0);

  QObject::connect(_cameraBackgroundForm->radialGradientRoundnessDoubleSpinBox,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&CameraTreeWidgetController::setRadialGradientRoundness);
  QObject::connect(_cameraBackgroundForm->radialGradientRoundnessDoubleSlider,static_cast<void (QDoubleSlider::*)(double)>(&QDoubleSlider::sliderMoved),this,&CameraTreeWidgetController::setRadialGradientRoundness);

  _cameraBackgroundForm->radialGradientRoundnessDoubleSlider->setDoubleMinimum(0.0);
  _cameraBackgroundForm->radialGradientRoundnessDoubleSlider->setDoubleMaximum(5.0);
}

void CameraTreeWidgetController::setMainWindow(MainWindow *mainWindow)
{
  _mainWindow = mainWindow;
}

void CameraTreeWidgetController::setProject(std::shared_ptr<ProjectTreeNode> projectTreeNode)
{
  _camera.reset();
  _project.reset();
  if (projectTreeNode)
  {
    if(std::shared_ptr<iRASPAProject> iraspaProject = projectTreeNode->representedObject())
    {
      if(std::shared_ptr<Project> project = iraspaProject->project())
      {
        if (std::shared_ptr<ProjectStructure> projectStructure = std::dynamic_pointer_cast<ProjectStructure>(project))
        {
          _camera = projectStructure->camera();
          _project = projectStructure;

          reloadCameraData();
          update();
        }
      }
    }
  }
}

void CameraTreeWidgetController::expandCameraItem()
{
  QModelIndex index = indexFromItem(topLevelItem(0),0);
  if(this->isExpanded(index))
  {
    this->collapse(index);
    pushButtonCamera->setIcon(QIcon(":/iRASPA/collapsed.png"));
  }
  else
  {
    this->expand(index);
    pushButtonCamera->setIcon(QIcon(":/iRASPA/expanded.png"));
  }
}

void CameraTreeWidgetController::expandSelectionItem()
{
  QModelIndex index = indexFromItem(topLevelItem(1),0);
  if(this->isExpanded(index))
  {
    this->collapse(index);
    pushButtonSelection->setIcon(QIcon(":/iRASPA/collapsed.png"));
  }
  else
  {
    this->expand(index);
    pushButtonSelection->setIcon(QIcon(":/iRASPA/expanded.png"));
  }
}

void CameraTreeWidgetController::expandLightsItem()
{
  QModelIndex index = indexFromItem(topLevelItem(2),0);
  if(this->isExpanded(index))
  {
    this->collapse(index);
    pushButtonLights->setIcon(QIcon(":/iRASPA/collapsed.png"));
  }
  else
  {
    this->expand(index);
    pushButtonLights->setIcon(QIcon(":/iRASPA/expanded.png"));
  }
}

void CameraTreeWidgetController::expandPicturesItem()
{
  QModelIndex index = indexFromItem(topLevelItem(3),0);
  if(this->isExpanded(index))
  {
    this->collapse(index);
    pushButtonPictures->setIcon(QIcon(":/iRASPA/collapsed.png"));
  }
  else
  {
    this->expand(index);
    pushButtonPictures->setIcon(QIcon(":/iRASPA/expanded.png"));
  }
}

void CameraTreeWidgetController::expandBackgroundItem()
{
  QModelIndex index = indexFromItem(topLevelItem(4),0);
  if(this->isExpanded(index))
  {
    this->collapse(index);
    pushButtonBackground->setIcon(QIcon(":/iRASPA/collapsed.png"));
  }
  else
  {
    this->expand(index);
    pushButtonBackground->setIcon(QIcon(":/iRASPA/expanded.png"));
  }
}

void CameraTreeWidgetController::renderWidgetResize(QSize size)
{

}


void CameraTreeWidgetController::reloadData()
{reloadCameraProperties();
  reloadCameraProjection();
  reloadCameraResetDirection();
  reloadCameraEulerAngles();
  reloadCameraModelViewMatrix();

  reloadSelectionProperties();
  reloadLightsProperties();
  reloadPictureProperties();
  reloadBackgroundProperties();
}

void CameraTreeWidgetController::reloadSelection()
{

}

void CameraTreeWidgetController::reloadCameraResetDirection()
{
  if(std::shared_ptr<RKCamera> camera = _camera.lock())
  {
    switch(camera->resetCameraDirection())
    {
      case ResetDirectionType::minus_X:
        whileBlocking(_cameraCameraForm->resetCameraToMinusXRadioButton)->setChecked(true);
        break;
      case ResetDirectionType::minus_Y:
        whileBlocking(_cameraCameraForm->resetCameraToMinusYRadioButton)->setChecked(true);
        break;
      case ResetDirectionType::minus_Z:
        whileBlocking(_cameraCameraForm->resetCameraToMinusZRadioButton)->setChecked(true);
        break;
      case ResetDirectionType::plus_X:
        whileBlocking(_cameraCameraForm->resetCameraToPlusXRadioButton)->setChecked(true);
        break;
      case ResetDirectionType::plus_Y:
        whileBlocking(_cameraCameraForm->resetCameraToPlusYRadioButton)->setChecked(true);
        break;
      case ResetDirectionType::plus_Z:
        whileBlocking(_cameraCameraForm->resetCameraToPlusZRadioButton)->setChecked(true);
        break;
     }
  }
}

void CameraTreeWidgetController::reloadCameraProjection()
{
  if(std::shared_ptr<RKCamera> camera = _camera.lock())
  {
    whileBlocking(_cameraCameraForm->resetPercentageLineEdit)->setText(QString::number(100.0*camera->resetFraction()));
    if(camera->isOrthographic()) whileBlocking(_cameraCameraForm->orthographicCameraRadioButton)->setChecked(true);
    if(camera->isPerspective()) whileBlocking(_cameraCameraForm->perspectiveCameraRadioButton)->setChecked(true);
  }
}

void CameraTreeWidgetController::reloadCameraProperties()
{
  if(std::shared_ptr<RKCamera> camera = _camera.lock())
  {
    switch(camera->resetCameraDirection())
    {
      case ResetDirectionType::minus_X:
        whileBlocking(_cameraCameraForm->resetCameraToMinusXRadioButton)->setChecked(true);
        break;
      case ResetDirectionType::minus_Y:
        whileBlocking(_cameraCameraForm->resetCameraToMinusYRadioButton)->setChecked(true);
        break;
      case ResetDirectionType::minus_Z:
        whileBlocking(_cameraCameraForm->resetCameraToMinusZRadioButton)->setChecked(true);
        break;
      case ResetDirectionType::plus_X:
        whileBlocking(_cameraCameraForm->resetCameraToPlusXRadioButton)->setChecked(true);
        break;
      case ResetDirectionType::plus_Y:
        whileBlocking(_cameraCameraForm->resetCameraToPlusYRadioButton)->setChecked(true);
        break;
      case ResetDirectionType::plus_Z:
        whileBlocking(_cameraCameraForm->resetCameraToPlusZRadioButton)->setChecked(true);
        break;
     }

     whileBlocking(_cameraCameraForm->angleOfViewSpinBox)->setValue(camera->angleOfView()*180.0/M_PI);

    double3 centerOfScene = camera->centerOfScene();
    whileBlocking(_cameraCameraForm->centerOfSceneXLineEdit)->setText(QString::number(centerOfScene.x, 'f', 5));
    whileBlocking(_cameraCameraForm->centerOfSceneYLineEdit)->setText(QString::number(centerOfScene.y, 'f', 5));
    whileBlocking(_cameraCameraForm->centerOfSceneZLineEdit)->setText(QString::number(centerOfScene.z, 'f', 5));

    double rotationAngle = camera->rotationAngle();
    whileBlocking(_cameraCameraForm->rotateAngleLineEdit)->setText(QString::number(rotationAngle, 'f', 2));
    whileBlocking(_cameraCameraForm->rotatePlusXPushButton)->setText(QString("Rotate +") + QString::number(rotationAngle, 'f', 2));
    whileBlocking(_cameraCameraForm->rotatePlusYPushButton)->setText(QString("Rotate +") + QString::number(rotationAngle, 'f', 2));
    whileBlocking(_cameraCameraForm->rotatePlusZPushButton)->setText(QString("Rotate +") + QString::number(rotationAngle, 'f', 2));
    whileBlocking(_cameraCameraForm->rotateMinusXPushButton)->setText(QString("Rotate ") + QString::number(-rotationAngle, 'f', 2));
    whileBlocking(_cameraCameraForm->rotateMinusYPushButton)->setText(QString("Rotate ") + QString::number(-rotationAngle, 'f', 2));
    whileBlocking(_cameraCameraForm->rotateMinusZPushButton)->setText(QString("Rotate ") + QString::number(-rotationAngle, 'f', 2));
  }
}

void CameraTreeWidgetController::reloadCameraEulerAngles()
{
  if(std::shared_ptr<RKCamera> camera = _camera.lock())
  {
    double3 EulerAngles = camera->EulerAngles();
    whileBlocking(_cameraCameraForm->EulerAngleXDial)->setMinimum(-180);
    whileBlocking(_cameraCameraForm->EulerAngleXDial)->setMaximum(180);
    whileBlocking(_cameraCameraForm->EulerAngleXDial)->setValue(int(EulerAngles.x*180.0/M_PI));
    whileBlocking(_cameraCameraForm->EulerAngleZDial)->setMinimum(-180);
    whileBlocking(_cameraCameraForm->EulerAngleZDial)->setMaximum(180);
    whileBlocking(_cameraCameraForm->EulerAngleZDial)->setValue(int(EulerAngles.z*180.0/M_PI));
    whileBlocking(_cameraCameraForm->EulerAngleYSlider)->setMinimum(-90);
    whileBlocking(_cameraCameraForm->EulerAngleYSlider)->setMaximum(90);
    whileBlocking(_cameraCameraForm->EulerAngleYSlider)->setValue(int(EulerAngles.y*180.0/M_PI));

    whileBlocking(_cameraCameraForm->EulerAngleXLineEdit)->setText(QString::number(EulerAngles.x*180.0/M_PI, 'f', 5));
    whileBlocking(_cameraCameraForm->EulerAngleZLineEdit)->setText(QString::number(EulerAngles.z*180.0/M_PI, 'f', 5));
    whileBlocking(_cameraCameraForm->EulerAngleYLineEdit)->setText(QString::number(EulerAngles.y*180.0/M_PI, 'f', 5));
  }
}

void CameraTreeWidgetController::reloadCameraModelViewMatrix()
{
  if(std::shared_ptr<RKCamera> camera = _camera.lock())
  {
    double4x4 viewMatrix = camera->modelViewMatrix();
    whileBlocking(_cameraCameraForm->viewMatrixM11)->setText(QString::number(viewMatrix.m11, 'f', 5));
    whileBlocking(_cameraCameraForm->viewMatrixM12)->setText(QString::number(viewMatrix.m12, 'f', 5));
    whileBlocking(_cameraCameraForm->viewMatrixM13)->setText(QString::number(viewMatrix.m13, 'f', 5));
    whileBlocking(_cameraCameraForm->viewMatrixM14)->setText(QString::number(viewMatrix.m14, 'f', 5));

    whileBlocking(_cameraCameraForm->viewMatrixM21)->setText(QString::number(viewMatrix.m21, 'f', 5));
    whileBlocking(_cameraCameraForm->viewMatrixM22)->setText(QString::number(viewMatrix.m22, 'f', 5));
    whileBlocking(_cameraCameraForm->viewMatrixM23)->setText(QString::number(viewMatrix.m23, 'f', 5));
    whileBlocking(_cameraCameraForm->viewMatrixM24)->setText(QString::number(viewMatrix.m24, 'f', 5));

    whileBlocking(_cameraCameraForm->viewMatrixM31)->setText(QString::number(viewMatrix.m31, 'f', 5));
    whileBlocking(_cameraCameraForm->viewMatrixM32)->setText(QString::number(viewMatrix.m32, 'f', 5));
    whileBlocking(_cameraCameraForm->viewMatrixM33)->setText(QString::number(viewMatrix.m33, 'f', 5));
    whileBlocking(_cameraCameraForm->viewMatrixM34)->setText(QString::number(viewMatrix.m34, 'f', 5));

    whileBlocking(_cameraCameraForm->viewMatrixM41)->setText(QString::number(viewMatrix.m41, 'f', 5));
    whileBlocking(_cameraCameraForm->viewMatrixM42)->setText(QString::number(viewMatrix.m42, 'f', 5));
    whileBlocking(_cameraCameraForm->viewMatrixM43)->setText(QString::number(viewMatrix.m43, 'f', 5));
    whileBlocking(_cameraCameraForm->viewMatrixM44)->setText(QString::number(viewMatrix.m44, 'f', 5));

    double3 cameraPosition = camera->position();
    whileBlocking(_cameraCameraForm->cameraPositionXLineEdit)->setText(QString::number(cameraPosition.x, 'f', 5));
    whileBlocking(_cameraCameraForm->cameraPositionYLineEdit)->setText(QString::number(cameraPosition.y, 'f', 5));
    whileBlocking(_cameraCameraForm->cameraPositionZLineEdit)->setText(QString::number(cameraPosition.z, 'f', 5));
    whileBlocking(_cameraCameraForm->cameraDistanceLineEdit)->setText(QString::number(cameraPosition.length(), 'f', 5));
  }
}

void CameraTreeWidgetController::reloadCameraData()
{
  reloadCameraProperties();
  reloadCameraProjection();
  reloadCameraResetDirection();
  reloadCameraEulerAngles();
  reloadCameraModelViewMatrix();

  reloadSelectionProperties();
  reloadLightsProperties();
  reloadPictureProperties();
  reloadBackgroundProperties();
}





void CameraTreeWidgetController::setResetPercentage(const QString &resetPercentageString)
{
  if(std::shared_ptr<RKCamera> camera = _camera.lock())
  {
    bool succes = false;
    double newValue = resetPercentageString.toDouble(&succes);

    if(succes)
    {
      camera->setResetFraction(newValue/100.0);
    }
  }
}


void CameraTreeWidgetController::resetCameraToDefaultDirection()
{
  if(std::shared_ptr<RKCamera> camera = _camera.lock())
  {
    camera->resetCameraToDirection();
    reloadCameraEulerAngles();
    reloadCameraModelViewMatrix();
    emit updateRenderer();
  }
}


void CameraTreeWidgetController::setResetDirectionToMinusX(bool checked)
{
  if(std::shared_ptr<RKCamera> camera = _camera.lock())
  {
    if(checked)
    {
      camera->setResetDirectionType(ResetDirectionType::minus_X);
      emit updateRenderer();
    }
  }
}

void CameraTreeWidgetController::setResetDirectionToPlusX(bool checked)
{
  if(std::shared_ptr<RKCamera> camera = _camera.lock())
  {
    if(checked)
    {
      camera->setResetDirectionType(ResetDirectionType::plus_X);
      emit updateRenderer();
    }
  }
}

void CameraTreeWidgetController::setResetDirectionToMinusY(bool checked)
{
  if(std::shared_ptr<RKCamera> camera = _camera.lock())
  {
    if(checked)
    {
      camera->setResetDirectionType(ResetDirectionType::minus_Y);
      emit updateRenderer();
    }
  }
}

void CameraTreeWidgetController::setResetDirectionToPlusY(bool checked)
{
  if(std::shared_ptr<RKCamera> camera = _camera.lock())
  {
    if(checked)
    {
      camera->setResetDirectionType(ResetDirectionType::plus_Y);
      emit updateRenderer();
    }
  }
}

void CameraTreeWidgetController::setResetDirectionToMinusZ(bool checked)
{
  if(std::shared_ptr<RKCamera> camera = _camera.lock())
  {
    if(checked)
    {
      camera->setResetDirectionType(ResetDirectionType::minus_Z);
      emit updateRenderer();
    }
  }
}

void CameraTreeWidgetController::setResetDirectionToPlusZ(bool checked)
{
  if(std::shared_ptr<RKCamera> camera = _camera.lock())
  {
    if(checked)
    {
      camera->setResetDirectionType(ResetDirectionType::plus_Z);
      emit updateRenderer();
    }
  }
}

void CameraTreeWidgetController::setCameraToOrthographic(bool checked)
{
  if(std::shared_ptr<RKCamera> camera = _camera.lock())
  {
    if(checked)
    {
      camera->setCameraToOrthographic();
    }
    else
    {
      camera->setCameraToPerspective();
    }
    emit updateRenderer();
  }
}

void CameraTreeWidgetController::setCameraToPerspective(bool checked)
{
  if(std::shared_ptr<RKCamera> camera = _camera.lock())
  {
    if(checked)
    {
      camera->setCameraToPerspective();
    }
    else
    {
      camera->setCameraToOrthographic();
    }
    emit updateRenderer();
  }
}

void CameraTreeWidgetController::setAngleOfView(double d)
{
  if(std::shared_ptr<RKCamera> camera = _camera.lock())
  {
    camera->setAngleOfView(d * M_PI/180.0);
    emit updateRenderer();
    emit reloadCameraModelViewMatrix();
  }
}

void CameraTreeWidgetController::setRotationAngle(const QString &angleString)
{
  if(std::shared_ptr<RKCamera> camera = _camera.lock())
  {
    bool succes = false;
    double newValue = angleString.toDouble(&succes);

    if(succes)
    {
      camera->setRotationAngle(newValue);
      whileBlocking(_cameraCameraForm->rotatePlusXPushButton)->setText(QString("Rotate +") + angleString);
      whileBlocking(_cameraCameraForm->rotatePlusYPushButton)->setText(QString("Rotate +") + angleString);
      whileBlocking(_cameraCameraForm->rotatePlusZPushButton)->setText(QString("Rotate +") + angleString);
      whileBlocking(_cameraCameraForm->rotateMinusXPushButton)->setText(QString("Rotate ") + angleString);
      whileBlocking(_cameraCameraForm->rotateMinusYPushButton)->setText(QString("Rotate ") + angleString);
      whileBlocking(_cameraCameraForm->rotateMinusZPushButton)->setText(QString("Rotate ") + angleString);
    }
  }
}

void CameraTreeWidgetController::rotateYawPlus()
{
  if(std::shared_ptr<RKCamera> camera = _camera.lock())
  {
    simd_quatd rotation = camera->worldRotation();
    simd_quatd dq = simd_quatd:: yaw(camera->rotationAngle());

    camera->setWorldRotation(rotation * dq);
    emit updateRenderer();
    reloadCameraEulerAngles();
    reloadCameraModelViewMatrix();
  }
}

void CameraTreeWidgetController::rotateYawMinus()
{
  if(std::shared_ptr<RKCamera> camera = _camera.lock())
  {
    simd_quatd rotation = camera->worldRotation();
    simd_quatd dq = simd_quatd:: yaw(-camera->rotationAngle());

    camera->setWorldRotation(rotation * dq);
    emit updateRenderer();
    reloadCameraEulerAngles();
    reloadCameraModelViewMatrix();
  }
}

void CameraTreeWidgetController::rotatePitchPlus()
{
  if(std::shared_ptr<RKCamera> camera = _camera.lock())
  {
    simd_quatd rotation = camera->worldRotation();
    simd_quatd dq = simd_quatd:: pitch(camera->rotationAngle());

    camera->setWorldRotation(rotation * dq);
    emit updateRenderer();
    reloadCameraEulerAngles();
    reloadCameraModelViewMatrix();
  }
}

void CameraTreeWidgetController::rotatePitchMinus()
{
  if(std::shared_ptr<RKCamera> camera = _camera.lock())
  {
    simd_quatd rotation = camera->worldRotation();
    simd_quatd dq = simd_quatd:: pitch(-camera->rotationAngle());

    camera->setWorldRotation(rotation * dq);
    emit updateRenderer();
    reloadCameraEulerAngles();
    reloadCameraModelViewMatrix();
  }
}

void CameraTreeWidgetController::rotateRollPlus()
{
  if(std::shared_ptr<RKCamera> camera = _camera.lock())
  {
    simd_quatd rotation = camera->worldRotation();
    simd_quatd dq = simd_quatd:: roll(camera->rotationAngle());

    camera->setWorldRotation(rotation * dq);
    emit updateRenderer();
    reloadCameraEulerAngles();
    reloadCameraModelViewMatrix();
  }
}

void CameraTreeWidgetController::rotateRollMinus()
{
  if(std::shared_ptr<RKCamera> camera = _camera.lock())
  {
    simd_quatd rotation = camera->worldRotation();
    simd_quatd dq = simd_quatd:: roll(-camera->rotationAngle());

    camera->setWorldRotation(rotation * dq);
    emit updateRenderer();
    reloadCameraEulerAngles();
    reloadCameraModelViewMatrix();
  }
}


void CameraTreeWidgetController::setEulerAngleX(int angle)
{
  if(std::shared_ptr<RKCamera> camera = _camera.lock())
  {
    double3 EulerAngles  = camera->EulerAngles();
    double newValue = double(angle*M_PI/180.0);

    simd_quatd worldRotation = simd_quatd(double3(newValue, EulerAngles.y, EulerAngles.z));
    camera->setWorldRotation(worldRotation);
    emit updateRenderer();
    reloadCameraEulerAngles();
    reloadCameraModelViewMatrix();
  }
}

void CameraTreeWidgetController::setEulerAngleX(const QString &angleString)
{
  if(std::shared_ptr<RKCamera> camera = _camera.lock())
  {
    bool succes = false;
    double newValue = angleString.toDouble(&succes)*M_PI/180.0;

    if(succes)
    {
      double3 EulerAngles  = camera->EulerAngles();

      simd_quatd worldRotation = simd_quatd(double3(newValue, EulerAngles.y, EulerAngles.z));
      camera->setWorldRotation(worldRotation);
      emit updateRenderer();
      reloadCameraEulerAngles();
      reloadCameraModelViewMatrix();
    }
  }
}

void CameraTreeWidgetController::setEulerAngleZ(int angle)
{
  if(std::shared_ptr<RKCamera> camera = _camera.lock())
  {
    double3 EulerAngles  = camera->EulerAngles();
    double newValue = double(angle*M_PI/180.0);

    simd_quatd worldRotation = simd_quatd(double3(EulerAngles.x, EulerAngles.y, newValue));
    camera->setWorldRotation(worldRotation);
    emit updateRenderer();
    reloadCameraEulerAngles();
    reloadCameraModelViewMatrix();
  }
}

void CameraTreeWidgetController::setEulerAngleZ(const QString &angleString)
{
  if(std::shared_ptr<RKCamera> camera = _camera.lock())
  {
    bool succes = false;
    double newValue = angleString.toDouble(&succes)*M_PI/180.0;

    if(succes)
    {
      double3 EulerAngles  = camera->EulerAngles();

      simd_quatd worldRotation = simd_quatd(double3(EulerAngles.x, EulerAngles.y, newValue));
      camera->setWorldRotation(worldRotation);
      emit updateRenderer();
      reloadCameraEulerAngles();
      reloadCameraModelViewMatrix();
    }
  }
}

void CameraTreeWidgetController::setEulerAngleY(int angle)
{
  if(std::shared_ptr<RKCamera> camera = _camera.lock())
  {
    double3 EulerAngles  = camera->EulerAngles();
    double newValue = double(angle*M_PI/180.0);

    simd_quatd worldRotation = simd_quatd(double3(EulerAngles.x, newValue, EulerAngles.z));
    camera->setWorldRotation(worldRotation);
    emit updateRenderer();
    reloadCameraEulerAngles();
    reloadCameraModelViewMatrix();
  }
}

void CameraTreeWidgetController::setEulerAngleY(const QString &angleString)
{
  if(std::shared_ptr<RKCamera> camera = _camera.lock())
  {
    bool succes = false;
    double newValue = angleString.toDouble(&succes)*M_PI/180.0;

    if(succes)
    {
      double3 EulerAngles  = camera->EulerAngles();

      simd_quatd worldRotation = simd_quatd(double3(EulerAngles.x, newValue, EulerAngles.z));
      camera->setWorldRotation(worldRotation);
      emit updateRenderer();
      reloadCameraEulerAngles();
      reloadCameraModelViewMatrix();
    }
  }
}

// Selection properties
//========================================================================================================================================

void CameraTreeWidgetController::reloadSelectionProperties()
{
  if(std::shared_ptr<RKCamera> camera = _camera.lock())
  {
    whileBlocking(_cameraSelectionForm->selectionIntensityDoubleSpinBox)->setValue(camera->bloomLevel());
    whileBlocking(_cameraSelectionForm->selectionIntensityDoubleSlider)->setDoubleValue(camera->bloomLevel());
  }
}

void CameraTreeWidgetController::setSelectionIntensity(double level)
{
  if (std::shared_ptr<RKCamera> camera = _camera.lock())
  {
    camera->setBloomLevel(level);
    whileBlocking(_cameraSelectionForm->selectionIntensityDoubleSpinBox)->setValue(level);
    whileBlocking(_cameraSelectionForm->selectionIntensityDoubleSlider)->setDoubleValue(level);
  }
  emit rendererReloadSelectionData();
  emit updateRenderer();
}

// Light properties
//========================================================================================================================================

void CameraTreeWidgetController::reloadLightsProperties()
{
  if(_project)
  {
    std::cout << "TEST: " << _project->renderLights().front()->ambientIntensity() << std::endl;
    whileBlocking(_cameraLightsForm->ambientLightIntensityDoubleSpinBox)->setValue(_project->renderLights().front()->ambientIntensity());
    whileBlocking(_cameraLightsForm->ambientLightIntensityDoubleSlider)->setDoubleValue(_project->renderLights().front()->ambientIntensity());
    whileBlocking(_cameraLightsForm->ambientLightPushButton)->setColor(_project->renderLights().front()->ambientColor());

    whileBlocking(_cameraLightsForm->diffuseLightIntensityDoubleSpinBox)->setValue(_project->renderLights().front()->diffuseIntensity());
    whileBlocking(_cameraLightsForm->diffuseLightIntensityDoubleSlider)->setDoubleValue(_project->renderLights().front()->diffuseIntensity());
    whileBlocking(_cameraLightsForm->diffuseLightPushButton)->setColor(_project->renderLights().front()->diffuseColor());

    whileBlocking(_cameraLightsForm->specularLightIntensityDoubleSpinBox)->setValue(_project->renderLights().front()->specularIntensity());
    whileBlocking(_cameraLightsForm->specularLightIntensityDoubleSlider)->setDoubleValue(_project->renderLights().front()->specularIntensity());
    whileBlocking(_cameraLightsForm->specularLightPushButton)->setColor(_project->renderLights().front()->specularColor());

  }
}

void CameraTreeWidgetController::setAmbientLightIntensity(double intensity)
{
  if (_project)
  {
    _project->renderLights().front()->setAmbientIntensity(intensity);
    whileBlocking(_cameraLightsForm->ambientLightIntensityDoubleSpinBox)->setValue(intensity);
    whileBlocking(_cameraLightsForm->ambientLightIntensityDoubleSlider)->setDoubleValue(intensity);
    emit updateRenderer();
  }
}

void CameraTreeWidgetController::setDiffuseLightIntensity(double intensity)
{
  if (_project)
  {
    _project->renderLights().front()->setDiffuseIntensity(intensity);
    whileBlocking(_cameraLightsForm->diffuseLightIntensityDoubleSpinBox)->setValue(intensity);
    whileBlocking(_cameraLightsForm->diffuseLightIntensityDoubleSlider)->setDoubleValue(intensity);
    emit updateRenderer();
  }
}

void CameraTreeWidgetController::setSpecularLightIntensity(double intensity)
{
  if (_project)
  {
    _project->renderLights().front()->setSpecularIntensity(intensity);
    whileBlocking(_cameraLightsForm->specularLightIntensityDoubleSpinBox)->setValue(intensity);
    whileBlocking(_cameraLightsForm->specularLightIntensityDoubleSlider)->setDoubleValue(intensity);
    emit updateRenderer();
  }
}

void CameraTreeWidgetController::setAmbientColor()
{
  if (_project)
  {
    QColor color = QColorDialog::getColor(Qt::white,this,"Choose Color");
    if(color.isValid())
    {
      _project->renderLights().front()->setAmbientColor(color);
      whileBlocking(_cameraLightsForm->ambientLightPushButton)->setColor(color);
      emit updateRenderer();
    }
  }
}

void CameraTreeWidgetController::setDiffuseColor()
{
  if (_project)
  {
    QColor color = QColorDialog::getColor(Qt::white,this,"Choose Color");
    if(color.isValid())
    {
      _project->renderLights().front()->setDiffuseColor(color);
      whileBlocking(_cameraLightsForm->diffuseLightPushButton)->setColor(color);
      emit updateRenderer();
    }
  }
}

void CameraTreeWidgetController::setSpecularColor()
{
  if (_project)
  {
    QColor color = QColorDialog::getColor(Qt::white,this,"Choose Color");
    if(color.isValid())
    {
      _project->renderLights().front()->setSpecularColor(color);
      whileBlocking(_cameraLightsForm->specularLightPushButton)->setColor(color);
      emit updateRenderer();
    }
  }
}

// Picture/movie properties
//========================================================================================================================================

void CameraTreeWidgetController::reloadPictureProperties()
{
  if(_project)
  {
    switch(_project->imageUnits())
    {
    case RKImageUnits::inch:
      whileBlocking(_cameraPicturesForm->inchRadioButton)->setChecked(true);
      whileBlocking(_cameraPicturesForm->physicalWidthDoubleSpinBox)->setValue(double(_project->renderImagePhysicalSizeInInches()));
      whileBlocking(_cameraPicturesForm->physicalHeightDoubleSpinBox)->setValue(double(_project->renderImagePhysicalSizeInInches())/(_project->imageAspectRatio()));
      break;
    case RKImageUnits::cm:
      whileBlocking(_cameraPicturesForm->cmRadioButton)->setChecked(true);
      whileBlocking(_cameraPicturesForm->physicalWidthDoubleSpinBox)->setValue(double(2.54 * _project->renderImagePhysicalSizeInInches()));
      whileBlocking(_cameraPicturesForm->physicalHeightDoubleSpinBox)->setValue(double(2.54 * _project->renderImagePhysicalSizeInInches())/(_project->imageAspectRatio()));
      break;
    }

    switch(_project->imageDimensions())
    {
    case RKImageDimensions::physical:
      whileBlocking(_cameraPicturesForm->physicalDimensionsRadioButton)->setChecked(true);
      whileBlocking(_cameraPicturesForm->physicalWidthDoubleSpinBox)->setEnabled(true);
      whileBlocking(_cameraPicturesForm->pixelWidthSpinBox)->setEnabled(false);
      break;
    case RKImageDimensions::pixels:
      whileBlocking(_cameraPicturesForm->pixelDimensionsRadioButton)->setChecked(true);
      whileBlocking(_cameraPicturesForm->pixelWidthSpinBox)->setEnabled(true);
      whileBlocking(_cameraPicturesForm->physicalWidthDoubleSpinBox)->setEnabled(false);
      break;
    }

    whileBlocking( _cameraPicturesForm->dotsPerInchComboBox)->setCurrentIndex(int(_project->imageDPI()));
    whileBlocking(_cameraPicturesForm->qualityComboBox)->setCurrentIndex(int(_project->renderImageQuality()));

    whileBlocking(_cameraPicturesForm->pixelWidthSpinBox)->setValue(_project->renderImageNumberOfPixels());
    whileBlocking(_cameraPicturesForm->pixelHeightSpinBox)->setValue(_project->renderImageNumberOfPixels() / _project->imageAspectRatio());


    switch(_project->imageUnits())
    {
    case RKImageUnits::inch:
      whileBlocking(_cameraPicturesForm->inchRadioButton)->setChecked(true);
      break;
    case RKImageUnits::cm:
      whileBlocking(_cameraPicturesForm->cmRadioButton)->setChecked(true);
      break;
    }

    whileBlocking(_cameraPicturesForm->framesPerSecondSpinBox)->setValue(_project->movieFramesPerSecond());
  }
}

void CameraTreeWidgetController::setPictureDotsPerInch(int value)
{
  if(_project)
  {
    _project->setImageDPI(RKImageDPI(value));

    switch(_project->imageDimensions())
    {
    case RKImageDimensions::physical:
      _project->setImagePhysicalSizeInInches(_project->renderImageNumberOfPixels() / _project->imageDotsPerInchValue());
      break;
    case RKImageDimensions::pixels:
       //_project->setImageNumberOfPixels(int(rint(_project->imageDotsPerInchValue() * _project->renderImagePhysicalSizeInInches())));
        _project->setImagePhysicalSizeInInches(_project->renderImageNumberOfPixels() / _project->imageDotsPerInchValue());
      break;
    }

    reloadPictureProperties();
  }
}


void CameraTreeWidgetController::setPictureDimensionToPhysical(bool checked)
{
   if(_project && checked)
   {
     std::cout << "check: " << _project.get() <<std::endl;
     _project->setImageDimensions(RKImageDimensions::physical);
     this->reloadPictureProperties();
   }
}

void CameraTreeWidgetController::setPictureDimensionToPixels(bool checked)
{
   if(_project && checked)
   {
     _project->setImageDimensions(RKImageDimensions::pixels);
     this->reloadPictureProperties();
   }
}

void CameraTreeWidgetController::setPictureUnitsToInch(bool checked)
{
  if(_project && checked)
  {
    _project->setImageUnits(RKImageUnits::inch);
    this->reloadPictureProperties();
  }
}

void CameraTreeWidgetController::setPictureUnitsToCentimeters(bool checked)
{
  if(_project && checked)
  {
    _project->setImageUnits(RKImageUnits::cm);
    this->reloadPictureProperties();
  }
}

void CameraTreeWidgetController::setPictureQuality(int value)
{
  if(_project)
  {
    _project->setImageQuality(RKImageQuality(value));
  }
}

void CameraTreeWidgetController::setPictureNumberOfPixels(int width)
{
  if(_project)
  {
    _project->setImageNumberOfPixels(width);

    _project->setImagePhysicalSizeInInches(_project->renderImageNumberOfPixels() / _project->imageDotsPerInchValue());
    reloadPictureProperties();
  }
}

void CameraTreeWidgetController::setPicturePhysicalSize(double width)
{
  if(_project)
  {
    switch(_project->imageUnits())
    {
    case RKImageUnits::inch:
      _project->setImagePhysicalSizeInInches(width);
      break;
    case RKImageUnits::cm:
      _project->setImagePhysicalSizeInInches(width/2.54);
      break;
    }

    _project->setImageNumberOfPixels(int(rint(_project->imageDotsPerInchValue() * _project->renderImagePhysicalSizeInInches())));


    reloadPictureProperties();
  }
}

void CameraTreeWidgetController::setPictureEditDimensions(int dimensions)
{
  if(_project)
  {
    _project->setImageDimensions(RKImageDimensions(dimensions));
    reloadPictureProperties();
  }
}

void CameraTreeWidgetController::setPictureEditUnits(int units)
{
  if(_project)
  {
    _project->setImageUnits(RKImageUnits(units));
    reloadPictureProperties();
  }
}

void CameraTreeWidgetController::setMovieFramesPerSecond(int fps)
{
  if(_project)
  {
    _project->setMovieFramesPerSecond(fps);
  }
}

void CameraTreeWidgetController::savePicture()
{
  QDir docsDir(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
  QUrl pName = QUrl::fromLocalFile(docsDir.filePath("picture.png"));
  QUrl fileURL = QFileDialog::getSaveFileUrl(this,tr("Save File"), pName,
        tr("Images (*.png *.xpm *.jpg *.tiff)"));

  if(!fileURL.isValid())
  {
    return;
  }
  else
  {
    if(_project)
    {
      int width = _project->renderImageNumberOfPixels();
      int height = _project->renderImageNumberOfPixels() / _project->imageAspectRatio();
      emit rendererCreatePicture(fileURL, width, height);
    }
  }
}

void CameraTreeWidgetController::saveMovie()
{

}

// Background properties
//========================================================================================================================================

void CameraTreeWidgetController::reloadBackgroundProperties()
{
  if(_project)
  {
    QString fileName = _project->backgroundImageFilename();


    if(fileName.isEmpty())
    {
      whileBlocking(_cameraBackgroundForm->selectImagePushButton)->setText("Select picture");
      whileBlocking(_cameraBackgroundForm->imagePlaceholderQLabel)->setText("Image placeholder");
    }
    else
    {
      QPixmap pixmap = QPixmap::fromImage(_project->renderBackgroundCachedImage(),Qt::AutoColor);
      QString baseFileName = QFileInfo(fileName).fileName();

      whileBlocking(_cameraBackgroundForm->selectImagePushButton)->setText(baseFileName);
      whileBlocking(_cameraBackgroundForm->imagePlaceholderQLabel)->setPixmap(pixmap);
    }

    whileBlocking(_cameraBackgroundForm->linearGradientAngleDoubleSlider)->setDoubleValue(_project->linearGradientAngle());
    whileBlocking(_cameraBackgroundForm->linearGradientAngleDoubleSpinBox)->setValue(_project->linearGradientAngle());

    whileBlocking(_cameraBackgroundForm->linearGradientFromColorPushButton)->setColor(_project->linearGradientFromColor());
    whileBlocking(_cameraBackgroundForm->linearGradientToColorPushButton)->setColor(_project->linearGradientToColor());

    whileBlocking(_cameraBackgroundForm->radialGradientRoundnessDoubleSlider)->setDoubleValue(_project->radialGradientRoundness());
    whileBlocking(_cameraBackgroundForm->radialGradientRoundnessDoubleSpinBox)->setValue(_project->radialGradientRoundness());

    whileBlocking(_cameraBackgroundForm->radialGradientFromColorPushButton)->setColor(_project->radialGradientFromColor());
    whileBlocking(_cameraBackgroundForm->radialGradientToColorPushButton)->setColor(_project->radialGradientToColor());

  }
}

void CameraTreeWidgetController::setBackgroundColor()
{
  QColor color = QColorDialog::getColor(Qt::white,this,"Choose Color");
  if(color.isValid())
  {
    if (_project)
    {

      _project->setBackgroundColor(color);
      std::cout << "Drawbackground image!!!" << std::endl;
    }
    emit rendererReloadBackgroundImage();
    emit updateRenderer();
  }
}

std::optional<QColor> CameraTreeWidgetController::backgroundColor()
{
  return std::nullopt;
}

void CameraTreeWidgetController::setLinearGradientFromColor()
{
  QColor color = QColorDialog::getColor(Qt::white,this,"Choose Color");
  if(color.isValid())
  {
    if (_project)
    {
      _project->setLinearGradientFromColor(color);
      whileBlocking(_cameraBackgroundForm->linearGradientFromColorPushButton)->setColor(color);
      std::cout << "Drawbackground image!!!" << std::endl;
    }
    emit rendererReloadBackgroundImage();
    emit updateRenderer();
  }
}

void CameraTreeWidgetController::setLinearGradientToColor()
{
  QColor color = QColorDialog::getColor(Qt::white,this,"Choose Color");
  if(color.isValid())
  {
    if (_project)
    {
      _project->setLinearGradientToColor(color);
      whileBlocking(_cameraBackgroundForm->linearGradientToColorPushButton)->setColor(color);
      std::cout << "Drawbackground image!!!" << std::endl;
    }
    emit rendererReloadBackgroundImage();
    emit updateRenderer();
  }
}

void CameraTreeWidgetController::setLinearGradientAngle(double angle)
{
  if (_project)
  {
    _project->setLinearGradientAngle(angle);
    whileBlocking(_cameraBackgroundForm->linearGradientAngleDoubleSlider)->setDoubleValue(angle);
    whileBlocking(_cameraBackgroundForm->linearGradientAngleDoubleSpinBox)->setValue(angle);
    std::cout << "Drawbackground image!!!" << std::endl;
  }
  emit rendererReloadBackgroundImage();
  emit updateRenderer();
}

void CameraTreeWidgetController::setRadialGradientFromColor()
{
  QColor color = QColorDialog::getColor(Qt::white,this,"Choose Color");
  if(color.isValid())
  {
    if (_project)
    {
      _project->setRadialGradientFromColor(color);
      whileBlocking(_cameraBackgroundForm->radialGradientFromColorPushButton)->setColor(color);
      std::cout << "Drawbackground image!!!" << std::endl;
    }
    emit rendererReloadBackgroundImage();
    emit updateRenderer();
  }
}

void CameraTreeWidgetController::setRadialGradientToColor()
{
  QColor color = QColorDialog::getColor(Qt::white,this,"Choose Color");
  if(color.isValid())
  {
    if (_project)
    {
      _project->setRadialGradientToColor(color);
      whileBlocking(_cameraBackgroundForm->radialGradientToColorPushButton)->setColor(color);
      std::cout << "Drawbackground image!!!" << std::endl;
    }
    emit rendererReloadBackgroundImage();
    emit updateRenderer();
  }
}

void CameraTreeWidgetController::setRadialGradientRoundness(double roundness)
{
  if (_project)
  {
    _project->setRadialGradientRoundness(roundness);
    whileBlocking(_cameraBackgroundForm->radialGradientRoundnessDoubleSlider)->setDoubleValue(roundness);
    whileBlocking(_cameraBackgroundForm->radialGradientRoundnessDoubleSpinBox)->setValue(roundness);
    std::cout << "Drawbackground image!!!" << std::endl;
  }
  emit rendererReloadBackgroundImage();
  emit updateRenderer();
}

void CameraTreeWidgetController::selectBackgroundImage()
{
  QUrl url = QFileDialog::getOpenFileUrl(this, QDir::homePath(),
             tr("Open background picture"),
             tr("png or jpg files (*.png *.jpg)"));


  if(!url.isValid())
  {
    whileBlocking(_cameraBackgroundForm->selectImagePushButton)->setText("Select picture");
    whileBlocking(_cameraBackgroundForm->imagePlaceholderQLabel)->setText("Image placeholder");
    return;
  }
  else
  {
    _project->loadBackgroundImage(url.path());


    QPixmap pixmap = QPixmap::fromImage(_project->renderBackgroundCachedImage(),Qt::AutoColor);
    QString baseFileName = QFileInfo(url.fileName()).fileName();

    whileBlocking(_cameraBackgroundForm->selectImagePushButton)->setText(baseFileName);
    whileBlocking(_cameraBackgroundForm->imagePlaceholderQLabel)->setPixmap(pixmap);
    emit rendererReloadBackgroundImage();
    emit updateRenderer();
  }
}

void CameraTreeWidgetController::setBackgroundToColor(bool checked)
{
  if(_project && checked)
  {
    _cameraBackgroundForm->stackedWidget->setCurrentIndex(0);
    _project->setBackgroundType(RKBackgroundType::color);
    emit rendererReloadBackgroundImage();
    emit updateRenderer();
  }
}

void CameraTreeWidgetController::setBackgroundToLinearGradient(bool checked)
{
  if(_project && checked)
  {
    _cameraBackgroundForm->stackedWidget->setCurrentIndex(1);
    _project->setBackgroundType(RKBackgroundType::linearGradient);
    emit rendererReloadBackgroundImage();
    emit updateRenderer();
  }
}

void CameraTreeWidgetController::setBackgroundToRadialGradient(bool checked)
{
  if(_project && checked)
  {
    _cameraBackgroundForm->stackedWidget->setCurrentIndex(2);
    _project->setBackgroundType(RKBackgroundType::radialGradient);
    emit rendererReloadBackgroundImage();
    emit updateRenderer();
  }
}

void CameraTreeWidgetController::setBackgroundToImage(bool checked)
{
  if(_project && checked)
  {
    _cameraBackgroundForm->stackedWidget->setCurrentIndex(3);
    _project->setBackgroundType(RKBackgroundType::image);
    emit rendererReloadBackgroundImage();
    emit updateRenderer();
  }
}


