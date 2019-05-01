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

FrameListView::FrameListView(QWidget* parent): QListView(parent ), _model(std::make_shared<FrameListViewModel>())
{
  this->setModel(_model.get());

  //this->viewport()->setMouseTracking(true);

  //this->setSelectionMode(QAbstractItemView::MultiSelection);

  this->setStyleSheet("background-color:rgb(240, 240, 240);");

  QObject::connect(this, &FrameListView::clicked,this, &FrameListView::currentFrameChanged);


}

void FrameListView::setProject(std::shared_ptr<ProjectTreeNode> projectTreeNode)
{
  if (projectTreeNode)
  {
    if(std::shared_ptr<iRASPAProject> iraspaProject = projectTreeNode->representedObject())
    {
      if(std::shared_ptr<Project> project = iraspaProject->project())
      {
        if (std::shared_ptr<ProjectStructure> projectStructure = std::dynamic_pointer_cast<ProjectStructure>(project))
        {
          _sceneList = projectStructure->sceneList();
          return;
        }
      }
    }
  }
  _sceneList = nullptr;
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
  if(_movie)
  {
    stdx::optional<int> frameIndex = _movie->selectedFrameIndex();
    if(frameIndex)
    {
      std::shared_ptr<iRASPAStructure> frame = _movie->selectedFrame();
      QModelIndex item = model()->index(*frameIndex,0,QModelIndex());

      selectionModel()->clearSelection();
      selectionModel()->select(item, QItemSelectionModel::Select);
      if(frame)
      {
        emit setCellTreeController(std::vector<std::shared_ptr<Structure>>{frame->structure()});
      }
      update();
    }
  }
}

void FrameListView::reloadData()
{

}

void FrameListView::currentFrameChanged(const QModelIndex &current)
{
  DisplayableProtocol *item = static_cast<DisplayableProtocol*>(current.internalPointer());

  if(iRASPAStructure* structure = dynamic_cast<iRASPAStructure*>(item))
  {
    int frameIndex = current.row();
    _sceneList->setSelectedFrameIndices(frameIndex);
    emit setAtomTreeController(structure->structure()->atomsTreeController());
    emit setCellTreeController(std::vector<std::shared_ptr<Structure>>{structure->structure()});
    emit updateRenderer();
  }
}

void FrameListView::setRootNode(std::shared_ptr<Movie> movie)
{
  _movie = movie;
  _model->setMovie(movie);
}

QSize FrameListView::sizeHint() const
{
    return QSize(200, 800);
}
