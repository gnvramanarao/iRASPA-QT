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

#include "scanner.h"


QString::const_iterator Scanner::find_first_not_of(const QString & chars, const QString & text, QString::const_iterator location)
{
  QString::const_iterator it = location;
  QString::const_iterator end = text.constEnd();
  while(it != end)
  {
    if(chars.contains(*it) == false)
        return it;
    ++it;
  }

  return  text.constEnd();
}


QString::const_iterator Scanner::find_first_of(const QString & chars, const QString & text, QString::const_iterator location)
{
  QString::const_iterator it = location;
  QString::const_iterator end = text.constEnd();
  while(it != end)
  {
    if(chars.contains(*it))
        return it;
    ++it;
  }

  return  text.constEnd();
}

bool Scanner::scanCharacters(CharacterSet set, QString &into)
{
  QString::const_iterator found = find_first_not_of(set.string(), _string, _scanLocation);

  if (found>=_string.constEnd())
  {
    _scanLocation = _string.constEnd();
    into = QString("");
    return false;
  }

  if(found < _string.constEnd())
  {
    into = QString(_scanLocation, found - _scanLocation);
    _scanLocation = found;
    return true;
  }
  _scanLocation = _string.constEnd();
  into = QString("");
  return false;

}

bool Scanner::scanUpToCharacters(CharacterSet set, QString &into)
{
  QString::const_iterator found = find_first_not_of(_charactersToBeSkipped.string(), _string, _scanLocation);

  if (found>=_string.constEnd())
  {
    _scanLocation = _string.constEnd();
    into = QString("");
    return false;
  }

  _scanLocation = found;

  found = find_first_of(set.string(), _string, _scanLocation);

  if(found < _string.constEnd())
  {
    into = QString(_scanLocation, found - _scanLocation);
    _scanLocation = found;
    return true;
  }
  _scanLocation = _string.constEnd();
  into = QString("");
  return false;

}

bool Scanner::isAtEnd()
{
  return _scanLocation >= _string.constEnd();
}

bool Scanner::scanDouble(double &value)
{
  QString::const_iterator found = find_first_not_of(_charactersToBeSkipped.string(), _string, _scanLocation);

  if (found>=_string.constEnd())
  {
    _scanLocation = _string.constEnd();
    return false;
  }

  _scanLocation = found;

  found = find_first_of(_charactersToBeSkipped.string(), _string, _scanLocation);

  if(found < _string.constEnd())
  {
    QString into = QString(_scanLocation, found - _scanLocation);
    _scanLocation = found;
    bool success = false;
    value = into.toDouble(&success);
    return success;
  }
  _scanLocation = _string.constEnd();
  return false;
}

bool Scanner::scanInt(int &value)
{
    QString::const_iterator found = find_first_not_of(_charactersToBeSkipped.string(), _string, _scanLocation);

    if (found>=_string.constEnd())
    {
      _scanLocation = _string.constEnd();
      return false;
    }

    _scanLocation = found;

    found = find_first_of(_charactersToBeSkipped.string(), _string, _scanLocation);

    if(found < _string.constEnd())
    {
      QString into = QString(_scanLocation, found - _scanLocation);
      _scanLocation = found;
      bool success = false;
      value = into.toInt(&success);
      return success;
    }
    _scanLocation = _string.constEnd();
    return false;
}
