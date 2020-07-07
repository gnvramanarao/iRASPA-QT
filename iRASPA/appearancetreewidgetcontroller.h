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
#include <QString>
#include <QColor>
#include <iraspakit.h>
#include "iraspamainwindowconsumerprotocol.h"
#include <simulationkit.h>
#include <optional>
#include "appearanceprimitiveform.h"
#include "appearanceatomsform.h"
#include "appearancebondsform.h"
#include "appearanceunitcellform.h"
#include "appearanceadsorptionsurfaceform.h"
#include "appearanceannotationform.h"

class AppearanceTreeWidgetController: public QTreeWidget, public MainWindowConsumer, public ProjectConsumer, public Reloadable
{
  Q_OBJECT

public:
  AppearanceTreeWidgetController(QWidget* parent = nullptr);
  void setProject(std::shared_ptr<ProjectTreeNode> projectTreeNode) override final;
  void setMainWindow(MainWindow *mainWindow) override final {_mainWindow = mainWindow;}
  void setFlattenedSelectedFrames(std::vector<std::shared_ptr<iRASPAStructure>> iraspa_structures);
private:
  AppearancePrimitiveForm* _appearancePrimitiveForm;
  AppearanceAtomsForm* _appearanceAtomsForm;
  AppearanceBondsForm* _appearanceBondsForm;
  AppearanceUnitCellForm* _appearanceUnitCellForm;
  AppearanceAdsorptionSurfaceForm* _appearanceAdsorptionSurfaceForm;
  AppearanceAnnotationForm* _appearanceAnnotationForm;

  QPushButton* pushButtonPrimitive;
  QPushButton* pushButtonAtoms;
  QPushButton* pushButtonBonds;
  QPushButton* pushButtonUnitCell;
  QPushButton* pushButtonAdsorptionSurface;
  QPushButton* pushButtonAnnotation;

  MainWindow *_mainWindow;
  std::shared_ptr<ProjectTreeNode> _projectTreeNode;
  std::shared_ptr<ProjectStructure> _projectStructure;
  std::vector<std::shared_ptr<iRASPAStructure>> _iraspa_structures{};
  void reloadData() override final;
  void reloadSelection() override final;

  void reloadPrimitiveProperties();
  void reloadAtomProperties();
  void reloadBondProperties();
  void reloadUnitCellProperties();
  void reloadAdsorptionSurfaceProperties();
  void reloadAnnotationProperties();

  // primitive properties
  void reloadRotationAngle();
  void reloadEulerAngles();
  void reloadTransformationMatrix();
  void reloadOpacity();
  void reloadNumberOfSides();
  void reloadIsCapped();
  void reloadFrontHDR();
  void reloadFrontExposure();
  void reloadFrontAmbientIntensity();
  void reloadFrontAmbientColor();
  void reloadFrontDiffuseIntensity();
  void reloadFrontDiffuseColor();
  void reloadFrontSpecularIntensity();
  void reloadFrontSpecularColor();
  void reloadFrontShininess();
  void reloadBackHDR();
  void reloadBackExposure();
  void reloadBackAmbientIntensity();
  void reloadBackAmbientColor();
  void reloadBackDiffuseIntensity();
  void reloadBackDiffuseColor();
  void reloadBackSpecularIntensity();
  void reloadBackSpecularColor();
  void reloadBackShininess();

  void setRatationAngle(double value);
  std::optional<double> rotationAngle();

  void setEulerAngleX(double value);
  std::optional<double> EulerAngleX();
  void setEulerAngleY(double value);
  std::optional<double> EulerAngleY();
  void setEulerAngleZ(double value);
  std::optional<double> EulerAngleZ();


  // atom properties

  void reloadAtomRepresentationType();
  void reloadAtomRepresentationStyle();
  void reloadAtomColorSet();
  void reloadAtomColorSetOrder();
  void reloadAtomForceFieldSet();
  void reloadAtomForceFieldSetOrder();
  void reloadAtomDrawAtoms();
  void reloadAtomSelectionStyle();
  void reloadAtomSelectionIntensity();
  void reloadAtomSelectionScaling();
  void reloadAtomSizeScalingDoubleSpinBox();
  void reloadAtomSizeScalingDoubleSlider();
  void reloadAtomHighDynamicRange();
  void reloadAtomHDRExposure();
  void reloadAtomHue();
  void reloadAtomSaturation();
  void reloadAtomValue();
  void reloadAtomAmbientOcclusion();
  void reloadAtomAmbientLight();
  void reloadAtomDiffuseLight();
  void reloadAtomSpecularLight();
  void reloadAtomShininess();

