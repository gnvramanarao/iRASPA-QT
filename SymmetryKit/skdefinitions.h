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

#include <iostream>

enum class Symmorphicity: qint64
{
  asymmorphic = 0, symmorphic = 1, hemisymmorphic = 2
};

enum class Centring: qint64
{
  none = 0, primitive = 1, body = 2, a_face = 3, b_face = 4, c_face = 5, face = 6, base = 7, r = 8, h = 9, d = 10
};

enum class RotationType: qint64
{
  axis_6m = -6, axis_4m = -4, axis_3m = -3, axis_2m = -2, axis_1m = -1, none = 0, axis_1 = 1, axis_2 = 2, axis_3 = 3, axis_4 = 4, axis_6 = 6
};

enum class SymmetryType: qint64
{
  unknown = 0, identity = 1, translation = 2, inversion = 3, pure_rotation = 4, pure_reflection = 5, screw_rotation = 6, glide_reflection = 7
};

enum class Holohedry: qint64
{
  none = 0, triclinic = 1, monoclinic = 2, orthorhombic = 3, tetragonal = 4, trigonal = 5, hexagonal = 6, cubic = 7
};

enum class Laue: qint64
{
  none = 0, laue_1 = 1, laue_2m = 2, laue_mmm = 3, laue_4m = 4, laue_4mmm = 5, laue_3 = 6, laue_3m = 7, laue_6m = 8, laue_6mmm = 9, laue_m3 = 10, laue_m3m = 11
};
