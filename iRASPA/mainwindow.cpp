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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"


#include <QUrl>
#include <QTimer>
#include <QFile>
#include <QDir>
#include <QByteArray>
#include <QDesktopServices>
#include <iraspakit.h>
#include <foundationkit.h>
#include <renderkit.h>
#include "glwidget.h"
#include "masterstackedwidget.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  // propagate mainWindow to all interested controllers
  this->propagateMainWindow(this,this);

  // propagate the logWidget to all interested controller
  this->propagateLogReporter(this->ui->logPlainTextEdit,this);

  ui->detailTabViewController->setCameraTreeView(ui->cameraTreeWidget);
  ui->detailTabViewController->setElementListView(ui->elementListWidget);
  ui->detailTabViewController->setInfoTreeView(ui->infoTreeWidget);
  ui->detailTabViewController->setCellTreeView(ui->cellTreeWidget);
  ui->detailTabViewController->setAtomTreeView(ui->atomTreeView);
  ui->detailTabViewController->setBondListView(ui->bondListView);


  //ui->projectTreeView->setModel(_documentData.projectTreeController());
  ui->projectTreeView->setController(_documentData.projectTreeController());
  QObject::connect(ui->projectTreeView->selectionModel(),&QItemSelectionModel::currentRowChanged,ui->projectTreeView,&ProjectTreeView::setSelectedProject);


  QModelIndex index1 = ui->projectTreeView->model()->index(0,0,QModelIndex());
  QModelIndex index2 = ui->projectTreeView->model()->index(2,0,QModelIndex());
  QModelIndex index3 = ui->projectTreeView->model()->index(4,0,QModelIndex());
  ui->projectTreeView->expand(index1);
  ui->projectTreeView->expand(index2);
  ui->projectTreeView->expand(index3);

  QModelIndex index4 = ui->projectTreeView->model()->index(0,0,index2);
  ui->projectTreeView->expand(index4);

  this->propagateProject(std::shared_ptr<ProjectTreeNode>(nullptr), this);

  QObject::connect(ui->mainToolBar->rightPanel(),&QToolButton::clicked,this,&MainWindow::slideRightPanel);
  QObject::connect(ui->mainToolBar->downPanel(),&QToolButton::clicked,this,&MainWindow::slideDownPanel);
  QObject::connect(ui->mainToolBar->leftPanel(),&QToolButton::clicked,this,&MainWindow::slideLeftPanel);


  QObject::connect(ui->stackedRenderers, &RenderStackedWidget::updateCameraModelViewMatrix, ui->cameraTreeWidget, &CameraTreeWidgetController::reloadCameraModelViewMatrix);
  QObject::connect(ui->stackedRenderers, &RenderStackedWidget::updateCameraEulerAngles, ui->cameraTreeWidget, &CameraTreeWidgetController::reloadCameraEulerAngles);
  QObject::connect(ui->stackedRenderers, &RenderStackedWidget::updateCameraProjection, ui->cameraTreeWidget, &CameraTreeWidgetController::reloadCameraProjection);
  QObject::connect(ui->stackedRenderers, &RenderStackedWidget::updateCameraResetDirection, ui->cameraTreeWidget, &CameraTreeWidgetController::reloadCameraResetDirection);

  QObject::connect(ui->cameraTreeWidget, &CameraTreeWidgetController::updateRenderer,ui->stackedRenderers, &RenderStackedWidget::redraw);
  QObject::connect(ui->cameraTreeWidget, &CameraTreeWidgetController::rendererReloadBackgroundImage,ui->stackedRenderers, &RenderStackedWidget::reloadBackgroundImage);
  QObject::connect(ui->cameraTreeWidget, &CameraTreeWidgetController::rendererReloadSelectionData,ui->stackedRenderers, &RenderStackedWidget::reloadSelectionData);

  QObject::connect(ui->cameraTreeWidget, &CameraTreeWidgetController::rendererCreatePicture,ui->stackedRenderers, &RenderStackedWidget::createPicture);


  QObject::connect(ui->frameListView, &FrameListView::updateRenderer,ui->stackedRenderers, &RenderStackedWidget::reloadData);

  // monitor the opengl mouse events in stackRenderers
  ui->glwidget->installEventFilter(ui->stackedRenderers);

  ui->detailTabViewController->setStyleSheet("QTabWidget::tab-bar { left: 0;}");

  // connect the project-toolbar to the stackedWidget
  QObject::connect(ui->masterToolBar->mapper(), static_cast<void (QSignalMapper::*)(int)>(&QSignalMapper::mapped),ui->masterStackedWidget, &MasterStackedWidget::reloadTab);


  QObject::connect(ui->sceneTreeView, &SceneTreeView::setSelectedMovie, ui->frameListView, &FrameListView::setRootNode);
  QObject::connect(ui->sceneTreeView, &SceneTreeView::setAtomTreeController, ui->detailTabViewController, &DetailTabViewController::setAtomTreeController);

  QObject::connect(ui->sceneTreeView, &SceneTreeView::setCellTreeController, ui->appearanceTreeWidget, &AppearanceTreeWidgetController::setStructures);
  QObject::connect(ui->sceneTreeView, &SceneTreeView::setCellTreeController, ui->cellTreeWidget, &CellTreeWidgetController::setStructures);

  QObject::connect(ui->frameListView, &FrameListView::setCellTreeController, ui->cellTreeWidget, &CellTreeWidgetController::setStructures);

  //QObject::connect(ui->frameListView, &FrameListView::setAtomTreeController, ui->atomTreeView, &AtomTreeView::setRootNode);
  QObject::connect(ui->frameListView, &FrameListView::setAtomTreeController, ui->detailTabViewController, &DetailTabViewController::setAtomTreeController);

  QObject::connect(ui->atomTreeView->atomTreeModel(), &AtomTreeViewModel::rendererReloadData,ui->stackedRenderers, &RenderStackedWidget::reloadRenderData);
  QObject::connect(ui->atomTreeView, &AtomTreeView::rendererReloadData,ui->stackedRenderers, &RenderStackedWidget::reloadRenderData);
  QObject::connect(ui->sceneTreeView->sceneTreeModel(), &SceneTreeViewModel::rendererReloadData,ui->stackedRenderers, &RenderStackedWidget::reloadRenderData);


  QObject::connect(ui->appearanceTreeWidget, &AppearanceTreeWidgetController::rendererReloadData,ui->stackedRenderers, &RenderStackedWidget::reloadRenderData);

  QObject::connect(ui->stackedRenderers, &RenderStackedWidget::rendererWidgetResized,ui->detailTabViewController, &DetailTabViewController::rendererWidgetResized);


  QObject::connect(ui->appearanceTreeWidget, &AppearanceTreeWidgetController::redrawRenderer,ui->stackedRenderers, &RenderStackedWidget::redraw);
  QObject::connect(ui->appearanceTreeWidget, &AppearanceTreeWidgetController::redrawRendererWithLowQuality,ui->stackedRenderers, &RenderStackedWidget::redrawWithLowQuality);
  QObject::connect(ui->appearanceTreeWidget, &AppearanceTreeWidgetController::redrawRendererWithMediumQuality,ui->stackedRenderers, &RenderStackedWidget::redrawWithMediumQuality);
  QObject::connect(ui->appearanceTreeWidget, &AppearanceTreeWidgetController::redrawRendererWithHighQuality,ui->stackedRenderers, &RenderStackedWidget::redrawWithHighQuality);
  QObject::connect(ui->appearanceTreeWidget, &AppearanceTreeWidgetController::redrawRendererWithPictureQuality,ui->stackedRenderers, &RenderStackedWidget::redrawWithPictureQuality);
  QObject::connect(ui->appearanceTreeWidget, &AppearanceTreeWidgetController::invalidateCachedAmbientOcclusionTexture,ui->stackedRenderers, &RenderStackedWidget::invalidateCachedAmbientOcclusionTexture);
  QObject::connect(ui->appearanceTreeWidget, &AppearanceTreeWidgetController::invalidateIsosurface,ui->stackedRenderers, &RenderStackedWidget::invalidateIsosurface);


  QObject::connect(ui->elementListWidget, &ElementListWidgetController::rendererReloadData,ui->stackedRenderers, &RenderStackedWidget::reloadRenderData);
  QObject::connect(ui->elementListWidget, &ElementListWidgetController::invalidateCachedAmbientOcclusionTexture,ui->stackedRenderers, &RenderStackedWidget::invalidateCachedAmbientOcclusionTexture);
  QObject::connect(ui->elementListWidget, &ElementListWidgetController::invalidateIsosurface,ui->stackedRenderers, &RenderStackedWidget::invalidateIsosurface);


  QObject::connect(ui->cellTreeWidget, &CellTreeWidgetController::rendererReloadData,ui->stackedRenderers, &RenderStackedWidget::reloadRenderData);
  QObject::connect(ui->cellTreeWidget, &CellTreeWidgetController::redrawRendererWithHighQuality,ui->stackedRenderers, &RenderStackedWidget::redrawWithHighQuality);
  QObject::connect(ui->cellTreeWidget, &CellTreeWidgetController::computeHeliumVoidFraction,ui->stackedRenderers, &RenderStackedWidget::computeHeliumVoidFraction);
  QObject::connect(ui->cellTreeWidget, &CellTreeWidgetController::computeNitrogenSurfaceArea,ui->stackedRenderers, &RenderStackedWidget::computeNitrogenSurfaceArea);
  QObject::connect(ui->cellTreeWidget, &CellTreeWidgetController::invalidateCachedAmbientOcclusionTexture,ui->stackedRenderers, &RenderStackedWidget::invalidateCachedAmbientOcclusionTexture);
  QObject::connect(ui->cellTreeWidget, &CellTreeWidgetController::invalidateIsosurface,ui->stackedRenderers, &RenderStackedWidget::invalidateIsosurface);


  QObject::connect(ui->atomTreeView, &AtomTreeView::invalidateCachedAmbientOcclusionTexture,ui->stackedRenderers, &RenderStackedWidget::invalidateCachedAmbientOcclusionTexture);


  QObject::connect(ui->stackedRenderers, &RenderStackedWidget::updateAtomSelection,ui->atomTreeView, &AtomTreeView::reloadSelection);

  QObject::connect(ui->actionContents, &QAction::triggered, [this](bool){
     HelpWidget* helpWindow = new HelpWidget(this);
     helpWindow->resize(1200,800);
     helpWindow->show();
  });

	readLibraryOfStructures();
}