  void setRepresentationType(int value);
  std::optional<Structure::RepresentationType> representationType();
  void setRepresentationStyle(int value);
  std::optional<Structure::RepresentationStyle> representationStyle();
  void setColorSchemeComboBoxIndex(int value);
  std::optional<QString> colorSchemeIdentifier();
  void setColorSchemeOrder(int value);
  std::optional<SKColorSet::ColorSchemeOrder> colorSchemeOrder();
  void setForcefieldSchemeComboBoxIndex(int value);
  std::optional<QString> forceFieldSchemeIdentifier();
  void setForceFieldSchemeOrder(int value);
  std::optional<ForceFieldSet::ForceFieldSchemeOrder> forceFieldSchemeOrder();
  void setAtomSelectionStyle(int value);
  std::optional<RKSelectionStyle> atomSelectionStyle();
  void setAtomSelectionStyleNu(double value);
  std::optional<double> atomSelectionStyleNu();
  void setAtomSelectionStyleRho(double value);
  std::optional<double> atomSelectionStyleRho();
  void setAtomSelectionIntensity(double value);
  std::optional<double> atomSelectionIntensity();
  void setAtomSelectionScaling(double value);
  std::optional<double> atomSelectionScaling();
  void setAtomDrawAtoms(int state);
  std::optional<bool> atomDrawAtoms();
  void setAtomSizeScalingDoubleSpinBox(double size);
  void setAtomSizeScalingDoubleSliderBegin();
  void setAtomSizeScalingDoubleSliderIntermediate(double size);
  void setAtomSizeScalingDoubleSliderFinal();
  std::optional<double> atomSizeScaling();
  void setAtomHighDynamicRange(int value);
  std::optional<bool> atomHighDynamicRange();
  void setAtomHDRExposure(double value);
  std::optional<double> atomHDRExposure();
  void setAtomHue(double value);
  std::optional<double> atomHue();
  void setAtomSaturation(double value);
  std::optional<double> atomSaturation();
  void setAtomValue(double value);
  std::optional<double> atomValue();
  void setAtomAmbientOcclusion(int value);
  std::optional<bool> atomAmbientOcclusion();
  void setAtomAmbientLightIntensity(double value);
  std::optional<double> atomAmbientLightIntensity();
  void setAtomAmbientLightColor();

  std::optional<QColor> atomAmbientLightColor();
  void setAtomDiffuseLightIntensity(double value);
  std::optional<double> atomDiffuseLightIntensity();
  void setAtomDiffuseLightColor();
  std::optional<QColor> atomDiffuseLightColor();
  void setAtomSpecularLightIntensity(double value);
  std::optional<double> atomSpecularLightIntensity();
  void setAtomSpecularLightColor();
  std::optional<QColor> atomSpecularLightColor();
  void setAtomShininess(double value);
  std::optional<double> atomShininessy();

  // bond properties

  void reloadDrawBondsCheckBox();
  void reloadBondSizeScaling();
  void reloadBondColorMode();
  void reloadBondHighDynamicRange();
  void reloadBondHDRExposure();
  void reloadBondHue();
  void reloadBondSaturation();
  void reloadBondValue();
  void reloadBondAmbientOcclusion();
  void reloadBondAmbientLight();
  void reloadBondDiffuseLight();
  void reloadBondSpecularLight();
  void reloadBondShininess();

  void setBondDrawBonds(int state);
  std::optional<bool> bondDrawBonds();
  void setBondSizeScaling(double size);
  std::optional<double> bondSizeScaling();
  void setBondColorMode(int value);
  std::optional<RKBondColorMode> bondColorMode();
  void setBondHighDynamicRange(int value);
  std::optional<bool> bondHighDynamicRange();
  void setBondHDRExposure(double value);
  std::optional<double> bondHDRExposure();
  void setBondHue(double value);
  std::optional<double> bondHue();
  void setBondSaturation(double value);
  std::optional<double> bondSaturation();
  void setBondValue(double value);
  std::optional<double> bondValue();
  void setBondAmbientOcclusion(int value);
  std::optional<bool> bondAmbientOcclusion();
  void setBondAmbientLightIntensity(double value);
  std::optional<double> bondAmbientLightIntensity();
  void setBondAmbientLightColor();
  std::optional<QColor> bondAmbientLightColor();
  void setBondDiffuseLightIntensity(double value);
  std::optional<double> bondDiffuseLightIntensity();
  void setBondDiffuseLightColor();
  std::optional<QColor> bondDiffuseLightColor();
  void setBondSpecularLightIntensity(double value);
  std::optional<double> bondSpecularLightIntensity();
  void setBondSpecularLightColor();
  std::optional<QColor> bondSpecularLightColor();
  void setBondShininess(double value);
  std::optional<double> bondShininessy();

  // unit cell properties

  void reloadDrawUnitCell();
  void reloadUnitCellSizeScaling();
  void reloadUnitCellDiffuseLight();

  void setDrawUnitCell(int state);
  std::optional<bool> drawUnitCell();
  void setUnitCellSizeScaling(double size);
  std::optional<double> unitCellSizeScaling();
  void setUnitCellDiffuseLightIntensity(double value);
  std::optional<double> unitCellDiffuseLightIntensity();
  void setUnitCellDiffuseLightColor();
  std::optional<QColor> unitCellDiffuseLightColor();

  // adsorption surface properties

