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
#include "optional.h"
#include <iraspakit.h>
#include "iraspamainwindowconsumerprotocol.h"
#include "infocreatorform.h"
#include "infocreationform.h"
#include "infochemicalform.h"
#include "infocitationform.h"

class InfoTreeWidgetController : public QTreeWidget, public ProjectConsumer, public MainWindowConsumer, public Reloadable
{
  Q_OBJECT

public:
  InfoTreeWidgetController(QWidget* parent = nullptr);
  void setProject(std::shared_ptr<ProjectTreeNode> projectTreeNode) override final;
  void setMainWindow(MainWindow *mainWindow) override final {_mainWindow = mainWindow;}
  void setStructures(std::vector<std::shared_ptr<Structure>> structures);
private:
  InfoCreatorForm* _infoCreatorForm;
  InfoCreationForm* _infoCreationForm;
  InfoChemicalForm* _infoChemicalForm;
  InfoCitationForm* _infoCitationForm;

  QPushButton* pushButtonCreator;
  QPushButton* pushButtonCreation;
  QPushButton* pushButtonChemical;
  QPushButton* pushButtonCitation;

  MainWindow *_mainWindow;
  std::shared_ptr<ProjectStructure> _projectStructure;
  std::vector<std::shared_ptr<Structure>> _structures{};

  void reloadData() override final;
  void reloadSelection() override final {;}
  void reloadCreatorData();
  void reloadCreationData();
  void reloadChemicalData();
  void reloadCitationData();

  void reloadAuthorFirstName();
  void reloadAuthorMiddleName();
  void reloadAuthorLastName();
  void reloadAuthorOrchidID();
  void reloadAuthorResearcherID();
  void reloadAuthorAffiliationUniversityName();
  void reloadAuthorAffiliationFacultyName();
  void reloadAuthorAffiliationInstituteName();
  void reloadAuthorAffiliationCityName();
  void reloadAuthorAffiliationCountryName();
  stdx::optional<QString> authorFirstName();
  void setAuthorFirstName(QString name);
  stdx::optional<QString> authorMiddleName();
  void setAuthorMiddleName(QString name);
  stdx::optional<QString> authorLastName();
  void setAuthorLastName(QString name);
  stdx::optional<QString> authorOrchidID();
  void setAuthorOrchidID(QString name);
  stdx::optional<QString> authorResearcherID();
  void setAuthorResearcherID(QString name);
  stdx::optional<QString> authorAffiliationUniversityName();
  void setAuthorAffiliationUniversityName(QString name);
  stdx::optional<QString> authorAffiliationFacultyName();
  void setAuthorAffiliationFacultyName(QString name);
  stdx::optional<QString> authorAffiliationInstituteName();
  void setAuthorAffiliationInstituteName(QString name);
  stdx::optional<QString> authorAffiliationCityName();
  void setAuthorAffiliationCityName(QString name);

  void reloadCreationDate();
  void reloadCreationTemperature();
  void reloadCreationPressure();
  void reloadCreationMethod();
  void reloadCreationRelaxUnitCell();
  void reloadCreationAtomicPositionsSoftwarePackage();
  void reloadCreationAtomicPositionsAlgorithm();
  void reloadCreationAtomicPositionsEigenvalues();
  void reloadCreationAtomicPositionsForceField();
  void reloadCreationAtomicPositionsForceFieldDetails();
  void reloadCreationAtomicChargesSoftwarePackage();
  void reloadCreationAtomicChargesAlgorithm();
  void reloadCreationAtomicChargesEigenvalues();
  void reloadCreationAtomicChargesForceField();
  void reloadCreationAtomicChargesForceFieldDetails();
  stdx::optional<QDate> creationDate();
  void setCreationDate(QDate date);
  stdx::optional<QString> creationTemperature();
  void setCreationTemperature(QString name);
  stdx::optional<QString> creationPressure();
  void setCreationPressure(QString name);
  stdx::optional<QString> creationMethod();
  void setCreationMethod(QString name);
  stdx::optional<QString> creationRelaxUnitCell();
  void setCreationRelaxUnitCell(QString name);
  stdx::optional<QString> creationAtomicPositionsSoftwarePackage();
  void setCreationAtomicPositionsSoftwarePackage(QString name);
  stdx::optional<QString> creationAtomicPositionsAlgorithm();
  void setCreationAtomicPositionsAlgorithm(QString name);
  stdx::optional<QString> creationAtomicPositionsEigenvalues();
  void setCreationAtomicPositionsEigenvalues(QString name);
  stdx::optional<QString> creationAtomicPositionsForceField();
  void setCreationAtomicPositionsForceField(QString name);
  stdx::optional<QString> creationAtomicPositionsForceFieldDetails();
  void setCreationAtomicPositionsForceFieldDetails(QString name);
  stdx::optional<QString> creationAtomicChargesSoftwarePackage();
  void setCreationAtomicChargesSoftwarePackage(QString name);
  stdx::optional<QString> creationAtomicChargesAlgorithm();
  void setCreationAtomicChargesAlgorithm(QString name);
  stdx::optional<QString> creationAtomicChargesEigenvalues();
  void setCreationAtomicChargesEigenvalues(QString name);
  stdx::optional<QString> creationAtomicChargesForceField();
  void setCreationAtomicChargesForceField(QString name);
  stdx::optional<QString> creationAtomicChargesForceFieldDetails();
  void setCreationAtomicChargesForceFieldDetails(QString name);

  void reloadChemicalFormulaMoiety();
  void reloadChemicalFormulaSum();
  void reloadChemicalNameSystematic();
  stdx::optional<QString> chemicalFormulaMoiety();
  void setChemicalFormulaMoiety(QString name);
  stdx::optional<QString> chemicalFormulaSum();
  void setChemicalFormulaSum(QString name);
  stdx::optional<QString> chemicalNameSystematic();
  void setChemicalNameSystematic(QString name);

  void reloadCitationArticleTitle();
  void reloadCitationArticleAuthors();
  void reloadCitationJournalVolume();
  void reloadCitationJournalNumber();
  void reloadCitationPublicationDate();
  void reloadCitationPublicationDOI();
  void reloadCitationPublicationDatabaseCodes();
  stdx::optional<QString> citationArticleTitle();
  void setCitationArticleTitle(QString name);
  stdx::optional<QString> citationJournalTitle();
  void setCitationJournalTitle(QString name);
  stdx::optional<QString> citationArticleAuthors();
  void setCitationArticleAuthors(QString name);
  stdx::optional<QString> citationJournalVolume();
  void setCitationJournalVolume(QString name);
  stdx::optional<QString> citationJournalNumber();
  void setCitationJournalNumber(QString name);
  stdx::optional<QDate> citationPublicationDate();
  void setCitationPublicationDate(QDate date);
  stdx::optional<QString> citationDOI();
  void setCitationDOI(QString name);
  stdx::optional<QString> citationDatabaseCodes();
  void setCitationDatabaseCodes(QString name);

private slots:
  void expandCreatorItem();
  void expandCreationItem();
  void expandChemicalItem();
  void expandCitationItem();
};
