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

#include <array>
#include <mathkit.h>
#include <foundationkit.h>

class SKBoundingBox
{
public:
  SKBoundingBox();
  SKBoundingBox(double3 minimum, double3 maximum);
  SKBoundingBox(const double3 center, const double3 width, const double scale);
  double3 widths() const;
  std::array<double3,8> const corners() const;
  std::array<std::pair<double3, double3>, 12> const sides() const;
  double3 center();
  double boundingSphereRadius();
  double3 maximum() const {return _maximum;}
  double3 minimum() const {return _minimum;}
  SKBoundingBox adjustForTransformation(double4x4 transformation);
  friend SKBoundingBox operator+(const SKBoundingBox left, const SKBoundingBox right);
  friend SKBoundingBox operator+(const SKBoundingBox left, double3 right);
  friend SKBoundingBox operator-(const SKBoundingBox left, double3 right);
private:
  qint64 _versionNumber{1};
  double3 _minimum = double3(0.0, 0.0, 0.0);
  double3 _maximum = double3(0.0, 0.0, 0.0);

  friend QDataStream &operator<<(QDataStream &, const SKBoundingBox &);
  friend QDataStream &operator>>(QDataStream &, SKBoundingBox &);
};
