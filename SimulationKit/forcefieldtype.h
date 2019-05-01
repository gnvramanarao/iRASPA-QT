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

#include "mathkit.h"
#include <foundationkit.h>
#include <QDataStream>

class ForceFieldType
{
public:
  ForceFieldType();
  ForceFieldType(const ForceFieldType& forceFieldType);
  ForceFieldType(QString forceFieldStringIdentifier, qint64 atomicNumber, double2 potentialParameters, double mass, double userDefinedRadius, bool editable);
  QString forceFieldStringIdentifier() const {return _forceFieldStringIdentifier;}
  void setForceFieldStringIdentifier(QString forceFieldStringIdentifier) {_forceFieldStringIdentifier = forceFieldStringIdentifier;}
  double2 potentialParameters() {return _potentialParameters;}
  qint64 atomicNumber() {return _atomicNumber;}
  void setAtomicNumber(int atomicNumber) {_atomicNumber = atomicNumber;}
  double userDefinedRadius() {return _userDefinedRadius;}
  double mass() {return _mass;}
  void setEpsilonPotentialParameter(double epsilon)  {_potentialParameters = double2(epsilon,_potentialParameters.y);}
  void setSigmaPotentialParameter(double sigma)  {_potentialParameters = double2(_potentialParameters.x, sigma);}
  void setMass(double mass) {_mass = mass;}
  void setUserDefinedRadius(double radius) {_userDefinedRadius = radius;}
  bool editable() {return _editable;}
  void setEditable(bool value) {_editable = value;}
private:
  qint64 _versionNumber{2};
  bool _editable = false;
  qint64 _atomicNumber{6};
  QString _forceFieldStringIdentifier;
  double2 _potentialParameters = double2(0.0,0.0);
  double _mass = 0.0;
  double _userDefinedRadius = 0.0;
  bool _isVisible = true;

  friend QDataStream &operator<<(QDataStream &, const ForceFieldType &);
  friend QDataStream &operator>>(QDataStream &, ForceFieldType &);
};
