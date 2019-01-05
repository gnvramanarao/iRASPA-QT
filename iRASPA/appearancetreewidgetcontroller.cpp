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
    _appearanceAtomsForm(new AppearanceAtomsForm),
    _appearanceBondsForm(new AppearanceBondsForm),
    _appearanceUnitCellForm(new AppearanceUnitCellForm),
    _appearanceAdsorptionSurfaceForm(new AppearanceAdsorptionSurfaceForm),
    _appearanceAnnotationForm(new AppearanceAnnotationForm)
{
    //this->viewport()->setMouseTracking(true);
    this->setHeaderHidden(true);
    this->setRootIsDecorated(true);
    this->setFrameStyle(QFrame::NoFrame);
    this->setAnimated(true);
    this->setVerticalScrollMode(QTreeWidget::ScrollPerPixel);
    this->setExpandsOnDoubleClick(false);
    this->setIndentation(0);
    this->setSelectionMode(QAbstractItemView::NoSelection);

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

    _appearanceAtomsForm->atomSelectionStyleComboBox->insertItem(0,"Worley Noise 3D");
    _appearanceAtomsForm->atomSelectionStyleComboBox->insertItem(1,"Stripes");
    _appearanceAtomsForm->atomSelectionStyleComboBox->insertItem(2,"Glow");

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


  _appearanceBondsForm->bondSizeScalingDoubleSlider->setDoubleMinimum(0.0);
  _appearanceBondsForm->bondSizeScalingDoubleSlider->setDoubleMaximum(2.0);


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


  _appearanceAdsorptionSurfaceForm->adsorptionSurfaceIsovalueDoubleSlider->setDoubleMinimum(-1000.0);
  _appearanceAdsorptionSurfaceForm->adsorptionSurfaceIsovalueDoubleSlider->setDoubleMaximum(1000.0);
  _appearanceAdsorptionSurfaceForm->adsorptionSurfaceIsovalueDoubleSpinBox->setMinimum(-1000.0);
  _appearanceAdsorptionSurfaceForm->adsorptionSurfaceIsovalueDoubleSpinBox->setMaximum(100000.0);
}


void AppearanceTreeWidgetController::setProject(std::shared_ptr<ProjectTreeNode> projectTreeNode)
{
  if (projectTreeNode)
  {
    if(std::shared_ptr<iRASPAProject> iraspaProject = projectTreeNode->representedObject())
    {
      if(std::shared_ptr<Project> project = iraspaProject->project())
      {
        if (std::shared_ptr<ProjectStructure> projectStructure = std::dynamic_pointer_cast<ProjectStructure>(project))
        {
           std::cout << "setProject : " << projectTreeNode->displayName().toStdString() << std::endl;
          _projectStructure = projectStructure;
          reloadData();
          return;
        }
      }
    }
  }
  _projectStructure = nullptr;

  emit rendererReloadData();
}

void AppearanceTreeWidgetController::setStructures(std::vector<std::shared_ptr<Structure>> structures)
{
  std::cout << "setStructures : " << structures.size() << std::endl;
  _structures = structures;
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

  reloadAtomProperties();
  reloadBondProperties();
  reloadUnitCellProperties();
  reloadAdsorptionSurfaceProperties();
  reloadAnnotationProperties();
}

void AppearanceTreeWidgetController::reloadSelection()
{

}


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
}

void AppearanceTreeWidgetController::reloadUnitCellProperties()
{
  reloadDrawUnitCell();
  reloadUnitCellSizeScaling();
  reloadUnitCellDiffuseLight();
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
}

void AppearanceTreeWidgetController::reloadAnnotationProperties()
{

}

// reload atom properties
//========================================================================================================================================

void AppearanceTreeWidgetController::reloadAtomRepresentationType()
{
  if(!_structures.empty())
  {
    if (stdx::optional<RepresentationType> type=representationType())
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
  if(!_structures.empty())
  {
    if (stdx::optional<RepresentationStyle> type=representationStyle())
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
         whileBlocking(_appearanceAtomsForm->atomRepresentationStyle)->insertItem(int(RepresentationStyle::multiple_values),"Custom");
         whileBlocking(_appearanceAtomsForm->atomRepresentationStyle)->setCurrentIndex(int(RepresentationStyle::multiple_values));
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
  if(!_structures.empty())
  {
    if (stdx::optional<QString> colorSchemName = colorSchemeIdentifier())
    {
      if(int index = _appearanceAtomsForm->atomRepresentationStyle->findText("Multiple values"); index>=0)
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
  if(!_structures.empty())
  {
    if (stdx::optional<ColorSchemeOrder> type = colorSchemeOrder())
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
  if(!_structures.empty())
  {
    if (stdx::optional<QString> forceFieldIdentifier = forceFieldSchemeIdentifier())
    {
      if(int index = _appearanceAtomsForm->atomRepresentationStyle->findText("Multiple values"); index>=0)
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
  if(!_structures.empty())
  {
    if (stdx::optional<ForceFieldSchemeOrder> type = forceFieldSchemeOrder())
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
  if(!_structures.empty())
  {
    if (stdx::optional<bool> state = atomDrawAtoms())
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
  if(!_structures.empty())
  {
    if (stdx::optional<RKSelectionStyle> type = atomSelectionStyle())
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

    if (stdx::optional<double> size = atomSelectionStyleNu())
    {
     whileBlocking(_appearanceAtomsForm->atomSelectionStyleNuDoubleSpinBox)->setValue(*size);
    }
    else
    {
      whileBlocking(_appearanceAtomsForm->atomSelectionStyleNuDoubleSpinBox)->setText("Mult. Val.");
    }

    if (stdx::optional<double> size = atomSelectionStyleRho())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> size = atomSelectionIntensity())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> size = atomSelectionScaling())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> size = atomSizeScaling())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> size = atomSizeScaling())
    {
      whileBlocking(_appearanceAtomsForm->atomAtomicSizeScalingSlider)->setDoubleValue(*size);
    }
  }
}


void AppearanceTreeWidgetController::reloadAtomHighDynamicRange()
{
  if(!_structures.empty())
  {
    if (stdx::optional<bool> state = atomHighDynamicRange())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = atomHDRExposure())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = atomHue())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = atomSaturation())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = atomValue())
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
  if(!_structures.empty())
  {
    if (stdx::optional<bool> state = atomAmbientOcclusion())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = atomAmbientLightIntensity())
    {
      whileBlocking(_appearanceAtomsForm->atomAmbientIntensityDoubleSpinBox)->setValue(*value);
      whileBlocking(_appearanceAtomsForm->atomAmbientIntensityDoubleSlider)->setDoubleValue(*value);
    }
    else
    {
      whileBlocking(_appearanceAtomsForm->atomAmbientIntensityDoubleSpinBox)->setText("Mult. Val.");
    }

    if (stdx::optional<QColor> value = atomAmbientLightColor())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = atomDiffuseLightIntensity())
    {
      whileBlocking(_appearanceAtomsForm->atomDiffuseItensityDoubleSpinBox)->setValue(*value);
      whileBlocking(_appearanceAtomsForm->atomDiffuseIntensityDoubleSlider)->setDoubleValue(*value);
    }
    else
    {
      whileBlocking(_appearanceAtomsForm->atomDiffuseItensityDoubleSpinBox)->setText("Mult. Val.");
    }

    if (stdx::optional<QColor> value = atomDiffuseLightColor())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = atomSpecularLightIntensity())
    {
      whileBlocking(_appearanceAtomsForm->atomSpecularIntensityDoubleSpinBoxBox)->setValue(*value);
      whileBlocking(_appearanceAtomsForm->atomSpecularIntensityDoubleSlider)->setDoubleValue(*value);
    }
    else
    {
      whileBlocking(_appearanceAtomsForm->atomSpecularIntensityDoubleSpinBoxBox)->setText("Mult. Val.");
    }

    if (stdx::optional<QColor> value = atomSpecularLightColor())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = atomShininessy())
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
  std::cout << "setRepresentationType " << value << std::endl;
  for(std::shared_ptr<Structure> structure: _structures)
  {
    if(value>=0 && value<int(RepresentationType::multiple_values))
    {
      structure->setRepresentationType(RepresentationType(value));
      structure->recheckRepresentationStyle();
    }
  }
  reloadAtomProperties();
  reloadBondProperties();
  emit rendererReloadData();
}

