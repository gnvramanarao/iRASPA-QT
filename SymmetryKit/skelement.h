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
#include <array>
#include <vector>
#include <map>

struct SKElement
{
  QString _chemicalSymbol = QString("Undefined");
  qint64 _atomicNumber = 0;
  qint64 _group = 0;
  qint64 _period = 0;
  QString _name = QString("Undefined");
  double _mass = 1.0;
  double _atomRadius = 0.0;
  double _covalentRadius = 0.0;
  double _singleBondCovalentRadius = 0.0;
  double _doubleBondCovalentRadius = 0.0;
  double _tripleBondCovalentRadius = 0.0;
  double _VDWRadius = 1.0;
  std::vector<int> _possibleOxidationStates;
  qint64 _oxidationState = 0;
  double _atomicPolarizability = 0.0;
  SKElement();
  SKElement(QString string, qint64 atomicNumber, qint64 group, qint64 period, QString name, double mass, double atomRadius, double covalentRadius, double singleBondCovalentRadius,
            double doubleBondCovalentRadius, double tripleBondCovalentRadius, double vDWRadius, std::vector<int> possibleOxidationStates);
};


struct PredefinedElements
{
  static std::vector<SKElement> predefinedElements;
  static std::map<QString, int> atomicNumberData;
};
