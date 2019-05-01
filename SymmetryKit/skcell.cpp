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

#include "skcell.h"
#include <math.h>
#include <iostream>
#include <vector>
#include <algorithm>

#define _USE_MATH_DEFINES
#include "math.h"

SKCell::SKCell()
{
  SKCell(90.0, 90.0, 90.0, 90.0*M_PI/180.0, 90.0*M_PI/180.0, 90.0*M_PI/180.0);
}

SKCell::SKCell(double a, double b, double c, double alpha, double beta, double gamma)
{
  double temp = (cos(alpha) - cos(gamma) * cos(beta)) / sin(gamma);

  double3 v1 = double3(a, 0.0, 0.0);
  double3 v2 = double3(b * cos(gamma), b * sin(gamma), 0.0);
  double3 v3 = double3(c * cos(beta), c * temp, c * sqrt(1.0 - cos(beta)*cos(beta)-temp*temp));
  _unitCell = double3x3(v1, v2, v3);

  _inverseUnitCell = _unitCell.inverse();
  _fullCell = _unitCell;

  int dx = _maximumReplica[0] - _minimumReplica[0] + 1;
  int dy = _maximumReplica[1] - _minimumReplica[1] + 1;
  int dz = _maximumReplica[2] - _minimumReplica[2] + 1;

  _fullCell[0][0] *= dx;  _fullCell[1][0] *= dy;  _fullCell[2][0] *= dz;
  _fullCell[0][1] *= dx;  _fullCell[1][1] *= dy;  _fullCell[2][1] *= dz;
  _fullCell[0][2] *= dx;  _fullCell[1][2] *= dy;  _fullCell[2][2] *= dz;

  _inverseFullCell = _fullCell.inverse();

  _boundingBox = enclosingBoundingBox();
}

SKCell::SKCell(SKBoundingBox boundingBox)
{
  _minimumReplica = int3(0,0,0);
  _maximumReplica = int3(0,0,0);

  double3 v1 = double3(boundingBox.maximum().x-boundingBox.minimum().x, 0.0, 0.0);
  double3 v2 = double3(0.0, boundingBox.maximum().y-boundingBox.minimum().y, 0.0);
  double3 v3 = double3(0.0, 0.0, boundingBox.maximum().z-boundingBox.minimum().z);

  _unitCell = double3x3(v1, v2, v3);
  _inverseUnitCell = _unitCell.inverse();
  _fullCell = _unitCell;
  _inverseFullCell = _fullCell.inverse();
}

SKCell::SKCell(SKCell superCell, int3 minimumReplica, int3 maximumReplica)
{
  double3 v1 = double(maximumReplica.x - minimumReplica.x + 1) * superCell._unitCell[0];
  double3 v2 = double(maximumReplica.y - minimumReplica.y + 1) * superCell._unitCell[1];
  double3 v3 = double(maximumReplica.z - minimumReplica.z + 1) * superCell._unitCell[2];
  _unitCell = double3x3(v1, v2, v3);
  _inverseUnitCell = _unitCell.inverse();
  _fullCell = _unitCell;
  _inverseFullCell = _fullCell.inverse();

  _minimumReplica = int3(0,0,0);
  _maximumReplica = int3(0,0,0);
}


double3x3 SKCell::unitCell() const
{
  return _unitCell;
}

void SKCell::setUnitCell(const double3x3& unitCell)
{
  _unitCell = unitCell;

  _inverseUnitCell = _unitCell.inverse();
  _fullCell = _unitCell;

  int dx = _maximumReplica[0] - _minimumReplica[0] + 1;
  int dy = _maximumReplica[1] - _minimumReplica[1] + 1;
  int dz = _maximumReplica[2] - _minimumReplica[2] + 1;

  _fullCell[0][0] *= dx;  _fullCell[1][0] *= dy;  _fullCell[2][0] *= dz;
  _fullCell[0][1] *= dx;  _fullCell[1][1] *= dy;  _fullCell[2][1] *= dz;
  _fullCell[0][2] *= dx;  _fullCell[1][2] *= dy;  _fullCell[2][2] *= dz;

  _inverseFullCell = _fullCell.inverse();
}

