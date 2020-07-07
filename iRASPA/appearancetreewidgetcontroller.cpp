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


#include "appearancetreewidgetcontroller.h"

#include <QObject>
#include <QComboBox>
#include <QColorDialog>
#include <QColor>
#include "qcolorhash.h"
#include "qdoubleslider.h"
#include <foundationkit.h>

AppearanceTreeWidgetController::AppearanceTreeWidgetController(QWidget* parent): QTreeWidget(parent),
    _appearancePrimitiveForm(new AppearancePrimitiveForm),
    _appearanceAtomsForm(new AppearanceAtomsForm),
    _appearanceBondsForm(new AppearanceBondsForm),
    _appearanceUnitCellForm(new AppearanceUnitCellForm),
    _appearanceAdsorptionSurfaceForm(new AppearanceAdsorptionSurfaceForm),
    _appearanceAnnotationForm(new AppearanceAnnotationForm)
{
  this->setHeaderHidden(true);
  this->setRootIsDecorated(true);
  this->setFrameStyle(QFrame::NoFrame);
  this->setAnimated(true);
  this->setVerticalScrollMode(QTreeWidget::ScrollPerPixel);
  this->setExpandsOnDoubleClick(false);
  this->setIndentation(0);
  this->setSelectionMode(QAbstractItemView::NoSelection);

  // Primtives
  //=========================================================================
  QTreeWidgetItem* PrimitiveItem = new QTreeWidgetItem(this);
  this->addTopLevelItem(PrimitiveItem);

  pushButtonPrimitive = new QPushButton(QString("Primitive"),this);
  pushButtonPrimitive->setIcon(QIcon(":/iRASPA/collapsed.png"));
  pushButtonPrimitive->setStyleSheet("text-align:left;");
  setItemWidget(PrimitiveItem,0,pushButtonPrimitive);

  QTreeWidgetItem *childPrimitiveItem = new QTreeWidgetItem(PrimitiveItem);
  this->setItemWidget(childPrimitiveItem,0, _appearancePrimitiveForm);

  // Atoms
  //=========================================================================
  QTreeWidgetItem* AtomsItem = new QTreeWidgetItem(this);
  this->addTopLevelItem(AtomsItem);

  pushButtonAtoms = new QPushButton(QString("Atoms"),this);
  pushButtonAtoms->setIcon(QIcon(":/iRASPA/collapsed.png"));
  pushButtonAtoms->setStyleSheet("text-align:left;");
  setItemWidget(AtomsItem,0,pushButtonAtoms);

  QTreeWidgetItem *childAtomsItem = new QTreeWidgetItem(AtomsItem);
  this->setItemWidget(childAtomsItem,0, _appearanceAtomsForm);

  _appearanceAtomsForm->atomRepresentationType->insertItem(0,"Ball and stick");
  _appearanceAtomsForm->atomRepresentationType->insertItem(1,"Van der Waals");
  _appearanceAtomsForm->atomRepresentationType->insertItem(2, "Unity");

  _appearanceAtomsForm->atomRepresentationStyle->insertItem(0,"Default");
  _appearanceAtomsForm->atomRepresentationStyle->insertItem(1,"Fancy");
  _appearanceAtomsForm->atomRepresentationStyle->insertItem(2,"Licorice");
  _appearanceAtomsForm->atomRepresentationStyle->insertItem(3,"Objects");

  _appearanceAtomsForm->colorSchemeComboBox->insertItem(0,"Jmol");
  _appearanceAtomsForm->colorSchemeComboBox->insertItem(1,"Rasmol modern");
  _appearanceAtomsForm->colorSchemeComboBox->insertItem(2,"Rasmol");
  _appearanceAtomsForm->colorSchemeComboBox->insertItem(3,"Vesta");

  _appearanceAtomsForm->colorSchemeOrderComboBox->insertItem(0,"Element");
  _appearanceAtomsForm->colorSchemeOrderComboBox->insertItem(1,"Force field first");
  _appearanceAtomsForm->colorSchemeOrderComboBox->insertItem(2,"Force field only");

  _appearanceAtomsForm->forceFieldComboBox->insertItem(0,"Default");

  _appearanceAtomsForm->forceFieldSchemeOrderComboBox->insertItem(0,"Element");
  _appearanceAtomsForm->forceFieldSchemeOrderComboBox->insertItem(1,"Force field first");
  _appearanceAtomsForm->forceFieldSchemeOrderComboBox->insertItem(2,"Force field only");

  _appearanceAtomsForm->atomSelectionStyleComboBox->insertItem(0,"None");
  _appearanceAtomsForm->atomSelectionStyleComboBox->insertItem(1,"Worley Noise 3D");
  _appearanceAtomsForm->atomSelectionStyleComboBox->insertItem(2,"Stripes");
  _appearanceAtomsForm->atomSelectionStyleComboBox->insertItem(3,"Glow");

  // Bonds
  //=========================================================================
  QTreeWidgetItem* BondsItem = new QTreeWidgetItem(this);
  this->addTopLevelItem(BondsItem);

  pushButtonBonds = new QPushButton(QString("Bonds"),this);
  pushButtonBonds->setIcon(QIcon(":/iRASPA/collapsed.png"));
  pushButtonBonds->setStyleSheet("text-align:left;");
  setItemWidget(BondsItem,0,pushButtonBonds);

  QTreeWidgetItem *childBondsItem = new QTreeWidgetItem(BondsItem);
  this->setItemWidget(childBondsItem,0, _appearanceBondsForm);

  _appearanceBondsForm->bondColorModeComboBox->insertItem(0,"Uniform");
  _appearanceBondsForm->bondColorModeComboBox->insertItem(1,"Split");
  _appearanceBondsForm->bondColorModeComboBox->insertItem(2,"Gradient");

  // Unit cell
  //=========================================================================
  QTreeWidgetItem* UnitCellItem = new QTreeWidgetItem(this);
  this->addTopLevelItem(UnitCellItem);

  pushButtonUnitCell = new QPushButton(QString("Unit cell"),this);
  pushButtonUnitCell->setIcon(QIcon(":/iRASPA/collapsed.png"));
  pushButtonUnitCell->setStyleSheet("text-align:left;");
  setItemWidget(UnitCellItem,0,pushButtonUnitCell);

  QTreeWidgetItem *childUnitCellItem = new QTreeWidgetItem(UnitCellItem);
  this->setItemWidget(childUnitCellItem,0, _appearanceUnitCellForm);

  // Adsorption surface
  //=========================================================================
  QTreeWidgetItem* AdsorptionSurfaceItem = new QTreeWidgetItem(this);
  this->addTopLevelItem(AdsorptionSurfaceItem);

  pushButtonAdsorptionSurface = new QPushButton(QString("Adsorption surface"),this);
  pushButtonAdsorptionSurface->setIcon(QIcon(":/iRASPA/collapsed.png"));
  pushButtonAdsorptionSurface->setStyleSheet("text-align:left;");
  setItemWidget(AdsorptionSurfaceItem,0,pushButtonAdsorptionSurface);

  QTreeWidgetItem *childAdsorptionSurfaceItem = new QTreeWidgetItem(AdsorptionSurfaceItem);
  this->setItemWidget(childAdsorptionSurfaceItem,0, _appearanceAdsorptionSurfaceForm);

  _appearanceAdsorptionSurfaceForm->probeParticleComboBox->insertItem(0, "Helium");
  _appearanceAdsorptionSurfaceForm->probeParticleComboBox->insertItem(1, "Methane");
  _appearanceAdsorptionSurfaceForm->probeParticleComboBox->insertItem(2, "Nitrogen");
  _appearanceAdsorptionSurfaceForm->probeParticleComboBox->insertItem(3, "Hydrogen");
  _appearanceAdsorptionSurfaceForm->probeParticleComboBox->insertItem(4, "Water");
  _appearanceAdsorptionSurfaceForm->probeParticleComboBox->insertItem(5, "CO₂");
  _appearanceAdsorptionSurfaceForm->probeParticleComboBox->insertItem(6, "Xenon");
  _appearanceAdsorptionSurfaceForm->probeParticleComboBox->insertItem(7, "Krypton");
  _appearanceAdsorptionSurfaceForm->probeParticleComboBox->insertItem(8, "Argon");

  // Annotation
  //=========================================================================
  QTreeWidgetItem* AnnotationItem = new QTreeWidgetItem(this);
  this->addTopLevelItem(AnnotationItem);

  pushButtonAnnotation = new QPushButton(QString("Annotation"),this);
  pushButtonAnnotation->setIcon(QIcon(":/iRASPA/collapsed.png"));
  pushButtonAnnotation->setStyleSheet("text-align:left;");
  setItemWidget(AnnotationItem,0,pushButtonAnnotation);

  QTreeWidgetItem *childAnnotationItem = new QTreeWidgetItem(AnnotationItem);
  this->setItemWidget(childAnnotationItem,0, _appearanceAnnotationForm);

  QObject::connect(pushButtonPrimitive, &QPushButton::clicked, this, &AppearanceTreeWidgetController::expandPrimitiveItem);
  QObject::connect(pushButtonAtoms, &QPushButton::clicked, this, &AppearanceTreeWidgetController::expandAtomsItem);
  QObject::connect(pushButtonBonds, &QPushButton::clicked, this, &AppearanceTreeWidgetController::expandBondsItem);
  QObject::connect(pushButtonUnitCell, &QPushButton::clicked, this, &AppearanceTreeWidgetController::expandUnitCellItem);
  QObject::connect(pushButtonAdsorptionSurface, &QPushButton::clicked, this, &AppearanceTreeWidgetController::expandAdsorptionSurfaceItem);
  QObject::connect(pushButtonAnnotation, &QPushButton::clicked, this, &AppearanceTreeWidgetController::expandAnnotationItem);


  QObject::connect(_appearanceAtomsForm->atomRepresentationType,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),this,&AppearanceTreeWidgetController::setRepresentationType);
  QObject::connect(_appearanceAtomsForm->atomRepresentationStyle,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),this,&AppearanceTreeWidgetController::setRepresentationStyle);
  QObject::connect(_appearanceAtomsForm->colorSchemeComboBox,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),this,&AppearanceTreeWidgetController::setColorSchemeComboBoxIndex);
  QObject::connect(_appearanceAtomsForm->colorSchemeOrderComboBox,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),this,&AppearanceTreeWidgetController::setColorSchemeOrder);
  QObject::connect(_appearanceAtomsForm->forceFieldComboBox,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),this,&AppearanceTreeWidgetController::setForcefieldSchemeComboBoxIndex);
  QObject::connect(_appearanceAtomsForm->forceFieldSchemeOrderComboBox,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),this,&AppearanceTreeWidgetController::setForceFieldSchemeOrder);

  QObject::connect(_appearanceAtomsForm->atomSelectionStyleComboBox,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),this,&AppearanceTreeWidgetController::setAtomSelectionStyle);
  QObject::connect(_appearanceAtomsForm->atomSelectionStyleNuDoubleSpinBox,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&AppearanceTreeWidgetController::setAtomSelectionStyleNu);
  QObject::connect(_appearanceAtomsForm->atomSelectionStyleRhoDoubleSpinBox,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&AppearanceTreeWidgetController::setAtomSelectionStyleRho);
  QObject::connect(_appearanceAtomsForm->atomSelectionItensityDoubleSpinBox,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&AppearanceTreeWidgetController::setAtomSelectionIntensity);
  QObject::connect(_appearanceAtomsForm->atomSelectionItensityDoubleSlider,static_cast<void (QDoubleSlider::*)(double)>(&QDoubleSlider::sliderMoved),this,&AppearanceTreeWidgetController::setAtomSelectionIntensity);
  QObject::connect(_appearanceAtomsForm->atomSelectionScalingDoubleSpinBox,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&AppearanceTreeWidgetController::setAtomSelectionScaling);
  QObject::connect(_appearanceAtomsForm->atomSelectionScalingDoubleSlider,static_cast<void (QDoubleSlider::*)(double)>(&QDoubleSlider::sliderMoved),this,&AppearanceTreeWidgetController::setAtomSelectionScaling);

  QObject::connect(_appearanceAtomsForm->atomDrawAtomsCheckBox,static_cast<void (QCheckBox::*)(int)>(&QCheckBox::stateChanged),this,&AppearanceTreeWidgetController::setAtomDrawAtoms);

  QObject::connect(_appearanceAtomsForm->atomSizeScalingSpinBox,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&AppearanceTreeWidgetController::setAtomSizeScalingDoubleSpinBox);
  QObject::connect(_appearanceAtomsForm->atomAtomicSizeScalingSlider,static_cast<void (QDoubleSlider::*)(void)>(&QDoubleSlider::sliderPressed),this,&AppearanceTreeWidgetController::setAtomSizeScalingDoubleSliderBegin);
  QObject::connect(_appearanceAtomsForm->atomAtomicSizeScalingSlider,static_cast<void (QDoubleSlider::*)(double)>(&QDoubleSlider::sliderMoved),this,&AppearanceTreeWidgetController::setAtomSizeScalingDoubleSliderIntermediate);
  QObject::connect(_appearanceAtomsForm->atomAtomicSizeScalingSlider,static_cast<void (QDoubleSlider::*)(void)>(&QDoubleSlider::sliderReleased),this,&AppearanceTreeWidgetController::setAtomSizeScalingDoubleSliderFinal);

  QObject::connect(_appearanceAtomsForm->atomHighDynamicRangeCheckBox,static_cast<void (QCheckBox::*)(int)>(&QCheckBox::stateChanged),this,&AppearanceTreeWidgetController::setAtomHighDynamicRange);
  QObject::connect(_appearanceAtomsForm->atomHDRExposureDoubleSpinBox,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&AppearanceTreeWidgetController::setAtomHDRExposure);
  QObject::connect(_appearanceAtomsForm->atomHDRExposureDoubleSlider,static_cast<void (QDoubleSlider::*)(double)>(&QDoubleSlider::sliderMoved),this,&AppearanceTreeWidgetController::setAtomHDRExposure);
  QObject::connect(_appearanceAtomsForm->atomHueDoubleSpinBox,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&AppearanceTreeWidgetController::setAtomHue);
  QObject::connect(_appearanceAtomsForm->atomHueDoubleSlider,static_cast<void (QDoubleSlider::*)(double)>(&QDoubleSlider::sliderMoved),this,&AppearanceTreeWidgetController::setAtomHue);
  QObject::connect(_appearanceAtomsForm->atomSaturationDoubleSpinBox,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&AppearanceTreeWidgetController::setAtomSaturation);
  QObject::connect(_appearanceAtomsForm->atomSaturationDoubleSlider,static_cast<void (QDoubleSlider::*)(double)>(&QDoubleSlider::sliderMoved),this,&AppearanceTreeWidgetController::setAtomSaturation);
  QObject::connect(_appearanceAtomsForm->atomValueDoubleSpinBox,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&AppearanceTreeWidgetController::setAtomValue);
  QObject::connect(_appearanceAtomsForm->atomValueDoubleSlider,static_cast<void (QDoubleSlider::*)(double)>(&QDoubleSlider::sliderMoved),this,&AppearanceTreeWidgetController::setAtomValue);


  QObject::connect(_appearanceAtomsForm->atomAmbientOcclusionCheckBox,static_cast<void (QCheckBox::*)(int)>(&QCheckBox::stateChanged),this,&AppearanceTreeWidgetController::setAtomAmbientOcclusion);
  QObject::connect(_appearanceAtomsForm->atomAmbientIntensityDoubleSpinBox,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&AppearanceTreeWidgetController::setAtomAmbientLightIntensity);
  QObject::connect(_appearanceAtomsForm->atomAmbientIntensityDoubleSlider,static_cast<void (QDoubleSlider::*)(double)>(&QDoubleSlider::sliderMoved),this,&AppearanceTreeWidgetController::setAtomAmbientLightIntensity);
  QObject::connect(_appearanceAtomsForm->atomDiffuseItensityDoubleSpinBox,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&AppearanceTreeWidgetController::setAtomDiffuseLightIntensity);
  QObject::connect(_appearanceAtomsForm->atomDiffuseIntensityDoubleSlider,static_cast<void (QDoubleSlider::*)(double)>(&QDoubleSlider::sliderMoved),this,&AppearanceTreeWidgetController::setAtomDiffuseLightIntensity);
  QObject::connect(_appearanceAtomsForm->atomSpecularIntensityDoubleSpinBoxBox,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&AppearanceTreeWidgetController::setAtomSpecularLightIntensity);
  QObject::connect(_appearanceAtomsForm->atomSpecularIntensityDoubleSlider,static_cast<void (QDoubleSlider::*)(double)>(&QDoubleSlider::sliderMoved),this,&AppearanceTreeWidgetController::setAtomSpecularLightIntensity);
  QObject::connect(_appearanceAtomsForm->atomShininessDoubleSpinBox,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&AppearanceTreeWidgetController::setAtomShininess);
  QObject::connect(_appearanceAtomsForm->atomShininessDoubleSlider,static_cast<void (QDoubleSlider::*)(double)>(&QDoubleSlider::sliderMoved),this,&AppearanceTreeWidgetController::setAtomShininess);

  QObject::connect(_appearanceAtomsForm->atomAmbientColorPushButton,&QPushButton::clicked,this,&AppearanceTreeWidgetController::setAtomAmbientLightColor);
  QObject::connect(_appearanceAtomsForm->atomDiffuseColorPushButton,&QPushButton::clicked,this,&AppearanceTreeWidgetController::setAtomDiffuseLightColor);
  QObject::connect(_appearanceAtomsForm->atomSpecularColorPushButton,&QPushButton::clicked,this,&AppearanceTreeWidgetController::setAtomSpecularLightColor);


  QObject::connect(_appearanceBondsForm->drawBondsCheckBox,static_cast<void (QCheckBox::*)(int)>(&QCheckBox::stateChanged),this,&AppearanceTreeWidgetController::setBondDrawBonds);
  QObject::connect(_appearanceBondsForm->bondSizeScalingDoubleSlider,static_cast<void (QDoubleSlider::*)(double)>(&QDoubleSlider::sliderMoved),this,&AppearanceTreeWidgetController::setBondSizeScaling);
  QObject::connect(_appearanceBondsForm->bondSizeScalingDoubleSpinBox,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&AppearanceTreeWidgetController::setBondSizeScaling);

  QObject::connect(_appearanceBondsForm->bondColorModeComboBox,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),this,&AppearanceTreeWidgetController::setBondColorMode);

  QObject::connect(_appearanceBondsForm->bondAmbientOcclusionCheckBox,static_cast<void (QCheckBox::*)(int)>(&QCheckBox::stateChanged),this,&AppearanceTreeWidgetController::setBondAmbientOcclusion);
  QObject::connect(_appearanceBondsForm->bondAmbientIntensityDoubleSpinBox,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&AppearanceTreeWidgetController::setBondAmbientLightIntensity);
  QObject::connect(_appearanceBondsForm->bondAmbientIntensityDoubleSlider,static_cast<void (QDoubleSlider::*)(double)>(&QDoubleSlider::sliderMoved),this,&AppearanceTreeWidgetController::setBondAmbientLightIntensity);
  QObject::connect(_appearanceBondsForm->bondDiffuseIntensityDoubleSpinBox,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&AppearanceTreeWidgetController::setBondDiffuseLightIntensity);
  QObject::connect(_appearanceBondsForm->bondDiffuseIntensityDoubleSlider,static_cast<void (QDoubleSlider::*)(double)>(&QDoubleSlider::sliderMoved),this,&AppearanceTreeWidgetController::setBondDiffuseLightIntensity);
  QObject::connect(_appearanceBondsForm->bondSpecularIntensityDoubleSpinBox,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&AppearanceTreeWidgetController::setBondSpecularLightIntensity);
  QObject::connect(_appearanceBondsForm->bondSpecularIntensityDoubleSlider,static_cast<void (QDoubleSlider::*)(double)>(&QDoubleSlider::sliderMoved),this,&AppearanceTreeWidgetController::setBondSpecularLightIntensity);
  QObject::connect(_appearanceBondsForm->bondShininessDoubleSpinBox,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&AppearanceTreeWidgetController::setBondShininess);
  QObject::connect(_appearanceBondsForm->bondShininessDoubleSlider,static_cast<void (QDoubleSlider::*)(double)>(&QDoubleSlider::sliderMoved),this,&AppearanceTreeWidgetController::setBondShininess);

  QObject::connect(_appearanceBondsForm->bondAmbientColorPushButton,&QPushButton::clicked,this,&AppearanceTreeWidgetController::setBondAmbientLightColor);
  QObject::connect(_appearanceBondsForm->bondDiffuseColorPushButton,&QPushButton::clicked,this,&AppearanceTreeWidgetController::setBondDiffuseLightColor);
  QObject::connect(_appearanceBondsForm->bondSpecularColorPushButton,&QPushButton::clicked,this,&AppearanceTreeWidgetController::setBondSpecularLightColor);

  QObject::connect(_appearanceUnitCellForm->drawUnitCellCheckBox,static_cast<void (QCheckBox::*)(int)>(&QCheckBox::stateChanged),this,&AppearanceTreeWidgetController::setDrawUnitCell);
  QObject::connect(_appearanceUnitCellForm->unitCellSizeScalingDoubleSlider,static_cast<void (QDoubleSlider::*)(double)>(&QDoubleSlider::sliderMoved),this,&AppearanceTreeWidgetController::setUnitCellSizeScaling);
  QObject::connect(_appearanceUnitCellForm->unitCellSizeScalingDoubleSpinBox,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&AppearanceTreeWidgetController::setUnitCellSizeScaling);
  QObject::connect(_appearanceUnitCellForm->diffuseIntensityDoubleSpinBox,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&AppearanceTreeWidgetController::setUnitCellDiffuseLightIntensity);
  QObject::connect(_appearanceUnitCellForm->diffuseIntensityDoubleSlider,static_cast<void (QDoubleSlider::*)(double)>(&QDoubleSlider::sliderMoved),this,&AppearanceTreeWidgetController::setUnitCellDiffuseLightIntensity);
  QObject::connect(_appearanceUnitCellForm->diffuseColorPushButton,&QPushButton::clicked,this,&AppearanceTreeWidgetController::setUnitCellDiffuseLightColor);

  QObject::connect(_appearanceAdsorptionSurfaceForm->adsorptionSurfaceCheckBox,static_cast<void (QCheckBox::*)(int)>(&QCheckBox::stateChanged),this,&AppearanceTreeWidgetController::setDrawAdsorptionSurface);
  QObject::connect(_appearanceAdsorptionSurfaceForm->probeParticleComboBox,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),this,&AppearanceTreeWidgetController::setAdsorptionSurfaceProbeMolecule);
  QObject::connect(_appearanceAdsorptionSurfaceForm->adsorptionSurfaceIsovalueDoubleSpinBox,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&AppearanceTreeWidgetController::setAdsorptionSurfaceIsovalue);
  QObject::connect(_appearanceAdsorptionSurfaceForm->adsorptionSurfaceIsovalueDoubleSlider,static_cast<void (QDoubleSlider::*)(double)>(&QDoubleSlider::sliderMoved),this,&AppearanceTreeWidgetController::setAdsorptionSurfaceIsovalue);
  QObject::connect(_appearanceAdsorptionSurfaceForm->adsorptionSurfaceOpacityDoubleSpinBox,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&AppearanceTreeWidgetController::setAdsorptionSurfaceOpacity);
  QObject::connect(_appearanceAdsorptionSurfaceForm->adsorptionSurfaceOpacityDoubleSlider,static_cast<void (QDoubleSlider::*)(double)>(&QDoubleSlider::sliderMoved),this,&AppearanceTreeWidgetController::setAdsorptionSurfaceOpacity);

  QObject::connect(_appearanceAdsorptionSurfaceForm->insideHighDynamicRangecheckBox,static_cast<void (QCheckBox::*)(int)>(&QCheckBox::stateChanged),this,&AppearanceTreeWidgetController::setAdsorptionSurfaceInsideHighDynamicRange);
  QObject::connect(_appearanceAdsorptionSurfaceForm->insideHDRExposureDoubleSpinBox,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&AppearanceTreeWidgetController::setAdsorptionSurfaceInsideHDRExposure);
  QObject::connect(_appearanceAdsorptionSurfaceForm->insideHDRExposureDoubleSlider,static_cast<void (QDoubleSlider::*)(double)>(&QDoubleSlider::sliderMoved),this,&AppearanceTreeWidgetController::setAdsorptionSurfaceInsideHDRExposure);
  QObject::connect(_appearanceAdsorptionSurfaceForm->insideAmbientLightIntensityDoubleSpinBox,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&AppearanceTreeWidgetController::setAdsorptionSurfaceInsideAmbientLightIntensity);
  QObject::connect(_appearanceAdsorptionSurfaceForm->insideAmbientLightIntensityDoubleSlider,static_cast<void (QDoubleSlider::*)(double)>(&QDoubleSlider::sliderMoved),this,&AppearanceTreeWidgetController::setAdsorptionSurfaceInsideAmbientLightIntensity);
  QObject::connect(_appearanceAdsorptionSurfaceForm->insideDiffuseLightIntensityDoubleSpinBox,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&AppearanceTreeWidgetController::setAdsorptionSurfaceInsideDiffuseLightIntensity);
  QObject::connect(_appearanceAdsorptionSurfaceForm->insideDiffuseLightIntensityDoubleSlider,static_cast<void (QDoubleSlider::*)(double)>(&QDoubleSlider::sliderMoved),this,&AppearanceTreeWidgetController::setAdsorptionSurfaceInsideDiffuseLightIntensity);
  QObject::connect(_appearanceAdsorptionSurfaceForm->insideSpecularLightIntensityDoubleSpinBox,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&AppearanceTreeWidgetController::setAdsorptionSurfaceInsideSpecularLightIntensity);
  QObject::connect(_appearanceAdsorptionSurfaceForm->insideSpecularLightIntensityDoubleSlider,static_cast<void (QDoubleSlider::*)(double)>(&QDoubleSlider::sliderMoved),this,&AppearanceTreeWidgetController::setAdsorptionSurfaceInsideSpecularLightIntensity);
  QObject::connect(_appearanceAdsorptionSurfaceForm->insideShininessDoubleSpinBox,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&AppearanceTreeWidgetController::setAdsorptionSurfaceInsideShininess);
  QObject::connect(_appearanceAdsorptionSurfaceForm->insideShininessDoubleSlider,static_cast<void (QDoubleSlider::*)(double)>(&QDoubleSlider::sliderMoved),this,&AppearanceTreeWidgetController::setAdsorptionSurfaceInsideShininess);
  QObject::connect(_appearanceAdsorptionSurfaceForm->insideAmbientColorPushButton,&QPushButton::clicked,this,&AppearanceTreeWidgetController::setAdsorptionSurfaceInsideAmbientLightColor);
  QObject::connect(_appearanceAdsorptionSurfaceForm->insideDiffuseColorPushButton,&QPushButton::clicked,this,&AppearanceTreeWidgetController::setAdsorptionSurfaceInsideDiffuseLightColor);
  QObject::connect(_appearanceAdsorptionSurfaceForm->insideSpecularColorPushButton,&QPushButton::clicked,this,&AppearanceTreeWidgetController::setAdsorptionSurfaceInsideSpecularLightColor);

  QObject::connect(_appearanceAdsorptionSurfaceForm->outsideHighDynamicRangecheckBox,static_cast<void (QCheckBox::*)(int)>(&QCheckBox::stateChanged),this,&AppearanceTreeWidgetController::setAdsorptionSurfaceOutsideHighDynamicRange);
  QObject::connect(_appearanceAdsorptionSurfaceForm->outsideHDRExposureDoubleSpinBox,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&AppearanceTreeWidgetController::setAdsorptionSurfaceOutsideHDRExposure);
  QObject::connect(_appearanceAdsorptionSurfaceForm->outsideHDRExposureDoubleSlider,static_cast<void (QDoubleSlider::*)(double)>(&QDoubleSlider::sliderMoved),this,&AppearanceTreeWidgetController::setAdsorptionSurfaceOutsideHDRExposure);
  QObject::connect(_appearanceAdsorptionSurfaceForm->outsideAmbientLightIntensityDoubleSpinBox,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&AppearanceTreeWidgetController::setAdsorptionSurfaceOutsideAmbientLightIntensity);
  QObject::connect(_appearanceAdsorptionSurfaceForm->outsideAmbientLightIntensityDoubleSlider,static_cast<void (QDoubleSlider::*)(double)>(&QDoubleSlider::sliderMoved),this,&AppearanceTreeWidgetController::setAdsorptionSurfaceOutsideAmbientLightIntensity);
  QObject::connect(_appearanceAdsorptionSurfaceForm->outsideDiffuseLightIntensityDoubleSpinBox,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&AppearanceTreeWidgetController::setAdsorptionSurfaceOutsideDiffuseLightIntensity);
  QObject::connect(_appearanceAdsorptionSurfaceForm->outsideDiffuseLightIntensityDoubleSlider,static_cast<void (QDoubleSlider::*)(double)>(&QDoubleSlider::sliderMoved),this,&AppearanceTreeWidgetController::setAdsorptionSurfaceOutsideDiffuseLightIntensity);
  QObject::connect(_appearanceAdsorptionSurfaceForm->outsideSpecularLightIntensityDoubleSpinBox,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&AppearanceTreeWidgetController::setAdsorptionSurfaceOutsideSpecularLightIntensity);
  QObject::connect(_appearanceAdsorptionSurfaceForm->outsideSpecularLightIntensityDoubleSlider,static_cast<void (QDoubleSlider::*)(double)>(&QDoubleSlider::sliderMoved),this,&AppearanceTreeWidgetController::setAdsorptionSurfaceOutsideSpecularLightIntensity);
  QObject::connect(_appearanceAdsorptionSurfaceForm->outsideShininessDoubleSpinBox,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,&AppearanceTreeWidgetController::setAdsorptionSurfaceOutsideShininess);
  QObject::connect(_appearanceAdsorptionSurfaceForm->outsideShininessDoubleSlider,static_cast<void (QDoubleSlider::*)(double)>(&QDoubleSlider::sliderMoved),this,&AppearanceTreeWidgetController::setAdsorptionSurfaceOutsideShininess);
  QObject::connect(_appearanceAdsorptionSurfaceForm->outsideAmbientColorPushButton,&QPushButton::clicked,this,&AppearanceTreeWidgetController::setAdsorptionSurfaceOutsideAmbientLightColor);
  QObject::connect(_appearanceAdsorptionSurfaceForm->outsideDiffuseColorPushButton,&QPushButton::clicked,this,&AppearanceTreeWidgetController::setAdsorptionSurfaceOutsideDiffuseLightColor);
  QObject::connect(_appearanceAdsorptionSurfaceForm->outsideSpecularColorPushButton,&QPushButton::clicked,this,&AppearanceTreeWidgetController::setAdsorptionSurfaceOutsideSpecularLightColor);


  _appearanceAtomsForm->atomAtomicSizeScalingSlider->setDoubleMinimum(0.0);
  _appearanceAtomsForm->atomAtomicSizeScalingSlider->setDoubleMaximum(2.0);
  _appearanceAtomsForm->atomHueDoubleSlider->setDoubleMinimum(0.0);
  _appearanceAtomsForm->atomHueDoubleSlider->setDoubleMaximum(2.0);
  _appearanceAtomsForm->atomSaturationDoubleSlider->setDoubleMinimum(0.0);
  _appearanceAtomsForm->atomSaturationDoubleSlider->setDoubleMaximum(2.0);
  _appearanceAtomsForm->atomValueDoubleSlider->setDoubleMinimum(0.0);
  _appearanceAtomsForm->atomValueDoubleSlider->setDoubleMaximum(2.0);

  _appearanceAtomsForm->atomSelectionItensityDoubleSlider->setDoubleMinimum(0.0);
  _appearanceAtomsForm->atomSelectionItensityDoubleSlider->setDoubleMaximum(3.0);

  _appearanceAtomsForm->atomSelectionScalingDoubleSlider->setDoubleMinimum(1.0);
  _appearanceAtomsForm->atomSelectionScalingDoubleSlider->setDoubleMaximum(3.0);

  _appearanceAtomsForm->atomHDRExposureDoubleSlider->setDoubleMinimum(0.0);
  _appearanceAtomsForm->atomHDRExposureDoubleSlider->setDoubleMaximum(3.0);

  _appearanceAtomsForm->atomAmbientIntensityDoubleSlider->setDoubleMinimum(0.0);
  _appearanceAtomsForm->atomAmbientIntensityDoubleSlider->setDoubleMaximum(1.0);
  _appearanceAtomsForm->atomDiffuseIntensityDoubleSlider->setDoubleMinimum(0.0);
  _appearanceAtomsForm->atomDiffuseIntensityDoubleSlider->setDoubleMaximum(1.0);
  _appearanceAtomsForm->atomSpecularIntensityDoubleSlider->setDoubleMinimum(0.0);
  _appearanceAtomsForm->atomSpecularIntensityDoubleSlider->setDoubleMaximum(1.0);
  _appearanceAtomsForm->atomShininessDoubleSlider->setDoubleMinimum(0.1);
  _appearanceAtomsForm->atomShininessDoubleSlider->setDoubleMaximum(128.0);


  _appearanceBondsForm->bondAmbientIntensityDoubleSlider->setDoubleMinimum(0.0);
  _appearanceBondsForm->bondAmbientIntensityDoubleSlider->setDoubleMaximum(1.0);
  _appearanceBondsForm->bondDiffuseIntensityDoubleSlider->setDoubleMinimum(0.0);
  _appearanceBondsForm->bondDiffuseIntensityDoubleSlider->setDoubleMaximum(1.0);
  _appearanceBondsForm->bondSpecularIntensityDoubleSlider->setDoubleMinimum(0.0);
  _appearanceBondsForm->bondSpecularIntensityDoubleSlider->setDoubleMaximum(1.0);
  _appearanceBondsForm->bondShininessDoubleSlider->setDoubleMinimum(0.1);
  _appearanceBondsForm->bondShininessDoubleSlider->setDoubleMaximum(128.0);

  _appearanceBondsForm->bondSizeScalingDoubleSlider->setDoubleMinimum(0.0);
  _appearanceBondsForm->bondSizeScalingDoubleSlider->setDoubleMaximum(1.0);

  _appearanceAdsorptionSurfaceForm->adsorptionSurfaceIsovalueDoubleSlider->setDoubleMinimum(-1000.0);
  _appearanceAdsorptionSurfaceForm->adsorptionSurfaceIsovalueDoubleSlider->setDoubleMaximum(1000.0);
  _appearanceAdsorptionSurfaceForm->adsorptionSurfaceIsovalueDoubleSpinBox->setMinimum(-1000.0);
  _appearanceAdsorptionSurfaceForm->adsorptionSurfaceIsovalueDoubleSpinBox->setMaximum(100000.0);
}


