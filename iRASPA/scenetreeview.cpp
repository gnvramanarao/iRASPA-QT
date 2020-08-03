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

#include "scenetreeview.h"
#include "scenetreeviewstyleditemdelegate.h"
#include "scenetreeviewproxystyle.h"
#include "scenetreeviewinsertmoviecommand.h"
#include "scenetreeviewinsertinitialscenecommand.h"
#include "scenetreeviewdeleteselectioncommand.h"
#include <QModelIndexList>
#include <map>
#include <unordered_set>

SceneTreeView::SceneTreeView(QWidget* parent): QTreeView(parent ), _model(std::make_shared<SceneTreeViewModel>())
{
  this->setModel(_model.get());

  this->setSelectionBehavior (QAbstractItemView::SelectRows);
  this->setSelectionMode(QAbstractItemView::ExtendedSelection);
  //this->viewport()->setMouseTracking(true);

  this->setRootIsDecorated(false);
  this->setItemsExpandable(false);
  this->setHeaderHidden(true);
  this->setIndentation(4);

  setDragEnabled(true);
  setAcceptDrops(true);
  setDropIndicatorShown(true);
  setDragDropMode(DragDropMode::DragDrop);
  setDragDropOverwriteMode(false);

  this->setAttribute(Qt::WA_MacShowFocusRect, false);
  this->setStyleSheet("background-color:rgb(240, 240, 240);");
  this->setStyle(new SceneTreeViewProxyStyle());

  this->setItemDelegateForColumn(0,new SceneTreeViewStyledItemDelegate(this));

  QObject::connect(_model.get(), &SceneTreeViewModel::updateSelection, this, &SceneTreeView::reloadSelection);
}

void SceneTreeView::setProject(std::shared_ptr<ProjectTreeNode> projectTreeNode)
{
  _projectTreeNode = projectTreeNode;
  if (projectTreeNode)
  {
    setDragEnabled(projectTreeNode->isEditable());
    setAcceptDrops(projectTreeNode->isEditable());

    if(std::shared_ptr<iRASPAProject> iraspaProject = projectTreeNode->representedObject())
    {
      if(std::shared_ptr<Project> project = iraspaProject->project())
      {
        if (std::shared_ptr<ProjectStructure> projectStructure = std::dynamic_pointer_cast<ProjectStructure>(project))
        {
          _sceneList = projectStructure->sceneList();
          _model->setProject(projectTreeNode);
          expandAll();
          return;
        }
      }
    }
  }

  // if no project is selected or the project is not of type 'ProjectStructure'
  setDragEnabled(false);
  setAcceptDrops(false);
  _sceneList = nullptr;
  _model->setProject(nullptr);
}


QSize SceneTreeView::sizeHint() const
{
  return QSize(200, 800);
}

void SceneTreeView::reloadSelection()
{
  qDebug() << "SceneTreeView::reloadSelection()";
  expandAll();
  if(_sceneList)
  {
    whileBlocking(selectionModel())->clearSelection();

    std::unordered_set<std::shared_ptr<Scene>> selectedScenes = _sceneList->selectedScenes();
    for(std::shared_ptr<Scene> scene: selectedScenes)
    {
      std::optional<int> sceneIndex = _sceneList->findChildIndex(scene);
      if(sceneIndex)
      {
        QModelIndex parentItem = model()->index(*sceneIndex,0,QModelIndex());
        std::unordered_set<std::shared_ptr<Movie>> selectedMovies = scene->selectedMovies();
        for(std::shared_ptr<Movie> movie: selectedMovies)
        {
          std::optional<int> movieIndex = scene->findChildIndex(movie);
          if(movieIndex)
          {
            QModelIndex selectedMovie = model()->index(*movieIndex,0, parentItem);
            whileBlocking(selectionModel())->select(selectedMovie, QItemSelectionModel::Select);
          }
        }
      }
    }
    emit setSelectedRenderFrames(_sceneList->selectediRASPARenderStructures());
    emit setFlattenedSelectedFrames(_sceneList->selectedMoviesIRASPAStructures());

    std::optional<int> sceneIndex = _sceneList->selectedSceneIndex();
    if(sceneIndex)
    {
      qDebug() << "sceneIndex" << *sceneIndex;
      QModelIndex parentItem = model()->index(*sceneIndex,0,QModelIndex());

      if(std::shared_ptr<Scene> selectedScene = _sceneList->selectedScene())
      {
        std::shared_ptr<Movie> movie =  selectedScene->selectedMovie();
        std::optional<int> movieIndex = selectedScene->selectMovieIndex();

        if(movieIndex)
        {
          QModelIndex selectedMovie = model()->index(*movieIndex,0,parentItem);

          whileBlocking(selectionModel())->select(selectedMovie, QItemSelectionModel::Select);

          // set currentIndex for keyboard navigation
          whileBlocking(selectionModel())->setCurrentIndex(selectedMovie, QItemSelectionModel::SelectionFlag::Current);

          emit setSelectedMovie(movie);
          emit setSelectedFrame(movie->selectedFrame());
        }
      }
    }
    else
    {
      qDebug() << "empty selection";
      _mainWindow->recheckRemovalButtons();
      emit setSelectedMovie(nullptr);
      emit setSelectedFrame(nullptr);
      emit setSelectedRenderFrames(std::vector<std::vector<std::shared_ptr<iRASPAStructure>>>{});
      emit setFlattenedSelectedFrames(std::vector<std::shared_ptr<iRASPAStructure>>{});
    }
  }
  update();
}