stdx::optional<RepresentationType> AppearanceTreeWidgetController::representationType()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<RepresentationType, enum_hash> set = std::unordered_set<RepresentationType, enum_hash>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    RepresentationType value = structure->atomRepresentationType();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setRepresentationStyle(int value)
{
  std::cout << "setRepresentationStyle " << value << std::endl;
  if(value >= 0 && value <= int(RepresentationStyle::multiple_values))  // also include "Custom"
  {
    for(std::shared_ptr<Structure> structure: _structures)
    {
      structure->setRepresentationStyle(RepresentationStyle(value), _mainWindow->colorSets());
    }

    reloadAtomProperties();
    reloadBondProperties();
    emit rendererReloadData();
  }
}

stdx::optional<RepresentationStyle> AppearanceTreeWidgetController::representationStyle()
{
  std::cout << "representationStyle " << std::endl;
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<RepresentationStyle, enum_hash> set = std::unordered_set<RepresentationStyle, enum_hash>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    RepresentationStyle value = structure->atomRepresentationStyle();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  std::cout << "representationStyle end" << std::endl;
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setColorSchemeComboBoxIndex(int value)
{
  QString stringValue = _appearanceAtomsForm->colorSchemeComboBox->currentText();
  if(QString::compare(stringValue, "Multiple values") != 0)
  {
    for(std::shared_ptr<Structure> structure: _structures)
    {
      std::cout << "setColorScheme apperance: " << stringValue.toStdString() << std::endl;
      structure->setRepresentationColorSchemeIdentifier(stringValue, _mainWindow->colorSets());
      structure->recheckRepresentationStyle();
    }
    reloadAtomProperties();
    emit rendererReloadData();
  }
}

stdx::optional<QString> AppearanceTreeWidgetController::colorSchemeIdentifier()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::set<QString> set = std::set<QString>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    QString value = structure->atomColorSchemeIdentifier();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setColorSchemeOrder(int value)
{
  if(value>=0 && value<int(ColorSchemeOrder::multiple_values))
  {
    for(std::shared_ptr<Structure> structure: _structures)
    {
      structure->setColorSchemeOrder(ColorSchemeOrder(value));
      structure->recheckRepresentationStyle();
    }
    reloadAtomProperties();
    emit rendererReloadData();
  }
}

stdx::optional<ColorSchemeOrder> AppearanceTreeWidgetController::colorSchemeOrder()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<ColorSchemeOrder, enum_hash> set = std::unordered_set<ColorSchemeOrder, enum_hash>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    ColorSchemeOrder value = structure->colorSchemeOrder();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setForcefieldSchemeComboBoxIndex(int value)
{
  QString stringValue = _appearanceAtomsForm->forceFieldComboBox->currentText();
  if(QString::compare(stringValue, "Multiple values") != 0)
  {
    for(std::shared_ptr<Structure> structure: _structures)
    {
      structure->setAtomForceFieldIdentifier(stringValue,_mainWindow->forceFieldSets());
    }
    reloadAtomProperties();
    emit invalidateIsosurface(std::vector<std::shared_ptr<RKRenderStructure>>{_structures.begin(), _structures.end()});
    emit rendererReloadData();
  }
}

stdx::optional<QString> AppearanceTreeWidgetController::forceFieldSchemeIdentifier()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::set<QString> set = std::set<QString>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    QString value = structure->atomForceFieldIdentifier();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setForceFieldSchemeOrder(int value)
{
  if(value>=0 && value<int(ForceFieldSchemeOrder::multiple_values))
  {
    for(std::shared_ptr<Structure> structure: _structures)
    {
      structure->setForceFieldSchemeOrder(ForceFieldSchemeOrder(value));
      structure->recheckRepresentationStyle();
    }
    reloadAtomProperties();
    emit rendererReloadData();
  }
}

stdx::optional<ForceFieldSchemeOrder> AppearanceTreeWidgetController::forceFieldSchemeOrder()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<ForceFieldSchemeOrder, enum_hash> set = std::unordered_set<ForceFieldSchemeOrder, enum_hash>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    ForceFieldSchemeOrder value = structure->forceFieldSchemeOrder();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setAtomSelectionStyle(int value)
{
  if(value>=0 && value<int(RKSelectionStyle::multiple_values))
  {
    for(std::shared_ptr<Structure> structure: _structures)
    {
      structure->setSelectionStyle(RKSelectionStyle(value));
      structure->recheckRepresentationStyle();
    }
    reloadAtomProperties();
    emit rendererReloadData();
  }
}

stdx::optional<RKSelectionStyle> AppearanceTreeWidgetController::atomSelectionStyle()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<RKSelectionStyle, enum_hash> set = std::unordered_set<RKSelectionStyle, enum_hash>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    RKSelectionStyle value = structure->renderSelectionStyle();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setAtomSelectionStyleNu(double value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setSelectionFrequency(value);
    structure->recheckRepresentationStyle();
   }
   reloadAtomProperties();
   emit rendererReloadData();
}

