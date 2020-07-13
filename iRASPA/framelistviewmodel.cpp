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

#include "framelistviewmodel.h"
#include <iostream>

FrameListViewModel::FrameListViewModel(): _movie(std::make_shared<Movie>())
{

}

void FrameListViewModel::setMovie(std::shared_ptr<Movie> movie)
{
  beginResetModel();
  _movie = movie;
  endResetModel();
}

bool FrameListViewModel::isMainSelectedItem(std::shared_ptr<iRASPAStructure> iraspa_structure)
{
  if(_movie)
  {
    return (_movie->selectedFrame() == iraspa_structure);
  }
  return false;
}

QModelIndex FrameListViewModel::index(int row, int column, const QModelIndex &parent) const
{
  if (_movie->frames().size() == 0)
    return QModelIndex();

  return createIndex(row, 0 , _movie->frames()[row].get());
}

QModelIndex FrameListViewModel::parent(const QModelIndex &child) const
{
  return QModelIndex();
}

int FrameListViewModel::rowCount(const QModelIndex &parent) const
{
  if(parent == QModelIndex())
  {
    return static_cast<int>(_movie->frames().size());
  }
  return 0;
}

int FrameListViewModel::columnCount(const QModelIndex &parent) const
{
  return 1;
}

QVariant FrameListViewModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
   return QVariant();

  if (role != Qt::DisplayRole)
   return QVariant();

  DisplayableProtocol *item = static_cast<DisplayableProtocol*>(index.internalPointer());
  if(iRASPAStructure* structure = dynamic_cast<iRASPAStructure*>(item))
  {
    return QString(structure->displayName());
  }
  return QString("Unknown");
}
