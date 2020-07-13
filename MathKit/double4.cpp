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

#include "double4.h"

#include <cmath>

/*
double4::double4()
{
  _x=0.0;
  _y=0.0;
  _z=0.0;
  _r=0.0;
}
*/

/*
double4::double4(double v1=0.0, double v2=0.0, double v3=0.0, double v4=0.0)
{
  _x=v1;
  _y=v2;
  _z=v3;
  _z=v4;
}
*/


void double4::normalise()
{
  double magnitude = sqrt((x * x) + (y * y) + (z * z) * (w * w));

  if (magnitude != 0)
  {
    x /= magnitude;
    y /= magnitude;
    z /= magnitude;
    w /= magnitude;
  }
}

double double4::dot(const double4 &v1, const double4 &v2)
{
  return v1.x*v2.x+v1.y*v2.y+v1.z*v2.z+v1.w*v2.w;
}

// http://www.gamedev.net/topic/456301-cross-product-vector-4d/
double4 double4::cross(const double4 &v1, const double4 &v2)
{
  return double4(v1.y*v2.z - v1.z* v2.y,
                 -v1.x*v2.z + v1.z*v2.x,
                 v1.x*v2.y - v1.y*v2.x,
                 0.0);
}


double4 double4::cross(const double4 &v1, const double4 &v2, const double4 &v3)
{
  return double4(v1.y*(v2.z*v3.w - v3.z*v2.w) - v1.z*(v2.y*v3.w - v3.y*v2.w) + v1.w*(v2.y*v3.z - v3.y*v2.z),
                 -v1.x*(v2.z*v3.w - v3.z*v2.w) + v1.z*(v2.x*v3.w - v3.x*v2.w) - v1.w*(v2.x*v3.z - v3.x*v2.z),
                 v1.x*(v2.y*v3.w - v3.y*v2.w) - v1.y*(v2.x*v3.w - v3.x*v2.w) + v1.w*(v2.x*v3.y - v3.x*v2.y),
                 -v1.x*(v2.y*v3.z - v3.y*v2.z) + v1.y*(v2.x*v3.z - v3.x*v2.z) - v1.z*(v2.x*v3.y - v3.x*v2.y));
}

double4 operator*(const double4 &v1, const double4 &v2)
{
   return double4(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z, v1.w*v2.w);
}

double4 operator*(const double4 &v, double value)
{
  return double4(value*v.x, value*v.y, value*v.z, value*v.w);
}

double4 operator*(double value, const double4 &v)
{
  return double4(value*v.x, value*v.y, value*v.z, value*v.w);
}

QDataStream &operator<<(QDataStream &stream, const double4 &v)
{
  stream << v.x;
  stream << v.y;
  stream << v.z;
  stream << v.w;
  return stream;
}

QDataStream &operator>>(QDataStream &stream, double4 &v)
{
  stream >> v.x;
  stream >> v.y;
  stream >> v.z;
  stream >> v.w;
  return stream;
}