void SceneTreeView::reloadData()
{
  _mainWindow->updateMenuToSceneTab();
}

void SceneTreeView::keyPressEvent(QKeyEvent *event)
{
  if( event->type() == QEvent::KeyPress )
  {
    QKeyEvent * keyEvent = dynamic_cast<QKeyEvent*>(event);
    if( keyEvent->key() == Qt::Key_Delete )
    {
      deleteSelection();
    }
  }
  QTreeView::keyPressEvent(event);
}

void SceneTreeView::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
  QAbstractItemView::selectionChanged(selected, deselected);

  // avoid empty selection
  if (selectedIndexes().isEmpty())
  {
    reloadSelection();
    return;
  }

  if(_sceneList)
  {
    // clear scene-movie selection
    _sceneList->selectedScenes().clear();
    for(std::shared_ptr<Scene> scene : _sceneList->scenes())
    {
      scene->selectedMovies().clear();
    }

    if(selectedIndexes().size() == 1)
    {
      QModelIndex current = selectedIndexes().front();
      DisplayableProtocol *item = static_cast<DisplayableProtocol*>(current.internalPointer());

      if(Movie* movie = dynamic_cast<Movie*>(item))
      {
        QModelIndex parentItem = current.parent();
        DisplayableProtocol *item = static_cast<DisplayableProtocol*>(parentItem.internalPointer());

        if(Scene* scene = dynamic_cast<Scene*>(item))
        {
          _sceneList->setSelectedScene(scene->shared_from_this());
          scene->setSelectedMovie(movie->shared_from_this());
        }

        emit setSelectedMovie(movie->shared_from_this());
        emit setSelectedFrame(movie->selectedFrame());
      }
    }

    // loop over all selected indexes
    for(QModelIndex index : selectedIndexes())
    {
      DisplayableProtocol *item = static_cast<DisplayableProtocol*>(index.internalPointer());

      if(Movie* movie = dynamic_cast<Movie*>(item))
      {
        QModelIndex parentItem = index.parent();
        DisplayableProtocol *item = static_cast<DisplayableProtocol*>(parentItem.internalPointer());

        if(Scene* scene = dynamic_cast<Scene*>(item))
        {
          _sceneList->selectedScenes().insert(scene->shared_from_this());
          scene->selectedMovies().insert(movie->shared_from_this());
        }
      }
    }

    // set currentIndex for keyboard navigation
    SceneTreeViewModel* pModel = qobject_cast<SceneTreeViewModel*>(model());
    QModelIndex selectedIndex = pModel->indexOfMainSelected();
    if (selectedIndex.isValid())
    {
      selectionModel()->setCurrentIndex(selectedIndex, QItemSelectionModel::SelectionFlag::Current);
    }

    emit setSelectedRenderFrames(_sceneList->selectediRASPARenderStructures());
    emit setFlattenedSelectedFrames(_sceneList->selectedMoviesIRASPAStructures());
    emit updateRenderer();

    update();
  }
}