void AppearanceTreeWidgetController::setProject(std::shared_ptr<ProjectTreeNode> projectTreeNode)
{
  _projectTreeNode = projectTreeNode;
   _projectStructure = nullptr;
  if (projectTreeNode)
  {
    if(std::shared_ptr<iRASPAProject> iraspaProject = projectTreeNode->representedObject())
    {
      if(std::shared_ptr<Project> project = iraspaProject->project())
      {
        if (std::shared_ptr<ProjectStructure> projectStructure = std::dynamic_pointer_cast<ProjectStructure>(project))
        {
          _projectStructure = projectStructure;
          _iraspa_structures = projectStructure->flattenediRASPAStructures();
          reloadData();
        }
      }
    }
  }
}

void AppearanceTreeWidgetController::setFlattenedSelectedFrames(std::vector<std::shared_ptr<iRASPAStructure>> iraspa_structures)
{
  _iraspa_structures = iraspa_structures;
  reloadData();
}

void AppearanceTreeWidgetController::reloadData()
{
  // refresh the color combobox (new colorsets could have been added)
  whileBlocking(_appearanceAtomsForm->colorSchemeComboBox)->clear();
  if(_mainWindow)
  {
    SKColorSets colorSets = _mainWindow->colorSets();
    for(SKColorSet& colorSet : colorSets.colorSets())
    {
      whileBlocking(_appearanceAtomsForm->colorSchemeComboBox)->addItem(colorSet.displayName());
    }
  }

  // refresh the forcefield combobox (new forcefields could have been added)
  whileBlocking(_appearanceAtomsForm->forceFieldComboBox)->clear();
  if(_mainWindow)
  {
    ForceFieldSets& forceFieldSets = _mainWindow->forceFieldSets();
    for(ForceFieldSet& forceFieldSet : forceFieldSets.forceFieldSets())
    {
      whileBlocking(_appearanceAtomsForm->forceFieldComboBox)->addItem(forceFieldSet.displayName());
    }
  }

  reloadPrimitiveProperties();
  reloadAtomProperties();
  reloadBondProperties();
  reloadUnitCellProperties();
  reloadAdsorptionSurfaceProperties();
  reloadAnnotationProperties();
}

