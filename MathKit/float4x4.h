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

#include "float3.h"
#include "float4.h"
#include "double3x3.h"
#include "double4x4.h"
#include "simd_quatd.h"

union float3x3;

union float4x4
{
  float m[16];
  float mm[4][4];
  float4 v[4];
  struct {float m11, m21, m31, m41,   // 1st column
                m12, m22, m32, m42,   // 2nd column
                m13, m23, m33, m43,   // 3rd column
                m14, m24, m34, m44;   // 4th column, {tx,ty,tz,1}
  };
  struct {float ax, ay, az, aw,    // 1st column
                bx, by, bz, bw,    // 2nd column
                cx, cy, cz, cw,    // 3rd column
                wx, wy, wz, ww;    // 4th column, {tx,ty,tz,1}
  };

  inline float4 & operator [] (int i) { return v[i]; }
  inline const float4 & operator [] (int i) const { return v[i]; }

  float4x4(float m11=0.0, float m21=0.0, float m31=0.0, float m41=0.0,
            float m12=0.0, float m22=0.0, float m32=0.0, float m42=0.0,
            float m13=0.0, float m23=0.0, float m33=0.0, float m43=0.0,
            float m14=0.0, float m24=0.0, float m34=0.0, float m44=0.0)
        : m11(m11), m21(m21), m31(m31), m41(m41),
          m12(m12), m22(m22), m32(m32), m42(m42),
          m13(m13), m23(m23), m33(m33), m43(m43),
          m14(m14), m24(m24), m34(m34), m44(m44)
          {

          };

  //float4x4(); // default constructor
  float4x4(const float4x4 &);  // copy constructor
  float4x4& operator=(const float4x4&);  //assignment operator
  float4x4(float4 v1, float4 v2, float4 v3, float4 v4): v{v1,v2,v3,v4} {}
  float4x4(const float3x3 &);
  float4x4(const double3x3 &);
  float4x4(const double4x4 &);



  float4x4 operator * (const float4x4& right) const;
  float4 operator * (const float4& right) const;
  inline bool operator==(const float4x4& rhs) const;
  inline bool operator!=(const float4x4& rhs) const;

  float4x4 const transpose(void);
  static float4x4 const inverse(const float4x4& right);
  void inverse();
  static float4x4 TransformationAroundArbitraryPoint(float4x4 m, float3 p);


  static float4x4 Identity(void);
  static float4x4 AffinityMatrixToTransformationAroundArbitraryPoint(float4x4 m, float3 p);
  static float4x4 Rotationfloat4x4FromQuaternion(simd_quatd q);
};