QString SceneTreeView::nameOfItem(const QModelIndex &current)
{
  DisplayableProtocol *item = static_cast<DisplayableProtocol*>(current.internalPointer());

  if(Movie* movie = dynamic_cast<Movie*>(item))
  {
    QModelIndex parentItem = current.parent();
    DisplayableProtocol *item = static_cast<DisplayableProtocol*>(parentItem.internalPointer());

    if(Scene* scene = dynamic_cast<Scene*>(item))
    {
      _sceneList->setSelectedScene(scene->shared_from_this());
      scene->setSelectedMovie(movie->shared_from_this());
      return movie->displayName();
    }
  }
  return QString("unknown");
}

QPixmap SceneTreeView::selectionToPixmap()
{
  QModelIndexList selectionIndexes = selectionModel()->selectedRows();
  QMimeData* mimeData = model()->mimeData(selectionIndexes);

  QDrag* drag = new QDrag(this);
  drag->setMimeData(mimeData);

  QFontMetrics fontMetrics = QFontMetrics(this->font());

  if(SceneTreeViewModel* pModel = qobject_cast<SceneTreeViewModel*>(model()))
  {
    int maxWidth=0;
    int height=4;

    for(QModelIndex index: selectionIndexes)
    {
      if(DisplayableProtocol *item = pModel->getItem(index))
      {
        QString text = item->displayName();
        QRect textBoundingRect = fontMetrics.boundingRect(text);
        if(textBoundingRect.width() > maxWidth) maxWidth = textBoundingRect.size().width();
        height += textBoundingRect.size().height();
      }
    }

    QRect rect = QRect(QPoint(0,0), QSize(maxWidth, height));
    QPixmap pix = QPixmap(maxWidth, height);
    pix.fill(Qt::transparent);

    QPainter painter( &pix );
    painter.setFont(this->font());
    painter.setOpacity(1.0);
    painter.setPen(Qt::black);

    int currentHeight=0;
    for(QModelIndex index: selectionIndexes)
    {
      if(DisplayableProtocol* item = pModel->getItem(index))
      {
        QString text = item->displayName();
        QRect rect = fontMetrics.boundingRect(text);
        currentHeight += rect.size().height();

        painter.save();
        painter.translate(QPointF(0,currentHeight));
        painter.drawText(rect,Qt::AlignLeft|Qt::AlignCenter, text);
        painter.restore();
      }
    }
    return pix;
  }

  return QPixmap();
}

void SceneTreeView::startDrag(Qt::DropActions supportedActions)
{
  QMimeData* mimeData = model()->mimeData(selectedIndexes());

  QDrag* drag = new QDrag(this);
  drag->setMimeData(mimeData);
  drag->setPixmap(selectionToPixmap());

  if(drag->exec(supportedActions))
  {
    reloadSelection();
  }
}

void SceneTreeView::dragMoveEvent(QDragMoveEvent* event)
{
  QModelIndex index = indexAt(event->pos());

  // use the visualRect of the index to avoid dropping on the background left to items
  if (index.isValid() && !visualRect(index).contains(event->pos()))
  {
    event->ignore();
  }
  else
  {
    event->accept();
  }

  QTreeView::dragMoveEvent(event);
}



void SceneTreeView::newCrystal()
{
  std::shared_ptr<Crystal> crystal = std::make_shared<Crystal>();
  std::shared_ptr<SKAsymmetricAtom> atom = std::make_shared<SKAsymmetricAtom>();
  std::shared_ptr<SKAtomTreeNode> atomtreeNode = std::make_shared<SKAtomTreeNode>(atom);
  crystal->atomsTreeController()->appendToRootnodes(atomtreeNode);
  crystal->expandSymmetry();

  crystal->setRepresentationStyle(Structure::RepresentationStyle::defaultStyle, _mainWindow->colorSets());
  crystal->setAtomForceFieldIdentifier("Default", _mainWindow->forceFieldSets());

  std::shared_ptr<iRASPAStructure> iraspaStructure = std::make_shared<iRASPAStructure>(crystal);

  insertMovie(iraspaStructure);
}

