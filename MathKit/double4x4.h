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

#include "double3.h"
#include "double4.h"
#include "simd_quatd.h"

union double3x3;

union double4x4
{
  double m[16];
  double mm[4][4];
  double4 v[4];
  struct {double m11, m21, m31, m41,   // 1st column
                 m12, m22, m32, m42,   // 2nd column
                 m13, m23, m33, m43,   // 3rd column
                 m14, m24, m34, m44;   // 4th column, {tx,ty,tz,1}
  };
  struct {double ax, ay, az, aw,    // 1st column
                 bx, by, bz, bw,    // 2nd column
                 cx, cy, cz, cw,    // 3rd column
                 wx, wy, wz, ww;    // 4th column, {tx,ty,tz,1}
  };

  inline double4 & operator [] (int i) { return v[i]; }
  inline const double4 & operator [] (int i) const { return v[i]; }

  double4x4(double m11=0.0, double m21=0.0, double m31=0.0, double m41=0.0,
            double m12=0.0, double m22=0.0, double m32=0.0, double m42=0.0,
            double m13=0.0, double m23=0.0, double m33=0.0, double m43=0.0,
            double m14=0.0, double m24=0.0, double m34=0.0, double m44=0.0)
        : m11(m11), m21(m21), m31(m31), m41(m41),
          m12(m12), m22(m22), m32(m32), m42(m42),
          m13(m13), m23(m23), m33(m33), m43(m43),
          m14(m14), m24(m24), m34(m34), m44(m44)
          {

          }

  double4x4(double4 v1, double4 v2, double4 v3, double4 v4): v{v1,v2,v3,v4} {}
  double4x4(double3x3);


  double4x4 operator * (const double4x4& right) const;
  double4 operator * (const double4& right) const;
  inline bool operator==(const double4x4& rhs) const;
  inline bool operator!=(const double4x4& rhs) const;

  double4x4 transpose(void);
  static double4x4 const inverse(const double4x4& right);
  void inverse();
  static double4x4 TransformationAroundArbitraryPoint(double4x4 m, double3 p);


  static double4x4 Identity(void);
  static double4x4 AffinityMatrixToTransformationAroundArbitraryPoint(double4x4 m, double3 p);
  static double4x4 AffinityMatrixToTransformationAroundArbitraryPointWithTranslation(double4x4 m, double3 p, double3 t);
  static double4x4 Rotationdouble4x4FromQuaternion(simd_quatd q);
};

