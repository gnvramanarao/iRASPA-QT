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

#define _USE_MATH_DEFINES
#include <cmath>
#include <ostream>
#include <QDataStream>

#include "int3.h"

union double3
{
  double v[3];
  struct {double x, y, z;};

  double3(double x=0,double y=0, double z=0):x(x),y(y),z(z) {}
  double3(int3 a):x(double(a.x)),y(double(a.y)),z(double(a.z)) {}
  inline double & operator [] (int i) { return v[i]; }
  inline const double & operator [] (int i) const { return v[i]; }
  double3 operator + (const double3& right) const;
  double3 operator - (const double3& right) const;
  double length() {return sqrt(x*x+y*y+z*z);}
  double length() const {return sqrt(x*x+y*y+z*z);}
  double3 normalise();
  double3 fract();

  static double3 randomVectorOnUnitSphere();

  static double3 fract(double3 const &v);
  static double dot(const double3 &v1, const double3 &v2);
  static double3 max(const double3 &v1, const double3 &v2);
  static double3 min(const double3 &v1, const double3 &v2);
  static double3 cross(const double3 &v1, const double3 &v2);
  static double3 normalize(const double3 &v);


  friend double3 operator*(const double3 &v1, const double3 &v2);
  friend double3 operator*(const double3 &v, double value);
  friend double3 operator*(double value, const double3 &v);
  friend std::ostream& operator<<(std::ostream& out, const double3& vec) ;

  friend QDataStream &operator<<(QDataStream &, const double3 &);
  friend QDataStream &operator>>(QDataStream &, double3 &);

  struct KeyHash{
      size_t operator()(const double3 &k) const{
      size_t h1 = std::hash<double>()(k.x);
      size_t h2 = std::hash<double>()(k.y);
      size_t h3 = std::hash<double>()(k.z);
      return (h1 ^ (h2 << 1)) ^ h3;
      }
  };

  struct KeyEqual{
    bool operator()( const double3& lhs, const double3& rhs ) const{
      return (std::fabs(lhs.x - rhs.x) < 1e-3) && std::fabs(lhs.y - rhs.y) < 1e-3 && std::fabs(lhs.z == rhs.z) < 1e-3;
    }
  };
};

