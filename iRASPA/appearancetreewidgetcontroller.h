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
#include "optional.h"
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
  void setStructures(std::vector<std::shared_ptr<Structure>> structures);
private:
  AppearanceAtomsForm* _appearanceAtomsForm;
  AppearanceBondsForm* _appearanceBondsForm;
  AppearanceUnitCellForm* _appearanceUnitCellForm;
  AppearanceAdsorptionSurfaceForm* _appearanceAdsorptionSurfaceForm;
  AppearanceAnnotationForm* _appearanceAnnotationForm;

  QPushButton* pushButtonAtoms;
  QPushButton* pushButtonBonds;
  QPushButton* pushButtonUnitCell;
  QPushButton* pushButtonAdsorptionSurface;
  QPushButton* pushButtonAnnotation;

  MainWindow *_mainWindow;
  std::shared_ptr<ProjectStructure> _projectStructure;
  std::vector<std::shared_ptr<Structure>> _structures{};
  void reloadData() override final;
  void reloadSelection() override final;
  void reloadAtomProperties();
  void reloadBondProperties();
  void reloadUnitCellProperties();
  void reloadAdsorptionSurfaceProperties();
  void reloadAnnotationProperties();

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
  stdx::optional<RepresentationType> representationType();
  void setRepresentationStyle(int value);
  stdx::optional<RepresentationStyle> representationStyle();
  void setColorSchemeComboBoxIndex(int value);
  stdx::optional<QString> colorSchemeIdentifier();
  void setColorSchemeOrder(int value);
  stdx::optional<ColorSchemeOrder> colorSchemeOrder();
  void setForcefieldSchemeComboBoxIndex(int value);
  stdx::optional<QString> forceFieldSchemeIdentifier();
  void setForceFieldSchemeOrder(int value);
  stdx::optional<ForceFieldSchemeOrder> forceFieldSchemeOrder();
  void setAtomSelectionStyle(int value);
  stdx::optional<RKSelectionStyle> atomSelectionStyle();
  void setAtomSelectionStyleNu(double value);
  stdx::optional<double> atomSelectionStyleNu();
  void setAtomSelectionStyleRho(double value);
  stdx::optional<double> atomSelectionStyleRho();
  void setAtomSelectionIntensity(double value);
  stdx::optional<double> atomSelectionIntensity();
  void setAtomSelectionScaling(double value);
  stdx::optional<double> atomSelectionScaling();
  void setAtomDrawAtoms(int state);
  stdx::optional<bool> atomDrawAtoms();
  void setAtomSizeScalingDoubleSpinBox(double size);
  void setAtomSizeScalingDoubleSliderBegin();
  void setAtomSizeScalingDoubleSliderIntermediate(double size);
  void setAtomSizeScalingDoubleSliderFinal();
  stdx::optional<double> atomSizeScaling();
  void setAtomHighDynamicRange(int value);
  stdx::optional<bool> atomHighDynamicRange();
  void setAtomHDRExposure(double value);
  stdx::optional<double> atomHDRExposure();
  void setAtomHue(double value);
  stdx::optional<double> atomHue();
  void setAtomSaturation(double value);
  stdx::optional<double> atomSaturation();
  void setAtomValue(double value);
  stdx::optional<double> atomValue();
  void setAtomAmbientOcclusion(int value);
  stdx::optional<bool> atomAmbientOcclusion();
  void setAtomAmbientLightIntensity(double value);
  stdx::optional<double> atomAmbientLightIntensity();
  void setAtomAmbientLightColor();

  stdx::optional<QColor> atomAmbientLightColor();
  void setAtomDiffuseLightIntensity(double value);
  stdx::optional<double> atomDiffuseLightIntensity();
  void setAtomDiffuseLightColor();
  stdx::optional<QColor> atomDiffuseLightColor();
  void setAtomSpecularLightIntensity(double value);
  stdx::optional<double> atomSpecularLightIntensity();
  void setAtomSpecularLightColor();
  stdx::optional<QColor> atomSpecularLightColor();
  void setAtomShininess(double value);
  stdx::optional<double> atomShininessy();

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
  stdx::optional<bool> bondDrawBonds();
  void setBondSizeScaling(double size);
  stdx::optional<double> bondSizeScaling();
  void setBondColorMode(int value);
  stdx::optional<RKBondColorMode> bondColorMode();
  void setBondHighDynamicRange(int value);
  stdx::optional<bool> bondHighDynamicRange();
  void setBondHDRExposure(double value);
  stdx::optional<double> bondHDRExposure();
  void setBondHue(double value);
  stdx::optional<double> bondHue();
  void setBondSaturation(double value);
  stdx::optional<double> bondSaturation();
  void setBondValue(double value);
  stdx::optional<double> bondValue();
  void setBondAmbientOcclusion(int value);
  stdx::optional<bool> bondAmbientOcclusion();
  void setBondAmbientLightIntensity(double value);
  stdx::optional<double> bondAmbientLightIntensity();
  void setBondAmbientLightColor();
  stdx::optional<QColor> bondAmbientLightColor();
  void setBondDiffuseLightIntensity(double value);
  stdx::optional<double> bondDiffuseLightIntensity();
  void setBondDiffuseLightColor();
  stdx::optional<QColor> bondDiffuseLightColor();
  void setBondSpecularLightIntensity(double value);
  stdx::optional<double> bondSpecularLightIntensity();
  void setBondSpecularLightColor();
  stdx::optional<QColor> bondSpecularLightColor();
  void setBondShininess(double value);
  stdx::optional<double> bondShininessy();

  // unit cell properties

  void reloadDrawUnitCell();
  void reloadUnitCellSizeScaling();
  void reloadUnitCellDiffuseLight();

  void setDrawUnitCell(int state);
  stdx::optional<bool> drawUnitCell();
  void setUnitCellSizeScaling(double size);
  stdx::optional<double> unitCellSizeScaling();
  void setUnitCellDiffuseLightIntensity(double value);
  stdx::optional<double> unitCellDiffuseLightIntensity();
  void setUnitCellDiffuseLightColor();
  stdx::optional<QColor> unitCellDiffuseLightColor();

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
  stdx::optional<bool> drawAdsorptionSurface();
  void setAdsorptionSurfaceProbeMolecule(int value);
  stdx::optional<ProbeMolecule> adsorptionSurfaceProbeMolecule();
  void setAdsorptionSurfaceIsovalue(double value);
  stdx::optional<double> adsorptionSurfaceIsovalue();
  stdx::optional<double> adsorptionSurfaceMinimumValue();
  void setAdsorptionSurfaceOpacity(double value);
  stdx::optional<double> adsorptionSurfaceOpacity();

  void setAdsorptionSurfaceInsideHighDynamicRange(int value);
  stdx::optional<bool> adsorptionSurfaceInsideHighDynamicRange();
  void setAdsorptionSurfaceInsideHDRExposure(double value);
  stdx::optional<double> adsorptionSurfaceInsideHDRExposure();
  void setAdsorptionSurfaceInsideAmbientLightIntensity(double value);
  stdx::optional<double> adsorptionSurfaceInsideAmbientLightIntensity();
  void setAdsorptionSurfaceInsideAmbientLightColor();
  stdx::optional<QColor> adsorptionSurfaceInsideAmbientLightColor();
  void setAdsorptionSurfaceInsideDiffuseLightIntensity(double value);
  stdx::optional<double> adsorptionSurfaceInsideDiffuseLightIntensity();
  void setAdsorptionSurfaceInsideDiffuseLightColor();
  stdx::optional<QColor> adsorptionSurfaceInsideDiffuseLightColor();
  void setAdsorptionSurfaceInsideSpecularLightIntensity(double value);
  stdx::optional<double> adsorptionSurfaceInsideSpecularLightIntensity();
  void setAdsorptionSurfaceInsideSpecularLightColor();
  stdx::optional<QColor> adsorptionSurfaceInsideSpecularLightColor();
  void setAdsorptionSurfaceInsideShininess(double value);
  stdx::optional<double> adsorptionSurfaceInsideShininessy();

  void setAdsorptionSurfaceOutsideHighDynamicRange(int value);
  stdx::optional<bool> adsorptionSurfaceOutsideHighDynamicRange();
  void setAdsorptionSurfaceOutsideHDRExposure(double value);
  stdx::optional<double> adsorptionSurfaceOutsideHDRExposure();
  void setAdsorptionSurfaceOutsideAmbientLightIntensity(double value);
  stdx::optional<double> adsorptionSurfaceOutsideAmbientLightIntensity();
  void setAdsorptionSurfaceOutsideAmbientLightColor();
  stdx::optional<QColor> adsorptionSurfaceOutsideAmbientLightColor();
  void setAdsorptionSurfaceOutsideDiffuseLightIntensity(double value);
  stdx::optional<double> adsorptionSurfaceOutsideDiffuseLightIntensity();
  void setAdsorptionSurfaceOutsideDiffuseLightColor();
  stdx::optional<QColor> adsorptionSurfaceOutsideDiffuseLightColor();
  void setAdsorptionSurfaceOutsideSpecularLightIntensity(double value);
  stdx::optional<double> adsorptionSurfaceOutsideSpecularLightIntensity();
  void setAdsorptionSurfaceOutsideSpecularLightColor();
  stdx::optional<QColor> adsorptionSurfaceOutsideSpecularLightColor();
  void setAdsorptionSurfaceOutsideShininess(double value);
  stdx::optional<double> adsorptionSurfaceOutsideShininessy();

private slots:
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

