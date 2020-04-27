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

#include "indexpath.h"
#include <algorithm>

IndexPath::IndexPath()
{
  _path.reserve(10);
}

IndexPath::IndexPath(const int index)
{
  _path.reserve(10);
  _path.push_back(index);
}

int& IndexPath::operator[] (const int index) // for non-const objects: can be used for assignment
{
  return _path[index];
}

const int& IndexPath::operator[] (const int index) const // for const objects: can only be used for access
{
  return _path[index];
}

void IndexPath::increaseValueAtLastIndex()
{
  if (!_path.empty())
  {
    _path.back() += 1;
  }
}

void IndexPath::decreaseValueAtLastIndex()
{
  if (!_path.empty())
  {
    _path.back() -= 1;
  }
}

int IndexPath::count()
{
  return _path.size();
}

IndexPath IndexPath::appending(int index)
{
  IndexPath indexpath{};
  indexpath._path.insert(indexpath._path.end(), _path.begin(), _path.end());
  indexpath._path.push_back(index);
  return indexpath;
}

IndexPath IndexPath::removingLastIndex()
{
  IndexPath indexPath{};
  if (_path.size() <= 1) {return indexPath;}
  indexPath._path = std::vector<int>(_path.begin(),std::prev(_path.end(), 1));
  return indexPath;
}

const IndexPath IndexPath::operator+(const IndexPath& rhs)
{
  IndexPath indexpath{};
  indexpath._path.insert(indexpath._path.end(), _path.begin(), _path.end());
  indexpath._path.insert(indexpath._path.end(), rhs._path.begin(), rhs._path.end());
  return indexpath;
}

bool IndexPath::operator<( const IndexPath& otherObject ) const
{
  int l1 = _path.size();
  int l2 = otherObject._path.size();
  for(int pos=0;pos<std::min(l1,l2); pos++)
  {
    int i1 = _path[pos];
    int i2 = otherObject._path[pos];
    if (i1 < i2)
    {
      return true;
    }
    else if (i1 > i2)
    {
      return false;
    }
  }
  if (l1 < l2)
  {
    return true;
  }
  return false;
}

bool IndexPath::operator>( const IndexPath& otherObject ) const
{
  int l1 = _path.size();
  int l2 = otherObject._path.size();
  for(int pos=0;pos<std::min(l1,l2); pos++)
  {
    int i1 = _path[pos];
    int i2 = otherObject._path[pos];
    if (i1 < i2)
    {
      return false;
    }
    else if (i1 > i2)
    {
      return true;
    }
  }
  if (l1 > l2)
  {
    return true;
  }
  return false;
}

bool IndexPath::operator=( const IndexPath& otherObject ) const
{
  int l1 = _path.size();
  int l2 = otherObject._path.size();
  for(int pos=0;pos<std::min(l1,l2); pos++)
  {
    int i1 = _path[pos];
    int i2 = otherObject._path[pos];
    if (i1 < i2)
    {
      return false;
    }
    else if (i1 > i2)
    {
      return false;
    }
  }
  if (l1 < l2)
  {
    return false;
  }
  else if (l1 > l2)
  {
    return false;
  }
  return true;
}

QDebug operator<<(QDebug debug, const IndexPath &c)
{
  QDebugStateSaver saver(debug);
  debug.nospace() << '(' << c._path.size() << ") , " << c._path;
  for(int i: c._path)
  {
    qDebug() << "value: " << i;
  }
  return debug;
}