void SceneTreeView::newMolecularCrystal()
{
  std::shared_ptr<MolecularCrystal> molecularCrystal = std::make_shared<MolecularCrystal>();
  std::shared_ptr<SKAsymmetricAtom> atom = std::make_shared<SKAsymmetricAtom>();
  std::shared_ptr<SKAtomTreeNode> atomtreeNode = std::make_shared<SKAtomTreeNode>(atom);
  molecularCrystal->atomsTreeController()->appendToRootnodes(atomtreeNode);
  molecularCrystal->expandSymmetry();

  molecularCrystal->setRepresentationStyle(Structure::RepresentationStyle::defaultStyle, _mainWindow->colorSets());
  molecularCrystal->setAtomForceFieldIdentifier("Default", _mainWindow->forceFieldSets());

  std::shared_ptr<iRASPAStructure> iraspaStructure = std::make_shared<iRASPAStructure>(molecularCrystal);

  insertMovie(iraspaStructure);
}

void SceneTreeView::newProteinCrystal()
{
  std::shared_ptr<ProteinCrystal> proteinCrystal = std::make_shared<ProteinCrystal>();
  std::shared_ptr<SKAsymmetricAtom> atom = std::make_shared<SKAsymmetricAtom>();
  std::shared_ptr<SKAtomTreeNode> atomtreeNode = std::make_shared<SKAtomTreeNode>(atom);
  proteinCrystal->atomsTreeController()->appendToRootnodes(atomtreeNode);
  proteinCrystal->expandSymmetry();

  proteinCrystal->setRepresentationStyle(Structure::RepresentationStyle::defaultStyle, _mainWindow->colorSets());
  proteinCrystal->setAtomForceFieldIdentifier("Default", _mainWindow->forceFieldSets());

  std::shared_ptr<iRASPAStructure> iraspaStructure = std::make_shared<iRASPAStructure>(proteinCrystal);

  insertMovie(iraspaStructure);
}

void SceneTreeView::newMolecule()
{
  std::shared_ptr<Molecule> molecule = std::make_shared<Molecule>();
  std::shared_ptr<SKAsymmetricAtom> atom = std::make_shared<SKAsymmetricAtom>();
  std::shared_ptr<SKAtomTreeNode> atomtreeNode = std::make_shared<SKAtomTreeNode>(atom);
  molecule->atomsTreeController()->appendToRootnodes(atomtreeNode);
  molecule->expandSymmetry();

  molecule->setRepresentationStyle(Structure::RepresentationStyle::defaultStyle, _mainWindow->colorSets());
  molecule->setAtomForceFieldIdentifier("Default", _mainWindow->forceFieldSets());

  std::shared_ptr<iRASPAStructure> iraspaStructure = std::make_shared<iRASPAStructure>(molecule);

  insertMovie(iraspaStructure);
}


void SceneTreeView::newProtein()
{
  std::shared_ptr<Protein> protein = std::make_shared<Protein>();
  std::shared_ptr<SKAsymmetricAtom> atom = std::make_shared<SKAsymmetricAtom>();
  std::shared_ptr<SKAtomTreeNode> atomtreeNode = std::make_shared<SKAtomTreeNode>(atom);
  protein->atomsTreeController()->appendToRootnodes(atomtreeNode);
  protein->expandSymmetry();

  protein->setRepresentationStyle(Structure::RepresentationStyle::defaultStyle, _mainWindow->colorSets());
  protein->setAtomForceFieldIdentifier("Default", _mainWindow->forceFieldSets());

  std::shared_ptr<iRASPAStructure> iraspaStructure = std::make_shared<iRASPAStructure>(protein);

  insertMovie(iraspaStructure);
}

void SceneTreeView::newCrystalEllipsoid()
{
  std::shared_ptr<CrystalEllipsoidPrimitive> crystalEllipsoid = std::make_shared<CrystalEllipsoidPrimitive>();
  std::shared_ptr<SKAsymmetricAtom> atom = std::make_shared<SKAsymmetricAtom>();
  std::shared_ptr<SKAtomTreeNode> atomtreeNode = std::make_shared<SKAtomTreeNode>(atom);
  crystalEllipsoid->atomsTreeController()->appendToRootnodes(atomtreeNode);
  crystalEllipsoid->expandSymmetry();

  crystalEllipsoid->setRepresentationStyle(Structure::RepresentationStyle::defaultStyle, _mainWindow->colorSets());
  crystalEllipsoid->setAtomForceFieldIdentifier("Default", _mainWindow->forceFieldSets());

  std::shared_ptr<iRASPAStructure> iraspaStructure = std::make_shared<iRASPAStructure>(crystalEllipsoid);

  insertMovie(iraspaStructure);
}

