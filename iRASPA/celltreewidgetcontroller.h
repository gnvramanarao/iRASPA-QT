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
#include "cellstructuralform.h"
#include "cellsymmetryform.h"

class CellTreeWidgetController : public QTreeWidget, public MainWindowConsumer, public ProjectConsumer, public Reloadable
{
  Q_OBJECT

public:
  CellTreeWidgetController(QWidget* parent = nullptr);
  void setMainWindow(MainWindow *mainWindow)  override{_mainWindow = mainWindow;}
  void setStructures(std::vector<std::shared_ptr<Structure>> structures);
  void setProject(std::shared_ptr<ProjectTreeNode> projectTreeNode) override;

  void reloadData() override;
  void reloadSelection() override;
private:
  CellCellForm* _cellCellForm;
  CellStructuralForm* _cellStructuralForm;
  CellSymmetryForm* _cellSymmetryForm;

  QPushButton* pushButtonCell;
  QPushButton* pushButtonStructural;
  QPushButton* pushButtonSymmetry;

  MainWindow *_mainWindow;
  std::shared_ptr<ProjectStructure> _projectStructure;
  std::vector<std::shared_ptr<Structure>> _structures{};
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
  void reloadCellOriginX();
  void reloadCellOriginY();
  void reloadCellOriginZ();

  stdx::optional<iRASPAStructureType>  structureType();
  SKBoundingBox boundingBox();

  stdx::optional<double> unitCellAX();
  stdx::optional<double> unitCellAY();
  stdx::optional<double> unitCellAZ();

  stdx::optional<double> unitCellBX();
  stdx::optional<double> unitCellBY();
  stdx::optional<double> unitCellBZ();

  stdx::optional<double> unitCellCX();
  stdx::optional<double> unitCellCY();
  stdx::optional<double> unitCellCZ();

  stdx::optional<double> unitCellLengthA();
  void setUnitCellLengthA(double value);
  stdx::optional<double> unitCellLengthB();
  void setUnitCellLengthB(double value);
  stdx::optional<double> unitCellLengthC();
  void setUnitCellLengthC(double value);

  stdx::optional<double> unitCellAngleAlpha();
  void setUnitCellAngleAlpha(double value);
  stdx::optional<double> unitCellAngleBeta();
  void setUnitCellAngleBeta(double value);
  stdx::optional<double> unitCellAngleGamma();
  void setUnitCellAngleGamma(double value);

  stdx::optional<double> unitCellVolume();
  stdx::optional<double> unitCellPerpendicularWidthX();
  stdx::optional<double> unitCellPerpendicularWidthY();
  stdx::optional<double> unitCellPerpendicularWidthZ();

  stdx::optional<int> maximumReplicasX();
  void setMaximumReplicasX(int value);
  stdx::optional<int> maximumReplicasY();
  void setMaximumReplicasY(int value);
  stdx::optional<int> maximumReplicasZ();
  void setMaximumReplicasZ(int value);
  stdx::optional<int> minimumReplicasX();
  void setMinimumReplicasX(int value);
  stdx::optional<int> minimumReplicasY();
  void setMinimumReplicasY(int value);
  stdx::optional<int> minimumReplicasZ();
  void setMinimumReplicasZ(int value);

  stdx::optional<double> originX();
  void setOriginX(double value);
  stdx::optional<double> originY();
  void setOriginY(double value);
  stdx::optional<double> originZ();
  void setOriginZ(double value);

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

  stdx::optional<double> structuralMass();
  stdx::optional<double> structuralDensity();
  stdx::optional<double> structureHeliumVoidFraction();
  stdx::optional<double> structureSpecificVolume();
  stdx::optional<double> structureAccessiblePoreVolume();
  void setFrameworkProbeMolecule(int value);
  stdx::optional<ProbeMolecule> frameworkProbeMolecule();
  stdx::optional<double> structureVolumetricNitrogenSurfaceArea();
  stdx::optional<double> structureGravimetricNitrogenSurfaceArea();
  stdx::optional<int> structureNumberOfChannelSystems();
  void setStructureNumberOfChannelSystems(int value);
  stdx::optional<int> structureNumberOfInaccessiblePockets();
  void setStructureNumberOfInaccessiblePockets(int value);
  stdx::optional<int> structureDimensionalityOfPoreSystem();
  void setStructureDimensionalityOfPoreSystem(int value);
  stdx::optional<double> structureLargestCavityDiameter();
  void setStructureLargestCavityDiameter(double value);
  stdx::optional<double> structureRestrictingPoreLimitingDiameter();
  void setStructureRestrictingPoreLimitingDiameter(double value);
  stdx::optional<double> structureLargestCavityDiameterAlongAviablePath();
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

  stdx::optional<int> symmetrySpaceGroupHallNumber();
  void setSymmetrySpaceGroupHallNumber(int value);
  stdx::optional<int> symmetrySpaceGroupStamdardNumber();
  void setSymmetrySpaceGroupStandardNumber(int value);
  stdx::optional<double> symmetryPrecision();
  void setSymmetryPrecision(double value);
  stdx::optional<QString> symmetryCenteringString();
  stdx::optional<QString> symmetryHolohedryString();
  stdx::optional<QString> symmetryQualifierString();

  stdx::optional<bool> symmetryInversion();
  stdx::optional<QString> symmetryInversionCenterString();
  stdx::optional<bool> symmetryCentrosymmetric();
  stdx::optional<bool> symmetryEnatiomorphic();
  stdx::optional<int> symmetryPointGroup();
  stdx::optional<QString> symmetrySymmorphicity();

private slots:
  void expandCellItem();
  void expandStructuralItem();
  void expandSymmetryItem();
signals:
  void rendererReloadData();
  void redrawRendererWithHighQuality();
  void computeHeliumVoidFraction(std::vector<std::shared_ptr<RKRenderStructure>> structures);
  void computeNitrogenSurfaceArea(std::vector<std::shared_ptr<RKRenderStructure>> structures);
  void invalidateCachedAmbientOcclusionTexture(std::vector<std::shared_ptr<RKRenderStructure>> structures);
  void invalidateIsosurface(std::vector<std::shared_ptr<RKRenderStructure>> structures);
};
