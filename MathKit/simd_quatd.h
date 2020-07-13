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

#include <cmath>
#include "double3.h"

union simd_quatd
{
  double v[4];
  struct {double ix, iy, iz, r;};

  simd_quatd(double ix, double iy, double iz, double r);
  simd_quatd(double real, double3 imag);
  simd_quatd(double3 EulerAngles);
  static simd_quatd smallRandomQuaternion(double angleRange);
  double3 EulerAngles();
  simd_quatd normalized();
  static simd_quatd random();
  static simd_quatd yaw(double angle);
  static simd_quatd pitch(double angle);
  static simd_quatd roll(double angle);
  simd_quatd operator +(const simd_quatd& right) const;
  simd_quatd operator /(const double& right) const;
  simd_quatd operator *(const simd_quatd& right) const;
  static simd_quatd data120[120];
  static simd_quatd data60[60];
  static simd_quatd data600[600];
  static simd_quatd data300[300];
  static simd_quatd data1992[1992];
  static double weights1992[1992];
  static simd_quatd data360[360];
  static double weights360[360];

  friend QDataStream &operator<<(QDataStream &, const simd_quatd &);
  friend QDataStream &operator>>(QDataStream &, simd_quatd &);
};

