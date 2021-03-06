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

#include "float3.h"

float3 float3::operator + (const float3& right) const
{
  float3 a;

  a.x=x+right.x;
  a.y=y+right.y;
  a.z=z+right.z;

  return a;
}

float3 float3::operator - (const float3& right) const
{
  float3 a;

  a.x=x-right.x;
  a.y=y-right.y;
  a.z=z-right.z;

  return a;
}


float3 float3::normalise()
{
  float magnitude = sqrt((x * x) + (y * y) + (z * z)) ;

  if (magnitude != 0)
  {
    x /= magnitude;
    y /= magnitude;
    z /= magnitude;
  }
  return *this;
}

float3 float3::fract()
{
  float3 s = float3(x,y,z);
  s.x -= rint(x);
  s.y -= rint(y);
  s.z -= rint(z);

  if(s.x<0.0)
  {
    s.x += 1.0;
  }
  if(s.x>1.0)
  {
    s.x -= 1.0;
  }

  if(s.y<0.0)
  {
    s.y += 1.0;
  }
  if(s.y>1.0)
  {
    s.y -= 1.0;
  }

  if(s.z<0.0)
  {
    s.z += 1.0;
  }
  if(s.z>1.0)
  {
    s.z -= 1.0;
  }
  return s;
}

float float3::dot(const float3 &v1, const float3 &v2)
{
  return v1.x*v2.x+v1.y*v2.y+v1.z*v2.z;
}

float3 float3::max(const float3 &v1, const float3 &v2)
{
  return float3(std::max(v1.x,v2.x),std::max(v1.y,v2.y),std::max(v1.z,v2.z));
}

float3 float3::min(const float3 &v1, const float3 &v2)
{
  return float3(std::min(v1.x,v2.x),std::min(v1.y,v2.y),std::min(v1.z,v2.z));
}

float3 float3::cross(const float3 &v1, const float3 &v2)
{
  return float3(v1.y*v2.z - v1.z*v2.y,
                -v1.x*v2.z + v1.z*v2.x,
                 v1.x*v2.y - v1.y*v2.x);
}

float3 operator*(const float3 &v1, const float3 &v2)
{
   return float3(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z);
}

float3 operator*(const float3 &v, float value)
{
  return float3(value*v.x, value*v.y, value*v.z);
}

float3 operator*(float value, const float3 &v)
{
  return float3(value*v.x, value*v.y, value*v.z);
}


std::ostream& operator<<(std::ostream& out, const float3& vec) // output
{
  out << vec.x;
  out << vec.y;
  out << vec.z;
  return out;
}

