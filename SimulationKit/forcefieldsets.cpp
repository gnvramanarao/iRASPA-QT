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

#include "forcefieldsets.h"

ForceFieldSets::ForceFieldSets(): _forceFieldSets{ForceFieldSet()}
{

}

ForceFieldSet* ForceFieldSets::operator[] (const QString name)
{
  for(ForceFieldSet& forceFieldSet: _forceFieldSets)
  {
    if(QString::compare(forceFieldSet.displayName(), name, Qt::CaseInsensitive) == 0)
    {
      return &forceFieldSet;
    }
  }

  return nullptr;
}


QDataStream &operator<<(QDataStream &stream, const ForceFieldSets &forcefieldTypes)
{
  stream << forcefieldTypes._versionNumber;

  stream << forcefieldTypes._forceFieldSets;

  return stream;
}

QDataStream &operator>>(QDataStream &stream, ForceFieldSets &forcefieldTypes)
{
  qint64 versionNumber;
  stream >> versionNumber;
  if(versionNumber > forcefieldTypes._versionNumber)
  {
    throw InvalidArchiveVersionException(__FILE__, __LINE__, "ForceFieldSets");
  }

  stream >> forcefieldTypes._forceFieldSets;

  return stream;
}
