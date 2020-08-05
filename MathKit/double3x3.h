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
#include "simd_quatd.h"

union double4x4;

union double3x3
{
  double m[9];
  double mm[3][3];
  double3 v[3];
  struct {double m11, m21, m31,     // 1st column
                 m12, m22, m32,     // 2nd column
                 m13, m23, m33;     // 3rd column
  };
  struct {double ax, ay, az,     // 1st column
                 bx, by, bz,     // 2nd column
                 cx, cy, cz;     // 3rd column
  };

  double3x3 operator * (const double3x3& right) const;
  inline double3 & operator [] (int i) { return v[i]; }
  inline const double3 & operator [] (int i) const { return v[i]; }

 double3 operator * (const double3& right) const;

   double3x3(double m11=0.0, double m21=0.0, double m31=0.0,
             double m12=0.0, double m22=0.0, double m32=0.0,
             double m13=0.0, double m23=0.0, double m33=0.0)
        : m11(m11), m21(m21), m31(m31),
          m12(m12), m22(m22), m32(m32),
          m13(m13), m23(m23), m33(m33)
          {

          };
  double3x3(double4x4);
  double3x3(double3 v1, double3 v2, double3 v3):
          m11(v1.x), m21(v1.y), m31(v1.z),
          m12(v2.x), m22(v2.y), m32(v2.z),
          m13(v3.x), m23(v3.y), m33(v3.z)

          {

          };
   double3x3(simd_quatd q);

   double determinant(void);
   double trace(void);
   double3x3 const inverse();
   static double3x3 const inverse(const double3x3& right);
   static double3x3 transpose(const double3x3& right);
   double3x3 const transpose(void) const;
   double3x3 inversetranpose(void);
   void solve_lambdas(double3 &res);
   void solve_angles_0( double3 &res, double3 lambdas );
   void solve_angles_1( double3 &res, double3 lambdas );
   void solve_angles_2( double3 &res, double3 lambdas );
   void EigenSystemSymmetric(double3 &eigenvalues,double3x3 &eigenvectors);
   void SingularValueDecompositionSymmetric(double3x3 &U,double3 &D,double3x3 &V);
   void SingularValueDecomposition(double3x3 &a,double3 &w,double3x3 &v);



  friend std::ostream& operator<<(std::ostream& out, const double3x3& vec);

  friend QDataStream &operator<<(QDataStream &, const double3x3 &);
  friend QDataStream &operator>>(QDataStream &, double3x3 &);
};
