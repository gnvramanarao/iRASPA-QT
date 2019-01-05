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

#include <math.h>
#include <QDataStream>

union double2
{
  double v[2];
  struct {double x, y;};

  inline double2(double x=0,double y=0):x(x),y(y) {};
  inline double & operator [] (int i) { return v[i]; }
  inline const double & operator [] (int i) const { return v[i]; }


  inline double2 operator + (const double2& rhs) const
  {
    return double2(x+rhs.x,y+rhs.y);
  }

  inline double2 operator - (const double2& rhs) const
  {
    return double2(x-rhs.x,y-rhs.y);
  }

  inline double2 operator * (const double2& rhs) const
  {
    return double2(x*rhs.x, y*rhs.y);
  }

  inline double2& operator += (const double2& rhs)
  {
    x += rhs.x;
    y += rhs.y;
    return *this;
  }

  inline double2& operator -= (const double2& rhs)
  {
    x -= rhs.x;
    y -= rhs.y;
    return *this;
  }

  inline double DotProduct(const double2 &v1, const double2 &v2)
  {
    return v1.x*v2.x+v1.y*v2.y;
  }

  friend QDataStream &operator<<(QDataStream &, const double2 &);
  friend QDataStream &operator>>(QDataStream &, double2 &);
};
