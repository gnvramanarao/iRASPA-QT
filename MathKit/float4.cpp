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

#include "float4.h"

#define _USE_MATH_DEFINES
#include <cmath>

/*
float4::float4()
{
  _x=0.0;
  _y=0.0;
  _z=0.0;
  _r=0.0;
}
*/

/*
float4::float4(float v1=0.0, float v2=0.0, float v3=0.0, float v4=0.0)
{
  _x=v1;
  _y=v2;
  _z=v3;
  _z=v4;
}
*/



void float4::normalise()
{
  float magnitude = sqrt((x * x) + (y * y) + (z * z) * (w * w));

  if (magnitude != 0)
  {
    x /= magnitude;
    y /= magnitude;
    z /= magnitude;
    w /= magnitude;
  }
}

float float4::dot(const float4 &v1, const float4 &v2)
{
  return v1.x*v2.x+v1.y*v2.y+v1.z*v2.z+v1.w*v2.w;
}

// http://www.gamedev.net/topic/456301-cross-product-vector-4d/
float4 float4::cross(const float4 &v1, const float4 &v2)
{
  return float4(v1.y*v2.z - v1.z* v2.y,
                 -v1.x*v2.z + v1.z*v2.x,
                 v1.x*v2.y - v1.y*v2.x,
                 0.0);
}


float4 float4::cross(const float4 &v1, const float4 &v2, const float4 &v3)
{
  return float4(v1.y*(v2.z*v3.w - v3.z*v2.w) - v1.z*(v2.y*v3.w - v3.y*v2.w) + v1.w*(v2.y*v3.z - v3.y*v2.z),
                 -v1.x*(v2.z*v3.w - v3.z*v2.w) + v1.z*(v2.x*v3.w - v3.x*v2.w) - v1.w*(v2.x*v3.z - v3.x*v2.z),
                 v1.x*(v2.y*v3.w - v3.y*v2.w) - v1.y*(v2.x*v3.w - v3.x*v2.w) + v1.w*(v2.x*v3.y - v3.x*v2.y),
                 -v1.x*(v2.y*v3.z - v3.y*v2.z) + v1.y*(v2.x*v3.z - v3.x*v2.z) - v1.z*(v2.x*v3.y - v3.x*v2.y));
}

float4 operator*(const float4 &v1, const float4 &v2)
{
   return float4(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z, v1.w*v2.w);
}

float4 operator*(const float4 &v, double value)
{
  return float4(value*v.x, value*v.y, value*v.z, value*v.w);
}

float4 operator*(double value, float4 const &v)
{
  return float4(value*v.x, value*v.y, value*v.z, value*v.w);
}

float4 operator*(const float4 &v, float value)
{
  return float4(value*v.x, value*v.y, value*v.z, value*v.w);
}

float4 operator*(float value, float4 const &v)
{
  return float4(value*v.x, value*v.y, value*v.z, value*v.w);
}


float4 operator+(const float4 &v1, const float4 &v2)
{
   return float4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
}

float4 operator-(const float4 &v1, const float4 &v2)
{
   return float4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
}