void AppearanceTreeWidgetController::reloadSelection()
{

}

void AppearanceTreeWidgetController::reloadPrimitiveProperties()
{
  reloadRotationAngle();
  reloadEulerAngles();
  reloadTransformationMatrix();
}

void AppearanceTreeWidgetController::reloadRotationAngle()
{
  _appearancePrimitiveForm->rotationAngleDoubleSpinBox->setReadOnly(true);
  if(_projectTreeNode)
  {
    _appearancePrimitiveForm->rotationAngleDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = rotationAngle())
    {
      whileBlocking(_appearancePrimitiveForm->rotationAngleDoubleSpinBox)->setValue(*value);
    }
    else
    {
      whileBlocking(_appearancePrimitiveForm->rotationAngleDoubleSpinBox)->setText("Mult. Val.");
    }
  }

  _appearancePrimitiveForm->rotatePlusXPushButton->setEnabled(false);
  _appearancePrimitiveForm->rotatePlusYPushButton->setEnabled(false);
  _appearancePrimitiveForm->rotatePlusZPushButton->setEnabled(false);
  _appearancePrimitiveForm->rotateMinusXPushButton->setEnabled(false);
  _appearancePrimitiveForm->rotateMinusYPushButton->setEnabled(false);
  _appearancePrimitiveForm->rotateMinusZPushButton->setEnabled(false);
}

void AppearanceTreeWidgetController::reloadEulerAngles()
{
  _appearancePrimitiveForm->EulerAngleXDoubleSpinBox->setReadOnly(true);
  _appearancePrimitiveForm->EulerAngleXDial->setEnabled(false);

  _appearancePrimitiveForm->EulerAngleYDoubleSpinBox->setReadOnly(true);
  _appearancePrimitiveForm->EulerAngleYSlider->setEnabled(false);

  _appearancePrimitiveForm->EulerAngleZDoubleSpinBox->setReadOnly(true);
  _appearancePrimitiveForm->EulerAngleZDial->setEnabled(false);

  if(_projectTreeNode)
  {
    _appearancePrimitiveForm->EulerAngleXDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _appearancePrimitiveForm->EulerAngleXDial->setEnabled(_projectTreeNode->isEditable());

    _appearancePrimitiveForm->EulerAngleYDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _appearancePrimitiveForm->EulerAngleYSlider->setEnabled(_projectTreeNode->isEditable());

    _appearancePrimitiveForm->EulerAngleZDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _appearancePrimitiveForm->EulerAngleZDial->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> size = EulerAngleX())
    {
      whileBlocking(_appearancePrimitiveForm->EulerAngleXDoubleSpinBox)->setValue(*size);
      whileBlocking(_appearancePrimitiveForm->EulerAngleXDial)->setDoubleValue(*size);
    }
    else
    {
      whileBlocking(_appearancePrimitiveForm->EulerAngleXDoubleSpinBox)->setText("Mult. Val.");
    }

    if (std::optional<double> size = EulerAngleY())
    {
      whileBlocking(_appearancePrimitiveForm->EulerAngleYDoubleSpinBox)->setValue(*size);
      whileBlocking(_appearancePrimitiveForm->EulerAngleYSlider)->setDoubleValue(*size);
    }
    else
    {
      whileBlocking(_appearancePrimitiveForm->EulerAngleYDoubleSpinBox)->setText("Mult. Val.");
    }

    if (std::optional<double> size = EulerAngleZ())
    {
      whileBlocking(_appearancePrimitiveForm->EulerAngleZDoubleSpinBox)->setValue(*size);
      whileBlocking(_appearancePrimitiveForm->EulerAngleZDial)->setDoubleValue(*size);
    }
    else
    {
      whileBlocking(_appearancePrimitiveForm->EulerAngleZDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}


void AppearanceTreeWidgetController::reloadTransformationMatrix()
{
  _appearancePrimitiveForm->transformationMatrixAXLineEdit->setReadOnly(true);
  _appearancePrimitiveForm->transformationMatrixAYLineEdit->setReadOnly(true);
  _appearancePrimitiveForm->transformationMatrixAZLineEdit->setReadOnly(true);
  _appearancePrimitiveForm->transformationMatrixBXLineEdit->setReadOnly(true);
  _appearancePrimitiveForm->transformationMatrixBYLineEdit->setReadOnly(true);
  _appearancePrimitiveForm->transformationMatrixBZLineEdit->setReadOnly(true);
  _appearancePrimitiveForm->transformationMatrixCXLineEdit->setReadOnly(true);
  _appearancePrimitiveForm->transformationMatrixCYLineEdit->setReadOnly(true);
  _appearancePrimitiveForm->transformationMatrixCZLineEdit->setReadOnly(true);
  if(_projectTreeNode)
  {
    _appearancePrimitiveForm->transformationMatrixAXLineEdit->setReadOnly(!_projectTreeNode->isEditable());
    _appearancePrimitiveForm->transformationMatrixAYLineEdit->setReadOnly(!_projectTreeNode->isEditable());
    _appearancePrimitiveForm->transformationMatrixAZLineEdit->setReadOnly(!_projectTreeNode->isEditable());
    _appearancePrimitiveForm->transformationMatrixBXLineEdit->setReadOnly(!_projectTreeNode->isEditable());
    _appearancePrimitiveForm->transformationMatrixBYLineEdit->setReadOnly(!_projectTreeNode->isEditable());
    _appearancePrimitiveForm->transformationMatrixBZLineEdit->setReadOnly(!_projectTreeNode->isEditable());
    _appearancePrimitiveForm->transformationMatrixCXLineEdit->setReadOnly(!_projectTreeNode->isEditable());
    _appearancePrimitiveForm->transformationMatrixCYLineEdit->setReadOnly(!_projectTreeNode->isEditable());
    _appearancePrimitiveForm->transformationMatrixCZLineEdit->setReadOnly(!_projectTreeNode->isEditable());
  }
}

void AppearanceTreeWidgetController::reloadOpacity()
{
  _appearancePrimitiveForm->opacityLineEdit->setReadOnly(true);
  _appearancePrimitiveForm->opacitySlider->setEnabled(false);
}

void AppearanceTreeWidgetController::reloadNumberOfSides()
{
  _appearancePrimitiveForm->numberOfSidesLineEdit->setReadOnly(true);
  _appearancePrimitiveForm->numberOfSidesSlider->setEnabled(false);
}

void AppearanceTreeWidgetController::reloadIsCapped()
{
  _appearancePrimitiveForm->cappedCheckBox->setEnabled(false);
}

void AppearanceTreeWidgetController::reloadFrontHDR()
{
  _appearancePrimitiveForm->frontHDRCheckBox->setEnabled(false);
}

void AppearanceTreeWidgetController::reloadFrontExposure()
{
  _appearancePrimitiveForm->frontExposureLineEdit->setReadOnly(true);
  _appearancePrimitiveForm->frontExposureSlider->setEnabled(false);
}

void AppearanceTreeWidgetController::reloadFrontAmbientIntensity()
{
  _appearancePrimitiveForm->frontAmbientIntensityLineEdit->setReadOnly(true);
  _appearancePrimitiveForm->frontAmbientIntensitySlider->setEnabled(false);
}

void AppearanceTreeWidgetController::reloadFrontAmbientColor()
{
  _appearancePrimitiveForm->frontAmbientColorPushButton->setEnabled(false);
}

void AppearanceTreeWidgetController::reloadFrontDiffuseIntensity()
{
  _appearancePrimitiveForm->frontDiffuseIntensityLineEdit->setReadOnly(true);
  _appearancePrimitiveForm->frontDiffuseIntensitySlider->setEnabled(false);
}

void AppearanceTreeWidgetController::reloadFrontDiffuseColor()
{
  _appearancePrimitiveForm->frontDiffuseColorPushButton->setEnabled(false);
}

void AppearanceTreeWidgetController::reloadFrontSpecularIntensity()
{
  _appearancePrimitiveForm->frontSpecularIntensityLineEdit->setReadOnly(true);
  _appearancePrimitiveForm->frontSpecularIntensitySlider->setEnabled(false);
}

void AppearanceTreeWidgetController::reloadFrontSpecularColor()
{
  _appearancePrimitiveForm->frontSpecularColorPushButton->setEnabled(false);
}

void AppearanceTreeWidgetController::reloadFrontShininess()
{
  _appearancePrimitiveForm->frontShininessLineEdit->setReadOnly(true);
  _appearancePrimitiveForm->frontShininessSlider->setEnabled(false);
}

void AppearanceTreeWidgetController::reloadBackHDR()
{
  _appearancePrimitiveForm->backHDRCheckBox->setEnabled(false);
}

void AppearanceTreeWidgetController::reloadBackExposure()
{
  _appearancePrimitiveForm->backExposureLineEdit->setReadOnly(true);
  _appearancePrimitiveForm->backExposureSlider->setEnabled(false);
}

void AppearanceTreeWidgetController::reloadBackAmbientIntensity()
{
  _appearancePrimitiveForm->backAmbientIntensityLineEdit->setReadOnly(true);
  _appearancePrimitiveForm->backAmbientIntensitySlider->setEnabled(false);
}

void AppearanceTreeWidgetController::reloadBackAmbientColor()
{
  _appearancePrimitiveForm->backAmbientColorPushButton->setEnabled(false);
}

void AppearanceTreeWidgetController::reloadBackDiffuseIntensity()
{
  _appearancePrimitiveForm->backDiffuseIntensityLineEdit->setReadOnly(true);
  _appearancePrimitiveForm->backDiffuseIntensitySlider->setEnabled(false);
}

void AppearanceTreeWidgetController::reloadBackDiffuseColor()
{
  _appearancePrimitiveForm->backDiffuseColorPushButton->setEnabled(false);
}

void AppearanceTreeWidgetController::reloadBackSpecularIntensity()
{
  _appearancePrimitiveForm->backSpecularIntensityLineEdit->setReadOnly(true);
  _appearancePrimitiveForm->backSpecularIntensitySlider->setEnabled(false);
}

void AppearanceTreeWidgetController::reloadBackSpecularColor()
{
  _appearancePrimitiveForm->backSpecularColorPushButton->setEnabled(false);
}

void AppearanceTreeWidgetController::reloadBackShininess()
{
  _appearancePrimitiveForm->backShininessLineEdit->setReadOnly(true);
  _appearancePrimitiveForm->backShininessSlider->setEnabled(false);
}

void AppearanceTreeWidgetController::setRatationAngle(double value)
{

}

std::optional<double> AppearanceTreeWidgetController::rotationAngle()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    set.insert(iraspa_structure->structure()->rotationDelta());
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setEulerAngleX(double value)
{

}

std::optional<double> AppearanceTreeWidgetController::EulerAngleX()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }

  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    simd_quatd orientation = iraspa_structure->structure()->primitiveOrientation();
    double3 EulerAngle = orientation.EulerAngles();
    set.insert(EulerAngle.x  * 180.0 / M_PI);
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setEulerAngleY(double value)
{

}

std::optional<double> AppearanceTreeWidgetController::EulerAngleY()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }

  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    simd_quatd orientation = iraspa_structure->structure()->primitiveOrientation();
    double3 EulerAngle = orientation.EulerAngles();
    set.insert(EulerAngle.y * 180.0 / M_PI);
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }
  return std::nullopt;
}


void AppearanceTreeWidgetController::setEulerAngleZ(double value)
{

}

std::optional<double> AppearanceTreeWidgetController::EulerAngleZ()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }

  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    simd_quatd orientation = iraspa_structure->structure()->primitiveOrientation();
    double3 EulerAngle = orientation.EulerAngles();
    set.insert(EulerAngle.z * 180.0 / M_PI);
  }

  if(set.size() == 1)
  {
    return *(set.begin());
  }
  return std::nullopt;
}



// atom properties

void AppearanceTreeWidgetController::reloadAtomProperties()
{
  reloadAtomRepresentationType();
  reloadAtomRepresentationStyle();
  reloadAtomColorSet();
  reloadAtomColorSetOrder();
  reloadAtomForceFieldSet();
  reloadAtomForceFieldSetOrder();
  reloadAtomDrawAtoms();
  reloadAtomSelectionStyle();
  reloadAtomSelectionIntensity();
  reloadAtomSelectionScaling();
  reloadAtomSizeScalingDoubleSpinBox();
  reloadAtomSizeScalingDoubleSlider();
  reloadAtomHighDynamicRange();
  reloadAtomHDRExposure();
  reloadAtomHue();
  reloadAtomSaturation();
  reloadAtomValue();
  reloadAtomAmbientOcclusion();
  reloadAtomAmbientLight();
  reloadAtomDiffuseLight();
  reloadAtomSpecularLight();
  reloadAtomShininess();

  _appearanceAtomsForm->atomAmbientColorPushButton->setEnabled(false);
  _appearanceAtomsForm->atomDiffuseColorPushButton->setEnabled(false);
  _appearanceAtomsForm->atomSpecularColorPushButton->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceAtomsForm->atomAmbientColorPushButton->setEnabled(_projectTreeNode->isEditable());
    _appearanceAtomsForm->atomDiffuseColorPushButton->setEnabled(_projectTreeNode->isEditable());
    _appearanceAtomsForm->atomSpecularColorPushButton->setEnabled(_projectTreeNode->isEditable());
  }
}



void AppearanceTreeWidgetController::reloadBondProperties()
{
  reloadDrawBondsCheckBox();
  reloadBondSizeScaling();
  reloadBondColorMode();
  reloadBondHighDynamicRange();
  reloadBondHDRExposure();
  reloadBondHue();
  reloadBondSaturation();
  reloadBondValue();
  reloadBondAmbientOcclusion();
  reloadBondAmbientLight();
  reloadBondDiffuseLight();
  reloadBondSpecularLight();
  reloadBondShininess();

  _appearanceBondsForm->bondAmbientColorPushButton->setEnabled(false);
  _appearanceBondsForm->bondDiffuseColorPushButton->setEnabled(false);
  _appearanceBondsForm->bondSpecularColorPushButton->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceBondsForm->bondAmbientColorPushButton->setEnabled(_projectTreeNode->isEditable());
    _appearanceBondsForm->bondDiffuseColorPushButton->setEnabled(_projectTreeNode->isEditable());
    _appearanceBondsForm->bondSpecularColorPushButton->setEnabled(_projectTreeNode->isEditable());
  }
}

void AppearanceTreeWidgetController::reloadUnitCellProperties()
{
  reloadDrawUnitCell();
  reloadUnitCellSizeScaling();
  reloadUnitCellDiffuseLight();

  _appearanceUnitCellForm->diffuseColorPushButton->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceUnitCellForm->diffuseColorPushButton->setEnabled(_projectTreeNode->isEditable());
  }
}

void AppearanceTreeWidgetController::reloadAdsorptionSurfaceProperties()
{
  reloadDrawAdsorptionSurfaceCheckBox();
  reloadAdsorptionSurfaceProbeMoleculePopupBox();
  reloadAdsorptionSurfaceIsovalue();
  reloadAdsorptionSurfaceOpacity();
  reloadAdsorptionSurfaceInsideHighDynamicRange();
  reloadAdsorptionSurfaceInsideHDRExposure();
  reloadAdsorptionSurfaceInsideAmbientLight();
  reloadAdsorptionSurfaceInsideDiffuseLight();
  reloadAdsorptionSurfaceInsideSpecularLight();
  reloadAdsorptionSurfaceInsideShininess();
  reloadAdsorptionSurfaceOutsideHighDynamicRange();
  reloadAdsorptionSurfaceOutsideHDRExposure();
  reloadAdsorptionSurfaceOutsideAmbientLight();
  reloadAdsorptionSurfaceOutsideDiffuseLight();
  reloadAdsorptionSurfaceOutsideSpecularLight();
  reloadAdsorptionSurfaceOutsideShininess();

  _appearanceAdsorptionSurfaceForm->insideAmbientColorPushButton->setEnabled(false);
  _appearanceAdsorptionSurfaceForm->insideDiffuseColorPushButton->setEnabled(false);
  _appearanceAdsorptionSurfaceForm->insideSpecularColorPushButton->setEnabled(false);
  _appearanceAdsorptionSurfaceForm->outsideAmbientColorPushButton->setEnabled(false);
  _appearanceAdsorptionSurfaceForm->outsideDiffuseColorPushButton->setEnabled(false);
  _appearanceAdsorptionSurfaceForm->outsideSpecularColorPushButton->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceAdsorptionSurfaceForm->insideAmbientColorPushButton->setEnabled(_projectTreeNode->isEditable());
    _appearanceAdsorptionSurfaceForm->insideDiffuseColorPushButton->setEnabled(_projectTreeNode->isEditable());
    _appearanceAdsorptionSurfaceForm->insideSpecularColorPushButton->setEnabled(_projectTreeNode->isEditable());
    _appearanceAdsorptionSurfaceForm->outsideAmbientColorPushButton->setEnabled(_projectTreeNode->isEditable());
    _appearanceAdsorptionSurfaceForm->outsideDiffuseColorPushButton->setEnabled(_projectTreeNode->isEditable());
    _appearanceAdsorptionSurfaceForm->outsideSpecularColorPushButton->setEnabled(_projectTreeNode->isEditable());
  }
}

void AppearanceTreeWidgetController::reloadAnnotationProperties()
{
  _appearanceAnnotationForm->typeComboBox->setEnabled(false);
  _appearanceAnnotationForm->colorPushButton->setEnabled(false);
  _appearanceAnnotationForm->fontComboBox->setEnabled(false);
  _appearanceAnnotationForm->fontSpecifierComboBox->setEnabled(false);
  _appearanceAnnotationForm->alignmentComboBox->setEnabled(false);
  _appearanceAnnotationForm->styleComboBox->setEnabled(false);
  _appearanceAnnotationForm->scalingLineEdit->setEnabled(false);
  _appearanceAnnotationForm->scalingHorizontalSlider->setEnabled(false);
  _appearanceAnnotationForm->offsetXDoubleSpinBox->setEnabled(false);
  _appearanceAnnotationForm->offsetYDoubleSpinBox->setEnabled(false);
  _appearanceAnnotationForm->offsetZDoubleSpinBox->setEnabled(false);
}