stdx::optional<double> AppearanceTreeWidgetController::atomSelectionStyleNu()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->renderSelectionFrequency();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setAtomSelectionStyleRho(double value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setSelectionDensity(value);
    structure->recheckRepresentationStyle();
   }
   reloadAtomProperties();
   emit rendererReloadData();
}

stdx::optional<double> AppearanceTreeWidgetController::atomSelectionStyleRho()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->renderSelectionDensity();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setAtomSelectionIntensity(double value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setSelectionIntensity(value);
    structure->recheckRepresentationStyle();
  }
  reloadAtomProperties();
  emit rendererReloadData();
}

stdx::optional<double> AppearanceTreeWidgetController::atomSelectionIntensity()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->renderAtomSelectionIntensity();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setAtomSelectionScaling(double value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setSelectionScaling(value);
    structure->recheckRepresentationStyle();
  }
  reloadAtomProperties();
  emit rendererReloadData();
}

stdx::optional<double> AppearanceTreeWidgetController::atomSelectionScaling()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->renderSelectionScaling();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}



void AppearanceTreeWidgetController::setAtomDrawAtoms(int state)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setDrawAtoms(bool(state));
    structure->recheckRepresentationStyle();
   }
   reloadAtomProperties();
   emit rendererReloadData();
}

stdx::optional<bool> AppearanceTreeWidgetController::atomDrawAtoms()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<bool> set = std::unordered_set<bool>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    bool value = structure->drawAtoms();
    set.insert(value);
  }
   if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setAtomSizeScalingDoubleSpinBox(double size)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setAtomScaleFactor(size);
    structure->recheckRepresentationStyle();
  }
  reloadAtomRepresentationType();
  reloadAtomSizeScalingDoubleSlider();
  reloadAtomRepresentationStyle();

  emit invalidateCachedAmbientOcclusionTexture(std::vector<std::shared_ptr<RKRenderStructure>>{_structures.begin(), _structures.end()});
  emit redrawRendererWithHighQuality();
}

void AppearanceTreeWidgetController::setAtomSizeScalingDoubleSliderBegin()
{
  emit redrawRendererWithLowQuality();
}

void AppearanceTreeWidgetController::setAtomSizeScalingDoubleSliderIntermediate(double size)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setAtomScaleFactor(size);
  }
  reloadAtomSizeScalingDoubleSpinBox();

  emit redrawRendererWithLowQuality();
}

void AppearanceTreeWidgetController::setAtomSizeScalingDoubleSliderFinal()
{
  std::cout << "setAtomSizeScalingDoubleSliderFinal" << std::endl;
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->recheckRepresentationStyle();
  }
  reloadAtomRepresentationStyle();
  reloadAtomSizeScalingDoubleSpinBox();

  emit invalidateCachedAmbientOcclusionTexture(std::vector<std::shared_ptr<RKRenderStructure>>{_structures.begin(), _structures.end()});
  emit rendererReloadData();
  emit redrawRendererWithHighQuality();
}

stdx::optional<double> AppearanceTreeWidgetController::atomSizeScaling()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->atomScaleFactor();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}


void AppearanceTreeWidgetController::setAtomHighDynamicRange(int value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setAtomHDR(bool(value));
    structure->recheckRepresentationStyle();
  }
  reloadAtomProperties();
  emit rendererReloadData();
}

stdx::optional<bool> AppearanceTreeWidgetController::atomHighDynamicRange()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<bool> set = std::unordered_set<bool>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    bool value = structure->atomHDR();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setAtomHDRExposure(double value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setAtomHDRExposure(value);
    structure->recheckRepresentationStyle();
  }
  reloadAtomProperties();
  emit rendererReloadData();
}

stdx::optional<double> AppearanceTreeWidgetController::atomHDRExposure()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->atomHDRExposure();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setAtomHue(double value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setAtomHue(value);
    structure->recheckRepresentationStyle();
   }
   reloadAtomProperties();
   emit rendererReloadData();
}

stdx::optional<double> AppearanceTreeWidgetController::atomHue()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->atomHue();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setAtomSaturation(double value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setAtomSaturation(value);
    structure->recheckRepresentationStyle();
   }
   reloadAtomProperties();
   emit rendererReloadData();
}

stdx::optional<double> AppearanceTreeWidgetController::atomSaturation()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->atomSaturation();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setAtomValue(double value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setAtomValue(value);
    structure->recheckRepresentationStyle();
  }
  reloadAtomProperties();
  emit rendererReloadData();
}

stdx::optional<double> AppearanceTreeWidgetController::atomValue()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->atomValue();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setAtomAmbientOcclusion(int value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setAtomAmbientOcclusion(bool(value));
    structure->recheckRepresentationStyle();
  }
  reloadAtomProperties();
  emit rendererReloadData();
}

stdx::optional<bool> AppearanceTreeWidgetController::atomAmbientOcclusion()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<bool> set = std::unordered_set<bool>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    bool value = structure->atomAmbientOcclusion();
    set.insert(value);
  }
  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setAtomAmbientLightIntensity(double value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setAtomAmbientIntensity(value);
    structure->recheckRepresentationStyle();
   }
   reloadAtomProperties();
   emit rendererReloadData();
}

