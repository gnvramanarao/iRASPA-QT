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

#include "double3.h"
#include "drand48.h"

double3 double3::operator + (const double3& right) const
{
  double3 a;

  a.x=x+right.x;
  a.y=y+right.y;
  a.z=z+right.z;

  return a;
}

double3 double3::operator - (const double3&  right) const
{
  double3 a;

  a.x=x-right.x;
  a.y=y-right.y;
  a.z=z-right.z;

  return a;
}

double3 double3::fract(double3 const &v)
{
  return double3(v.x - floor(v.x), v.y - floor(v.y), v.z - floor(v.z));
}

double3 double3::normalise()
{
  double magnitude = sqrt((x * x) + (y * y) + (z * z)) ;

  if (magnitude != 0)
  {
    x /= magnitude;
    y /= magnitude;
    z /= magnitude;
  }
  return *this;
}

double3 double3::fract()
{
  double3 s = double3(x,y,z);
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

double double3::dot(const double3 &v1, const double3 &v2)
{
  return v1.x*v2.x+v1.y*v2.y+v1.z*v2.z;
}

double3 double3::max(const double3 &v1, const double3 &v2)
{
  return double3(std::max(v1.x,v2.x),std::max(v1.y,v2.y),std::max(v1.z,v2.z));
}

double3 double3::min(const double3 &v1, const double3 &v2)
{
  return double3(std::min(v1.x,v2.x),std::min(v1.y,v2.y),std::min(v1.z,v2.z));
}

double3 double3::cross(const double3 &v1, const double3 &v2)
{
  return double3(v1.y*v2.z - v2.y*v1.z,
                 v1.z*v2.x - v2.z*v1.x,
                 v1.x*v2.y - v2.x*v1.y);
}

double3 double3::normalize(const double3 &v)
{
  double inv_magnitude = 1.0 / sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z)) ;
  return v * inv_magnitude;
}

double3 double3::randomVectorOnUnitSphere()
{
  double ran1,ran2,ranh,ransq = 0.0;

  do
  {
    ran1=2.0*drand48()-1.0;
    ran2=2.0*drand48()-1.0;
    ransq=ran1*ran1+ran2*ran2;
  }
  while(ransq>=1.0);

  ranh=2.0*sqrt(1.0-ransq);
  return double3(ran1*ranh,ran2*ranh,1.0-2.0*ransq);
}

double3 operator*(const double3 &v1, const double3 &v2)
{
   return double3(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z);
}

double3 operator*(const double3 &v, double value)
{
  return double3(value*v.x, value*v.y, value*v.z);
}

double3 operator*(double value, const double3 &v)
{
  return double3(value*v.x, value*v.y, value*v.z);
}


std::ostream& operator<<(std::ostream& out, const double3& vec) // output
{
  out << vec.x;
  out << ",";
  out << vec.y;
  out << ",";
  out << vec.z;
  return out;
}

QDataStream &operator<<(QDataStream &stream, const double3 &v)
{
  stream << v.x;
  stream << v.y;
  stream << v.z;
  return stream;
}

QDataStream &operator>>(QDataStream &stream, double3 &v)
{
  stream >> v.x;
  stream >> v.y;
  stream >> v.z;
  return stream;
}