// reload atom properties
//========================================================================================================================================

void AppearanceTreeWidgetController::reloadAtomRepresentationType()
{
  _appearanceAtomsForm->atomRepresentationType->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceAtomsForm->atomRepresentationType->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<Structure::RepresentationType> type=representationType())
    {
      if(int index = _appearanceAtomsForm->atomRepresentationType->findText("Multiple values"); index>=0)
      {
        whileBlocking(_appearanceAtomsForm->atomRepresentationType)->removeItem(index);
      }
      whileBlocking(_appearanceAtomsForm->atomRepresentationType)->setCurrentIndex(int(*type));
    }
    else
    {
      if(int index = _appearanceAtomsForm->atomRepresentationType->findText("Multiple values"); index<0)
      {
        whileBlocking(_appearanceAtomsForm->atomRepresentationType)->addItem("Multiple values");
      }
      whileBlocking(_appearanceAtomsForm->atomRepresentationType)->setCurrentText("Multiple values");
    }
  }
}

void AppearanceTreeWidgetController::reloadAtomRepresentationStyle()
{
  _appearanceAtomsForm->atomRepresentationStyle->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceAtomsForm->atomRepresentationStyle->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<Structure::RepresentationStyle> type=representationStyle())
    {
      if(int index = _appearanceAtomsForm->atomRepresentationStyle->findText("Multiple values"); index>=0)
      {
        whileBlocking(_appearanceAtomsForm->atomRepresentationStyle)->removeItem(index);
      }
      if(int index = _appearanceAtomsForm->atomRepresentationStyle->findText("Custom"); index>=0)
      {
        whileBlocking(_appearanceAtomsForm->atomRepresentationStyle)->removeItem(index);
      }
      if(int(*type)<0)
      {
         whileBlocking(_appearanceAtomsForm->atomRepresentationStyle)->insertItem(int(Structure::RepresentationStyle::multiple_values),"Custom");
         whileBlocking(_appearanceAtomsForm->atomRepresentationStyle)->setCurrentIndex(int(Structure::RepresentationStyle::multiple_values));
      }
      else
      {
        whileBlocking(_appearanceAtomsForm->atomRepresentationStyle)->setCurrentIndex(int(*type));
      }
    }
    else
    {
      if(int index = _appearanceAtomsForm->atomRepresentationStyle->findText("Custom"); index>=0)
      {
        whileBlocking(_appearanceAtomsForm->atomRepresentationStyle)->removeItem(index);
      }
      if(int index = _appearanceAtomsForm->atomRepresentationStyle->findText("Multiple values"); index<0)
      {
        whileBlocking(_appearanceAtomsForm->atomRepresentationStyle)->addItem("Multiple values");
      }
      whileBlocking(_appearanceAtomsForm->atomRepresentationStyle)->setCurrentText("Multiple values");
    }
  }
}

void AppearanceTreeWidgetController::reloadAtomColorSet()
{
  _appearanceAtomsForm->colorSchemeComboBox->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceAtomsForm->colorSchemeComboBox->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<QString> colorSchemName = colorSchemeIdentifier())
    {
      if(int index = _appearanceAtomsForm->colorSchemeComboBox->findText("Multiple values"); index>=0)
      {
        whileBlocking(_appearanceAtomsForm->colorSchemeComboBox)->removeItem(index);
      }
      whileBlocking(_appearanceAtomsForm->colorSchemeComboBox)->setCurrentText(*colorSchemName);
    }
    else
    {
      if(int index = _appearanceAtomsForm->colorSchemeComboBox->findText("Multiple values"); index<0)
      {
        whileBlocking(_appearanceAtomsForm->colorSchemeComboBox)->addItem("Multiple values");
      }
      whileBlocking(_appearanceAtomsForm->colorSchemeComboBox)->setCurrentText("Multiple values");
    }
  }
}

void AppearanceTreeWidgetController::reloadAtomColorSetOrder()
{
  _appearanceAtomsForm->colorSchemeOrderComboBox->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceAtomsForm->colorSchemeOrderComboBox->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<SKColorSet::ColorSchemeOrder> type = colorSchemeOrder())
    {
      if(int index = _appearanceAtomsForm->colorSchemeOrderComboBox->findText("Multiple values"); index>=0)
      {
        whileBlocking(_appearanceAtomsForm->colorSchemeOrderComboBox)->removeItem(index);
      }
      whileBlocking(_appearanceAtomsForm->colorSchemeOrderComboBox)->setCurrentIndex(int(*type));
    }
    else
    {
      if(int index = _appearanceAtomsForm->colorSchemeOrderComboBox->findText("Multiple values"); index<0)
      {
        whileBlocking(_appearanceAtomsForm->colorSchemeOrderComboBox)->addItem("Multiple values");
      }
      whileBlocking(_appearanceAtomsForm->colorSchemeOrderComboBox)->setCurrentText("Multiple values");
    }
  }
}

void AppearanceTreeWidgetController::reloadAtomForceFieldSet()
{
  _appearanceAtomsForm->forceFieldComboBox->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceAtomsForm->forceFieldComboBox->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<QString> forceFieldIdentifier = forceFieldSchemeIdentifier())
    {
      if(int index = _appearanceAtomsForm->forceFieldComboBox->findText("Multiple values"); index>=0)
      {
        whileBlocking(_appearanceAtomsForm->forceFieldComboBox)->removeItem(index);
      }
      whileBlocking(_appearanceAtomsForm->forceFieldComboBox)->setCurrentText(*forceFieldIdentifier);
    }
    else
    {
      if(int index = _appearanceAtomsForm->forceFieldComboBox->findText("Multiple values"); index<0)
      {
        whileBlocking(_appearanceAtomsForm->forceFieldComboBox)->addItem("Multiple values");
      }
      whileBlocking(_appearanceAtomsForm->forceFieldComboBox)->setCurrentText("Multiple values");
    }
  }
}

void AppearanceTreeWidgetController::reloadAtomForceFieldSetOrder()
{
  _appearanceAtomsForm->forceFieldSchemeOrderComboBox->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceAtomsForm->forceFieldSchemeOrderComboBox->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<ForceFieldSet::ForceFieldSchemeOrder> type = forceFieldSchemeOrder())
    {
      if(int index = _appearanceAtomsForm->forceFieldSchemeOrderComboBox->findText("Multiple values"); index>=0)
      {
        whileBlocking(_appearanceAtomsForm->forceFieldSchemeOrderComboBox)->removeItem(index);
      }
      whileBlocking(_appearanceAtomsForm->forceFieldSchemeOrderComboBox)->setCurrentIndex(int(*type));
    }
    else
    {
      if(int index = _appearanceAtomsForm->forceFieldSchemeOrderComboBox->findText("Multiple values"); index<0)
      {
        whileBlocking(_appearanceAtomsForm->forceFieldSchemeOrderComboBox)->addItem("Multiple values");
      }
      whileBlocking(_appearanceAtomsForm->forceFieldSchemeOrderComboBox)->setCurrentText("Multiple values");
    }
  }
}

void AppearanceTreeWidgetController::reloadAtomDrawAtoms()
{
  _appearanceAtomsForm->atomDrawAtomsCheckBox->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceAtomsForm->atomDrawAtomsCheckBox->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<bool> state = atomDrawAtoms())
    {
      whileBlocking(_appearanceAtomsForm->atomDrawAtomsCheckBox)->setTristate(false);
      whileBlocking(_appearanceAtomsForm->atomDrawAtomsCheckBox)->setCheckState(*state ? Qt::Checked : Qt::Unchecked);
    }
    else
    {
      whileBlocking(_appearanceAtomsForm->atomDrawAtomsCheckBox)->setTristate(true);
      whileBlocking(_appearanceAtomsForm->atomDrawAtomsCheckBox)->setCheckState(Qt::PartiallyChecked);
    }
  }
}

void AppearanceTreeWidgetController::reloadAtomSelectionStyle()
{
  _appearanceAtomsForm->atomSelectionStyleComboBox->setEnabled(false);
  _appearanceAtomsForm->atomSelectionStyleNuDoubleSpinBox->setReadOnly(true);
  _appearanceAtomsForm->atomSelectionStyleRhoDoubleSpinBox->setReadOnly(true);
  if(_projectTreeNode)
  {
    _appearanceAtomsForm->atomSelectionStyleComboBox->setEnabled(_projectTreeNode->isEditable());
    _appearanceAtomsForm->atomSelectionStyleNuDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _appearanceAtomsForm->atomSelectionStyleRhoDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<RKSelectionStyle> type = atomSelectionStyle())
    {
      if(int index = _appearanceAtomsForm->atomSelectionStyleComboBox->findText("Multiple values"); index>=0)
      {
        whileBlocking(_appearanceAtomsForm->atomSelectionStyleComboBox)->removeItem(index);
      }
      whileBlocking(_appearanceAtomsForm->atomSelectionStyleComboBox)->setCurrentIndex(int(*type));
    }
    else
    {
      if(int index = _appearanceAtomsForm->atomSelectionStyleComboBox->findText("Multiple values"); index<0)
      {
        whileBlocking(_appearanceAtomsForm->atomSelectionStyleComboBox)->addItem("Multiple values");
      }
      whileBlocking(_appearanceAtomsForm->atomSelectionStyleComboBox)->setCurrentText("Multiple values");
    }

    if (std::optional<double> size = atomSelectionStyleNu())
    {
     whileBlocking(_appearanceAtomsForm->atomSelectionStyleNuDoubleSpinBox)->setValue(*size);
    }
    else
    {
      whileBlocking(_appearanceAtomsForm->atomSelectionStyleNuDoubleSpinBox)->setText("Mult. Val.");
    }

    if (std::optional<double> size = atomSelectionStyleRho())
    {
      whileBlocking(_appearanceAtomsForm->atomSelectionStyleRhoDoubleSpinBox)->setValue(*size);
    }
    else
    {
      whileBlocking(_appearanceAtomsForm->atomSelectionStyleRhoDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}

void AppearanceTreeWidgetController::reloadAtomSelectionIntensity()
{
  _appearanceAtomsForm->atomSelectionItensityDoubleSpinBox->setReadOnly(true);
  _appearanceAtomsForm->atomSelectionItensityDoubleSlider->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceAtomsForm->atomSelectionItensityDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _appearanceAtomsForm->atomSelectionItensityDoubleSlider->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> size = atomSelectionIntensity())
    {
      whileBlocking(_appearanceAtomsForm->atomSelectionItensityDoubleSpinBox)->setValue(*size);
      whileBlocking(_appearanceAtomsForm->atomSelectionItensityDoubleSlider)->setDoubleValue(*size);
    }
    else
    {
      whileBlocking(_appearanceAtomsForm->atomSelectionItensityDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}

void AppearanceTreeWidgetController::reloadAtomSelectionScaling()
{
  _appearanceAtomsForm->atomSelectionScalingDoubleSpinBox->setReadOnly(true);
  _appearanceAtomsForm->atomSelectionScalingDoubleSlider->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceAtomsForm->atomSelectionScalingDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _appearanceAtomsForm->atomSelectionScalingDoubleSlider->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> size = atomSelectionScaling())
    {
      whileBlocking(_appearanceAtomsForm->atomSelectionScalingDoubleSpinBox)->setValue(*size);
      whileBlocking(_appearanceAtomsForm->atomSelectionScalingDoubleSlider)->setDoubleValue(*size);
    }
    else
    {
      whileBlocking(_appearanceAtomsForm->atomSelectionScalingDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}

void AppearanceTreeWidgetController::reloadAtomSizeScalingDoubleSpinBox()
{
  _appearanceAtomsForm->atomSizeScalingSpinBox->setReadOnly(true);
  if(_projectTreeNode)
  {
    _appearanceAtomsForm->atomSizeScalingSpinBox->setReadOnly(!_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> size = atomSizeScaling())
    {
      whileBlocking(_appearanceAtomsForm->atomSizeScalingSpinBox)->setValue(*size);
    }
    else
    {
      whileBlocking(_appearanceAtomsForm->atomSizeScalingSpinBox)->setText("Mult. Val.");
    }
  }
}

void AppearanceTreeWidgetController::reloadAtomSizeScalingDoubleSlider()
{
  _appearanceAtomsForm->atomAtomicSizeScalingSlider->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceAtomsForm->atomAtomicSizeScalingSlider->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> size = atomSizeScaling())
    {
      whileBlocking(_appearanceAtomsForm->atomAtomicSizeScalingSlider)->setDoubleValue(*size);
    }
  }
}


void AppearanceTreeWidgetController::reloadAtomHighDynamicRange()
{
  _appearanceAtomsForm->atomHighDynamicRangeCheckBox->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceAtomsForm->atomHighDynamicRangeCheckBox->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<bool> state = atomHighDynamicRange())
    {
      whileBlocking(_appearanceAtomsForm->atomHighDynamicRangeCheckBox)->setTristate(false);
      whileBlocking(_appearanceAtomsForm->atomHighDynamicRangeCheckBox)->setCheckState(*state ? Qt::Checked : Qt::Unchecked);
    }
    else
    {
      whileBlocking(_appearanceAtomsForm->atomHighDynamicRangeCheckBox)->setTristate(true);
      whileBlocking(_appearanceAtomsForm->atomHighDynamicRangeCheckBox)->setCheckState(Qt::PartiallyChecked);
    }
  }
}

void AppearanceTreeWidgetController::reloadAtomHDRExposure()
{
  _appearanceAtomsForm->atomHDRExposureDoubleSpinBox->setReadOnly(true);
  _appearanceAtomsForm->atomHDRExposureDoubleSlider->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceAtomsForm->atomHDRExposureDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _appearanceAtomsForm->atomHDRExposureDoubleSlider->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = atomHDRExposure())
    {
      whileBlocking(_appearanceAtomsForm->atomHDRExposureDoubleSpinBox)->setValue(*value);
      whileBlocking(_appearanceAtomsForm->atomHDRExposureDoubleSlider)->setDoubleValue(*value);
    }
    else
    {
      whileBlocking(_appearanceAtomsForm->atomHDRExposureDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}

void AppearanceTreeWidgetController::reloadAtomHue()
{
  _appearanceAtomsForm->atomHueDoubleSpinBox->setReadOnly(true);
  _appearanceAtomsForm->atomHueDoubleSlider->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceAtomsForm->atomHueDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _appearanceAtomsForm->atomHueDoubleSlider->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = atomHue())
    {
      whileBlocking(_appearanceAtomsForm->atomHueDoubleSpinBox)->setValue(*value);
      whileBlocking(_appearanceAtomsForm->atomHueDoubleSlider)->setDoubleValue(*value);
    }
    else
    {
      whileBlocking(_appearanceAtomsForm->atomHueDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}

void AppearanceTreeWidgetController::reloadAtomSaturation()
{
  _appearanceAtomsForm->atomSaturationDoubleSpinBox->setReadOnly(true);
  _appearanceAtomsForm->atomSaturationDoubleSlider->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceAtomsForm->atomSaturationDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _appearanceAtomsForm->atomSaturationDoubleSlider->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = atomSaturation())
    {
      whileBlocking(_appearanceAtomsForm->atomSaturationDoubleSpinBox)->setValue(*value);
      whileBlocking(_appearanceAtomsForm->atomSaturationDoubleSlider)->setDoubleValue(*value);
    }
    else
    {
      whileBlocking(_appearanceAtomsForm->atomSaturationDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}

void AppearanceTreeWidgetController::reloadAtomValue()
{
  _appearanceAtomsForm->atomValueDoubleSpinBox->setReadOnly(true);
  _appearanceAtomsForm->atomValueDoubleSlider->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceAtomsForm->atomValueDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _appearanceAtomsForm->atomValueDoubleSlider->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = atomValue())
    {
      whileBlocking(_appearanceAtomsForm->atomValueDoubleSpinBox)->setValue(*value);
      whileBlocking(_appearanceAtomsForm->atomValueDoubleSlider)->setDoubleValue(*value);
    }
    else
    {
      whileBlocking(_appearanceAtomsForm->atomValueDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}

void AppearanceTreeWidgetController::reloadAtomAmbientOcclusion()
{
  _appearanceAtomsForm->atomAmbientOcclusionCheckBox->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceAtomsForm->atomAmbientOcclusionCheckBox->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<bool> state = atomAmbientOcclusion())
    {
      whileBlocking(_appearanceAtomsForm->atomAmbientOcclusionCheckBox)->setTristate(false);
      whileBlocking(_appearanceAtomsForm->atomAmbientOcclusionCheckBox)->setCheckState(*state ? Qt::Checked : Qt::Unchecked);
    }
    else
    {
      whileBlocking(_appearanceAtomsForm->atomAmbientOcclusionCheckBox)->setTristate(true);
      whileBlocking(_appearanceAtomsForm->atomAmbientOcclusionCheckBox)->setCheckState(Qt::PartiallyChecked);
    }
  }
}

void AppearanceTreeWidgetController::reloadAtomAmbientLight()
{
  _appearanceAtomsForm->atomAmbientIntensityDoubleSpinBox->setReadOnly(true);
  _appearanceAtomsForm->atomAmbientIntensityDoubleSlider->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceAtomsForm->atomAmbientIntensityDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _appearanceAtomsForm->atomAmbientIntensityDoubleSlider->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = atomAmbientLightIntensity())
    {
      whileBlocking(_appearanceAtomsForm->atomAmbientIntensityDoubleSpinBox)->setValue(*value);
      whileBlocking(_appearanceAtomsForm->atomAmbientIntensityDoubleSlider)->setDoubleValue(*value);
    }
    else
    {
      whileBlocking(_appearanceAtomsForm->atomAmbientIntensityDoubleSpinBox)->setText("Mult. Val.");
    }

    if (std::optional<QColor> value = atomAmbientLightColor())
    {
      whileBlocking(_appearanceAtomsForm->atomAmbientColorPushButton)->setText("color");
      whileBlocking(_appearanceAtomsForm->atomAmbientColorPushButton)->setColor(*value);
    }
    else
    {
      whileBlocking(_appearanceAtomsForm->atomAmbientColorPushButton)->setColor(QColor(255,255,255,255));
      whileBlocking(_appearanceAtomsForm->atomAmbientColorPushButton)->setText("Mult. Val.");
    }
  }
}