stdx::optional<double> AppearanceTreeWidgetController::atomAmbientLightIntensity()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->atomAmbientIntensity();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setAtomAmbientLightColor()
{
  QColor color = QColorDialog::getColor(Qt::white,this,"Choose Color");
  if(color.isValid())
  {
    _appearanceAtomsForm->atomAmbientColorPushButton->setColor(color);
    for(std::shared_ptr<Structure> structure: _structures)
    {
      structure->setAtomAmbientColor(color);
      structure->recheckRepresentationStyle();
    }
    reloadAtomProperties();
    emit rendererReloadData();
  }
}

stdx::optional<QColor> AppearanceTreeWidgetController::atomAmbientLightColor()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<QColor> set = std::unordered_set<QColor>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    QColor value = structure->atomAmbientColor();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setAtomDiffuseLightIntensity(double value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setAtomDiffuseIntensity(value);
    structure->recheckRepresentationStyle();
   }
   reloadAtomProperties();
   emit rendererReloadData();
}

stdx::optional<double> AppearanceTreeWidgetController::atomDiffuseLightIntensity()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->atomDiffuseIntensity();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setAtomDiffuseLightColor()
{
  QColor color = QColorDialog::getColor(Qt::white,this,"Choose Color");
  if(color.isValid())
  {
    _appearanceAtomsForm->atomDiffuseColorPushButton->setColor(color);
    for(std::shared_ptr<Structure> structure: _structures)
    {
      structure->setAtomDiffuseColor(color);
      structure->recheckRepresentationStyle();
    }
    reloadAtomProperties();
    emit rendererReloadData();
  }
}

stdx::optional<QColor> AppearanceTreeWidgetController::atomDiffuseLightColor()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<QColor> set = std::unordered_set<QColor>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    QColor value = structure->atomDiffuseColor();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setAtomSpecularLightIntensity(double value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setAtomSpecularIntensity(value);
    structure->recheckRepresentationStyle();
   }
   reloadAtomProperties();
   emit rendererReloadData();
}

stdx::optional<double> AppearanceTreeWidgetController::atomSpecularLightIntensity()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->atomSpecularIntensity();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setAtomSpecularLightColor()
{
  QColor color = QColorDialog::getColor(Qt::white,this,"Choose Color");
  if(color.isValid())
  {
    _appearanceAtomsForm->atomSpecularColorPushButton->setColor(color);
    for(std::shared_ptr<Structure> structure: _structures)
    {
      structure->setAtomSpecularColor(color);
      structure->recheckRepresentationStyle();
    }
    reloadAtomProperties();
    emit rendererReloadData();
  }
}

stdx::optional<QColor> AppearanceTreeWidgetController::atomSpecularLightColor()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<QColor> set = std::unordered_set<QColor>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    QColor value = structure->atomSpecularColor();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setAtomShininess(double value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setAtomShininess(value);
    structure->recheckRepresentationStyle();
   }
   reloadAtomProperties();
   emit rendererReloadData();
}

stdx::optional<double> AppearanceTreeWidgetController::atomShininessy()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->atomShininess();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

// reload bond surface properties
//========================================================================================================================================



void AppearanceTreeWidgetController::reloadDrawBondsCheckBox()
{
  if(!_structures.empty())
  {
    if (stdx::optional<bool> state = bondDrawBonds())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> size = bondSizeScaling())
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
  if(!_structures.empty())
  {
    if (stdx::optional<RKBondColorMode> type=bondColorMode())
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
  if(!_structures.empty())
  {
    if (stdx::optional<bool> state = bondHighDynamicRange())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = bondHDRExposure())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = bondHue())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = bondSaturation())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = bondValue())
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
  if(!_structures.empty())
  {
    if (stdx::optional<bool> state = bondAmbientOcclusion())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = bondAmbientLightIntensity())
    {
      whileBlocking(_appearanceBondsForm->bondAmbientIntensityDoubleSpinBox)->setValue(*value);
      whileBlocking(_appearanceBondsForm->bondAmbientIntensityDoubleSlider)->setDoubleValue(*value);
    }
    else
    {
      whileBlocking(_appearanceBondsForm->bondAmbientIntensityDoubleSpinBox)->setText("Mult. Val.");
    }

    if (stdx::optional<QColor> value = bondAmbientLightColor())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = bondDiffuseLightIntensity())
    {
      whileBlocking(_appearanceBondsForm->bondDiffuseIntensityDoubleSpinBox)->setValue(*value);
      whileBlocking(_appearanceBondsForm->bondDiffuseIntensityDoubleSlider)->setDoubleValue(*value);
    }
    else
    {
      whileBlocking(_appearanceBondsForm->bondDiffuseIntensityDoubleSpinBox)->setText("Mult. Val.");
    }

    if (stdx::optional<QColor> value = bondDiffuseLightColor())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = bondSpecularLightIntensity())
    {
      whileBlocking(_appearanceBondsForm->bondSpecularIntensityDoubleSpinBox)->setValue(*value);
      whileBlocking(_appearanceBondsForm->bondSpecularIntensityDoubleSlider)->setDoubleValue(*value);
    }
    else
    {
      whileBlocking(_appearanceBondsForm->bondSpecularIntensityDoubleSpinBox)->setText("Mult. Val.");
    }

    if (stdx::optional<QColor> value = bondSpecularLightColor())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = bondShininessy())
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
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setDrawBonds(bool(state));
    structure->recheckRepresentationStyle();
   }
   reloadAtomProperties();
   emit rendererReloadData();
}

stdx::optional<bool> AppearanceTreeWidgetController::bondDrawBonds()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<bool> set = std::unordered_set<bool>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    bool value = structure->drawBonds();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setBondSizeScaling(double size)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setBondScaleFactor(size);
    structure->recheckRepresentationStyle();
   }
   reloadAtomProperties();
   reloadBondProperties();
   emit rendererReloadData();
}

stdx::optional<double> AppearanceTreeWidgetController::bondSizeScaling()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->bondScaleFactor();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setBondColorMode(int value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setBondColorMode(RKBondColorMode(value));
    structure->recheckRepresentationStyle();
   }
   reloadBondProperties();
   emit rendererReloadData();
}

stdx::optional<RKBondColorMode> AppearanceTreeWidgetController::bondColorMode()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<RKBondColorMode, enum_hash> set = std::unordered_set<RKBondColorMode, enum_hash>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    RKBondColorMode value = structure->bondColorMode();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setBondHighDynamicRange(int value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setBondHDR(bool(value));
    structure->recheckRepresentationStyle();
  }
  reloadAtomProperties();
  emit rendererReloadData();
}