void MainWindow::setProject(std::shared_ptr<ProjectTreeNode> project)
{
  if (project)
  {
    if(std::shared_ptr<iRASPAProject> iraspaProject = project->representedObject())
    {
      if(std::shared_ptr<Project> project = iraspaProject->project())
      {
        if (std::shared_ptr<ProjectStructure> projectStructure = std::dynamic_pointer_cast<ProjectStructure>(project))
        {
          ui->sceneTreeView->setRootNode(projectStructure->getSceneTreeModel());
          ui->sceneTreeView->expandAll();

          ui->frameListView->setRootNode(projectStructure->getFrameListModel());

          ui->infoTreeWidget->setStructures(projectStructure->flattenedStructures());
          ui->appearanceTreeWidget->setStructures(projectStructure->flattenedStructures());
          ui->elementListWidget->setStructures(projectStructure->flattenedStructures());
          ui->cellTreeWidget->setStructures(projectStructure->flattenedStructures());
          //ui->atomTreeView->setRootNode(projectStructure->getAtomTreeModel());
          ui->detailTabViewController->setAtomTreeController(projectStructure->getAtomTreeModel());
          ui->detailTabViewController->setBondSetController(projectStructure);

          ui->cameraTreeWidget->reloadData();
        }
      }
    }
  }
}