void AppearanceTreeWidgetController::reloadAtomDiffuseLight()
{
  _appearanceAtomsForm->atomDiffuseItensityDoubleSpinBox->setReadOnly(true);
  _appearanceAtomsForm->atomDiffuseIntensityDoubleSlider->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceAtomsForm->atomDiffuseItensityDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _appearanceAtomsForm->atomDiffuseIntensityDoubleSlider->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = atomDiffuseLightIntensity())
    {
      whileBlocking(_appearanceAtomsForm->atomDiffuseItensityDoubleSpinBox)->setValue(*value);
      whileBlocking(_appearanceAtomsForm->atomDiffuseIntensityDoubleSlider)->setDoubleValue(*value);
    }
    else
    {
      whileBlocking(_appearanceAtomsForm->atomDiffuseItensityDoubleSpinBox)->setText("Mult. Val.");
    }

    if (std::optional<QColor> value = atomDiffuseLightColor())
    {
      whileBlocking(_appearanceAtomsForm->atomDiffuseColorPushButton)->setText("color");
      whileBlocking(_appearanceAtomsForm->atomDiffuseColorPushButton)->setColor(*value);
    }
    else
    {
      whileBlocking(_appearanceAtomsForm->atomDiffuseColorPushButton)->setColor(QColor(255,255,255,255));
      whileBlocking(_appearanceAtomsForm->atomDiffuseColorPushButton)->setText("Mult. Val.");
    }
  }
}

void AppearanceTreeWidgetController::reloadAtomSpecularLight()
{
  _appearanceAtomsForm->atomSpecularIntensityDoubleSpinBoxBox->setReadOnly(true);
  _appearanceAtomsForm->atomSpecularIntensityDoubleSlider->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceAtomsForm->atomSpecularIntensityDoubleSpinBoxBox->setReadOnly(!_projectTreeNode->isEditable());
    _appearanceAtomsForm->atomSpecularIntensityDoubleSlider->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = atomSpecularLightIntensity())
    {
      whileBlocking(_appearanceAtomsForm->atomSpecularIntensityDoubleSpinBoxBox)->setValue(*value);
      whileBlocking(_appearanceAtomsForm->atomSpecularIntensityDoubleSlider)->setDoubleValue(*value);
    }
    else
    {
      whileBlocking(_appearanceAtomsForm->atomSpecularIntensityDoubleSpinBoxBox)->setText("Mult. Val.");
    }

    if (std::optional<QColor> value = atomSpecularLightColor())
    {
      whileBlocking(_appearanceAtomsForm->atomSpecularColorPushButton)->setText("color");
      whileBlocking(_appearanceAtomsForm->atomSpecularColorPushButton)->setColor(*value);
    }
    else
    {
      whileBlocking(_appearanceAtomsForm->atomSpecularColorPushButton)->setColor(QColor(255,255,255,255));
      whileBlocking(_appearanceAtomsForm->atomSpecularColorPushButton)->setText("Mult. Val.");
    }
  }
}

void AppearanceTreeWidgetController::reloadAtomShininess()
{
  _appearanceAtomsForm->atomShininessDoubleSpinBox->setReadOnly(true);
  _appearanceAtomsForm->atomShininessDoubleSlider->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceAtomsForm->atomShininessDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _appearanceAtomsForm->atomShininessDoubleSlider->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = atomShininessy())
    {
      whileBlocking(_appearanceAtomsForm->atomShininessDoubleSpinBox)->setValue(*value);
      whileBlocking(_appearanceAtomsForm->atomShininessDoubleSlider)->setDoubleValue(*value);
    }
    else
    {
      whileBlocking(_appearanceAtomsForm->atomShininessDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}



void AppearanceTreeWidgetController::setRepresentationType(int value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    if(value>=0 && value<int(Structure::RepresentationType::multiple_values))
    {
      iraspa_structure->structure()->setRepresentationType(Structure::RepresentationType(value));
      iraspa_structure->structure()->recheckRepresentationStyle();
    }
  }
  reloadAtomProperties();
  reloadBondProperties();
  emit rendererReloadData();
}

std::optional<Structure::RepresentationType> AppearanceTreeWidgetController::representationType()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<Structure::RepresentationType, enum_hash> set = std::unordered_set<Structure::RepresentationType, enum_hash>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    Structure::RepresentationType value = iraspa_structure->structure()->atomRepresentationType();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setRepresentationStyle(int value)
{
  if(value >= 0 && value <= int(Structure::RepresentationStyle::multiple_values))  // also include "Custom"
  {
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      iraspa_structure->structure()->setRepresentationStyle(Structure::RepresentationStyle(value), _mainWindow->colorSets());
    }

    reloadAtomProperties();
    reloadBondProperties();
    emit rendererReloadData();
  }
}

std::optional<Structure::RepresentationStyle> AppearanceTreeWidgetController::representationStyle()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<Structure::RepresentationStyle, enum_hash> set = std::unordered_set<Structure::RepresentationStyle, enum_hash>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    Structure::RepresentationStyle value = iraspa_structure->structure()->atomRepresentationStyle();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setColorSchemeComboBoxIndex(int value)
{
  QString stringValue = _appearanceAtomsForm->colorSchemeComboBox->currentText();
  if(QString::compare(stringValue, "Multiple values") != 0)
  {
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      iraspa_structure->structure()->setRepresentationColorSchemeIdentifier(stringValue, _mainWindow->colorSets());
      iraspa_structure->structure()->recheckRepresentationStyle();
    }
    reloadAtomProperties();
    emit rendererReloadData();
  }
}

std::optional<QString> AppearanceTreeWidgetController::colorSchemeIdentifier()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::set<QString> set = std::set<QString>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    QString value = iraspa_structure->structure()->atomColorSchemeIdentifier();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setColorSchemeOrder(int value)
{
  if(value>=0 && value<int(SKColorSet::ColorSchemeOrder::multiple_values))
  {
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      iraspa_structure->structure()->setColorSchemeOrder(SKColorSet::ColorSchemeOrder(value));
      iraspa_structure->structure()->recheckRepresentationStyle();
    }
    reloadAtomProperties();
    emit rendererReloadData();
  }
}

std::optional<SKColorSet::ColorSchemeOrder> AppearanceTreeWidgetController::colorSchemeOrder()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<SKColorSet::ColorSchemeOrder, enum_hash> set = std::unordered_set<SKColorSet::ColorSchemeOrder, enum_hash>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    SKColorSet::ColorSchemeOrder value = iraspa_structure->structure()->colorSchemeOrder();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setForcefieldSchemeComboBoxIndex(int value)
{
  QString stringValue = _appearanceAtomsForm->forceFieldComboBox->currentText();
  if(QString::compare(stringValue, "Multiple values") != 0)
  {
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      iraspa_structure->structure()->setAtomForceFieldIdentifier(stringValue,_mainWindow->forceFieldSets());
    }
    reloadAtomProperties();

    std::vector<std::shared_ptr<RKRenderStructure>> render_structures{};
    std::transform(_iraspa_structures.begin(),_iraspa_structures.end(),std::back_inserter(render_structures),
                    [](std::shared_ptr<iRASPAStructure> iraspastructure) -> std::shared_ptr<RKRenderStructure> {return iraspastructure->structure();});
    emit invalidateIsosurface(render_structures);
    emit rendererReloadData();
  }
}

std::optional<QString> AppearanceTreeWidgetController::forceFieldSchemeIdentifier()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::set<QString> set = std::set<QString>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    QString value = iraspa_structure->structure()->atomForceFieldIdentifier();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setForceFieldSchemeOrder(int value)
{
  if(value>=0 && value<int(ForceFieldSet::ForceFieldSchemeOrder::multiple_values))
  {
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      iraspa_structure->structure()->setForceFieldSchemeOrder(ForceFieldSet::ForceFieldSchemeOrder(value));
      iraspa_structure->structure()->recheckRepresentationStyle();
    }
    reloadAtomProperties();
    emit rendererReloadData();
  }
}

std::optional<ForceFieldSet::ForceFieldSchemeOrder> AppearanceTreeWidgetController::forceFieldSchemeOrder()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<ForceFieldSet::ForceFieldSchemeOrder, enum_hash> set = std::unordered_set<ForceFieldSet::ForceFieldSchemeOrder, enum_hash>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    ForceFieldSet::ForceFieldSchemeOrder value = iraspa_structure->structure()->forceFieldSchemeOrder();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setAtomSelectionStyle(int value)
{
  if(value>=0 && value<int(RKSelectionStyle::multiple_values))
  {
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      iraspa_structure->structure()->setAtomSelectionStyle(RKSelectionStyle(value));
      iraspa_structure->structure()->recheckRepresentationStyle();
    }
    reloadAtomProperties();
    emit rendererReloadData();
  }
}

std::optional<RKSelectionStyle> AppearanceTreeWidgetController::atomSelectionStyle()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<RKSelectionStyle, enum_hash> set = std::unordered_set<RKSelectionStyle, enum_hash>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    RKSelectionStyle value = iraspa_structure->structure()->atomSelectionStyle();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setAtomSelectionStyleNu(double value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setAtomSelectionFrequency(value);
    iraspa_structure->structure()->recheckRepresentationStyle();
   }
   reloadAtomProperties();
   emit rendererReloadData();
}

std::optional<double> AppearanceTreeWidgetController::atomSelectionStyleNu()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->atomSelectionFrequency();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setAtomSelectionStyleRho(double value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setAtomSelectionDensity(value);
    iraspa_structure->structure()->recheckRepresentationStyle();
   }
   reloadAtomProperties();
   emit rendererReloadData();
}

std::optional<double> AppearanceTreeWidgetController::atomSelectionStyleRho()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->atomSelectionDensity();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setAtomSelectionIntensity(double value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setSelectionIntensity(value);
    iraspa_structure->structure()->recheckRepresentationStyle();
  }
  reloadAtomProperties();
  emit rendererReloadData();
}

std::optional<double> AppearanceTreeWidgetController::atomSelectionIntensity()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->atomSelectionIntensity();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setAtomSelectionScaling(double value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setAtomSelectionScaling(value);
    iraspa_structure->structure()->recheckRepresentationStyle();
  }
  reloadAtomProperties();
  emit rendererReloadData();
}

std::optional<double> AppearanceTreeWidgetController::atomSelectionScaling()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->atomSelectionScaling();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}



void AppearanceTreeWidgetController::setAtomDrawAtoms(int state)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setDrawAtoms(bool(state));
    iraspa_structure->structure()->recheckRepresentationStyle();
   }
   reloadAtomProperties();
   emit rendererReloadData();
}

std::optional<bool> AppearanceTreeWidgetController::atomDrawAtoms()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<bool> set = std::unordered_set<bool>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    bool value = iraspa_structure->structure()->drawAtoms();
    set.insert(value);
  }
   if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setAtomSizeScalingDoubleSpinBox(double size)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setAtomScaleFactor(size);
    iraspa_structure->structure()->recheckRepresentationStyle();
  }
  reloadAtomRepresentationType();
  reloadAtomSizeScalingDoubleSlider();
  reloadAtomRepresentationStyle();

  std::vector<std::shared_ptr<RKRenderStructure>> render_structures{};
  std::transform(_iraspa_structures.begin(),_iraspa_structures.end(),std::back_inserter(render_structures),
                  [](std::shared_ptr<iRASPAStructure> iraspastructure) -> std::shared_ptr<RKRenderStructure> {return iraspastructure->structure();});
  emit invalidateCachedAmbientOcclusionTexture(render_structures);
  emit redrawRendererWithHighQuality();
}

void AppearanceTreeWidgetController::setAtomSizeScalingDoubleSliderBegin()
{
  emit redrawRendererWithLowQuality();
}

void AppearanceTreeWidgetController::setAtomSizeScalingDoubleSliderIntermediate(double size)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setAtomScaleFactor(size);
  }
  reloadAtomSizeScalingDoubleSpinBox();

  emit redrawRendererWithLowQuality();
}

void AppearanceTreeWidgetController::setAtomSizeScalingDoubleSliderFinal()
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->recheckRepresentationStyle();
  }
  reloadAtomRepresentationStyle();
  reloadAtomSizeScalingDoubleSpinBox();

  std::vector<std::shared_ptr<RKRenderStructure>> render_structures{};
  std::transform(_iraspa_structures.begin(),_iraspa_structures.end(),std::back_inserter(render_structures),
                  [](std::shared_ptr<iRASPAStructure> iraspastructure) -> std::shared_ptr<RKRenderStructure> {return iraspastructure->structure();});
  emit invalidateCachedAmbientOcclusionTexture(render_structures);
  emit rendererReloadData();
  emit redrawRendererWithHighQuality();
}

std::optional<double> AppearanceTreeWidgetController::atomSizeScaling()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->atomScaleFactor();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}


void AppearanceTreeWidgetController::setAtomHighDynamicRange(int value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setAtomHDR(bool(value));
    iraspa_structure->structure()->recheckRepresentationStyle();
  }
  reloadAtomProperties();
  emit rendererReloadData();
}

std::optional<bool> AppearanceTreeWidgetController::atomHighDynamicRange()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<bool> set = std::unordered_set<bool>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    bool value = iraspa_structure->structure()->atomHDR();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setAtomHDRExposure(double value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setAtomHDRExposure(value);
    iraspa_structure->structure()->recheckRepresentationStyle();
  }
  reloadAtomProperties();
  emit rendererReloadData();
}

std::optional<double> AppearanceTreeWidgetController::atomHDRExposure()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->atomHDRExposure();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setAtomHue(double value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setAtomHue(value);
    iraspa_structure->structure()->recheckRepresentationStyle();
   }
   reloadAtomProperties();
   emit rendererReloadData();
}

std::optional<double> AppearanceTreeWidgetController::atomHue()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->atomHue();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setAtomSaturation(double value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setAtomSaturation(value);
    iraspa_structure->structure()->recheckRepresentationStyle();
   }
   reloadAtomProperties();
   emit rendererReloadData();
}

std::optional<double> AppearanceTreeWidgetController::atomSaturation()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->atomSaturation();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setAtomValue(double value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setAtomValue(value);
    iraspa_structure->structure()->recheckRepresentationStyle();
  }
  reloadAtomProperties();
  emit rendererReloadData();
}

std::optional<double> AppearanceTreeWidgetController::atomValue()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->atomValue();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setAtomAmbientOcclusion(int value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setAtomAmbientOcclusion(bool(value));
    iraspa_structure->structure()->recheckRepresentationStyle();
  }
  reloadAtomProperties();
  emit rendererReloadData();
}

std::optional<bool> AppearanceTreeWidgetController::atomAmbientOcclusion()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<bool> set = std::unordered_set<bool>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    bool value = iraspa_structure->structure()->atomAmbientOcclusion();
    set.insert(value);
  }
  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setAtomAmbientLightIntensity(double value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setAtomAmbientIntensity(value);
    iraspa_structure->structure()->recheckRepresentationStyle();
   }
   reloadAtomProperties();
   emit rendererReloadData();
}

std::optional<double> AppearanceTreeWidgetController::atomAmbientLightIntensity()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->atomAmbientIntensity();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setAtomAmbientLightColor()
{
  QColor color = QColorDialog::getColor(Qt::white,this,"Choose Color");
  if(color.isValid())
  {
    _appearanceAtomsForm->atomAmbientColorPushButton->setColor(color);
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      iraspa_structure->structure()->setAtomAmbientColor(color);
      iraspa_structure->structure()->recheckRepresentationStyle();
    }
    reloadAtomProperties();
    emit rendererReloadData();
  }
}

std::optional<QColor> AppearanceTreeWidgetController::atomAmbientLightColor()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<QColor> set = std::unordered_set<QColor>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    QColor value = iraspa_structure->structure()->atomAmbientColor();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setAtomDiffuseLightIntensity(double value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setAtomDiffuseIntensity(value);
    iraspa_structure->structure()->recheckRepresentationStyle();
   }
   reloadAtomProperties();
   emit rendererReloadData();
}

std::optional<double> AppearanceTreeWidgetController::atomDiffuseLightIntensity()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->atomDiffuseIntensity();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setAtomDiffuseLightColor()
{
  QColor color = QColorDialog::getColor(Qt::white,this,"Choose Color");
  if(color.isValid())
  {
    _appearanceAtomsForm->atomDiffuseColorPushButton->setColor(color);
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      iraspa_structure->structure()->setAtomDiffuseColor(color);
      iraspa_structure->structure()->recheckRepresentationStyle();
    }
    reloadAtomProperties();
    emit rendererReloadData();
  }
}

std::optional<QColor> AppearanceTreeWidgetController::atomDiffuseLightColor()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<QColor> set = std::unordered_set<QColor>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    QColor value = iraspa_structure->structure()->atomDiffuseColor();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setAtomSpecularLightIntensity(double value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setAtomSpecularIntensity(value);
    iraspa_structure->structure()->recheckRepresentationStyle();
   }
   reloadAtomProperties();
   emit rendererReloadData();
}

std::optional<double> AppearanceTreeWidgetController::atomSpecularLightIntensity()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->atomSpecularIntensity();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setAtomSpecularLightColor()
{
  QColor color = QColorDialog::getColor(Qt::white,this,"Choose Color");
  if(color.isValid())
  {
    _appearanceAtomsForm->atomSpecularColorPushButton->setColor(color);
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      iraspa_structure->structure()->setAtomSpecularColor(color);
      iraspa_structure->structure()->recheckRepresentationStyle();
    }
    reloadAtomProperties();
    emit rendererReloadData();
  }
}

std::optional<QColor> AppearanceTreeWidgetController::atomSpecularLightColor()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<QColor> set = std::unordered_set<QColor>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    QColor value = iraspa_structure->structure()->atomSpecularColor();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setAtomShininess(double value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setAtomShininess(value);
    iraspa_structure->structure()->recheckRepresentationStyle();
   }
   reloadAtomProperties();
   emit rendererReloadData();
}

std::optional<double> AppearanceTreeWidgetController::atomShininessy()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->atomShininess();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

// reload bond surface properties
//========================================================================================================================================