  void reloadDrawAdsorptionSurfaceCheckBox();
  void reloadAdsorptionSurfaceProbeMoleculePopupBox();
  void reloadAdsorptionSurfaceIsovalue();
  void reloadAdsorptionSurfaceOpacity();
  void reloadAdsorptionSurfaceInsideHighDynamicRange();
  void reloadAdsorptionSurfaceInsideHDRExposure();
  void reloadAdsorptionSurfaceInsideAmbientLight();
  void reloadAdsorptionSurfaceInsideDiffuseLight();
  void reloadAdsorptionSurfaceInsideSpecularLight();
  void reloadAdsorptionSurfaceInsideShininess();
  void reloadAdsorptionSurfaceOutsideHighDynamicRange();
  void reloadAdsorptionSurfaceOutsideHDRExposure();
  void reloadAdsorptionSurfaceOutsideAmbientLight();
  void reloadAdsorptionSurfaceOutsideDiffuseLight();
  void reloadAdsorptionSurfaceOutsideSpecularLight();
  void reloadAdsorptionSurfaceOutsideShininess();

  void setDrawAdsorptionSurface(int state);
  std::optional<bool> drawAdsorptionSurface();
  void setAdsorptionSurfaceProbeMolecule(int value);
  std::optional<Structure::ProbeMolecule> adsorptionSurfaceProbeMolecule();
  void setAdsorptionSurfaceIsovalue(double value);
  std::optional<double> adsorptionSurfaceIsovalue();
  std::optional<double> adsorptionSurfaceMinimumValue();
  void setAdsorptionSurfaceOpacity(double value);
  std::optional<double> adsorptionSurfaceOpacity();

  void setAdsorptionSurfaceInsideHighDynamicRange(int value);
  std::optional<bool> adsorptionSurfaceInsideHighDynamicRange();
  void setAdsorptionSurfaceInsideHDRExposure(double value);
  std::optional<double> adsorptionSurfaceInsideHDRExposure();
  void setAdsorptionSurfaceInsideAmbientLightIntensity(double value);
  std::optional<double> adsorptionSurfaceInsideAmbientLightIntensity();
  void setAdsorptionSurfaceInsideAmbientLightColor();
  std::optional<QColor> adsorptionSurfaceInsideAmbientLightColor();
  void setAdsorptionSurfaceInsideDiffuseLightIntensity(double value);
  std::optional<double> adsorptionSurfaceInsideDiffuseLightIntensity();
  void setAdsorptionSurfaceInsideDiffuseLightColor();
  std::optional<QColor> adsorptionSurfaceInsideDiffuseLightColor();
  void setAdsorptionSurfaceInsideSpecularLightIntensity(double value);
  std::optional<double> adsorptionSurfaceInsideSpecularLightIntensity();
  void setAdsorptionSurfaceInsideSpecularLightColor();
  std::optional<QColor> adsorptionSurfaceInsideSpecularLightColor();
  void setAdsorptionSurfaceInsideShininess(double value);
  std::optional<double> adsorptionSurfaceInsideShininessy();

  void setAdsorptionSurfaceOutsideHighDynamicRange(int value);
  std::optional<bool> adsorptionSurfaceOutsideHighDynamicRange();
  void setAdsorptionSurfaceOutsideHDRExposure(double value);
  std::optional<double> adsorptionSurfaceOutsideHDRExposure();
  void setAdsorptionSurfaceOutsideAmbientLightIntensity(double value);
  std::optional<double> adsorptionSurfaceOutsideAmbientLightIntensity();
  void setAdsorptionSurfaceOutsideAmbientLightColor();
  std::optional<QColor> adsorptionSurfaceOutsideAmbientLightColor();
  void setAdsorptionSurfaceOutsideDiffuseLightIntensity(double value);
  std::optional<double> adsorptionSurfaceOutsideDiffuseLightIntensity();
  void setAdsorptionSurfaceOutsideDiffuseLightColor();
  std::optional<QColor> adsorptionSurfaceOutsideDiffuseLightColor();
  void setAdsorptionSurfaceOutsideSpecularLightIntensity(double value);
  std::optional<double> adsorptionSurfaceOutsideSpecularLightIntensity();
  void setAdsorptionSurfaceOutsideSpecularLightColor();
  std::optional<QColor> adsorptionSurfaceOutsideSpecularLightColor();
  void setAdsorptionSurfaceOutsideShininess(double value);
  std::optional<double> adsorptionSurfaceOutsideShininessy();

private slots:
  void expandPrimitiveItem();
  void expandAtomsItem();
  void expandBondsItem();
  void expandUnitCellItem();
  void expandAdsorptionSurfaceItem();
  void expandAnnotationItem();
signals:
  void rendererReloadData();

  void redrawRenderer();
  void redrawRendererWithLowQuality();
  void redrawRendererWithMediumQuality();
  void redrawRendererWithHighQuality();
  void redrawRendererWithPictureQuality();

  void invalidateCachedAmbientOcclusionTexture(std::vector<std::shared_ptr<RKRenderStructure>> structures);
  void invalidateIsosurface(std::vector<std::shared_ptr<RKRenderStructure>> structures);
};