void SceneTreeView::newCrystalCylinder()
{
  std::shared_ptr<CrystalCylinderPrimitive> crystalCylinder = std::make_shared<CrystalCylinderPrimitive>();
  std::shared_ptr<SKAsymmetricAtom> atom = std::make_shared<SKAsymmetricAtom>();
  std::shared_ptr<SKAtomTreeNode> atomtreeNode = std::make_shared<SKAtomTreeNode>(atom);
  crystalCylinder->atomsTreeController()->appendToRootnodes(atomtreeNode);
  crystalCylinder->expandSymmetry();

  crystalCylinder->setRepresentationStyle(Structure::RepresentationStyle::defaultStyle, _mainWindow->colorSets());
  crystalCylinder->setAtomForceFieldIdentifier("Default", _mainWindow->forceFieldSets());

  std::shared_ptr<iRASPAStructure> iraspaStructure = std::make_shared<iRASPAStructure>(crystalCylinder);

  insertMovie(iraspaStructure);
}

void SceneTreeView::newCrystalPolygonalPrism()
{
  std::shared_ptr<CrystalPolygonalPrismPrimitive> crystalPolygonalPrismPrimitive = std::make_shared<CrystalPolygonalPrismPrimitive>();
  std::shared_ptr<SKAsymmetricAtom> atom = std::make_shared<SKAsymmetricAtom>();
  std::shared_ptr<SKAtomTreeNode> atomtreeNode = std::make_shared<SKAtomTreeNode>(atom);
  crystalPolygonalPrismPrimitive->atomsTreeController()->appendToRootnodes(atomtreeNode);
  crystalPolygonalPrismPrimitive->expandSymmetry();

  crystalPolygonalPrismPrimitive->setRepresentationStyle(Structure::RepresentationStyle::defaultStyle, _mainWindow->colorSets());
  crystalPolygonalPrismPrimitive->setAtomForceFieldIdentifier("Default", _mainWindow->forceFieldSets());

  std::shared_ptr<iRASPAStructure> iraspaStructure = std::make_shared<iRASPAStructure>(crystalPolygonalPrismPrimitive);

  insertMovie(iraspaStructure);
}

void SceneTreeView::newEllipsoid()
{
  std::shared_ptr<EllipsoidPrimitive> ellipsoidPrimitive = std::make_shared<EllipsoidPrimitive>();
  std::shared_ptr<SKAsymmetricAtom> atom = std::make_shared<SKAsymmetricAtom>();
  std::shared_ptr<SKAtomTreeNode> atomtreeNode = std::make_shared<SKAtomTreeNode>(atom);
  ellipsoidPrimitive->atomsTreeController()->appendToRootnodes(atomtreeNode);
  ellipsoidPrimitive->expandSymmetry();

  ellipsoidPrimitive->setRepresentationStyle(Structure::RepresentationStyle::defaultStyle, _mainWindow->colorSets());
  ellipsoidPrimitive->setAtomForceFieldIdentifier("Default", _mainWindow->forceFieldSets());

  std::shared_ptr<iRASPAStructure> iraspaStructure = std::make_shared<iRASPAStructure>(ellipsoidPrimitive);

  insertMovie(iraspaStructure);
}

void SceneTreeView::newCylinder()
{
  std::shared_ptr<CylinderPrimitive> cylinderPrimitive = std::make_shared<CylinderPrimitive>();
  std::shared_ptr<SKAsymmetricAtom> atom = std::make_shared<SKAsymmetricAtom>();
  std::shared_ptr<SKAtomTreeNode> atomtreeNode = std::make_shared<SKAtomTreeNode>(atom);
  cylinderPrimitive->atomsTreeController()->appendToRootnodes(atomtreeNode);
  cylinderPrimitive->expandSymmetry();

  cylinderPrimitive->setRepresentationStyle(Structure::RepresentationStyle::defaultStyle, _mainWindow->colorSets());
  cylinderPrimitive->setAtomForceFieldIdentifier("Default", _mainWindow->forceFieldSets());

  std::shared_ptr<iRASPAStructure> iraspaStructure = std::make_shared<iRASPAStructure>(cylinderPrimitive);

  insertMovie(iraspaStructure);
}