void AppearanceTreeWidgetController::reloadDrawBondsCheckBox()
{
  _appearanceBondsForm->drawBondsCheckBox->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceBondsForm->drawBondsCheckBox->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<bool> state = bondDrawBonds())
    {
      whileBlocking(_appearanceBondsForm->drawBondsCheckBox)->setTristate(false);
      whileBlocking(_appearanceBondsForm->drawBondsCheckBox)->setCheckState(*state ? Qt::Checked : Qt::Unchecked);
    }
    else
    {
      whileBlocking(_appearanceBondsForm->drawBondsCheckBox)->setTristate(true);
      whileBlocking(_appearanceBondsForm->drawBondsCheckBox)->setCheckState(Qt::PartiallyChecked);
    }
  }
}

void AppearanceTreeWidgetController::reloadBondSizeScaling()
{
  _appearanceBondsForm->bondSizeScalingDoubleSpinBox->setReadOnly(true);
  _appearanceBondsForm->bondSizeScalingDoubleSlider->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceBondsForm->bondSizeScalingDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _appearanceBondsForm->bondSizeScalingDoubleSlider->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> size = bondSizeScaling())
    {
      whileBlocking(_appearanceBondsForm->bondSizeScalingDoubleSpinBox)->setValue(*size);
      whileBlocking(_appearanceBondsForm->bondSizeScalingDoubleSlider)->setDoubleValue(*size);
    }
    else
    {
      whileBlocking(_appearanceBondsForm->bondSizeScalingDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}


void AppearanceTreeWidgetController::reloadBondColorMode()
{
  _appearanceBondsForm->bondColorModeComboBox->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceBondsForm->bondColorModeComboBox->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<RKBondColorMode> type=bondColorMode())
    {
      if(int index = _appearanceBondsForm->bondColorModeComboBox->findText("Multiple values"); index>=0)
      {
        whileBlocking(_appearanceBondsForm->bondColorModeComboBox)->removeItem(index);
      }
      whileBlocking(_appearanceBondsForm->bondColorModeComboBox)->setCurrentIndex(int(*type));
    }
    else
    {
      if(int index = _appearanceBondsForm->bondColorModeComboBox->findText("Multiple values"); index<0)
      {
        whileBlocking(_appearanceBondsForm->bondColorModeComboBox)->addItem("Multiple values");
      }
      whileBlocking(_appearanceBondsForm->bondColorModeComboBox)->setCurrentText("Multiple values");
    }
  }
}

void AppearanceTreeWidgetController::reloadBondHighDynamicRange()
{
  _appearanceBondsForm->bondHighDynamicRangeCheckBox->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceBondsForm->bondHighDynamicRangeCheckBox->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<bool> state = bondHighDynamicRange())
    {
      whileBlocking(_appearanceBondsForm->bondHighDynamicRangeCheckBox)->setTristate(false);
      whileBlocking(_appearanceBondsForm->bondHighDynamicRangeCheckBox)->setCheckState(*state ? Qt::Checked : Qt::Unchecked);
    }
    else
    {
      whileBlocking(_appearanceBondsForm->bondHighDynamicRangeCheckBox)->setTristate(true);
      whileBlocking(_appearanceBondsForm->bondHighDynamicRangeCheckBox)->setCheckState(Qt::PartiallyChecked);
    }
  }
}

void AppearanceTreeWidgetController::reloadBondHDRExposure()
{
  _appearanceBondsForm->bondHDRExposureDoubleSpinBox->setReadOnly(true);
  _appearanceBondsForm->bondHDRExposureDoubleSlider->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceBondsForm->bondHDRExposureDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _appearanceBondsForm->bondHDRExposureDoubleSlider->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = bondHDRExposure())
    {
      whileBlocking(_appearanceBondsForm->bondHDRExposureDoubleSpinBox)->setValue(*value);
      whileBlocking(_appearanceBondsForm->bondHDRExposureDoubleSlider)->setDoubleValue(*value);
    }
    else
    {
      whileBlocking(_appearanceBondsForm->bondHDRExposureDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}

void AppearanceTreeWidgetController::reloadBondHue()
{
  _appearanceBondsForm->bondHueDoubleSpinBox->setReadOnly(true);
  _appearanceBondsForm->bondHueDoubleSlider->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceBondsForm->bondHueDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _appearanceBondsForm->bondHueDoubleSlider->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = bondHue())
    {
      whileBlocking(_appearanceBondsForm->bondHueDoubleSpinBox)->setValue(*value);
      whileBlocking(_appearanceBondsForm->bondHueDoubleSlider)->setDoubleValue(*value);
    }
    else
    {
      whileBlocking(_appearanceBondsForm->bondHueDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}

void AppearanceTreeWidgetController::reloadBondSaturation()
{
  _appearanceBondsForm->bondSaturationDoubleSpinBox->setReadOnly(true);
  _appearanceBondsForm->bondSaturationDoubleSlider->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceBondsForm->bondSaturationDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _appearanceBondsForm->bondSaturationDoubleSlider->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = bondSaturation())
    {
      whileBlocking(_appearanceBondsForm->bondSaturationDoubleSpinBox)->setValue(*value);
      whileBlocking(_appearanceBondsForm->bondSaturationDoubleSlider)->setDoubleValue(*value);
    }
    else
    {
      whileBlocking(_appearanceBondsForm->bondSaturationDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}

void AppearanceTreeWidgetController::reloadBondValue()
{
  _appearanceBondsForm->bondValueDoubleSpinBox->setReadOnly(true);
  _appearanceBondsForm->bondValueDoubleSlider->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceBondsForm->bondValueDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _appearanceBondsForm->bondValueDoubleSlider->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = bondValue())
    {
      whileBlocking(_appearanceBondsForm->bondValueDoubleSpinBox)->setValue(*value);
      whileBlocking(_appearanceBondsForm->bondValueDoubleSlider)->setDoubleValue(*value);
    }
    else
    {
      whileBlocking(_appearanceBondsForm->bondValueDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}

void AppearanceTreeWidgetController::reloadBondAmbientOcclusion()
{
  _appearanceBondsForm->bondAmbientOcclusionCheckBox->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceBondsForm->bondAmbientOcclusionCheckBox->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<bool> state = bondAmbientOcclusion())
    {
      whileBlocking(_appearanceBondsForm->bondAmbientOcclusionCheckBox)->setTristate(false);
      whileBlocking(_appearanceBondsForm->bondAmbientOcclusionCheckBox)->setCheckState(*state ? Qt::Checked : Qt::Unchecked);
    }
    else
    {
      whileBlocking(_appearanceBondsForm->bondAmbientOcclusionCheckBox)->setTristate(true);
      whileBlocking(_appearanceBondsForm->bondAmbientOcclusionCheckBox)->setCheckState(Qt::PartiallyChecked);
    }
  }
}

void AppearanceTreeWidgetController::reloadBondAmbientLight()
{
  _appearanceBondsForm->bondAmbientIntensityDoubleSpinBox->setReadOnly(true);
  _appearanceBondsForm->bondAmbientIntensityDoubleSlider->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceBondsForm->bondAmbientIntensityDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _appearanceBondsForm->bondAmbientIntensityDoubleSlider->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = bondAmbientLightIntensity())
    {
      whileBlocking(_appearanceBondsForm->bondAmbientIntensityDoubleSpinBox)->setValue(*value);
      whileBlocking(_appearanceBondsForm->bondAmbientIntensityDoubleSlider)->setDoubleValue(*value);
    }
    else
    {
      whileBlocking(_appearanceBondsForm->bondAmbientIntensityDoubleSpinBox)->setText("Mult. Val.");
    }

    if (std::optional<QColor> value = bondAmbientLightColor())
    {
      whileBlocking(_appearanceBondsForm->bondAmbientColorPushButton)->setText("color");
      whileBlocking(_appearanceBondsForm->bondAmbientColorPushButton)->setColor(*value);
    }
    else
    {
      whileBlocking(_appearanceBondsForm->bondAmbientColorPushButton)->setColor(QColor(255,255,255,255));
      whileBlocking(_appearanceBondsForm->bondAmbientColorPushButton)->setText("Mult. Val.");
    }
  }
}

void AppearanceTreeWidgetController::reloadBondDiffuseLight()
{
  _appearanceBondsForm->bondDiffuseIntensityDoubleSpinBox->setReadOnly(true);
  _appearanceBondsForm->bondDiffuseIntensityDoubleSlider->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceBondsForm->bondDiffuseIntensityDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _appearanceBondsForm->bondDiffuseIntensityDoubleSlider->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = bondDiffuseLightIntensity())
    {
      whileBlocking(_appearanceBondsForm->bondDiffuseIntensityDoubleSpinBox)->setValue(*value);
      whileBlocking(_appearanceBondsForm->bondDiffuseIntensityDoubleSlider)->setDoubleValue(*value);
    }
    else
    {
      whileBlocking(_appearanceBondsForm->bondDiffuseIntensityDoubleSpinBox)->setText("Mult. Val.");
    }

    if (std::optional<QColor> value = bondDiffuseLightColor())
    {
      whileBlocking(_appearanceBondsForm->bondDiffuseColorPushButton)->setText("color");
      whileBlocking(_appearanceBondsForm->bondDiffuseColorPushButton)->setColor(*value);
    }
    else
    {
      whileBlocking(_appearanceBondsForm->bondDiffuseColorPushButton)->setColor(QColor(255,255,255,255));
      whileBlocking(_appearanceBondsForm->bondDiffuseColorPushButton)->setText("Mult. Val.");
    }
  }
}

void AppearanceTreeWidgetController::reloadBondSpecularLight()
{
  _appearanceBondsForm->bondSpecularIntensityDoubleSpinBox->setReadOnly(true);
  _appearanceBondsForm->bondSpecularIntensityDoubleSlider->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceBondsForm->bondSpecularIntensityDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _appearanceBondsForm->bondSpecularIntensityDoubleSlider->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = bondSpecularLightIntensity())
    {
      whileBlocking(_appearanceBondsForm->bondSpecularIntensityDoubleSpinBox)->setValue(*value);
      whileBlocking(_appearanceBondsForm->bondSpecularIntensityDoubleSlider)->setDoubleValue(*value);
    }
    else
    {
      whileBlocking(_appearanceBondsForm->bondSpecularIntensityDoubleSpinBox)->setText("Mult. Val.");
    }

    if (std::optional<QColor> value = bondSpecularLightColor())
    {
      whileBlocking(_appearanceBondsForm->bondSpecularColorPushButton)->setText("color");
      whileBlocking(_appearanceBondsForm->bondSpecularColorPushButton)->setColor(*value);
    }
    else
    {
      whileBlocking(_appearanceBondsForm->bondSpecularColorPushButton)->setColor(QColor(255,255,255,255));
      whileBlocking(_appearanceBondsForm->bondSpecularColorPushButton)->setText("Mult. Val.");
    }
  }
}

void AppearanceTreeWidgetController::reloadBondShininess()
{
  _appearanceBondsForm->bondShininessDoubleSpinBox->setReadOnly(true);
  _appearanceBondsForm->bondShininessDoubleSlider->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceBondsForm->bondShininessDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _appearanceBondsForm->bondShininessDoubleSlider->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = bondShininessy())
    {
      whileBlocking(_appearanceBondsForm->bondShininessDoubleSpinBox)->setValue(*value);
      whileBlocking(_appearanceBondsForm->bondShininessDoubleSlider)->setDoubleValue(*value);
    }
    else
    {
      whileBlocking(_appearanceBondsForm->bondShininessDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}



void AppearanceTreeWidgetController::setBondDrawBonds(int state)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setDrawBonds(bool(state));
    iraspa_structure->structure()->recheckRepresentationStyle();
   }
   reloadAtomProperties();
   emit rendererReloadData();
}

std::optional<bool> AppearanceTreeWidgetController::bondDrawBonds()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<bool> set = std::unordered_set<bool>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    bool value = iraspa_structure->structure()->drawBonds();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setBondSizeScaling(double size)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setBondScaleFactor(size);
    iraspa_structure->structure()->recheckRepresentationStyle();
   }
   reloadAtomProperties();
   reloadBondProperties();
   emit rendererReloadData();
}

std::optional<double> AppearanceTreeWidgetController::bondSizeScaling()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->bondScaleFactor();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setBondColorMode(int value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setBondColorMode(RKBondColorMode(value));
    iraspa_structure->structure()->recheckRepresentationStyle();
   }
   reloadBondProperties();
   emit rendererReloadData();
}

std::optional<RKBondColorMode> AppearanceTreeWidgetController::bondColorMode()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<RKBondColorMode, enum_hash> set = std::unordered_set<RKBondColorMode, enum_hash>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    RKBondColorMode value = iraspa_structure->structure()->bondColorMode();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setBondHighDynamicRange(int value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setBondHDR(bool(value));
    iraspa_structure->structure()->recheckRepresentationStyle();
  }
  reloadAtomProperties();
  emit rendererReloadData();
}

std::optional<bool> AppearanceTreeWidgetController::bondHighDynamicRange()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<bool> set = std::unordered_set<bool>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    bool value = iraspa_structure->structure()->bondHDR();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setBondHDRExposure(double value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setBondHDRExposure(value);
    iraspa_structure->structure()->recheckRepresentationStyle();
  }
  reloadAtomProperties();
  emit rendererReloadData();
}

std::optional<double> AppearanceTreeWidgetController::bondHDRExposure()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->bondHDRExposure();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setBondHue(double value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setBondHue(value);
    iraspa_structure->structure()->recheckRepresentationStyle();
   }
   reloadAtomProperties();
   emit rendererReloadData();
}

std::optional<double> AppearanceTreeWidgetController::bondHue()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->bondHue();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setBondSaturation(double value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setBondSaturation(value);
    iraspa_structure->structure()->recheckRepresentationStyle();
   }
   reloadAtomProperties();
   emit rendererReloadData();
}

std::optional<double> AppearanceTreeWidgetController::bondSaturation()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->bondSaturation();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setBondValue(double value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setBondValue(value);
    iraspa_structure->structure()->recheckRepresentationStyle();
  }
  reloadAtomProperties();
  emit rendererReloadData();
}

std::optional<double> AppearanceTreeWidgetController::bondValue()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->bondValue();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setBondAmbientOcclusion(int value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setBondAmbientOcclusion(bool(value));
    iraspa_structure->structure()->recheckRepresentationStyle();
  }
  reloadAtomProperties();
  emit rendererReloadData();
}

std::optional<bool> AppearanceTreeWidgetController::bondAmbientOcclusion()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<bool> set = std::unordered_set<bool>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    bool value = iraspa_structure->structure()->bondAmbientOcclusion();
    set.insert(value);
  }
  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setBondAmbientLightIntensity(double value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setBondAmbientIntensity(value);
    iraspa_structure->structure()->recheckRepresentationStyle();
   }
   reloadAtomProperties();
   emit rendererReloadData();
}

std::optional<double> AppearanceTreeWidgetController::bondAmbientLightIntensity()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->bondAmbientIntensity();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setBondAmbientLightColor()
{
  QColor color = QColorDialog::getColor(Qt::white,this,"Choose Color");
  if(color.isValid())
  {
    _appearanceBondsForm->bondAmbientColorPushButton->setColor(color);
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      iraspa_structure->structure()->setBondAmbientColor(color);
      iraspa_structure->structure()->recheckRepresentationStyle();
    }
    reloadAtomProperties();
    emit rendererReloadData();
  }
}

std::optional<QColor> AppearanceTreeWidgetController::bondAmbientLightColor()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<QColor> set = std::unordered_set<QColor>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    QColor value = iraspa_structure->structure()->bondAmbientColor();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setBondDiffuseLightIntensity(double value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setBondDiffuseIntensity(value);
    iraspa_structure->structure()->recheckRepresentationStyle();
   }
   reloadAtomProperties();
   emit rendererReloadData();
}

std::optional<double> AppearanceTreeWidgetController::bondDiffuseLightIntensity()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->bondDiffuseIntensity();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setBondDiffuseLightColor()
{
  QColor color = QColorDialog::getColor(Qt::white,this,"Choose Color");
  if(color.isValid())
  {
    _appearanceBondsForm->bondDiffuseColorPushButton->setColor(color);
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      iraspa_structure->structure()->setBondDiffuseColor(color);
      iraspa_structure->structure()->recheckRepresentationStyle();
    }
    reloadAtomProperties();
    emit rendererReloadData();
  }
}

std::optional<QColor> AppearanceTreeWidgetController::bondDiffuseLightColor()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<QColor> set = std::unordered_set<QColor>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    QColor value = iraspa_structure->structure()->bondDiffuseColor();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setBondSpecularLightIntensity(double value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setBondSpecularIntensity(value);
    iraspa_structure->structure()->recheckRepresentationStyle();
   }
   reloadAtomProperties();
   emit rendererReloadData();
}

std::optional<double> AppearanceTreeWidgetController::bondSpecularLightIntensity()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->bondSpecularIntensity();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setBondSpecularLightColor()
{
  QColor color = QColorDialog::getColor(Qt::white,this,"Choose Color");
  if(color.isValid())
  {
    _appearanceBondsForm->bondSpecularColorPushButton->setColor(color);
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      iraspa_structure->structure()->setBondSpecularColor(color);
      iraspa_structure->structure()->recheckRepresentationStyle();
    }
    reloadAtomProperties();
    emit rendererReloadData();
  }
}

std::optional<QColor> AppearanceTreeWidgetController::bondSpecularLightColor()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<QColor> set = std::unordered_set<QColor>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    QColor value = iraspa_structure->structure()->bondSpecularColor();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setBondShininess(double value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setBondShininess(value);
    iraspa_structure->structure()->recheckRepresentationStyle();
   }
   reloadAtomProperties();
   emit rendererReloadData();
}

std::optional<double> AppearanceTreeWidgetController::bondShininessy()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->bondShininess();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}


// reload unit cell properties
//========================================================================================================================================

void AppearanceTreeWidgetController::reloadDrawUnitCell()
{
  _appearanceUnitCellForm->drawUnitCellCheckBox->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceUnitCellForm->drawUnitCellCheckBox->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<bool> state = drawUnitCell())
    {
      whileBlocking(_appearanceUnitCellForm->drawUnitCellCheckBox)->setTristate(false);
      whileBlocking(_appearanceUnitCellForm->drawUnitCellCheckBox)->setCheckState(*state ? Qt::Checked : Qt::Unchecked);
    }
    else
    {
      whileBlocking(_appearanceUnitCellForm->drawUnitCellCheckBox)->setTristate(true);
      whileBlocking(_appearanceUnitCellForm->drawUnitCellCheckBox)->setCheckState(Qt::PartiallyChecked);
    }
  }
}