int3 SKCell::minimumReplicas() const
{
  return _minimumReplica;
}

void SKCell::setMinimumReplicas(const int3& minimumReplicas)
{
  _minimumReplica = minimumReplicas;

  int dx = _maximumReplica.x - _minimumReplica.x + 1;

  _fullCell[0][0] = _unitCell[0][0] * double(dx);
  _fullCell[0][1] = _unitCell[0][1] * double(dx);
  _fullCell[0][2] = _unitCell[0][2] * double(dx);

  int dy = _maximumReplica.y - _minimumReplica.y + 1;

  _fullCell[1][0] = _unitCell[1][0] * double(dy);
  _fullCell[1][1] = _unitCell[1][1] * double(dy);
  _fullCell[1][2] = _unitCell[1][2] * double(dy);

  int dz = _maximumReplica.z - _minimumReplica.z + 1;

  _fullCell[2][0] = _unitCell[2][0] * double(dz);
  _fullCell[2][1] = _unitCell[2][1] * double(dz);
  _fullCell[2][2] = _unitCell[2][2] * double(dz);


  _inverseFullCell = _fullCell.inverse();
}

int3 SKCell::maximumReplicas() const
{
  return _maximumReplica;
}

void SKCell::setMaximumReplicas(const int3& maximumReplicas)
{
  _maximumReplica = maximumReplicas;

  int dx = _maximumReplica.x - _minimumReplica.x + 1;

  _fullCell[0][0] = _unitCell[0][0] * double(dx);
  _fullCell[0][1] = _unitCell[0][1] * double(dx);
  _fullCell[0][2] = _unitCell[0][2] * double(dx);

  int dy = _maximumReplica.y - _minimumReplica.y + 1;

  _fullCell[1][0] = _unitCell[1][0] * double(dy);
  _fullCell[1][1] = _unitCell[1][1] * double(dy);
  _fullCell[1][2] = _unitCell[1][2] * double(dy);

  int dz = _maximumReplica.z - _minimumReplica.z + 1;

  _fullCell[2][0] = _unitCell[2][0] * double(dz);
  _fullCell[2][1] = _unitCell[2][1] * double(dz);
  _fullCell[2][2] = _unitCell[2][2] * double(dz);

  _inverseFullCell = _fullCell.inverse();
}

SKBoundingBox SKCell::enclosingBoundingBox()
{
  double3 c0 = _unitCell * double3(double(_minimumReplica.x),   double(_minimumReplica.y),   double(_minimumReplica.z));
  double3 c1 = _unitCell * double3(double(_maximumReplica.x+1), double(_minimumReplica.y),   double(_minimumReplica.z));
  double3 c2 = _unitCell * double3(double(_maximumReplica.x+1), double(_maximumReplica.y+1), double(_minimumReplica.z));
  double3 c3 = _unitCell * double3(double(_minimumReplica.x),   double(_maximumReplica.y+1), double(_minimumReplica.z));
  double3 c4 = _unitCell * double3(double(_minimumReplica.x),   double(_minimumReplica.y),   double(_maximumReplica.z+1));
  double3 c5 = _unitCell * double3(double(_maximumReplica.x+1), double(_minimumReplica.y),   double(_maximumReplica.z+1));
  double3 c6 = _unitCell * double3(double(_maximumReplica.x+1), double(_maximumReplica.y+1), double(_maximumReplica.z+1));
  double3 c7 = _unitCell * double3(double(_minimumReplica.x),   double(_maximumReplica.y+1), double(_maximumReplica.z+1));

  double listx[] = {c0.x, c1.x, c2.x, c3.x, c4.x, c5.x, c6.x, c7.x};
  double listy[] = {c0.y, c1.y, c2.y, c3.y, c4.y, c5.y, c6.y, c7.y};
  double listz[] = {c0.z, c1.z, c2.z, c3.z, c4.z, c5.z, c6.z, c7.z};
  double3 minimum = double3(*std::min_element(listx,listx+8),*std::min_element(listy,listy+8),*std::min_element(listz,listz+8));
  double3 maximum = double3(*std::max_element(listx,listx+8),*std::max_element(listy,listy+8),*std::max_element(listz,listz+8));

  return SKBoundingBox(minimum, maximum);
}