void MainWindow::propagateProject(std::shared_ptr<ProjectTreeNode> project, QObject *widget)
{
  if (ProjectConsumer *projectConsumer = dynamic_cast<ProjectConsumer*>(widget))
  {
    projectConsumer->setProject(project);
  }

  // set the project for a RKRenderViewController
  if (RKRenderViewController *renderViewController = dynamic_cast<RKRenderViewController*>(widget))
  {
    renderViewController->setRenderDataSource(nullptr);
    if (project)
    {
      if(std::shared_ptr<iRASPAProject> iraspaProject = project->representedObject())
      {
        if(std::shared_ptr<Project> project = iraspaProject->project())
        {
          if (std::shared_ptr<ProjectStructure> projectStructure = std::dynamic_pointer_cast<ProjectStructure>(project))
          {
            renderViewController->setRenderDataSource(projectStructure);
          }
        }
      }
    }
  }

  foreach(QObject *child, widget->children())
  {
    this->propagateProject(project, child);
  }
}

void MainWindow::propagateMainWindow(MainWindow* mainWindow, QObject *widget)
{
  if (MainWindowConsumer *mainWindowConsumer = dynamic_cast<MainWindowConsumer*>(widget))
  {
    mainWindowConsumer->setMainWindow(mainWindow);
  }

  foreach(QObject *child, widget->children())
  {
    this->propagateMainWindow(mainWindow, child);
  }
}