void AppearanceTreeWidgetController::reloadUnitCellSizeScaling()
{
  _appearanceUnitCellForm->unitCellSizeScalingDoubleSpinBox->setReadOnly(true);
  _appearanceUnitCellForm->unitCellSizeScalingDoubleSlider->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceUnitCellForm->unitCellSizeScalingDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _appearanceUnitCellForm->unitCellSizeScalingDoubleSlider->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> size = unitCellSizeScaling())
    {
      whileBlocking(_appearanceUnitCellForm->unitCellSizeScalingDoubleSpinBox)->setValue(*size);
      whileBlocking(_appearanceUnitCellForm->unitCellSizeScalingDoubleSlider)->setDoubleValue(*size);
    }
    else
    {
      whileBlocking(_appearanceUnitCellForm->unitCellSizeScalingDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}

void AppearanceTreeWidgetController::reloadUnitCellDiffuseLight()
{
  _appearanceUnitCellForm->diffuseIntensityDoubleSpinBox->setReadOnly(true);
  _appearanceUnitCellForm->diffuseIntensityDoubleSlider->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceUnitCellForm->diffuseIntensityDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _appearanceUnitCellForm->diffuseIntensityDoubleSlider->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = unitCellDiffuseLightIntensity())
    {
      whileBlocking(_appearanceUnitCellForm->diffuseIntensityDoubleSpinBox)->setValue(*value);
      whileBlocking(_appearanceUnitCellForm->diffuseIntensityDoubleSlider)->setDoubleValue(*value);
    }
    else
    {
      whileBlocking(_appearanceUnitCellForm->diffuseIntensityDoubleSpinBox)->setText("Mult. Val.");
    }

    if (std::optional<QColor> value = bondDiffuseLightColor())
    {
      whileBlocking(_appearanceUnitCellForm->diffuseColorPushButton)->setText("color");
      whileBlocking(_appearanceUnitCellForm->diffuseColorPushButton)->setColor(*value);
    }
    else
    {
      whileBlocking(_appearanceUnitCellForm->diffuseColorPushButton)->setColor(QColor(255,255,255,255));
      whileBlocking(_appearanceUnitCellForm->diffuseColorPushButton)->setText("Mult. Val.");
    }
  }
}


void AppearanceTreeWidgetController::setDrawUnitCell(int state)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setDrawUnitCell(bool(state));
    iraspa_structure->structure()->recheckRepresentationStyle();
   }
   reloadAtomProperties();
   emit rendererReloadData();
}

std::optional<bool> AppearanceTreeWidgetController::drawUnitCell()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<bool> set = std::unordered_set<bool>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    bool value = iraspa_structure->structure()->drawUnitCell();
    set.insert(value);
  }
  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setUnitCellSizeScaling(double size)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setUnitCellScaleFactor(size);
    iraspa_structure->structure()->recheckRepresentationStyle();
   }
   reloadAtomProperties();
   emit rendererReloadData();
}

std::optional<double> AppearanceTreeWidgetController::unitCellSizeScaling()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->unitCellScaleFactor();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setUnitCellDiffuseLightIntensity(double value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setUnitCellDiffuseIntensity(value);
    iraspa_structure->structure()->recheckRepresentationStyle();
   }
   reloadAtomProperties();
   emit rendererReloadData();
}

std::optional<double> AppearanceTreeWidgetController::unitCellDiffuseLightIntensity()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->unitCellDiffuseIntensity();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setUnitCellDiffuseLightColor()
{
  QColor color = QColorDialog::getColor(Qt::white,this,"Choose Color");
  if(color.isValid())
  {
    _appearanceUnitCellForm->diffuseColorPushButton->setColor(color);
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      iraspa_structure->structure()->setUnitCellDiffuseColor(color);
      iraspa_structure->structure()->recheckRepresentationStyle();
    }
    reloadAtomProperties();
    emit rendererReloadData();
  }
}

std::optional<QColor> AppearanceTreeWidgetController::unitCellDiffuseLightColor()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<QColor> set = std::unordered_set<QColor>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    QColor value = iraspa_structure->structure()->unitCellDiffuseColor();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}


// reload Adsorption surface properties
//========================================================================================================================================

void AppearanceTreeWidgetController::reloadDrawAdsorptionSurfaceCheckBox()
{
  _appearanceAdsorptionSurfaceForm->adsorptionSurfaceCheckBox->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceAdsorptionSurfaceForm->adsorptionSurfaceCheckBox->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<bool> state = drawAdsorptionSurface())
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->adsorptionSurfaceCheckBox)->setTristate(false);
      whileBlocking(_appearanceAdsorptionSurfaceForm->adsorptionSurfaceCheckBox)->setCheckState(*state ? Qt::Checked : Qt::Unchecked);
    }
    else
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->adsorptionSurfaceCheckBox)->setTristate(true);
      whileBlocking(_appearanceAdsorptionSurfaceForm->adsorptionSurfaceCheckBox)->setCheckState(Qt::PartiallyChecked);
    }
  }
}

void AppearanceTreeWidgetController::reloadAdsorptionSurfaceProbeMoleculePopupBox()
{
  _appearanceAdsorptionSurfaceForm->probeParticleComboBox->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceAdsorptionSurfaceForm->probeParticleComboBox->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<Structure::ProbeMolecule> type=adsorptionSurfaceProbeMolecule())
    {
      if(int index = _appearanceAdsorptionSurfaceForm->probeParticleComboBox->findText("Mult. Val."); index>=0)
      {
        whileBlocking(_appearanceAdsorptionSurfaceForm->probeParticleComboBox)->removeItem(index);
      }
      if(int(*type)<0)
      {
       whileBlocking(_appearanceAdsorptionSurfaceForm->probeParticleComboBox)->setCurrentIndex(int(Structure::ProbeMolecule::multiple_values));
      }
      else
      {
        whileBlocking(_appearanceAdsorptionSurfaceForm->probeParticleComboBox)->setCurrentIndex(int(*type));
      }
    }
    else
    {
      if(int index = _appearanceAdsorptionSurfaceForm->probeParticleComboBox->findText("Mult. Val."); index<0)
      {
        whileBlocking(_appearanceAdsorptionSurfaceForm->probeParticleComboBox)->addItem("Mult. Val.");
      }
      whileBlocking(_appearanceAdsorptionSurfaceForm->probeParticleComboBox)->setCurrentText("Mult. Val.");
    }
  }
}

void AppearanceTreeWidgetController::reloadAdsorptionSurfaceIsovalue()
{
  _appearanceAdsorptionSurfaceForm->adsorptionSurfaceIsovalueDoubleSpinBox->setReadOnly(true);
  _appearanceAdsorptionSurfaceForm->adsorptionSurfaceIsovalueDoubleSlider->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceAdsorptionSurfaceForm->adsorptionSurfaceIsovalueDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _appearanceAdsorptionSurfaceForm->adsorptionSurfaceIsovalueDoubleSlider->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = adsorptionSurfaceMinimumValue())
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->adsorptionSurfaceIsovalueDoubleSlider)->setDoubleMinimum(*value);
      whileBlocking(_appearanceAdsorptionSurfaceForm->adsorptionSurfaceIsovalueDoubleSpinBox)->setMinimum(*value);
    }

    if (std::optional<double> value = adsorptionSurfaceIsovalue())
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->adsorptionSurfaceIsovalueDoubleSpinBox)->setValue(*value);
      whileBlocking(_appearanceAdsorptionSurfaceForm->adsorptionSurfaceIsovalueDoubleSlider)->setDoubleValue(*value);
    }
    else
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->adsorptionSurfaceIsovalueDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}

void AppearanceTreeWidgetController::reloadAdsorptionSurfaceOpacity()
{
  _appearanceAdsorptionSurfaceForm->adsorptionSurfaceOpacityDoubleSpinBox->setReadOnly(true);
  _appearanceAdsorptionSurfaceForm->adsorptionSurfaceOpacityDoubleSlider->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceAdsorptionSurfaceForm->adsorptionSurfaceOpacityDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _appearanceAdsorptionSurfaceForm->adsorptionSurfaceOpacityDoubleSlider->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = adsorptionSurfaceOpacity())
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->adsorptionSurfaceOpacityDoubleSpinBox)->setValue(*value);
      whileBlocking(_appearanceAdsorptionSurfaceForm->adsorptionSurfaceOpacityDoubleSlider)->setDoubleValue(*value);
    }
    else
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->adsorptionSurfaceOpacityDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}

void AppearanceTreeWidgetController::reloadAdsorptionSurfaceInsideHighDynamicRange()
{
  _appearanceAdsorptionSurfaceForm->insideHighDynamicRangecheckBox->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceAdsorptionSurfaceForm->insideHighDynamicRangecheckBox->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<bool> state = adsorptionSurfaceInsideHighDynamicRange())
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->insideHighDynamicRangecheckBox)->setTristate(false);
      whileBlocking(_appearanceAdsorptionSurfaceForm->insideHighDynamicRangecheckBox)->setCheckState(*state ? Qt::Checked : Qt::Unchecked);
    }
    else
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->insideHighDynamicRangecheckBox)->setTristate(true);
      whileBlocking(_appearanceAdsorptionSurfaceForm->insideHighDynamicRangecheckBox)->setCheckState(Qt::PartiallyChecked);
    }
  }
}

void AppearanceTreeWidgetController::reloadAdsorptionSurfaceInsideHDRExposure()
{
  _appearanceAdsorptionSurfaceForm->insideHDRExposureDoubleSpinBox->setReadOnly(true);
  _appearanceAdsorptionSurfaceForm->insideHDRExposureDoubleSlider->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceAdsorptionSurfaceForm->insideHDRExposureDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _appearanceAdsorptionSurfaceForm->insideHDRExposureDoubleSlider->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = adsorptionSurfaceInsideHDRExposure())
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->insideHDRExposureDoubleSpinBox)->setValue(*value);
      whileBlocking(_appearanceAdsorptionSurfaceForm->insideHDRExposureDoubleSlider)->setDoubleValue(*value);
    }
    else
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->insideHDRExposureDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}

void AppearanceTreeWidgetController::reloadAdsorptionSurfaceInsideAmbientLight()
{
  _appearanceAdsorptionSurfaceForm->insideAmbientLightIntensityDoubleSpinBox->setReadOnly(true);
  _appearanceAdsorptionSurfaceForm->insideAmbientLightIntensityDoubleSlider->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceAdsorptionSurfaceForm->insideAmbientLightIntensityDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _appearanceAdsorptionSurfaceForm->insideAmbientLightIntensityDoubleSlider->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = adsorptionSurfaceInsideAmbientLightIntensity())
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->insideAmbientLightIntensityDoubleSpinBox)->setValue(*value);
      whileBlocking(_appearanceAdsorptionSurfaceForm->insideAmbientLightIntensityDoubleSlider)->setDoubleValue(*value);
    }
    else
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->insideAmbientLightIntensityDoubleSpinBox)->setText("Mult. Val.");
    }

    if (std::optional<QColor> value = adsorptionSurfaceInsideAmbientLightColor())
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->insideAmbientColorPushButton)->setText("color");
      whileBlocking(_appearanceAdsorptionSurfaceForm->insideAmbientColorPushButton)->setColor(*value);
    }
    else
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->insideAmbientColorPushButton)->setColor(QColor(255,255,255,255));
      whileBlocking(_appearanceAdsorptionSurfaceForm->insideAmbientColorPushButton)->setText("Mult. Val.");
    }
  }
}

void AppearanceTreeWidgetController::reloadAdsorptionSurfaceInsideDiffuseLight()
{
  _appearanceAdsorptionSurfaceForm->insideDiffuseLightIntensityDoubleSpinBox->setReadOnly(true);
  _appearanceAdsorptionSurfaceForm->insideDiffuseLightIntensityDoubleSlider->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceAdsorptionSurfaceForm->insideDiffuseLightIntensityDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _appearanceAdsorptionSurfaceForm->insideDiffuseLightIntensityDoubleSlider->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = adsorptionSurfaceInsideDiffuseLightIntensity())
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->insideDiffuseLightIntensityDoubleSpinBox)->setValue(*value);
      whileBlocking(_appearanceAdsorptionSurfaceForm->insideDiffuseLightIntensityDoubleSlider)->setDoubleValue(*value);
    }
    else
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->insideDiffuseLightIntensityDoubleSpinBox)->setText("Mult. Val.");
    }

    if (std::optional<QColor> value = adsorptionSurfaceInsideDiffuseLightColor())
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->insideDiffuseColorPushButton)->setText("color");
      whileBlocking(_appearanceAdsorptionSurfaceForm->insideDiffuseColorPushButton)->setColor(*value);
    }
    else
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->insideDiffuseColorPushButton)->setColor(QColor(255,255,255,255));
      whileBlocking(_appearanceAdsorptionSurfaceForm->insideDiffuseColorPushButton)->setText("Mult. Val.");
    }
  }
}

void AppearanceTreeWidgetController::reloadAdsorptionSurfaceInsideSpecularLight()
{
  _appearanceAdsorptionSurfaceForm->insideSpecularLightIntensityDoubleSpinBox->setReadOnly(true);
  _appearanceAdsorptionSurfaceForm->insideSpecularLightIntensityDoubleSlider->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceAdsorptionSurfaceForm->insideSpecularLightIntensityDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _appearanceAdsorptionSurfaceForm->insideSpecularLightIntensityDoubleSlider->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = adsorptionSurfaceInsideSpecularLightIntensity())
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->insideSpecularLightIntensityDoubleSpinBox)->setValue(*value);
      whileBlocking(_appearanceAdsorptionSurfaceForm->insideSpecularLightIntensityDoubleSlider)->setDoubleValue(*value);
    }
    else
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->insideSpecularLightIntensityDoubleSpinBox)->setText("Mult. Val.");
    }

    if (std::optional<QColor> value = adsorptionSurfaceInsideSpecularLightColor())
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->insideSpecularColorPushButton)->setText("color");
      whileBlocking(_appearanceAdsorptionSurfaceForm->insideSpecularColorPushButton)->setColor(*value);
    }
    else
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->insideSpecularColorPushButton)->setColor(QColor(255,255,255,255));
      whileBlocking(_appearanceAdsorptionSurfaceForm->insideSpecularColorPushButton)->setText("Mult. Val.");
    }
  }
}

void AppearanceTreeWidgetController::reloadAdsorptionSurfaceInsideShininess()
{
  _appearanceAdsorptionSurfaceForm->insideShininessDoubleSpinBox->setReadOnly(true);
  _appearanceAdsorptionSurfaceForm->insideShininessDoubleSlider->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceAdsorptionSurfaceForm->insideShininessDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _appearanceAdsorptionSurfaceForm->insideShininessDoubleSlider->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = adsorptionSurfaceInsideShininessy())
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->insideShininessDoubleSpinBox)->setValue(*value);
      whileBlocking(_appearanceAdsorptionSurfaceForm->insideShininessDoubleSlider)->setDoubleValue(*value);
    }
    else
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->insideShininessDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}

void AppearanceTreeWidgetController::reloadAdsorptionSurfaceOutsideHighDynamicRange()
{
  _appearanceAdsorptionSurfaceForm->outsideHighDynamicRangecheckBox->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceAdsorptionSurfaceForm->outsideHighDynamicRangecheckBox->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<bool> state = adsorptionSurfaceOutsideHighDynamicRange())
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->outsideHighDynamicRangecheckBox)->setTristate(false);
      whileBlocking(_appearanceAdsorptionSurfaceForm->outsideHighDynamicRangecheckBox)->setCheckState(*state ? Qt::Checked : Qt::Unchecked);
    }
    else
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->outsideHighDynamicRangecheckBox)->setTristate(true);
      whileBlocking(_appearanceAdsorptionSurfaceForm->outsideHighDynamicRangecheckBox)->setCheckState(Qt::PartiallyChecked);
    }
  }
}

void AppearanceTreeWidgetController::reloadAdsorptionSurfaceOutsideHDRExposure()
{
  _appearanceAdsorptionSurfaceForm->outsideHDRExposureDoubleSpinBox->setReadOnly(true);
  _appearanceAdsorptionSurfaceForm->outsideHDRExposureDoubleSlider->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceAdsorptionSurfaceForm->outsideHDRExposureDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _appearanceAdsorptionSurfaceForm->outsideHDRExposureDoubleSlider->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = adsorptionSurfaceOutsideHDRExposure())
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->outsideHDRExposureDoubleSpinBox)->setValue(*value);
      whileBlocking(_appearanceAdsorptionSurfaceForm->outsideHDRExposureDoubleSlider)->setDoubleValue(*value);
    }
    else
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->outsideHDRExposureDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}

void AppearanceTreeWidgetController::reloadAdsorptionSurfaceOutsideAmbientLight()
{
  _appearanceAdsorptionSurfaceForm->outsideAmbientLightIntensityDoubleSpinBox->setReadOnly(true);
  _appearanceAdsorptionSurfaceForm->outsideAmbientLightIntensityDoubleSlider->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceAdsorptionSurfaceForm->outsideAmbientLightIntensityDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _appearanceAdsorptionSurfaceForm->outsideAmbientLightIntensityDoubleSlider->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = adsorptionSurfaceOutsideAmbientLightIntensity())
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->outsideAmbientLightIntensityDoubleSpinBox)->setValue(*value);
      whileBlocking(_appearanceAdsorptionSurfaceForm->outsideAmbientLightIntensityDoubleSlider)->setDoubleValue(*value);
    }
    else
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->outsideAmbientLightIntensityDoubleSpinBox)->setText("Mult. Val.");
    }

    if (std::optional<QColor> value = adsorptionSurfaceOutsideAmbientLightColor())
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->outsideAmbientColorPushButton)->setText("color");
      whileBlocking(_appearanceAdsorptionSurfaceForm->outsideAmbientColorPushButton)->setColor(*value);
    }
    else
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->outsideAmbientColorPushButton)->setColor(QColor(255,255,255,255));
      whileBlocking(_appearanceAdsorptionSurfaceForm->outsideAmbientColorPushButton)->setText("Mult. Val.");
    }
  }
}