std::array<double3,8> SKCell::corners()
{
  std::array<double3,8> temp = {{
    double3(0.0, 0.0, 0.0),
    double3(1.0, 0.0, 0.0),
    double3(1.0, 1.0, 0.0),
    double3(0.0, 1.0, 0.0),
    double3(0.0, 0.0, 1.0),
    double3(1.0, 0.0, 1.0),
    double3(1.0, 1.0, 1.0),
    double3(0.0, 1.0, 1.0)
  }};
  return temp;
}

int3 SKCell::replicaFromIndex(int index)
{
  int dx = _maximumReplica.x - _minimumReplica.x + 1;
  int dy = _maximumReplica.y - _minimumReplica.y + 1;
  int dz = _maximumReplica.z - _minimumReplica.z + 1;
  int n = dx * dy * dz;

  int k1 = ( (index % n) / (dz * dy) ) + _minimumReplica.x;
  int k2 = ( (index % n) / dz ) % dy + _minimumReplica.y;
  int k3 = (index % n) % dz + _minimumReplica.z;

   return int3(k1, k2, k3);
}

double3x3 SKCell::box() const
{
  return _fullCell;
}
void SKCell::setBox(const double3x3& fullCell)
{
  _fullCell = fullCell;
  _inverseFullCell = _fullCell.inverse();

  int dx = _maximumReplica.x - _minimumReplica.x + 1;
  int dy = _maximumReplica.y - _minimumReplica.y + 1;
  int dz = _maximumReplica.z - _minimumReplica.z + 1;

  _unitCell[0][0] /= double(dx);  _unitCell[1][0] /= double(dy);  _unitCell[2][0] /= double(dz);
  _unitCell[0][1] /= double(dx);  _unitCell[1][1] /= double(dy);  _unitCell[2][1] /= double(dz);
  _unitCell[0][2] /= double(dx);  _unitCell[1][2] /= double(dy);  _unitCell[2][2] /= double(dz);

  _inverseUnitCell = _unitCell.inverse();
}


int SKCell::numberOfReplicas() const
{
  int dx = _maximumReplica.x - _minimumReplica.x + 1;
  int dy = _maximumReplica.y - _minimumReplica.y + 1;
  int dz = _maximumReplica.z - _minimumReplica.z + 1;

  return dx * dy * dz;
}

int SKCell::minimumReplicaX() const
{
  return _minimumReplica.x;
}

void SKCell::setMinimumReplicaX(const int newValue)
{
  _minimumReplica.x = newValue;

  int dx = _maximumReplica.x - _minimumReplica.x + 1;

  _fullCell[0][0] = _unitCell[0][0] * double(dx);
  _fullCell[0][1] = _unitCell[0][1] * double(dx);
  _fullCell[0][2] = _unitCell[0][2] * double(dx);

  _inverseFullCell = _fullCell.inverse();
}

int SKCell::maximumReplicaX() const
{
  return _maximumReplica.x;
}

void SKCell::setMaximumReplicaX(const int newValue)
{
  _maximumReplica.x = newValue;

  int dx = _maximumReplica.x - _minimumReplica.x + 1;

  _fullCell[0][0] = _unitCell[0][0] * double(dx);
  _fullCell[0][1] = _unitCell[0][1] * double(dx);
  _fullCell[0][2] = _unitCell[0][2] * double(dx);

  _inverseFullCell = _fullCell.inverse();
}

int SKCell::minimumReplicaY() const
{
  return _minimumReplica.y;
}

