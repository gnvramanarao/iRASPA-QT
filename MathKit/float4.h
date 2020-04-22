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

#include <QColor>
#include "double4.h"
#include "double3.h"
#include "float3.h"

union float4
{
  float v[4];
  struct {float x, y, z, w;};


  float4(float x=0,float y=0, float z=0, float w=0):x(x),y(y),z(z),w(w) {}
  float4(double4 a):x(float(a.x)),y(float(a.y)),z(float(a.z)),w(float(a.w)) {}
  float4(double3 a, double b):x(float(a.x)),y(float(a.y)),z(float(a.z)),w(float(b)) {}
  float4(QColor a, double b):x(float(a.redF())),y(float(a.greenF())),z(float(a.blueF())),w(float(b)) {}
  float4(float3 a, float b):x(a.x),y(a.y),z(a.z),w(b) {}
  inline float & operator [] (int i) { return v[i]; }
  inline const float & operator [] (int i) const { return v[i]; }

  void normalise();
  static float dot(const float4 &v1, const float4 &v2);
  static float4 cross(const float4 &v1, const float4 &v2);
  static float4 cross(const float4 &v1, const float4 &v2, const float4 &v3);

  friend float4 operator*(const float4 &v1, const float4 &v2);
  friend float4 operator*(const float4 &v, double value);
  friend float4 operator*(double value, float4 const &v);
  friend float4 operator*(const float4 &v, float value);
  friend float4 operator*(float value, float4 const &v);

  friend float4 operator+(const float4 &v1, const float4 &v2);
  friend float4 operator-(const float4 &v1, const float4 &v2);

};