void SceneTreeView::newPolygonalPrism()
{
  std::shared_ptr<PolygonalPrismPrimitive> polygonalPrismPrimitive = std::make_shared<PolygonalPrismPrimitive>();
  std::shared_ptr<SKAsymmetricAtom> atom = std::make_shared<SKAsymmetricAtom>();
  std::shared_ptr<SKAtomTreeNode> atomtreeNode = std::make_shared<SKAtomTreeNode>(atom);
  polygonalPrismPrimitive->atomsTreeController()->appendToRootnodes(atomtreeNode);
  polygonalPrismPrimitive->expandSymmetry();

  polygonalPrismPrimitive->setRepresentationStyle(Structure::RepresentationStyle::defaultStyle, _mainWindow->colorSets());
  polygonalPrismPrimitive->setAtomForceFieldIdentifier("Default", _mainWindow->forceFieldSets());

  std::shared_ptr<iRASPAStructure> iraspaStructure = std::make_shared<iRASPAStructure>(polygonalPrismPrimitive);

  insertMovie(iraspaStructure);
}



void SceneTreeView::insertMovie(std::shared_ptr<iRASPAStructure> iraspaStructure)
{
  static int number = 0;
  if(_sceneList)
  {
    if(SceneTreeViewModel* pModel = qobject_cast<SceneTreeViewModel*>(model()))
    {
      if(selectedIndexes().empty())
      {
        std::shared_ptr<Movie> movie = std::make_shared<Movie>(iraspaStructure);
        movie->setSelectedFrame(iraspaStructure);
        std::shared_ptr<Scene> scene = std::make_shared<Scene>(movie);
        scene->setSelectedMovie(movie);
        std::map<std::shared_ptr<Scene>, std::unordered_set<std::shared_ptr<Movie>>> selection = {{scene, {movie}}};
        SceneTreeViewInsertInitialSceneCommand *insertSceneCommand = new SceneTreeViewInsertInitialSceneCommand(_mainWindow, this, _sceneList, scene, movie, 0, nullptr);
        _projectTreeNode->representedObject()->undoManager().push(insertSceneCommand);
      }
      else
      {
        QModelIndex current = currentIndex();

        DisplayableProtocol *item = pModel->getItem(current);

        if(Movie* selectedMovie = dynamic_cast<Movie*>(item))
        {
          if(std::shared_ptr<Scene> scene = pModel->parentForMovie(selectedMovie->shared_from_this()))
          {
            int row = current.row() + 1;

            std::shared_ptr<Movie> movie = std::make_shared<Movie>(iraspaStructure);
            movie->setSelectedFrame(iraspaStructure);
            movie->setDisplayName(QString::number(number));
            number++;

            SceneTreeViewInsertMovieCommand *insertMovieCommand = new SceneTreeViewInsertMovieCommand(_mainWindow, this, _sceneList, movie, scene, row, _sceneList->allSelection(), nullptr);
            _projectTreeNode->representedObject()->undoManager().push(insertMovieCommand);
          }
        }
      }
    }
  }
}


void SceneTreeView::restoreSelection(SceneListSelection selection)
{
  _sceneList->setSelection(selection);
}


void SceneTreeView::deleteSelection()
{
  if(SceneTreeViewModel* pModel = qobject_cast<SceneTreeViewModel*>(model()))
  {
    if(!selectedIndexes().empty())
    {
      std::vector<std::shared_ptr<Movie>> selectedMovies{};
      for(QModelIndex index : selectedIndexes())
      {
        DisplayableProtocol *item = pModel->getItem(index);

        if(Movie *movie = dynamic_cast<Movie*>(item))
        {
          selectedMovies.push_back(movie->shared_from_this());
        }
      }

      SceneTreeViewDeleteSelectionCommand *deleteSelectionCommand = new SceneTreeViewDeleteSelectionCommand(_mainWindow, this, _sceneList, selectedMovies, _sceneList->allSelection(), nullptr);
      _projectTreeNode->representedObject()->undoManager().push(deleteSelectionCommand);
    }
  }
}