void SKCell::setMinimumReplicaY(const int newValue)
{
  _minimumReplica.y = newValue;

  int dy = _maximumReplica.y - _minimumReplica.y + 1;

  _fullCell[1][0] = _unitCell[1][0] * double(dy);
  _fullCell[1][1] = _unitCell[1][1] * double(dy);
  _fullCell[1][2] = _unitCell[1][2] * double(dy);

  _inverseFullCell = _fullCell.inverse();
}

int SKCell::maximumReplicaY() const
{
  return _maximumReplica.y;
}

void SKCell::setMaximumReplicaY(const int newValue)
{
  _maximumReplica.y = newValue;

  int dy = _maximumReplica.y - _minimumReplica.y + 1;

  _fullCell[1][0] = _unitCell[1][0] * double(dy);
  _fullCell[1][1] = _unitCell[1][1] * double(dy);
  _fullCell[1][2] = _unitCell[1][2] * double(dy);

  _inverseFullCell = _fullCell.inverse();
}

int SKCell::minimumReplicaZ() const
{
  return _minimumReplica.z;
}

void SKCell::setMinimumReplicaZ(const int newValue)
{
  _minimumReplica.z = newValue;

  int dz = _maximumReplica.z - _minimumReplica.z + 1;

  _fullCell[2][0] = _unitCell[2][0] * double(dz);
  _fullCell[2][1] = _unitCell[2][1] * double(dz);
  _fullCell[2][2] = _unitCell[2][2] * double(dz);

  _inverseFullCell = _fullCell.inverse();
}

int SKCell::maximumReplicaZ() const
{
  return _maximumReplica.z;
}

void SKCell::setMaximumReplicaZ(const int newValue)
{
  _maximumReplica.z = newValue;

  int dz = _maximumReplica.z - _minimumReplica.z + 1;

  _fullCell[2][0] = _unitCell[2][0] * double(dz);
  _fullCell[2][1] = _unitCell[2][1] * double(dz);
  _fullCell[2][2] = _unitCell[2][2] * double(dz);

  _inverseFullCell = _fullCell.inverse();
}

double SKCell::a() const
{
  return _unitCell[0].length();
}

void SKCell::a(const double& newValue)
{
  double multiplier = newValue/a();
  _unitCell = double3x3(multiplier * _unitCell[0], _unitCell[1], _unitCell[2]);
}

void SKCell::setLengthA(double value)
{
  double multiplier = value/a();
  _unitCell = double3x3(multiplier * _unitCell[0], _unitCell[1], _unitCell[2]);
}

double SKCell::b() const
{
  return _unitCell[1].length();
}
void SKCell::b(const double& newValue)
{
  double multiplier = newValue/b();
  _unitCell = double3x3(_unitCell[0], multiplier * _unitCell[1], _unitCell[2]);
}

void SKCell::setLengthB(double value)
{
  double multiplier = value/b();
  _unitCell = double3x3(_unitCell[0], multiplier * _unitCell[1], _unitCell[2]);
}

double SKCell::c() const
{
  return _unitCell[2].length();
}

void SKCell::setLengthC(double value)
{
  double multiplier = value/c();
  _unitCell = double3x3(_unitCell[0], _unitCell[1], multiplier * _unitCell[2]);
}

void SKCell::c(const double& newValue)
{
  double multiplier = newValue/c();
  _unitCell = double3x3(_unitCell[0], _unitCell[1], multiplier * _unitCell[2]);
}
double SKCell::alpha() const
{
  double3 column2 = _unitCell[1];
  double3 column3  = _unitCell[2];
  double length2 = column2.length();
  double length3 = column3.length();

  return acos(double3::dot(column2, column3) / (length2 * length3));
}
void SKCell::alpha(const double& newValue)
{
  double betaAngle = beta();
  double gammaAngle = gamma();
  double temp = (cos(newValue) - cos(gammaAngle) * cos(betaAngle)) / sin(gammaAngle);
  double lengthC = c();

  double3 v3 = lengthC * double3(cos(betaAngle), temp, sqrt(1.0 - cos(betaAngle)*cos(betaAngle)-temp*temp));
  _unitCell = double3x3(_unitCell[0], _unitCell[1], v3);
}

