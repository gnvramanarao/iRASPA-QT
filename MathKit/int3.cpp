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

#include "int3.h"

int3 int3::operator + (const int3& right) const
{
  int3 a(0,0,0);

  a.x=x+right.x;
  a.y=y+right.y;
  a.z=z+right.z;

  return a;
}

int3 int3::operator - (const int3& right) const
{
  int3 a(0,0,0);

  a.x=x-right.x;
  a.y=y-right.y;
  a.z=z-right.z;

  return a;
}

int modulo(int a, int b)
{
  return a - b * int(floor(double(a) / double(b)));
}

int greatestCommonDivisor(int arg1, int arg2)
{
  int a = arg1;
  int b = arg2;
  while( b != 0 )
  {
    int tempa = b;
    int tempb = modulo(a, b);
    a = tempa;
    b = tempb;
  }
  return abs(a);
}

int3 greatestCommonDivisor(int3 a, int b)
{
  return int3(greatestCommonDivisor(a.x,b), greatestCommonDivisor(a.y,b), greatestCommonDivisor(a.z,b));
}

QDataStream &operator<<(QDataStream &stream, const int3 &v)
{
  stream << v.x;
  stream << v.y;
  stream << v.z;
  return stream;
}

QDataStream &operator>>(QDataStream &stream, int3 &v)
{
  stream >> v.x;
  stream >> v.y;
  stream >> v.z;
  return stream;
}