stdx::optional<bool> AppearanceTreeWidgetController::bondHighDynamicRange()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<bool> set = std::unordered_set<bool>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    bool value = structure->bondHDR();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setBondHDRExposure(double value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setBondHDRExposure(value);
    structure->recheckRepresentationStyle();
  }
  reloadAtomProperties();
  emit rendererReloadData();
}

stdx::optional<double> AppearanceTreeWidgetController::bondHDRExposure()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->bondHDRExposure();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setBondHue(double value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setBondHue(value);
    structure->recheckRepresentationStyle();
   }
   reloadAtomProperties();
   emit rendererReloadData();
}

stdx::optional<double> AppearanceTreeWidgetController::bondHue()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->bondHue();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setBondSaturation(double value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setBondSaturation(value);
    structure->recheckRepresentationStyle();
   }
   reloadAtomProperties();
   emit rendererReloadData();
}

stdx::optional<double> AppearanceTreeWidgetController::bondSaturation()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->bondSaturation();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setBondValue(double value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setBondValue(value);
    structure->recheckRepresentationStyle();
  }
  reloadAtomProperties();
  emit rendererReloadData();
}

stdx::optional<double> AppearanceTreeWidgetController::bondValue()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->bondValue();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setBondAmbientOcclusion(int value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setBondAmbientOcclusion(bool(value));
    structure->recheckRepresentationStyle();
  }
  reloadAtomProperties();
  emit rendererReloadData();
}

stdx::optional<bool> AppearanceTreeWidgetController::bondAmbientOcclusion()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<bool> set = std::unordered_set<bool>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    bool value = structure->bondAmbientOcclusion();
    set.insert(value);
  }
  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setBondAmbientLightIntensity(double value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setBondAmbientIntensity(value);
    structure->recheckRepresentationStyle();
   }
   reloadAtomProperties();
   emit rendererReloadData();
}

stdx::optional<double> AppearanceTreeWidgetController::bondAmbientLightIntensity()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->bondAmbientIntensity();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setBondAmbientLightColor()
{
  QColor color = QColorDialog::getColor(Qt::white,this,"Choose Color");
  if(color.isValid())
  {
    _appearanceBondsForm->bondAmbientColorPushButton->setColor(color);
    for(std::shared_ptr<Structure> structure: _structures)
    {
      structure->setBondAmbientColor(color);
      structure->recheckRepresentationStyle();
    }
    reloadAtomProperties();
    emit rendererReloadData();
  }
}

stdx::optional<QColor> AppearanceTreeWidgetController::bondAmbientLightColor()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<QColor> set = std::unordered_set<QColor>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    QColor value = structure->bondAmbientColor();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setBondDiffuseLightIntensity(double value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setBondDiffuseIntensity(value);
    structure->recheckRepresentationStyle();
   }
   reloadAtomProperties();
   emit rendererReloadData();
}

stdx::optional<double> AppearanceTreeWidgetController::bondDiffuseLightIntensity()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->bondDiffuseIntensity();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setBondDiffuseLightColor()
{
  QColor color = QColorDialog::getColor(Qt::white,this,"Choose Color");
  if(color.isValid())
  {
    _appearanceBondsForm->bondDiffuseColorPushButton->setColor(color);
    for(std::shared_ptr<Structure> structure: _structures)
    {
      structure->setBondDiffuseColor(color);
      structure->recheckRepresentationStyle();
    }
    reloadAtomProperties();
    emit rendererReloadData();
  }
}

stdx::optional<QColor> AppearanceTreeWidgetController::bondDiffuseLightColor()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<QColor> set = std::unordered_set<QColor>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    QColor value = structure->bondDiffuseColor();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setBondSpecularLightIntensity(double value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setBondSpecularIntensity(value);
    structure->recheckRepresentationStyle();
   }
   reloadAtomProperties();
   emit rendererReloadData();
}

stdx::optional<double> AppearanceTreeWidgetController::bondSpecularLightIntensity()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->bondSpecularIntensity();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setBondSpecularLightColor()
{
  QColor color = QColorDialog::getColor(Qt::white,this,"Choose Color");
  if(color.isValid())
  {
    _appearanceBondsForm->bondSpecularColorPushButton->setColor(color);
    for(std::shared_ptr<Structure> structure: _structures)
    {
      structure->setBondSpecularColor(color);
      structure->recheckRepresentationStyle();
    }
    reloadAtomProperties();
    emit rendererReloadData();
  }
}

stdx::optional<QColor> AppearanceTreeWidgetController::bondSpecularLightColor()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<QColor> set = std::unordered_set<QColor>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    QColor value = structure->bondSpecularColor();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setBondShininess(double value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setBondShininess(value);
    structure->recheckRepresentationStyle();
   }
   reloadAtomProperties();
   emit rendererReloadData();
}

stdx::optional<double> AppearanceTreeWidgetController::bondShininessy()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->bondShininess();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}


// reload unit cell properties
//========================================================================================================================================

void AppearanceTreeWidgetController::reloadDrawUnitCell()
{
  if(!_structures.empty())
  {
    if (stdx::optional<bool> state = drawUnitCell())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> size = unitCellSizeScaling())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = unitCellDiffuseLightIntensity())
    {
      whileBlocking(_appearanceUnitCellForm->diffuseIntensityDoubleSpinBox)->setValue(*value);
      whileBlocking(_appearanceUnitCellForm->diffuseIntensityDoubleSlider)->setDoubleValue(*value);
    }
    else
    {
      whileBlocking(_appearanceUnitCellForm->diffuseIntensityDoubleSpinBox)->setText("Mult. Val.");
    }

    if (stdx::optional<QColor> value = bondDiffuseLightColor())
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
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setDrawUnitCell(bool(state));
    structure->recheckRepresentationStyle();
   }
   reloadAtomProperties();
   emit rendererReloadData();
}

stdx::optional<bool> AppearanceTreeWidgetController::drawUnitCell()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<bool> set = std::unordered_set<bool>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    bool value = structure->drawUnitCell();
    set.insert(value);
  }
  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setUnitCellSizeScaling(double size)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setUnitCellScaleFactor(size);
    structure->recheckRepresentationStyle();
   }
   reloadAtomProperties();
   emit rendererReloadData();
}