void SKCell::setAlphaAngle(double value)
{
  double betaAngle = beta();
  double gammaAngle = gamma();
  double temp = (cos(value) - cos(gammaAngle) * cos(betaAngle)) / sin(gammaAngle);
  double lengthC = c();

  double3 v3 = lengthC * double3(cos(betaAngle), temp, sqrt(1.0 - cos(betaAngle)*cos(betaAngle)-temp*temp));
  _unitCell = double3x3(_unitCell[0], _unitCell[1], v3);
}

double SKCell::beta() const
{
  double3 column1 = _unitCell[0];
  double3 column3 = _unitCell[2];
  double length1 = column1.length();
  double length3 = column3.length();

  return acos(double3::dot(column1, column3) / (length1 * length3));
}


void SKCell::setBetaAngle(double value)
{
  double alphaAngle = alpha();
  double gammaAngle = gamma();
  double temp = (cos(alphaAngle) - cos(gammaAngle) * cos(value)) / sin(gammaAngle);
  double lengthC = c();

  double3 v3  = lengthC * double3(cos(value), temp, sqrt(1.0 - cos(value)*cos(value)-temp*temp));
  _unitCell = double3x3(_unitCell[0], _unitCell[1], v3);
}

void SKCell::beta(const double& newValue)
{
  double alphaAngle = alpha();
  double gammaAngle = gamma();
  double temp = (cos(alphaAngle) - cos(gammaAngle) * cos(newValue)) / sin(gammaAngle);
  double lengthC = c();

  double3 v3  = lengthC * double3(cos(newValue), temp, sqrt(1.0 - cos(newValue)*cos(newValue)-temp*temp));
  _unitCell = double3x3(_unitCell[0], _unitCell[1], v3);
}
double SKCell::gamma() const
{
  double3 column1 = _unitCell[0];
  double3 column2 = _unitCell[1];
  double length1 = column1.length();
  double length2 = column2.length();

  return acos(double3::dot(column1, column2) / (length1 * length2));
}

void SKCell::setGammaAngle(double value)
{
  double alphaAngle = alpha();
  double betaAngle = beta();
  double lengthB = b();
  double lengthC = c();
  double temp = (cos(alphaAngle) - cos(value) * cos(betaAngle)) / sin(value);

  double3 v2 = lengthB * double3(cos(value), sin(value), 0.0);
  double3 v3 = lengthC * double3(cos(betaAngle), temp, sqrt(1.0 - cos(betaAngle)*cos(betaAngle)-temp*temp));
  _unitCell = double3x3(_unitCell[0], v2, v3);
}

void SKCell::gamma(const double& newValue)
{
  double alphaAngle = alpha();
  double betaAngle = beta();
  double lengthB = b();
  double lengthC = c();
  double temp = (cos(alphaAngle) - cos(newValue) * cos(betaAngle)) / sin(newValue);

  double3 v2 = lengthB * double3(cos(newValue), sin(newValue), 0.0);
  double3 v3 = lengthC * double3(cos(betaAngle), temp, sqrt(1.0 - cos(betaAngle)*cos(betaAngle)-temp*temp));
  _unitCell = double3x3(_unitCell[0], v2, v3);
}

bool SKCell::orthorhombic() const
{
  return (fabs(alpha() - M_PI / 2.0) < 0.001) && (fabs(beta() - M_PI / 2.0) < 0.001) && (fabs(gamma() - M_PI / 2.0) < 0.001);
}

double SKCell::volume() const
{
  double3 column1 = _unitCell[0];
  double3 column2 = _unitCell[1];
  double3 column3 = _unitCell[2];

  double3 v2 = double3::cross(column2, column3);

  return double3::dot(column1,v2);
}

