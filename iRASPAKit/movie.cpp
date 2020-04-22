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

#include "movie.h"

Movie::Movie()
{

}

Movie::Movie(QString displayName): _displayName(displayName)
{

}

void Movie::setVisibility(bool visibility)
{
  _isVisible = visibility;
  for(std::shared_ptr<iRASPAStructure> frame : _frames)
  {
    frame->structure()->setVisibility(visibility);
  }
}

std::shared_ptr<iRASPAStructure> Movie::selectedFrame()
{
  if(_selectedFrame)
    return _selectedFrame;
  if(!_frames.empty())
  {
    _selectedFrame = _frames.front();
    return _selectedFrame;
  }
  return nullptr;
}

void Movie::setSelectedFrameIndex(int frameIndex)
{
  int index = std::min(frameIndex, int(_frames.size())-1);
  _selectedFrame = _frames[index];
}

std::optional<int> Movie::selectedFrameIndex()
{
  std::vector<std::shared_ptr<iRASPAStructure>>::const_iterator itr = std::find(_frames.begin(), _frames.end(), selectedFrame());
  if (itr != _frames.end())
  {
    int row = itr-_frames.begin();
    return row;
  }

  return std::nullopt;
}

std::vector<std::shared_ptr<Structure>> Movie::structures() const
{
  std::vector<std::shared_ptr<Structure>> structures = std::vector<std::shared_ptr<Structure>>();

  for(std::shared_ptr<iRASPAStructure> frame: _frames)
  {
    if(std::shared_ptr<Structure> structure = frame->structure())
    {
      structures.push_back(structure);
    }
  }
  return structures;
}

QString Movie::displayName() const
{
 return _displayName;
}

QDataStream &operator<<(QDataStream &stream, const std::shared_ptr<Movie> &movie)
{
  stream << movie->_versionNumber;
  stream << movie->_displayName;
  stream << movie->_frames;
  return stream;
}

QDataStream &operator>>(QDataStream &stream, std::shared_ptr<Movie> &movie)
{
  qint64 versionNumber;
  stream >> versionNumber;
  if(versionNumber > movie->_versionNumber)
  {
    throw InvalidArchiveVersionException(__FILE__, __LINE__, "Movie");
  }
  stream >> movie->_displayName;
  stream >> movie->_frames;
  return stream;
}