void MainWindow::propagateLogReporter(LogReporting* logReporting, QObject *widget)
{
  if (LogReportingConsumer *logReportingConsumer = dynamic_cast<LogReportingConsumer*>(widget))
  {
    logReportingConsumer->setLogReportingWidget(logReporting);
  }

  foreach(QObject *child, widget->children())
  {
    this->propagateLogReporter(logReporting, child);
  }
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::showAboutDialog()
{
  AboutDialog* about = new AboutDialog(this);
  QObject::connect(about->acknowledgmentsPushButton, &QPushButton::clicked, this, &MainWindow::acknowledgements);

  about->acknowledgmentsPushButton->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #dadbde, stop: 1 #f6f7fa);");

  about->exec();
}

void MainWindow::importFile()
{
  qDebug() << "Import cif-file";

  QString selFilter = tr("cif/pdb files (*.cif *.pdb )");
  QList<QUrl> fileURLs = QFileDialog::getOpenFileUrls(this, tr("Open CIF or PDB-files"), tr("Open CIF or PDB-files"), tr("PDB or CIF files (*.cif *.pdb)"));

  if(fileURLs.isEmpty())
  {
    return;
  }
  else
  {  
    ProjectTreeViewModel* pModel = qobject_cast<ProjectTreeViewModel*>(ui->projectTreeView->model());

    QModelIndex rootLocalProjectsParentIndex = ui->projectTreeView->model()->index(2,0,QModelIndex());
    QModelIndex localProjectsParentIndex = ui->projectTreeView->model()->index(0,0,rootLocalProjectsParentIndex);

    std::shared_ptr<ProjectTreeNode> localProjects =  _documentData.projectTreeController()->localProjects();

    QAbstractItemModel* model = ui->projectTreeView->model();
    QModelIndex index = ui->projectTreeView->selectionModel()->currentIndex();
    //int insertionIndex = index.row() + 1;

    QString fileName = fileURLs.first().toString();


    QFileInfo fileInfo(fileName);
    QModelIndex insertedIndex = model->index(0,0,localProjectsParentIndex);

    ui->logPlainTextEdit->logMessage(LogReporting::ErrorLevel::info,"start reading CIF-file: " + fileInfo.baseName());

    std::shared_ptr<ProjectStructure> project = std::make_shared<ProjectStructure>(fileURLs, _colorSets, _forceFieldSets, ui->logPlainTextEdit);
    std::shared_ptr<iRASPAProject>  iraspaproject = std::make_shared<iRASPAProject>(project);
    std::shared_ptr<ProjectTreeNode> newProject = std::make_shared<ProjectTreeNode>(fileInfo.baseName(), iraspaproject, true, true);

    ui->projectTreeView->insertRows(0, 1, localProjectsParentIndex, newProject);
  }
}

void MainWindow::readLibraryOfStructures()
{
  #ifdef Q_OS_LINUX
    #ifdef USE_SNAP
      QDir dir("/snap/iraspa/current/usr/share/iraspa/");
    #else
      QDir dir("/usr/share/iraspa/");
    #endif
    QUrl fileURL(dir.absoluteFilePath("LibraryOfStructures.irspdoc"));
  #else
    QDir dir(QCoreApplication::applicationDirPath());
    QUrl fileURL(dir.absoluteFilePath("LibraryOfStructures.irspdoc"));
  #endif
    QFileInfo info(fileURL.toString());

  if(info.exists())
  {
	ZipReader reader = ZipReader(fileURL.toString(), QIODevice::ReadOnly);

	QByteArray data = reader.fileData("nl.darkwing.iRASPA_projectData");
	QDataStream stream(&data, QIODevice::ReadOnly);

	DocumentData libraryData;
	try
	{
		std::cout << "start reading library document data: " << data.size() << std::endl;
		stream >> libraryData;
	}
	catch (InvalidArchiveVersionException ex)
	{
		std::cout << "Error: " << ex.message().toStdString() << std::endl;
		std::cout << ex.what() << ex.get_file() << std::endl;
		std::cout << "Function: " << ex.get_func() << std::endl;
	}
	catch (InconsistentArchiveException ex)
	{
		std::cout << "Error: " << ex.message().toStdString() << std::endl;
		std::cout << ex.what() << ex.get_file() << std::endl;
		std::cout << "Function: " << ex.get_func() << std::endl;
	}
	catch (std::exception e)
	{
		std::cout << "Error: " << e.what() << std::endl;
	}

	for (std::shared_ptr<ProjectTreeNode> localNode : libraryData.projectTreeController()->localProjects()->descendantNodes())
	{
		localNode->representedObject()->readData(reader);
	}
	for (std::shared_ptr<ProjectTreeNode> localNode : libraryData.projectTreeController()->localProjects()->childNodes())
	{
		localNode->appendToParent(_documentData.projectTreeController()->galleryProjects());
		
	}

	reader.close();

	ui->projectTreeView->reset();

	QModelIndex index1 = ui->projectTreeView->model()->index(0, 0, QModelIndex());
	QModelIndex index2 = ui->projectTreeView->model()->index(2, 0, QModelIndex());
	QModelIndex index3 = ui->projectTreeView->model()->index(4, 0, QModelIndex());
	ui->projectTreeView->expand(index1);
	ui->projectTreeView->expand(index2);
	ui->projectTreeView->expand(index3);

	QModelIndex index4 = ui->projectTreeView->model()->index(0, 0, index2);
	ui->projectTreeView->expand(index4);
  }
  else
  {
    ui->logPlainTextEdit->logMessage(LogReporting::ErrorLevel::error,"Gallery structure not found");
  }
}

void MainWindow::openFile()
{
  QString selFilter = tr("IRASPA (*.irspdoc )");
  QUrl fileURL = QFileDialog::getOpenFileUrl(this,tr("Open File"),QDir::homePath() + QDir::separator() + "project.irspdoc",
                                             tr("iRASPA (*.irspdoc)"),&selFilter);

  if(!fileURL.isValid())
  {
    return;
  }
  else
  {
    ZipReader reader = ZipReader(fileURL.toLocalFile(), QIODevice::ReadOnly);

    QByteArray colorData = reader.fileData("nl.darkwing.iRASPA_colorData");
    QDataStream colorStream(&colorData, QIODevice::ReadOnly);

    try
    {
      std::cout << "start reading color data: " << colorData.size() << std::endl;
      colorStream >> _colorSets;
    }
    catch(std::exception e)
    {
      std::cout << "Error: " << e.what() << std::endl;
    }

    QByteArray forceFieldData = reader.fileData("nl.darkwing.iRASPA_forceFieldData");
    QDataStream forceFieldStream(&forceFieldData, QIODevice::ReadOnly);

    try
    {
      std::cout << "start reading force field data: " << forceFieldData.size() << std::endl;
      forceFieldStream >> _forceFieldSets;
    }
    catch(std::exception e)
    {
      std::cout << "Error: " << e.what() << std::endl;
    }

    QByteArray data = reader.fileData("nl.darkwing.iRASPA_projectData");
    QDataStream stream(&data, QIODevice::ReadOnly);

    try
    {
      std::cout << "start reading document data: " << data.size() << std::endl;
      stream >> _documentData;
    }
    catch (InvalidArchiveVersionException ex)
    {
      std::cout << "Error: " << ex.message().toStdString() << std::endl;
      std::cout << ex.what() << ex.get_file() << std::endl;
      std::cout << "Function: " << ex.get_func() << std::endl;
    }
    catch(InconsistentArchiveException ex)
    {
      std::cout << "Error: " << ex.message().toStdString() << std::endl;
      std::cout << ex.what() << ex.get_file() << std::endl;
      std::cout << "Function: " << ex.get_func() << std::endl;
    }
    catch(std::exception e)
    {
      std::cout << "Error: " << e.what() << std::endl;
    }

    for(std::shared_ptr<ProjectTreeNode> localNode : _documentData.projectTreeController()->localProjects()->descendantNodes())
    {
      qDebug() << "readdata: " << localNode->displayName();
      localNode->representedObject()->readData(reader);
    }
    reader.close();

    ui->projectTreeView->reset();

    QModelIndex index1 = ui->projectTreeView->model()->index(0,0,QModelIndex());
    QModelIndex index2 = ui->projectTreeView->model()->index(2,0,QModelIndex());
    QModelIndex index3 = ui->projectTreeView->model()->index(4,0,QModelIndex());
    ui->projectTreeView->expand(index1);
    ui->projectTreeView->expand(index2);
    ui->projectTreeView->expand(index3);

    QModelIndex index4 = ui->projectTreeView->model()->index(0,0,index2);
    ui->projectTreeView->expand(index4);
  }
}

void MainWindow::saveFile()
{
  QString selFilter = tr("IRASPA (*.irspdoc )");
  QUrl fileURL = QFileDialog::getSaveFileUrl(this,tr("Save File"),QDir::homePath() + QDir::separator() + "project.irspdoc",
        tr("Images (*.irspdoc)"),&selFilter);

  if(!fileURL.isValid())
  {
    return;
  }
  else
  {
    QFileInfo info(fileURL.toLocalFile());
    QString strNewName = info.path() + "/" + info.completeBaseName() + ".irspdoc";

    ZipWriter zipwriter = ZipWriter(strNewName, QIODevice::WriteOnly|QIODevice::Truncate);
    zipwriter.setCompressionPolicy(ZipWriter::CompressionPolicy::NeverCompress);

    QByteArray byteArray = QByteArray();
    QDataStream stream(&byteArray, QIODevice::WriteOnly);

    stream << _documentData;
    zipwriter.addFile("nl.darkwing.iRASPA_projectData", byteArray);

    std::vector<std::shared_ptr<ProjectTreeNode>> nodes = _documentData.projectTreeController()->localProjects()->descendantNodes();

    std::cout << "Numner of saved nodes: " << nodes.size() << std::endl;
    for(std::shared_ptr<ProjectTreeNode> &node : nodes)
    {
      node->representedObject()->saveData(zipwriter);
    }

    QByteArray colorByteArray = QByteArray();
    QDataStream colorStream(&colorByteArray, QIODevice::WriteOnly);
    colorStream << _colorSets;
    zipwriter.addFile(QString("nl.darkwing.iRASPA_colorData"), colorByteArray);

    QByteArray forceFieldSetsByteArray = QByteArray();
    QDataStream forceFieldSetsStream(&forceFieldSetsByteArray, QIODevice::WriteOnly);
    forceFieldSetsStream << _forceFieldSets;
    zipwriter.addFile(QString("nl.darkwing.iRASPA_forceFieldData"), forceFieldSetsByteArray);

    zipwriter.close();
  }
}


int MainWindow::slideRightPanel(void)
{
  if(ui->structureMainHorizontalSplitter->widget(2)->isHidden())
    ui->structureMainHorizontalSplitter->widget(2)->show();
  else
    ui->structureMainHorizontalSplitter->widget(2)->hide();
  return 0;
}

int MainWindow::slideDownPanel(void)
{
  qDebug() << "slideRightPanel";
  if(ui->structureDetailVerticalSplitter->widget(1)->isHidden())
    ui->structureDetailVerticalSplitter->widget(1)->show();
  else
    ui->structureDetailVerticalSplitter->widget(1)->hide();

  return 0;
}

int MainWindow::slideLeftPanel(void)
{
  if(ui->structureMainHorizontalSplitter->widget(0)->isHidden())
    ui->structureMainHorizontalSplitter->widget(0)->show();
  else
    ui->structureMainHorizontalSplitter->widget(0)->hide();

  return 0;
}

void MainWindow::acknowledgements()
{
#ifdef Q_OS_LINUX
  #ifdef USE_SNAP
    QDir dir("/snap/iraspa/current/usr/share/iraspa/");
  #else
    QDir dir("/usr/share/iraspa/");
  #endif
  QUrl url(dir.absoluteFilePath("AcknowledgedLicenses.pdf"));
#else
  QDir dir(QApplication::applicationDirPath());
  QUrl url(dir.absoluteFilePath("AcknowledgedLicenses.pdf"));
#endif
  url.setScheme("file");
  QDesktopServices::openUrl(url);
}

QDir MainWindow::directoryOf(const QString &subdir)
{
  QDir dir(QApplication::applicationDirPath());
#if defined(Q_OS_WIN)
  if (dir.dirName().toLower() == "debug" || dir.dirName().toLower() == "release")
    dir.cdUp();
#elif defined(Q_OS_MAC)
  if (dir.dirName() == "MacOS")
  {
    dir.cdUp();
    dir.cd("Resources");
  }
#endif
  dir.cd(subdir);
  return dir;
}
