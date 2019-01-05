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

#include <string>
#include <vector>
#include <mathkit.h>
#include "skrotationmatrix.h"
#include "skonethirdseitzmatrix.h"
#include "skdefinitions.h"


class SKSeitzMatrix
{
public:
  SKSeitzMatrix();
  SKSeitzMatrix(SKRotationMatrix rotation, int3 translation);
  SKSeitzMatrix(char xvalue, char yvalue, char zvalue);
  static std::vector<SKSeitzMatrix> SeitzMatrices(std::string encoding);

  SKRotationMatrix rotation() const {return _rotation;}
  int3 translation() const {return _translation;}
  void setTranslation(int3 t) {_translation = t;}
  int3 normalizedTranslation() const {return int3(_translation.x%12, _translation.y%12, _translation.z%12);}

  double3 operator * (const double3& right) const;
  SKSeitzMatrix operator * (const SKSeitzMatrix &right) const;
  bool operator==(SKSeitzMatrix const& rhs) const;
private:
  SKRotationMatrix _rotation;
  int3 _translation;   // denominator = 12

  static std::vector<SKOneThirdSeitzMatrix> _SeitzData;
};

namespace std
{
  template <> struct hash<SKSeitzMatrix>
  {
    size_t operator()(const SKSeitzMatrix& k) const
    {
      // Compute individual hash values for two data members and combine them using XOR and bit shifting
      int3 normalizedTranslation = k.normalizedTranslation();
      int r1 = (k.rotation().m11+1);
      int r2 = (3 * (k.rotation().m12+1));
      int r3 = ((3*3) * (k.rotation().m13+1));
      int r4 = ((3*3*3) * (k.rotation().m12+1));
      int r5 = ((3*3*3*3) * (k.rotation().m22+1));
      int r6 = ((3*3*3*3*3) * (k.rotation().m32+1));
      int r7 = ((3*3*3*3*3*3) * (k.rotation().m31+1));
      int r8 = ((3*3*3*3*3*3*3) * (k.rotation().m23+1));
      int r9 = ((3*3*3*3*3*3*3*3) * (k.rotation().m33+1));
      int v1 = ((3*3*3*3*3*3*3*3*12) * normalizedTranslation.x);
      int v2 = ((3*3*3*3*3*3*3*3*12*12) * normalizedTranslation.y);
      int v3 = ((3*3*3*3*3*3*3*3*12*12*12) * normalizedTranslation.z);
      return r1 + r2 + r3 + r4 + r5 + r6 + r7 + r8 + r9 + v1 + v2 + v3;
    }
  };
}
