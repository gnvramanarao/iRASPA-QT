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

#include <QWidget>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QPushButton>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include <QUrl>
#include <optional>
#include <iraspakit.h>
#include "iraspamainwindowconsumerprotocol.h"
#include "cameracameraform.h"
#include "cameraselectionform.h"
#include "cameralightsform.h"
#include "camerapicturesform.h"
#include "camerabackgroundform.h"

class CameraTreeWidgetController : public QTreeWidget, public ProjectConsumer, public MainWindowConsumer, Reloadable
{
  Q_OBJECT

public:
  CameraTreeWidgetController(QWidget* parent = nullptr);
  void setProject(std::shared_ptr<ProjectTreeNode> projectTreeNode) override final;
  void setMainWindow(MainWindow *mainWindow) override final;

  void reloadData() override final;
  void reloadSelection() override final;
private:
  CameraCameraForm* _cameraCameraForm;
  CameraSelectionForm* _cameraSelectionForm;
  CameraLightsForm* _cameraLightsForm;
  CameraPicturesForm* _cameraPicturesForm;
  CameraBackgroundForm* _cameraBackgroundForm;

  QPushButton* pushButtonCamera;
  QPushButton* pushButtonSelection;
  QPushButton* pushButtonLights;
  QPushButton* pushButtonPictures;
  QPushButton* pushButtonBackground;


  MainWindow *_mainWindow;
  std::weak_ptr<RKCamera> _camera;
  std::shared_ptr<ProjectStructure> _project;
private slots:
  void expandCameraItem();
  void expandSelectionItem();
  void expandLightsItem();
  void expandPicturesItem();
  void expandBackgroundItem();

  void setResetPercentage(const QString &resetPercentageString);
  void resetCameraToDefaultDirection();
  void setResetDirectionToMinusX(bool checked = false);
  void setResetDirectionToPlusX(bool checked = false);
  void setResetDirectionToMinusY(bool checked = false);
  void setResetDirectionToPlusY(bool checked = false);
  void setResetDirectionToMinusZ(bool checked = false);
  void setResetDirectionToPlusZ(bool checked = false);
  void setCameraToOrthographic(bool checked = false);
  void setCameraToPerspective(bool checked = false);
  void setAngleOfView(double d);
  void setRotationAngle(const QString &angleString);
  void rotateYawPlus();
  void rotateYawMinus();
  void rotatePitchPlus();
  void rotatePitchMinus();
  void rotateRollPlus();
  void rotateRollMinus();
  void setEulerAngleX(int angle);
  void setEulerAngleX(const QString &angleString);
  void setEulerAngleZ(int angle);
  void setEulerAngleZ(const QString &angleString);
  void setEulerAngleY(int angle);
  void setEulerAngleY(const QString &angleString);

  void setSelectionIntensity(double level);

  void setAmbientLightIntensity(double intensity);
  void setDiffuseLightIntensity(double intensity);
  void setSpecularLightIntensity(double intensity);

  void setAmbientColor();
  void setDiffuseColor();
  void setSpecularColor();

  void setPictureDotsPerInch(int value);
  void setPictureQuality(int value);
  void setPictureNumberOfPixels(int width);
  void setPicturePhysicalSize(double width);
  void setPictureEditDimensions(int dimensions);
  void setPictureEditUnits(int units);
  void setMovieFramesPerSecond(int fps);
  void setPictureDimensionToPhysical(bool checked);
  void setPictureDimensionToPixels(bool checked);
  void setPictureUnitsToInch(bool checked);
  void setPictureUnitsToCentimeters(bool checked);
  void savePicture();
  void saveMovie();

  void setBackgroundColor();
  std::optional<QColor> backgroundColor();
  void setLinearGradientFromColor();
  void setLinearGradientToColor();
  void setLinearGradientAngle(double angle);
  void setRadialGradientFromColor();
  void setRadialGradientToColor();
  void setRadialGradientRoundness(double roundness);
  void selectBackgroundImage();

  void setBackgroundToColor(bool checked);
  void setBackgroundToLinearGradient(bool checked);
  void setBackgroundToRadialGradient(bool checked);
  void setBackgroundToImage(bool checked);
public slots:
  void reloadCameraData();
  void reloadCameraResetDirection();
  void reloadCameraProjection();
  void reloadCameraProperties();
  void reloadCameraEulerAngles();
  void reloadCameraModelViewMatrix();

  void reloadSelectionProperties();

  void reloadLightsProperties();
  void reloadPictureProperties();

  void reloadBackgroundProperties();

  void renderWidgetResize(QSize size);
signals:
  void rendererReloadData();
  void rendererReloadSelectionData();
  void rendererReloadBackgroundImage();
  void updateRenderer();
  void rendererCreatePicture(QUrl filenName, int width, int height);
  void rendererCreateMovie(QUrl filenName, int width, int height);
};


