#include "importfiledialog.h"



ImportFileDialog::ImportFileDialog(QWidget *parent) : QFileDialog(parent)
{
  setWindowTitle("Import structures");
  setOption(QFileDialog::DontUseNativeDialog);
  setFileMode(QFileDialog::ExistingFiles);

  setNameFilters(QStringList({"PDB or CIF files (*.cif *.pdb)","CIF files (*.cif)","PDB files (*.pdb)"}));
  selectNameFilter(QString("PDB or CIF files (*.cif *.pdb)"));

  checkboxSeperateProjects = new QCheckBox(this);
  checkboxSeperateProjects->setText("As seperate projects");
  layout()->addWidget(checkboxSeperateProjects);


  QWidget* frame = new QWidget(this);
  QHBoxLayout* horizontalLayout = new QHBoxLayout();
  frame->setLayout(horizontalLayout);
  checkboxOnlyAsymmetricUnitCell = new QCheckBox(frame);
  checkboxOnlyAsymmetricUnitCell->setText("Only assymmetric unit");
  horizontalLayout->addWidget(checkboxOnlyAsymmetricUnitCell);

  checkboxImportAsMolecule = new QCheckBox(frame);
  checkboxImportAsMolecule->setText("As molecule");
  horizontalLayout->addWidget(checkboxImportAsMolecule);

  layout()->addWidget(frame);
}
