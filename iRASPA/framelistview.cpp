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

#include "framelistview.h"
#include "frameliststyleditemdelegate.h"
#include "framelistviewproxystyle.h"
#include <optional>

FrameListView::FrameListView(QWidget* parent): QListView(parent ), _model(std::make_shared<FrameListViewModel>())
{
  this->setModel(_model.get());

  this->viewport()->setMouseTracking(true);

  this->setSelectionBehavior (QAbstractItemView::SelectRows);
  this->setSelectionMode(QAbstractItemView::ExtendedSelection);

  this->setAttribute(Qt::WA_MacShowFocusRect, false);
  this->setStyleSheet("background-color:rgb(240, 240, 240);");
  this->setStyle(new FrameListViewProxyStyle());

  this->setItemDelegateForColumn(0,new FrameListViewStyledItemDelegate(this));
}

void FrameListView::setProject(std::shared_ptr<ProjectTreeNode> projectTreeNode)
{
  _projectTreeNode = projectTreeNode;
  _sceneList = nullptr;
  if (projectTreeNode)
  {
    if(std::shared_ptr<iRASPAProject> iraspaProject = projectTreeNode->representedObject())
    {
      if(std::shared_ptr<Project> project = iraspaProject->project())
      {
        if (std::shared_ptr<ProjectStructure> projectStructure = std::dynamic_pointer_cast<ProjectStructure>(project))
        {
          _sceneList = projectStructure->sceneList();

          _movie = projectStructure->getFrameListModel();
          _model->setMovie(_movie);
        }
      }
    }
  }
}

void FrameListView::setSelectedMovie(std::shared_ptr<Movie> movie)
{
 _movie = movie;
 _model->setMovie(_movie);
}

void FrameListView::keyPressEvent(QKeyEvent *event)
{
  QModelIndex oldIdx = currentIndex();
  QListView::keyPressEvent(event);
  QModelIndex newIdx = currentIndex();
  if(oldIdx.row() != newIdx.row())
  {
    emit clicked(newIdx);
  }
}

void FrameListView::reloadSelection()
{
  if (_movie)
  {
    selectionModel()->clearSelection();
    for (int frameIndex : _movie->selectedFramesIndexSet())
    {
      QModelIndex item = model()->index(frameIndex, 0, QModelIndex());
      selectionModel()->select(item, QItemSelectionModel::Select);
    }
    if (std::optional<int> selectedIndex = _movie->selectedFrameIndex())
    {
      QModelIndex item = model()->index(*selectedIndex, 0, QModelIndex());
      selectionModel()->select(item, QItemSelectionModel::Select);
    }
    update();
  }
}

void FrameListView::reloadData()
{

}


void FrameListView::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
  if (selectedIndexes().isEmpty())
  {
    selectionModel()->select(selected, QItemSelectionModel::QItemSelectionModel::Deselect);
    selectionModel()->select(deselected, QItemSelectionModel::QItemSelectionModel::SelectCurrent);
    return;
  }

  QAbstractItemView::selectionChanged(selected, deselected);

  if(selectedIndexes().size() == 1)
  {
    QModelIndex current = selectedIndexes().front();
    DisplayableProtocol *item = static_cast<DisplayableProtocol*>(current.internalPointer());

    if(iRASPAStructure* iraspa_structure = dynamic_cast<iRASPAStructure*>(item))
    {
      std::shared_ptr<iRASPAStructure> iraspaStructure = iraspa_structure->shared_from_this();

      _sceneList->setSelectedFrameIndices(current.row());
      emit setSelectedFrame(iraspaStructure);

      emit updateRenderer();
    }
  }

  _movie->selectedFramesSet().clear();
  for(QModelIndex index : selectedIndexes())
  {
    iRASPAStructure *item = static_cast<iRASPAStructure*>(index.internalPointer());
    std::shared_ptr<iRASPAStructure> iraspa_structure = item->shared_from_this();
    _movie->selectedFramesSet().insert(iraspa_structure);
  }

  // set currentIndex for keyboard navigation
  if (std::optional<int> selectedIndex = _movie->selectedFrameIndex())
  {
      QModelIndex item = model()->index(*selectedIndex, 0, QModelIndex());
      selectionModel()->setCurrentIndex(item, QItemSelectionModel::SelectionFlag::Current);
  }

  emit setSelectedRenderFrames(_sceneList->selectediRASPARenderStructures());
  emit setFlattenedSelectedFrames(_movie->selectedFramesiRASPAStructures());

  update();
}

QSize FrameListView::sizeHint() const
{
  return QSize(200, 800);
}