stdx::optional<double> AppearanceTreeWidgetController::unitCellSizeScaling()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->unitCellScaleFactor();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setUnitCellDiffuseLightIntensity(double value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setUnitCellDiffuseIntensity(value);
    structure->recheckRepresentationStyle();
   }
   reloadAtomProperties();
   emit rendererReloadData();
}

stdx::optional<double> AppearanceTreeWidgetController::unitCellDiffuseLightIntensity()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->unitCellDiffuseIntensity();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setUnitCellDiffuseLightColor()
{
  QColor color = QColorDialog::getColor(Qt::white,this,"Choose Color");
  if(color.isValid())
  {
    _appearanceUnitCellForm->diffuseColorPushButton->setColor(color);
    for(std::shared_ptr<Structure> structure: _structures)
    {
      structure->setUnitCellDiffuseColor(color);
      structure->recheckRepresentationStyle();
    }
    reloadAtomProperties();
    emit rendererReloadData();
  }
}

stdx::optional<QColor> AppearanceTreeWidgetController::unitCellDiffuseLightColor()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<QColor> set = std::unordered_set<QColor>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    QColor value = structure->unitCellDiffuseColor();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}


// reload Adsorption surface properties
//========================================================================================================================================

void AppearanceTreeWidgetController::reloadDrawAdsorptionSurfaceCheckBox()
{
  if(!_structures.empty())
  {
    if (stdx::optional<bool> state = drawAdsorptionSurface())
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
  if(!_structures.empty())
  {
    if (stdx::optional<ProbeMolecule> type=adsorptionSurfaceProbeMolecule())
    {
      if(int index = _appearanceAdsorptionSurfaceForm->probeParticleComboBox->findText("Mult. Val."); index>=0)
      {
        whileBlocking(_appearanceAdsorptionSurfaceForm->probeParticleComboBox)->removeItem(index);
      }
      if(int(*type)<0)
      {
       whileBlocking(_appearanceAdsorptionSurfaceForm->probeParticleComboBox)->setCurrentIndex(int(ProbeMolecule::multiple_values));
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = adsorptionSurfaceMinimumValue())
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->adsorptionSurfaceIsovalueDoubleSlider)->setDoubleMinimum(*value);
      whileBlocking(_appearanceAdsorptionSurfaceForm->adsorptionSurfaceIsovalueDoubleSpinBox)->setMinimum(*value);
    }

    if (stdx::optional<double> value = adsorptionSurfaceIsovalue())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = adsorptionSurfaceOpacity())
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
  if(!_structures.empty())
  {
    if (stdx::optional<bool> state = adsorptionSurfaceInsideHighDynamicRange())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = adsorptionSurfaceInsideHDRExposure())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = adsorptionSurfaceInsideAmbientLightIntensity())
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->insideAmbientLightIntensityDoubleSpinBox)->setValue(*value);
      whileBlocking(_appearanceAdsorptionSurfaceForm->insideAmbientLightIntensityDoubleSlider)->setDoubleValue(*value);
    }
    else
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->insideAmbientLightIntensityDoubleSpinBox)->setText("Mult. Val.");
    }

    if (stdx::optional<QColor> value = adsorptionSurfaceInsideAmbientLightColor())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = adsorptionSurfaceInsideDiffuseLightIntensity())
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->insideDiffuseLightIntensityDoubleSpinBox)->setValue(*value);
      whileBlocking(_appearanceAdsorptionSurfaceForm->insideDiffuseLightIntensityDoubleSlider)->setDoubleValue(*value);
    }
    else
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->insideDiffuseLightIntensityDoubleSpinBox)->setText("Mult. Val.");
    }

    if (stdx::optional<QColor> value = adsorptionSurfaceInsideDiffuseLightColor())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = adsorptionSurfaceInsideSpecularLightIntensity())
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->insideSpecularLightIntensityDoubleSpinBox)->setValue(*value);
      whileBlocking(_appearanceAdsorptionSurfaceForm->insideSpecularLightIntensityDoubleSlider)->setDoubleValue(*value);
    }
    else
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->insideSpecularLightIntensityDoubleSpinBox)->setText("Mult. Val.");
    }

    if (stdx::optional<QColor> value = adsorptionSurfaceInsideSpecularLightColor())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = adsorptionSurfaceInsideShininessy())
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
  if(!_structures.empty())
  {
    if (stdx::optional<bool> state = adsorptionSurfaceOutsideHighDynamicRange())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = adsorptionSurfaceOutsideHDRExposure())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = adsorptionSurfaceOutsideAmbientLightIntensity())
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->outsideAmbientLightIntensityDoubleSpinBox)->setValue(*value);
      whileBlocking(_appearanceAdsorptionSurfaceForm->outsideAmbientLightIntensityDoubleSlider)->setDoubleValue(*value);
    }
    else
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->outsideAmbientLightIntensityDoubleSpinBox)->setText("Mult. Val.");
    }

    if (stdx::optional<QColor> value = adsorptionSurfaceOutsideAmbientLightColor())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = adsorptionSurfaceOutsideDiffuseLightIntensity())
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->outsideDiffuseLightIntensityDoubleSpinBox)->setValue(*value);
      whileBlocking(_appearanceAdsorptionSurfaceForm->outsideDiffuseLightIntensityDoubleSlider)->setDoubleValue(*value);
    }
    else
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->outsideDiffuseLightIntensityDoubleSpinBox)->setText("Mult. Val.");
    }

    if (stdx::optional<QColor> value = adsorptionSurfaceOutsideDiffuseLightColor())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = adsorptionSurfaceOutsideSpecularLightIntensity())
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->outsideSpecularLightIntensityDoubleSpinBox)->setValue(*value);
      whileBlocking(_appearanceAdsorptionSurfaceForm->outsideSpecularLightIntensityDoubleSlider)->setDoubleValue(*value);
    }
    else
    {
      whileBlocking(_appearanceAdsorptionSurfaceForm->outsideSpecularLightIntensityDoubleSpinBox)->setText("Mult. Val.");
    }

    if (stdx::optional<QColor> value = adsorptionSurfaceOutsideSpecularLightColor())
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
  if(!_structures.empty())
  {
    if (stdx::optional<double> value = adsorptionSurfaceOutsideShininessy())
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
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setDrawAdsorptionSurface(bool(state));
    structure->recheckRepresentationStyle();
   }
   reloadAtomProperties();
   emit rendererReloadData();
}

