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

#include "int3.h"

union float3
{
  float v[3];
  struct {float x, y, z;};

  float3(float x=0,float y=0, float z=0):x(x),y(y),z(z) {}
  float3(int3 a):x(float(a.x)),y(float(a.y)),z(float(a.z)) {}
  inline float & operator [] (int i) { return v[i]; }
  inline const float & operator [] (int i) const { return v[i]; }
  float3 operator + (const float3& right) const;
  float3 operator - (const float3& right) const;
  float length() {return sqrt(x*x+y*y+z*z);}
  float length() const {return sqrt(x*x+y*y+z*z);}
  float3 normalise();
  float3 fract();

  static float dot(const float3 &v1, const float3 &v2);
  static float3 max(const float3 &v1, const float3 &v2);
  static float3 min(const float3 &v1, const float3 &v2);
  static float3 cross(const float3 &v1, const float3 &v2);


  friend float3 operator*(const float3 &v1, const float3 &v2);
  friend float3 operator*(const float3 &v, float value);
  friend float3 operator*(float value, const float3 &v);
  friend std::ostream& operator<<(std::ostream& out, const float3& vec) ;
};

