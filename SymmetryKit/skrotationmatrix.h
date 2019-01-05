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

#include <ostream>
#include <mathkit.h>
#include "skdefinitions.h"


struct SKRotationMatrix: public int3x3
{
  SKRotationMatrix();
  SKRotationMatrix(int3 v1, int3 v2, int3 v3);

 // SKRotationMatrix proper();
 friend std::ostream& operator<<(std::ostream& os, const SKRotationMatrix& setting);

  SKRotationMatrix operator * (const SKRotationMatrix& right) const;
  int3 operator * (const int3& right) const;
  double3 operator * (const double3& right) const;
  SKRotationMatrix operator-() const;

  static SKRotationMatrix zero;
  static SKRotationMatrix identity;
  static SKRotationMatrix inversionIdentity;

  // rotations for principle axes
  static SKRotationMatrix r_2_100;
  static SKRotationMatrix r_2i_100;
  static SKRotationMatrix r_3_100;
  static SKRotationMatrix r_3i_100;
  static SKRotationMatrix r_4_100;
  static SKRotationMatrix r_4i_100;
  static SKRotationMatrix r_6_100;
  static SKRotationMatrix r_6i_100;

  static SKRotationMatrix r_2_010;
  static SKRotationMatrix r_2i_010;
  static SKRotationMatrix r_3_010;
  static SKRotationMatrix r_3i_010;
  static SKRotationMatrix r_4_010;
  static SKRotationMatrix r_4i_010;
  static SKRotationMatrix r_6_010;
  static SKRotationMatrix r_6i_010;

  static SKRotationMatrix r_2_001;
  static SKRotationMatrix r_2i_001;
  static SKRotationMatrix r_3_001;
  static SKRotationMatrix r_3i_001;
  static SKRotationMatrix r_4_001;
  static SKRotationMatrix r_4i_001;
  static SKRotationMatrix r_6_001;
  static SKRotationMatrix r_6i_001;

  static SKRotationMatrix r_3_111;
  static SKRotationMatrix r_3i_111;

  static SKRotationMatrix r_2prime_100;
  static SKRotationMatrix r_2iprime_100;
  static SKRotationMatrix r_2doubleprime_100;
  static SKRotationMatrix r_2idoubleprime_100;

  static SKRotationMatrix r_2prime_010;
  static SKRotationMatrix r_2iprime_010;
  static SKRotationMatrix r_2doubleprime_010;
  static SKRotationMatrix r_2idoubleprime_010;

  static SKRotationMatrix r_2prime_001;
  static SKRotationMatrix r_2iprime_001;
  static SKRotationMatrix r_2doubleprime_001;
  static SKRotationMatrix r_2idoubleprime_001;
};