stdx::optional<bool> AppearanceTreeWidgetController::drawAdsorptionSurface()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<bool> set = std::unordered_set<bool>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    bool value = structure->drawAdsorptionSurface();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setAdsorptionSurfaceProbeMolecule(int value)
{
  if(value>=0 && value<int(ProbeMolecule::multiple_values))
  {
    for(std::shared_ptr<Structure> structure: _structures)
    {
      structure->setAdsorptionSurfaceProbeMolecule(ProbeMolecule(value));
      structure->recheckRepresentationStyle();
    }
    emit invalidateIsosurface(std::vector<std::shared_ptr<RKRenderStructure>>{_structures.begin(), _structures.end()});

    emit rendererReloadData();

    reloadAdsorptionSurfaceIsovalue();
  }
}

stdx::optional<ProbeMolecule> AppearanceTreeWidgetController::adsorptionSurfaceProbeMolecule()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<ProbeMolecule, enum_hash> set = std::unordered_set<ProbeMolecule, enum_hash>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    ProbeMolecule value = structure->adsorptionSurfaceProbeMolecule();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setAdsorptionSurfaceIsovalue(double value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setAdsorptionSurfaceIsoValue(value);
  }
  reloadAdsorptionSurfaceProperties();
  emit rendererReloadData();
}

stdx::optional<double> AppearanceTreeWidgetController::adsorptionSurfaceIsovalue()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->adsorptionSurfaceIsoValue();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}


stdx::optional<double> AppearanceTreeWidgetController::adsorptionSurfaceMinimumValue()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->adsorptionSurfaceMinimumValue();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}


void AppearanceTreeWidgetController::setAdsorptionSurfaceOpacity(double value)
{
  if(value>=0 && value<=1.0)
  {
    for(std::shared_ptr<Structure> structure: _structures)
    {
      structure->setAdsorptionSurfaceOpacity(value);
    }
    reloadAdsorptionSurfaceProperties();
    emit rendererReloadData();
  }
}

stdx::optional<double> AppearanceTreeWidgetController::adsorptionSurfaceOpacity()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->adsorptionSurfaceOpacity();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setAdsorptionSurfaceInsideHighDynamicRange(int value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setAdsorptionSurfaceFrontSideHDR(bool(value));
  }
  reloadAdsorptionSurfaceProperties();
  emit rendererReloadData();
}

stdx::optional<bool> AppearanceTreeWidgetController::adsorptionSurfaceInsideHighDynamicRange()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<bool> set = std::unordered_set<bool>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    bool state = structure->adsorptionSurfaceFrontSideHDR();
    set.insert(state);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setAdsorptionSurfaceInsideHDRExposure(double value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setAdsorptionSurfaceFrontSideHDRExposure(value);
  }
  reloadAdsorptionSurfaceProperties();
  emit rendererReloadData();
}

stdx::optional<double> AppearanceTreeWidgetController::adsorptionSurfaceInsideHDRExposure()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->adsorptionSurfaceFrontSideHDRExposure();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setAdsorptionSurfaceInsideAmbientLightIntensity(double value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setAdsorptionSurfaceFrontSideAmbientIntensity(value);
  }
  reloadAdsorptionSurfaceProperties();
  emit rendererReloadData();
}

stdx::optional<double> AppearanceTreeWidgetController::adsorptionSurfaceInsideAmbientLightIntensity()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->adsorptionSurfaceFrontSideAmbientIntensity();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setAdsorptionSurfaceInsideAmbientLightColor()
{
  QColor color = QColorDialog::getColor(Qt::white,this,"Choose Color");
  if(color.isValid())
  {
    for(std::shared_ptr<Structure> structure: _structures)
    {
      structure->setAdsorptionSurfaceFrontSideAmbientColor(color);
    }
    reloadAdsorptionSurfaceProperties();
    emit rendererReloadData();
  }
}

stdx::optional<QColor> AppearanceTreeWidgetController::adsorptionSurfaceInsideAmbientLightColor()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<QColor> set = std::unordered_set<QColor>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    QColor color = structure->adsorptionSurfaceFrontSideAmbientColor();
    set.insert(color);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setAdsorptionSurfaceInsideDiffuseLightIntensity(double value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setAdsorptionSurfaceFrontSideDiffuseIntensity(value);
  }
  reloadAdsorptionSurfaceProperties();
  emit rendererReloadData();
}

stdx::optional<double> AppearanceTreeWidgetController::adsorptionSurfaceInsideDiffuseLightIntensity()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->adsorptionSurfaceFrontSideDiffuseIntensity();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setAdsorptionSurfaceInsideDiffuseLightColor()
{
  QColor color = QColorDialog::getColor(Qt::white,this,"Choose Color");
  if(color.isValid())
  {
    for(std::shared_ptr<Structure> structure: _structures)
    {
      structure->setAdsorptionSurfaceFrontSideDiffuseColor(color);
    }
    reloadAdsorptionSurfaceProperties();
    emit rendererReloadData();
  }
}

stdx::optional<QColor> AppearanceTreeWidgetController::adsorptionSurfaceInsideDiffuseLightColor()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<QColor> set = std::unordered_set<QColor>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    QColor color = structure->adsorptionSurfaceFrontSideDiffuseColor();
    set.insert(color);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setAdsorptionSurfaceInsideSpecularLightIntensity(double value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setAdsorptionSurfaceFrontSideSpecularIntensity(value);
  }
  reloadAdsorptionSurfaceProperties();
  emit rendererReloadData();
}

stdx::optional<double> AppearanceTreeWidgetController::adsorptionSurfaceInsideSpecularLightIntensity()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->adsorptionSurfaceFrontSideSpecularIntensity();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setAdsorptionSurfaceInsideSpecularLightColor()
{
  QColor color = QColorDialog::getColor(Qt::white,this,"Choose Color");
  if(color.isValid())
  {
    for(std::shared_ptr<Structure> structure: _structures)
    {
      structure->setAdsorptionSurfaceFrontSideSpecularColor(color);
    }
    reloadAdsorptionSurfaceProperties();
    emit rendererReloadData();
  }
}