double3 SKCell::perpendicularWidths() const
{
  double3 column1 = _unitCell[0];
  double3 column2 = _unitCell[1];
  double3 column3 = _unitCell[2];

  double3 v1 = double3::cross(column1, column2);
  double3 v2 = double3::cross(column2, column3);
  double3 v3 = double3::cross(column3, column1);

  double volume = double3::dot(column1,v2);

  return double3(volume/v2.length(), volume/v3.length(), volume/v1.length());
}

double3 SKCell::applyFullCellBoundaryCondition(double3 dr)
{
  // convert from xyz to abc
  double3 s = _inverseFullCell * dr;

  // apply boundary condition
  s.x -= rint(s.x);
  s.y -= rint(s.y);
  s.z -= rint(s.z);

  // convert from abc to xyz and return value
  return _fullCell * s;
}

double3 SKCell::applyUnitCellBoundaryCondition(double3 dr)
{
  // convert from xyz to abc
  double3 s = _inverseUnitCell * dr;

  // apply boundary condition
  s.x -= rint(s.x);
  s.y -= rint(s.y);
  s.z -= rint(s.z);

  // convert from abc to xyz and return value
  return _unitCell * s;
}

int3 SKCell::numberOfReplicas(double cutoff)
{
  double3 column1 = _unitCell[0];
  double3 column2 = _unitCell[1];
  double3 column3 = _unitCell[2];

  double3 v1 = double3::cross(column1, column2);
  double3 v2 = double3::cross(column2, column3);
  double3 v3 = double3::cross(column3, column1);

  double volume = double3::dot(column1,v2);

  double3 perpendicularWith = double3(volume/v2.length(), volume/v3.length(), volume/v1.length());

  int3 replicas = int3(int(ceil(2.0 * cutoff / (perpendicularWith.x + 0.000001))),
                       int(ceil(2.0 * cutoff / (perpendicularWith.y + 0.000001))),
                       int(ceil(2.0 * cutoff / (perpendicularWith.z + 0.000001))));


   return replicas;
}

std::vector<float4> SKCell::renderTranslationVectors()
{
  std::vector<float4> vectors{};

  for(int k3=_minimumReplica.z; k3<=_maximumReplica.z; k3++)
  {
    for(int k2=_minimumReplica.y; k2<=_maximumReplica.y; k2++)
    {
      for(int k1=_minimumReplica.x; k1<=_maximumReplica.x; k1++)
      {
        vectors.push_back(float4{float(k1),float(k2),float(k3),0.0f});
      }
    }
  }

   return vectors;
}

QDataStream &operator<<(QDataStream &stream, const std::shared_ptr<SKCell> &cell)
{
  stream << cell->_versionNumber;
  stream << cell->_unitCell;
  stream << cell->_minimumReplica;
  stream << cell->_maximumReplica;

  stream << cell->_unitCell;
  stream << cell->_inverseUnitCell;

  stream << cell->_fullCell;
  stream << cell->_inverseFullCell;

  stream << cell->_boundingBox;

  stream << cell->_contentShift;

  stream << cell->_precision;

  stream << cell->_contentFlip;

  return stream;
}

QDataStream &operator>>(QDataStream &stream, std::shared_ptr<SKCell> &cell)
{
  qint64 versionNumber;
  stream >> versionNumber;
  if(versionNumber > cell->_versionNumber)
  {
    throw InvalidArchiveVersionException(__FILE__, __LINE__, "SKCell");
  }

  stream >> cell->_unitCell;
  stream >> cell->_minimumReplica;
  stream >> cell->_maximumReplica;

  stream >> cell->_unitCell;
  stream >> cell->_inverseUnitCell;

  stream >> cell->_fullCell;
  stream >> cell->_inverseFullCell;

  stream >> cell->_boundingBox;

  stream >> cell->_contentShift;

  stream >> cell->_precision;

  if(versionNumber >= 2)
  {
    stream >> cell->_contentFlip;
  }

  return stream;
}