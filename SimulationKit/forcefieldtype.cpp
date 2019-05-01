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

#include "forcefieldtype.h"


ForceFieldType::ForceFieldType()
{

}

ForceFieldType::ForceFieldType(const ForceFieldType& forceFieldType): _editable(forceFieldType._editable), _atomicNumber(forceFieldType._atomicNumber),
    _forceFieldStringIdentifier(forceFieldType._forceFieldStringIdentifier), _potentialParameters(forceFieldType._potentialParameters),
    _mass(forceFieldType._mass), _userDefinedRadius(forceFieldType._userDefinedRadius)
{

}

ForceFieldType::ForceFieldType(QString forceFieldStringIdentifier, qint64 atomicNumber, double2 potentialParameters, double mass, double userDefinedRadius, bool editable)
{
  _forceFieldStringIdentifier = forceFieldStringIdentifier;
  _editable = editable;
  _atomicNumber = atomicNumber;
  _potentialParameters = potentialParameters;
  _mass = mass;
  _userDefinedRadius = userDefinedRadius;
}

QDataStream &operator<<(QDataStream &stream, const ForceFieldType &forcefieldType)
{
  stream << forcefieldType._versionNumber;
  stream << forcefieldType._editable;
  stream << forcefieldType._atomicNumber;
  stream << forcefieldType._forceFieldStringIdentifier;
  stream << forcefieldType._potentialParameters;
  stream << forcefieldType._mass;
  stream << forcefieldType._userDefinedRadius;
  stream << forcefieldType._isVisible;
  return stream;
}

QDataStream &operator>>(QDataStream &stream, ForceFieldType &forcefieldType)
{
  qint64 versionNumber;
  stream >> versionNumber;
  if(versionNumber > forcefieldType._versionNumber)
  {
    throw InvalidArchiveVersionException(__FILE__, __LINE__, "ForceFieldType");
  }

  stream >> forcefieldType._editable;
  stream >> forcefieldType._atomicNumber;
  stream >> forcefieldType._forceFieldStringIdentifier;
  stream >> forcefieldType._potentialParameters;
  stream >> forcefieldType._mass;
  stream >> forcefieldType._userDefinedRadius;

  if(versionNumber >= 2)
  {
    stream >> forcefieldType._isVisible;
  }

  return stream;
}
