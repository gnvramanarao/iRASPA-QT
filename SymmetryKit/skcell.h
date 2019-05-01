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
#include <mathkit.h>
#include <vector>
#include "skboundingbox.h"

class SKCell
{
public:
  SKCell();
  SKCell(double a, double b, double c, double alpha, double beta, double gamma);
  SKCell(SKBoundingBox boundingBox);
  SKCell(SKCell superCell, int3 minimumReplica, int3 maximumReplica);
  double3x3 unitCell() const;
  void setUnitCell(const double3x3& unitCell);
  int3 minimumReplicas() const;
  void setMinimumReplicas(const int3& minimumReplicas);
  int3 maximumReplicas() const;
  void setMaximumReplicas(const int3& maximumReplicas);
  SKBoundingBox enclosingBoundingBox();
  std::array<double3,8> corners();
  int3 replicaFromIndex(int index);
  double3x3 box() const;
  void setBox(const double3x3& fullCell);
  int numberOfReplicas() const;

  int minimumReplicaX() const;
  void setMinimumReplicaX(const int newValue);
  int maximumReplicaX() const;
  void setMaximumReplicaX(const int newValue);
  int minimumReplicaY() const;
  void setMinimumReplicaY(const int newValue);
  int maximumReplicaY() const;
  void setMaximumReplicaY(const int newValue);
  int minimumReplicaZ() const;
  void setMinimumReplicaZ(const int newValue);
  int maximumReplicaZ() const;
  void setMaximumReplicaZ(const int newValue);

  double a() const;
  void setLengthA(double value);
  void a(const double& newValue);
  double b() const;
  void setLengthB(double value);
  void b(const double& newValue);
  double c() const;
  void setLengthC(double value);
  void c(const double& newValue);
  double alpha() const;
  void setAlphaAngle(double value);
  void alpha(const double& newValue);
  double beta() const;
  void setBetaAngle(double value);
  void beta(const double& newValue);
  double gamma() const;
  void setGammaAngle(double value);
  void gamma(const double& newValue);

  bool orthorhombic() const;
  double volume() const;
  double3 perpendicularWidths() const;

  double3 applyFullCellBoundaryCondition(double3 dr);
  double3 applyUnitCellBoundaryCondition(double3 dr);

  inline double3 convertToCartesian(double3 s) {return _unitCell * s;}
  inline double3 convertToFractional(double3 r) {return _inverseUnitCell * r;}
  inline double3 convertToNormalizedFractional(double3 r) {return (_inverseFullCell * r).fract();}

  int3 numberOfReplicas(double cutoff);

  SKBoundingBox boundingBox() const {return _boundingBox;}
  void setBoundingBox(SKBoundingBox boundingBox) {_boundingBox = boundingBox;}

  std::vector<float4> renderTranslationVectors();

  double precision() const {return _precision;}
  void setPrecision(double precision) {_precision = precision;}
private:
  qint64 _versionNumber{2};

  qint64 _zValue{1};

  int3 _minimumReplica = int3(0,0,0);
  int3 _maximumReplica = int3(0,0,0);

  double3x3 _unitCell = double3x3();
  double3x3 _inverseUnitCell = double3x3();

  double3x3 _fullCell = double3x3();
  double3x3 _inverseFullCell = double3x3();

  SKBoundingBox _boundingBox = SKBoundingBox();

  double3 _contentShift = double3(0.0,0.0,0.0);
  bool3 _contentFlip = bool3(false,false,false);

  double3 _origin = double3(0.0, 0.0, 0.0);

  double _precision = 1e-4;

  friend QDataStream &operator<<(QDataStream &, const std::shared_ptr<SKCell> &);
  friend QDataStream &operator>>(QDataStream &, std::shared_ptr<SKCell> &);
};

