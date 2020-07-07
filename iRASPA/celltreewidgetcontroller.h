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
#include <unordered_set>
#include <vector>
#include <iraspakit.h>
#include "iraspamainwindowconsumerprotocol.h"

#include "cellcellform.h"
#include "celltransformcontentform.h"
#include "cellstructuralform.h"
#include "cellsymmetryform.h"

class CellTreeWidgetController : public QTreeWidget, public MainWindowConsumer, public ProjectConsumer, public Reloadable
{
  Q_OBJECT

public:
  CellTreeWidgetController(QWidget* parent = nullptr);
  void setMainWindow(MainWindow *mainWindow)  override{_mainWindow = mainWindow;}
  void setFlattenedSelectedFrames(std::vector<std::shared_ptr<iRASPAStructure>> iraspa_structures);
  void setProject(std::shared_ptr<ProjectTreeNode> projectTreeNode) override;

  void reloadData() override;
  void reloadSelection() override;
private:
  CellCellForm* _cellCellForm;
  CellTransformContentForm* _cellTransformContentForm;
  CellStructuralForm* _cellStructuralForm;
  CellSymmetryForm* _cellSymmetryForm;

  QPushButton* pushButtonCell;
  QPushButton* pushButtonTransformContent;
  QPushButton* pushButtonStructural;
  QPushButton* pushButtonSymmetry;

  MainWindow *_mainWindow;
  std::shared_ptr<ProjectTreeNode> _projectTreeNode;
  std::shared_ptr<ProjectStructure> _projectStructure;
  std::vector<std::shared_ptr<iRASPAStructure>> _iraspa_structures{};
  void reloadCellProperties();

	void reloadStructureType();
  void reloadBoundingBox();
  void reloadUnitCell();
  void reloadUnitCellLengths();
  void reloadUnitCellAngles();
  void reloadCellVolume();
  void reloadCellPerpendicularWidthX();
  void reloadCellPerpendicularWidthY();
  void reloadCellPerpendicularWidthZ();
  void reloadCellMaximumReplicasX();
  void reloadCellMinimumReplicasX();
  void reloadCellMaximumReplicasY();
  void reloadCellMinimumReplicasY();
  void reloadCellMaximumReplicasZ();
  void reloadCellMinimumReplicasZ();
  void reloadRotationAngle();
  void reloadEulerAngles();
  void reloadCellOriginX();
  void reloadCellOriginY();
  void reloadCellOriginZ();

  void setStructureType(int value);
  void replaceStructure(std::vector<std::pair<std::shared_ptr<iRASPAStructure>,std::shared_ptr<iRASPAStructure>>>);
  std::optional<iRASPAStructureType>  structureType();
  SKBoundingBox boundingBox();

  std::optional<double> unitCellAX();
  std::optional<double> unitCellAY();
  std::optional<double> unitCellAZ();

  std::optional<double> unitCellBX();
  std::optional<double> unitCellBY();
  std::optional<double> unitCellBZ();

  std::optional<double> unitCellCX();
  std::optional<double> unitCellCY();
  std::optional<double> unitCellCZ();

  std::optional<double> unitCellLengthA();
  void setUnitCellLengthA(double value);
  std::optional<double> unitCellLengthB();
  void setUnitCellLengthB(double value);
  std::optional<double> unitCellLengthC();
  void setUnitCellLengthC(double value);

  std::optional<double> unitCellAngleAlpha();
  void setUnitCellAngleAlpha(double value);
  std::optional<double> unitCellAngleBeta();
  void setUnitCellAngleBeta(double value);
  std::optional<double> unitCellAngleGamma();
  void setUnitCellAngleGamma(double value);

  std::optional<double> unitCellVolume();
  std::optional<double> unitCellPerpendicularWidthX();
  std::optional<double> unitCellPerpendicularWidthY();
  std::optional<double> unitCellPerpendicularWidthZ();

  std::optional<int> maximumReplicasX();
  void setMaximumReplicasX(int value);
  std::optional<int> maximumReplicasY();
  void setMaximumReplicasY(int value);
  std::optional<int> maximumReplicasZ();
  void setMaximumReplicasZ(int value);
  std::optional<int> minimumReplicasX();
  void setMinimumReplicasX(int value);
  std::optional<int> minimumReplicasY();
  void setMinimumReplicasY(int value);
  std::optional<int> minimumReplicasZ();
  void setMinimumReplicasZ(int value);

  std::optional<double> rotationAngle();
  void setRotationAngle(double angle);
  void rotateYawPlus();
  void rotateYawMinus();
  void rotatePitchPlus();
  void rotatePitchMinus();
  void rotateRollPlus();
  void rotateRollMinus();

  std::optional<double> EulerAngleX();
  void setEulerAngleX(double angle);
  std::optional<double> EulerAngleY();
  void setEulerAngleY(double angle);
  std::optional<double> EulerAngleZ();
  void setEulerAngleZ(double angle);