stdx::optional<QColor> AppearanceTreeWidgetController::adsorptionSurfaceInsideSpecularLightColor()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<QColor> set = std::unordered_set<QColor>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    QColor color = structure->adsorptionSurfaceFrontSideSpecularColor();
    set.insert(color);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setAdsorptionSurfaceInsideShininess(double value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setAdsorptionSurfaceFrontSideShininess(value);
  }
  reloadAdsorptionSurfaceProperties();
  emit rendererReloadData();
}

stdx::optional<double> AppearanceTreeWidgetController::adsorptionSurfaceInsideShininessy()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->adsorptionSurfaceFrontSideShininess();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setAdsorptionSurfaceOutsideHighDynamicRange(int value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setAdsorptionSurfaceBackSideHDR(bool(value));
  }
  reloadAdsorptionSurfaceProperties();
  emit rendererReloadData();
}

stdx::optional<bool> AppearanceTreeWidgetController::adsorptionSurfaceOutsideHighDynamicRange()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<bool> set = std::unordered_set<bool>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    bool value = structure->adsorptionSurfaceBackSideHDR();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setAdsorptionSurfaceOutsideHDRExposure(double value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setAdsorptionSurfaceBackSideHDRExposure(value);
  }
  reloadAdsorptionSurfaceProperties();
  emit rendererReloadData();
}

stdx::optional<double> AppearanceTreeWidgetController::adsorptionSurfaceOutsideHDRExposure()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->adsorptionSurfaceBackSideHDRExposure();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setAdsorptionSurfaceOutsideAmbientLightIntensity(double value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setAdsorptionSurfaceBackSideAmbientIntensity(value);
  }
  reloadAdsorptionSurfaceProperties();
  emit rendererReloadData();
}

stdx::optional<double> AppearanceTreeWidgetController::adsorptionSurfaceOutsideAmbientLightIntensity()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->adsorptionSurfaceBackSideAmbientIntensity();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setAdsorptionSurfaceOutsideAmbientLightColor()
{
  QColor color = QColorDialog::getColor(Qt::white,this,"Choose Color");
  if(color.isValid())
  {
    for(std::shared_ptr<Structure> structure: _structures)
    {
      structure->setAdsorptionSurfaceBackSideAmbientColor(color);
    }
    reloadAdsorptionSurfaceProperties();
    emit rendererReloadData();
  }
}

stdx::optional<QColor> AppearanceTreeWidgetController::adsorptionSurfaceOutsideAmbientLightColor()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<QColor> set = std::unordered_set<QColor>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    QColor value = structure->adsorptionSurfaceBackSideAmbientColor();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setAdsorptionSurfaceOutsideDiffuseLightIntensity(double value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setAdsorptionSurfaceBackSideDiffuseIntensity(value);
  }
  reloadAdsorptionSurfaceProperties();
  emit rendererReloadData();
}

stdx::optional<double> AppearanceTreeWidgetController::adsorptionSurfaceOutsideDiffuseLightIntensity()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->adsorptionSurfaceBackSideDiffuseIntensity();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setAdsorptionSurfaceOutsideDiffuseLightColor()
{
  QColor color = QColorDialog::getColor(Qt::white,this,"Choose Color");
  if(color.isValid())
  {
    for(std::shared_ptr<Structure> structure: _structures)
    {
      structure->setAdsorptionSurfaceBackSideDiffuseColor(color);
    }
    reloadAdsorptionSurfaceProperties();
    emit rendererReloadData();
  }
}

stdx::optional<QColor> AppearanceTreeWidgetController::adsorptionSurfaceOutsideDiffuseLightColor()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<QColor> set = std::unordered_set<QColor>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    QColor value = structure->adsorptionSurfaceBackSideDiffuseColor();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setAdsorptionSurfaceOutsideSpecularLightIntensity(double value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setAdsorptionSurfaceBackSideSpecularIntensity(value);
  }
  reloadAdsorptionSurfaceProperties();
  emit rendererReloadData();
}

stdx::optional<double> AppearanceTreeWidgetController::adsorptionSurfaceOutsideSpecularLightIntensity()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->adsorptionSurfaceBackSideSpecularIntensity();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setAdsorptionSurfaceOutsideSpecularLightColor()
{
  QColor color = QColorDialog::getColor(Qt::white,this,"Choose Color");
  if(color.isValid())
  {
    for(std::shared_ptr<Structure> structure: _structures)
    {
      structure->setAdsorptionSurfaceBackSideSpecularColor(color);
    }
    reloadAdsorptionSurfaceProperties();
    emit rendererReloadData();
  }
}

stdx::optional<QColor> AppearanceTreeWidgetController::adsorptionSurfaceOutsideSpecularLightColor()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<QColor> set = std::unordered_set<QColor>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    QColor value = structure->adsorptionSurfaceBackSideSpecularColor();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

void AppearanceTreeWidgetController::setAdsorptionSurfaceOutsideShininess(double value)
{
  for(std::shared_ptr<Structure> structure: _structures)
  {
    structure->setAdsorptionSurfaceBackSideShininess(value);
  }
  reloadAdsorptionSurfaceProperties();
  emit rendererReloadData();
}

stdx::optional<double> AppearanceTreeWidgetController::adsorptionSurfaceOutsideShininessy()
{
  if(_structures.empty())
  {
    return stdx::nullopt;
  }
  std::unordered_set<double> set = std::unordered_set<double>{};
  for(std::shared_ptr<Structure> structure: _structures)
  {
    double value = structure->adsorptionSurfaceBackSideShininess();
    set.insert(value);
  }

  if(set.size() == 1)
  {
    return *set.begin();
  }
  return stdx::nullopt;
}

// expansion
//========================================================================================================================================

void AppearanceTreeWidgetController::expandAtomsItem()
{
  QModelIndex index = indexFromItem(topLevelItem(0),0);
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
  QModelIndex index = indexFromItem(topLevelItem(1),0);
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
  QModelIndex index = indexFromItem(topLevelItem(2),0);
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
  QModelIndex index = indexFromItem(topLevelItem(3),0);
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
  QModelIndex index = indexFromItem(topLevelItem(4),0);
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
