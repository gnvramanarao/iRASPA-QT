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

#pragma once

#include <QString>
#include <QDataStream>
#include <foundationkit.h>
#include <map>
#include <mathkit.h>
#include <QColor>



class SKColorSet
{
public:
  SKColorSet() {}

  enum class ColorScheme: qint64
  {
    jmol = 0, rasmol_modern = 1, rasmol = 2, vesta = 3, multiple_values = 4
  };

  enum class ColorSchemeOrder: qint64
  {
    elementOnly = 0, forceFieldFirst = 1, forceFieldOnly = 2, multiple_values = 3
  };

  SKColorSet(QString name, SKColorSet& from, bool editable);
  SKColorSet(ColorScheme scheme);
  const QString displayName() const {return _displayName;}
  QColor& operator[] (const QString colorName) {return _colors[colorName];}
  const QColor* operator[] (QString colorName) const;
  bool editable() {return _editable;}
private:
  qint64 _versionNumber{1};

  QString _displayName;
  bool _editable = false;
  std::map<QString, QColor> _colors{};

  static std::map<QString, QColor> jMol;
  static std::map<QString, QColor> rasmol;
  static std::map<QString, QColor> rasmolModern;
  static std::map<QString, QColor> vesta;

  friend QDataStream &operator<<(QDataStream &, const SKColorSet &);
  friend QDataStream &operator>>(QDataStream &, SKColorSet &);
};