  std::optional<double> originX();
  void setOriginX(double value);
  std::optional<double> originY();
  void setOriginY(double value);
  std::optional<double> originZ();
  void setOriginZ(double value);


  void reloadTransformContentProperties();

  void reloadFlipAxesProperties();
  void reloadShiftAxesProperties();

  void reloadStructureProperties();

  void reloadStructuralMaterialType();
  void reloadStructuralMass();
  void reloadStructuralDensity();
  void reloadStructuralHeliumVoidFraction();
  void reloadStructuralSpecificVolume();
  void reloadStructuralAccessiblePoreVolume();
  void reloadFrameworkProbeMoleculePopupBox();
  void reloadStructuralVolumetricSurfaceArea();
  void reloadStructuralGravimetricSurfaceArea();
  void reloadStructuralNumberOfChannelSystems();
  void reloadStructuralNumberOfInaccessiblePockets();
  void reloadStructuralDimensionalityOfPoreSystem();
  void reloadStructuralLargestOverallCavityDiameter();
  void reloadStructuralRestrictingPoreDiameter();
  void reloadStructuralLargestDiamtereAlongAViablePath();

  std::optional<double> structuralMass();
  std::optional<double> structuralDensity();
  std::optional<double> structureHeliumVoidFraction();
  std::optional<double> structureSpecificVolume();
  std::optional<double> structureAccessiblePoreVolume();
  void setFrameworkProbeMolecule(int value);
  std::optional<Structure::ProbeMolecule> frameworkProbeMolecule();
  std::optional<double> structureVolumetricNitrogenSurfaceArea();
  std::optional<double> structureGravimetricNitrogenSurfaceArea();
  std::optional<int> structureNumberOfChannelSystems();
  void setStructureNumberOfChannelSystems(int value);
  std::optional<int> structureNumberOfInaccessiblePockets();
  void setStructureNumberOfInaccessiblePockets(int value);
  std::optional<int> structureDimensionalityOfPoreSystem();
  void setStructureDimensionalityOfPoreSystem(int value);
  std::optional<double> structureLargestCavityDiameter();
  void setStructureLargestCavityDiameter(double value);
  std::optional<double> structureRestrictingPoreLimitingDiameter();
  void setStructureRestrictingPoreLimitingDiameter(double value);
  std::optional<double> structureLargestCavityDiameterAlongAviablePath();
  void setStructureLargestCavityDiameterAlongAviablePath(double value);

  void reloadSymmetryProperties();

  void reloadSpaceGroupHallName();
  void reloadSpaceGroupITNumber();
  void reloadSpaceGroupHolohedry();
  void reloadSpaceGroupQualifier();
  void reloadSpaceGroupPrecision();
  void reloadSpaceGroupCenteringType();
  void reloadSpaceGroupCenteringVectors();
  void reloadSpaceGroupInversion();
  void reloadSpaceGroupInversionCenter();
  void reloadSpaceGroupCentroSymmetric();
  void reloadSpaceGroupEnantiomorphic();
  void reloadSpaceGroupLaueGroup();
  void reloadSpaceGroupPointGroup();
  void reloadSpaceGroupSchoenfliesSymbol();
  void reloadSpaceGroupSymmorphicity();

  std::optional<int> symmetrySpaceGroupHallNumber();
  void setSymmetrySpaceGroupHallNumber(int value);
  std::optional<int> symmetrySpaceGroupStamdardNumber();
  void setSymmetrySpaceGroupStandardNumber(int value);
  std::optional<double> symmetryPrecision();
  void setSymmetryPrecision(double value);
  std::optional<QString> symmetryCenteringString();
  std::optional<QString> symmetryHolohedryString();
  std::optional<QString> symmetryQualifierString();

  std::optional<bool> symmetryInversion();
  std::optional<QString> symmetryInversionCenterString();
  std::optional<bool> symmetryCentrosymmetric();
  std::optional<bool> symmetryEnatiomorphic();
  std::optional<int> symmetryPointGroup();
  std::optional<QString> symmetrySymmorphicity();

private slots:
  void expandCellItem();
  void expandTransformContentItem();
  void expandStructuralItem();
  void expandSymmetryItem();

  void computeHeliumVoidFractionPushButton();
  void computeGravimetricSurfaceAreaPushButton();
signals:
  void rendererReloadData();
  void reloadAllViews();
  void redrawRendererWithHighQuality();
  void computeHeliumVoidFraction(std::vector<std::shared_ptr<RKRenderStructure>> structures);
  void computeNitrogenSurfaceArea(std::vector<std::shared_ptr<RKRenderStructure>> structures);
  void invalidateCachedAmbientOcclusionTexture(std::vector<std::shared_ptr<RKRenderStructure>> structures);
  void invalidateIsosurface(std::vector<std::shared_ptr<RKRenderStructure>> structures);
};