void AppearanceTreeWidgetController::reloadAdsorptionSurfaceOutsideDiffuseLight()
{
  _appearanceAdsorptionSurfaceForm->outsideDiffuseLightIntensityDoubleSpinBox->setReadOnly(true);
  _appearanceAdsorptionSurfaceForm->outsideDiffuseLightIntensityDoubleSlider->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceAdsorptionSurfaceForm->outsideDiffuseLightIntensityDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _appearanceAdsorptionSurfaceForm->outsideDiffuseLightIntensityDoubleSlider->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = adsorptionSurfaceOutsideDiffuseLightIntensity())
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->outsideDiffuseLightIntensityDoubleSpinBox)->setValue(*value);
      whileBlocking(_appearanceAdsorptionSurfaceForm->outsideDiffuseLightIntensityDoubleSlider)->setDoubleValue(*value);
    }
    else
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->outsideDiffuseLightIntensityDoubleSpinBox)->setText("Mult. Val.");
    }

    if (std::optional<QColor> value = adsorptionSurfaceOutsideDiffuseLightColor())
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->outsideDiffuseColorPushButton)->setText("color");
      whileBlocking(_appearanceAdsorptionSurfaceForm->outsideDiffuseColorPushButton)->setColor(*value);
    }
    else
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->outsideDiffuseColorPushButton)->setColor(QColor(255,255,255,255));
      whileBlocking(_appearanceAdsorptionSurfaceForm->outsideDiffuseColorPushButton)->setText("Mult. Val.");
    }
  }
}

void AppearanceTreeWidgetController::reloadAdsorptionSurfaceOutsideSpecularLight()
{
  _appearanceAdsorptionSurfaceForm->outsideSpecularLightIntensityDoubleSpinBox->setReadOnly(true);
  _appearanceAdsorptionSurfaceForm->outsideSpecularLightIntensityDoubleSlider->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceAdsorptionSurfaceForm->outsideSpecularLightIntensityDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _appearanceAdsorptionSurfaceForm->outsideSpecularLightIntensityDoubleSlider->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = adsorptionSurfaceOutsideSpecularLightIntensity())
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->outsideSpecularLightIntensityDoubleSpinBox)->setValue(*value);
      whileBlocking(_appearanceAdsorptionSurfaceForm->outsideSpecularLightIntensityDoubleSlider)->setDoubleValue(*value);
    }
    else
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->outsideSpecularLightIntensityDoubleSpinBox)->setText("Mult. Val.");
    }

    if (std::optional<QColor> value = adsorptionSurfaceOutsideSpecularLightColor())
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->outsideSpecularColorPushButton)->setText("color");
      whileBlocking(_appearanceAdsorptionSurfaceForm->outsideSpecularColorPushButton)->setColor(*value);
    }
    else
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->outsideSpecularColorPushButton)->setColor(QColor(255,255,255,255));
      whileBlocking(_appearanceAdsorptionSurfaceForm->outsideSpecularColorPushButton)->setText("Mult. Val.");
    }
  }
}

void AppearanceTreeWidgetController::reloadAdsorptionSurfaceOutsideShininess()
{
  _appearanceAdsorptionSurfaceForm->outsideShininessDoubleSpinBox->setReadOnly(true);
  _appearanceAdsorptionSurfaceForm->outsideShininessDoubleSlider->setEnabled(false);
  if(_projectTreeNode)
  {
    _appearanceAdsorptionSurfaceForm->outsideShininessDoubleSpinBox->setReadOnly(!_projectTreeNode->isEditable());
    _appearanceAdsorptionSurfaceForm->outsideShininessDoubleSlider->setEnabled(_projectTreeNode->isEditable());
  }

  if(!_iraspa_structures.empty())
  {
    if (std::optional<double> value = adsorptionSurfaceOutsideShininessy())
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->outsideShininessDoubleSpinBox)->setValue(*value);
      whileBlocking(_appearanceAdsorptionSurfaceForm->outsideShininessDoubleSlider)->setDoubleValue(*value);
    }
    else
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->outsideShininessDoubleSpinBox)->setText("Mult. Val.");
    }
  }
}

void AppearanceTreeWidgetController::setDrawAdsorptionSurface(int state)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setDrawAdsorptionSurface(bool(state));
    iraspa_structure->structure()->recheckRepresentationStyle();
   }
   reloadAtomProperties();
   emit rendererReloadData();
}

std::optional<bool> AppearanceTreeWidgetController::drawAdsorptionSurface()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<bool> set = std::unordered_set<bool>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    bool value = iraspa_structure->structure()->drawAdsorptionSurface();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setAdsorptionSurfaceProbeMolecule(int value)
{
  if(value>=0 && value<int(Structure::ProbeMolecule::multiple_values))
  {
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      iraspa_structure->structure()->setAdsorptionSurfaceProbeMolecule(Structure::ProbeMolecule(value));
      iraspa_structure->structure()->recheckRepresentationStyle();
    }

    std::vector<std::shared_ptr<RKRenderStructure>> render_structures{};
    std::transform(_iraspa_structures.begin(),_iraspa_structures.end(),std::back_inserter(render_structures),
                    [](std::shared_ptr<iRASPAStructure> iraspastructure) -> std::shared_ptr<RKRenderStructure> {return iraspastructure->structure();});
    emit invalidateIsosurface(render_structures);

    emit rendererReloadData();

    reloadAdsorptionSurfaceIsovalue();
  }
}

std::optional<Structure::ProbeMolecule> AppearanceTreeWidgetController::adsorptionSurfaceProbeMolecule()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<Structure::ProbeMolecule, enum_hash> set = std::unordered_set<Structure::ProbeMolecule, enum_hash>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    Structure::ProbeMolecule value = iraspa_structure->structure()->adsorptionSurfaceProbeMolecule();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setAdsorptionSurfaceIsovalue(double value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setAdsorptionSurfaceIsoValue(value);
  }
  reloadAdsorptionSurfaceProperties();
  emit rendererReloadData();
}

std::optional<double> AppearanceTreeWidgetController::adsorptionSurfaceIsovalue()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->adsorptionSurfaceIsoValue();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}


std::optional<double> AppearanceTreeWidgetController::adsorptionSurfaceMinimumValue()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->adsorptionSurfaceMinimumValue();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}


void AppearanceTreeWidgetController::setAdsorptionSurfaceOpacity(double value)
{
  if(value>=0 && value<=1.0)
  {
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      iraspa_structure->structure()->setAdsorptionSurfaceOpacity(value);
    }
    reloadAdsorptionSurfaceProperties();
    emit rendererReloadData();
  }
}

std::optional<double> AppearanceTreeWidgetController::adsorptionSurfaceOpacity()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->adsorptionSurfaceOpacity();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setAdsorptionSurfaceInsideHighDynamicRange(int value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setAdsorptionSurfaceFrontSideHDR(bool(value));
  }
  reloadAdsorptionSurfaceProperties();
  emit rendererReloadData();
}

std::optional<bool> AppearanceTreeWidgetController::adsorptionSurfaceInsideHighDynamicRange()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<bool> set = std::unordered_set<bool>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    bool state = iraspa_structure->structure()->adsorptionSurfaceFrontSideHDR();
    set.insert(state);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setAdsorptionSurfaceInsideHDRExposure(double value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setAdsorptionSurfaceFrontSideHDRExposure(value);
  }
  reloadAdsorptionSurfaceProperties();
  emit rendererReloadData();
}

std::optional<double> AppearanceTreeWidgetController::adsorptionSurfaceInsideHDRExposure()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->adsorptionSurfaceFrontSideHDRExposure();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setAdsorptionSurfaceInsideAmbientLightIntensity(double value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setAdsorptionSurfaceFrontSideAmbientIntensity(value);
  }
  reloadAdsorptionSurfaceProperties();
  emit rendererReloadData();
}

std::optional<double> AppearanceTreeWidgetController::adsorptionSurfaceInsideAmbientLightIntensity()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->adsorptionSurfaceFrontSideAmbientIntensity();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setAdsorptionSurfaceInsideAmbientLightColor()
{
  QColor color = QColorDialog::getColor(Qt::white,this,"Choose Color");
  if(color.isValid())
  {
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      iraspa_structure->structure()->setAdsorptionSurfaceFrontSideAmbientColor(color);
    }
    reloadAdsorptionSurfaceProperties();
    emit rendererReloadData();
  }
}

std::optional<QColor> AppearanceTreeWidgetController::adsorptionSurfaceInsideAmbientLightColor()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<QColor> set = std::unordered_set<QColor>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    QColor color = iraspa_structure->structure()->adsorptionSurfaceFrontSideAmbientColor();
    set.insert(color);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setAdsorptionSurfaceInsideDiffuseLightIntensity(double value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setAdsorptionSurfaceFrontSideDiffuseIntensity(value);
  }
  reloadAdsorptionSurfaceProperties();
  emit rendererReloadData();
}

std::optional<double> AppearanceTreeWidgetController::adsorptionSurfaceInsideDiffuseLightIntensity()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->adsorptionSurfaceFrontSideDiffuseIntensity();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setAdsorptionSurfaceInsideDiffuseLightColor()
{
  QColor color = QColorDialog::getColor(Qt::white,this,"Choose Color");
  if(color.isValid())
  {
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      iraspa_structure->structure()->setAdsorptionSurfaceFrontSideDiffuseColor(color);
    }
    reloadAdsorptionSurfaceProperties();
    emit rendererReloadData();
  }
}

std::optional<QColor> AppearanceTreeWidgetController::adsorptionSurfaceInsideDiffuseLightColor()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<QColor> set = std::unordered_set<QColor>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    QColor color = iraspa_structure->structure()->adsorptionSurfaceFrontSideDiffuseColor();
    set.insert(color);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setAdsorptionSurfaceInsideSpecularLightIntensity(double value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setAdsorptionSurfaceFrontSideSpecularIntensity(value);
  }
  reloadAdsorptionSurfaceProperties();
  emit rendererReloadData();
}

std::optional<double> AppearanceTreeWidgetController::adsorptionSurfaceInsideSpecularLightIntensity()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->adsorptionSurfaceFrontSideSpecularIntensity();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setAdsorptionSurfaceInsideSpecularLightColor()
{
  QColor color = QColorDialog::getColor(Qt::white,this,"Choose Color");
  if(color.isValid())
  {
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      iraspa_structure->structure()->setAdsorptionSurfaceFrontSideSpecularColor(color);
    }
    reloadAdsorptionSurfaceProperties();
    emit rendererReloadData();
  }
}

std::optional<QColor> AppearanceTreeWidgetController::adsorptionSurfaceInsideSpecularLightColor()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<QColor> set = std::unordered_set<QColor>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    QColor color = iraspa_structure->structure()->adsorptionSurfaceFrontSideSpecularColor();
    set.insert(color);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setAdsorptionSurfaceInsideShininess(double value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setAdsorptionSurfaceFrontSideShininess(value);
  }
  reloadAdsorptionSurfaceProperties();
  emit rendererReloadData();
}

std::optional<double> AppearanceTreeWidgetController::adsorptionSurfaceInsideShininessy()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->adsorptionSurfaceFrontSideShininess();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setAdsorptionSurfaceOutsideHighDynamicRange(int value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setAdsorptionSurfaceBackSideHDR(bool(value));
  }
  reloadAdsorptionSurfaceProperties();
  emit rendererReloadData();
}

std::optional<bool> AppearanceTreeWidgetController::adsorptionSurfaceOutsideHighDynamicRange()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<bool> set = std::unordered_set<bool>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    bool value = iraspa_structure->structure()->adsorptionSurfaceBackSideHDR();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setAdsorptionSurfaceOutsideHDRExposure(double value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setAdsorptionSurfaceBackSideHDRExposure(value);
  }
  reloadAdsorptionSurfaceProperties();
  emit rendererReloadData();
}

std::optional<double> AppearanceTreeWidgetController::adsorptionSurfaceOutsideHDRExposure()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->adsorptionSurfaceBackSideHDRExposure();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setAdsorptionSurfaceOutsideAmbientLightIntensity(double value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setAdsorptionSurfaceBackSideAmbientIntensity(value);
  }
  reloadAdsorptionSurfaceProperties();
  emit rendererReloadData();
}

std::optional<double> AppearanceTreeWidgetController::adsorptionSurfaceOutsideAmbientLightIntensity()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->adsorptionSurfaceBackSideAmbientIntensity();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setAdsorptionSurfaceOutsideAmbientLightColor()
{
  QColor color = QColorDialog::getColor(Qt::white,this,"Choose Color");
  if(color.isValid())
  {
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      iraspa_structure->structure()->setAdsorptionSurfaceBackSideAmbientColor(color);
    }
    reloadAdsorptionSurfaceProperties();
    emit rendererReloadData();
  }
}

std::optional<QColor> AppearanceTreeWidgetController::adsorptionSurfaceOutsideAmbientLightColor()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<QColor> set = std::unordered_set<QColor>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    QColor value = iraspa_structure->structure()->adsorptionSurfaceBackSideAmbientColor();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setAdsorptionSurfaceOutsideDiffuseLightIntensity(double value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setAdsorptionSurfaceBackSideDiffuseIntensity(value);
  }
  reloadAdsorptionSurfaceProperties();
  emit rendererReloadData();
}

std::optional<double> AppearanceTreeWidgetController::adsorptionSurfaceOutsideDiffuseLightIntensity()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->adsorptionSurfaceBackSideDiffuseIntensity();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setAdsorptionSurfaceOutsideDiffuseLightColor()
{
  QColor color = QColorDialog::getColor(Qt::white,this,"Choose Color");
  if(color.isValid())
  {
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      iraspa_structure->structure()->setAdsorptionSurfaceBackSideDiffuseColor(color);
    }
    reloadAdsorptionSurfaceProperties();
    emit rendererReloadData();
  }
}

std::optional<QColor> AppearanceTreeWidgetController::adsorptionSurfaceOutsideDiffuseLightColor()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<QColor> set = std::unordered_set<QColor>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    QColor value = iraspa_structure->structure()->adsorptionSurfaceBackSideDiffuseColor();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setAdsorptionSurfaceOutsideSpecularLightIntensity(double value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setAdsorptionSurfaceBackSideSpecularIntensity(value);
  }
  reloadAdsorptionSurfaceProperties();
  emit rendererReloadData();
}

std::optional<double> AppearanceTreeWidgetController::adsorptionSurfaceOutsideSpecularLightIntensity()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->adsorptionSurfaceBackSideSpecularIntensity();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setAdsorptionSurfaceOutsideSpecularLightColor()
{
  QColor color = QColorDialog::getColor(Qt::white,this,"Choose Color");
  if(color.isValid())
  {
    for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
    {
      iraspa_structure->structure()->setAdsorptionSurfaceBackSideSpecularColor(color);
    }
    reloadAdsorptionSurfaceProperties();
    emit rendererReloadData();
  }
}

std::optional<QColor> AppearanceTreeWidgetController::adsorptionSurfaceOutsideSpecularLightColor()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<QColor> set = std::unordered_set<QColor>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    QColor value = iraspa_structure->structure()->adsorptionSurfaceBackSideSpecularColor();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

void AppearanceTreeWidgetController::setAdsorptionSurfaceOutsideShininess(double value)
{
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    iraspa_structure->structure()->setAdsorptionSurfaceBackSideShininess(value);
  }
  reloadAdsorptionSurfaceProperties();
  emit rendererReloadData();
}

std::optional<double> AppearanceTreeWidgetController::adsorptionSurfaceOutsideShininessy()
{
  if(_iraspa_structures.empty())
  {
    return std::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<iRASPAStructure> iraspa_structure: _iraspa_structures)
  {
    double value = iraspa_structure->structure()->adsorptionSurfaceBackSideShininess();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return std::nullopt;
}

// expansion
//========================================================================================================================================

void AppearanceTreeWidgetController::expandPrimitiveItem()
{
  QModelIndex index = indexFromItem(topLevelItem(0),0);
  if(this->isExpanded(index))
  {
    this->collapse(index);
    pushButtonPrimitive->setIcon(QIcon(":/iRASPA/collapsed.png"));
  }
  else
  {
    this->expand(index);
    pushButtonPrimitive->setIcon(QIcon(":/iRASPA/expanded.png"));
    //reloadAtomProperties();
  }
}

void AppearanceTreeWidgetController::expandAtomsItem()
{
  QModelIndex index = indexFromItem(topLevelItem(1),0);
  if(this->isExpanded(index))
  {
    this->collapse(index);
    pushButtonAtoms->setIcon(QIcon(":/iRASPA/collapsed.png"));
  }
  else
  {
    this->expand(index);
    pushButtonAtoms->setIcon(QIcon(":/iRASPA/expanded.png"));
    //reloadAtomProperties();
  }
}

void AppearanceTreeWidgetController::expandBondsItem()
{
  QModelIndex index = indexFromItem(topLevelItem(2),0);
  if(this->isExpanded(index))
  {
    this->collapse(index);
    pushButtonBonds->setIcon(QIcon(":/iRASPA/collapsed.png"));
  }
  else
  {
    this->expand(index);
    pushButtonBonds->setIcon(QIcon(":/iRASPA/expanded.png"));
  }
}

void AppearanceTreeWidgetController::expandUnitCellItem()
{
  QModelIndex index = indexFromItem(topLevelItem(3),0);
  if(this->isExpanded(index))
  {
    this->collapse(index);
    pushButtonUnitCell->setIcon(QIcon(":/iRASPA/collapsed.png"));
  }
  else
  {
    this->expand(index);
    pushButtonUnitCell->setIcon(QIcon(":/iRASPA/expanded.png"));
  }
}

void AppearanceTreeWidgetController::expandAdsorptionSurfaceItem()
{
  QModelIndex index = indexFromItem(topLevelItem(4),0);
  if(this->isExpanded(index))
  {
    this->collapse(index);
    pushButtonAdsorptionSurface->setIcon(QIcon(":/iRASPA/collapsed.png"));
  }
  else
  {
    this->expand(index);
    pushButtonAdsorptionSurface->setIcon(QIcon(":/iRASPA/expanded.png"));
  }
}

void AppearanceTreeWidgetController::expandAnnotationItem()
{
  QModelIndex index = indexFromItem(topLevelItem(5),0);
  if(this->isExpanded(index))
  {
    this->collapse(index);
    pushButtonAnnotation->setIcon(QIcon(":/iRASPA/collapsed.png"));
  }
  else
  {
    this->expand(index);
    pushButtonAnnotation->setIcon(QIcon(":/iRASPA/expanded.png"));
  }
}
