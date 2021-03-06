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

#include "skpointgroup.h"

SKPointGroup::SKPointGroup(SKRotationalOccuranceTable table, qint64 number, QString symbol, QString schoenflies, Holohedry holohedry, Laue laue, bool centrosymmetric, bool enantiomorphic):
    _table(table), _number(number), _symbol(symbol), _schoenflies(schoenflies), _holohedry(holohedry), _laue(laue), _centrosymmetric(centrosymmetric), _enantiomorphic(enantiomorphic)
{
}

QString SKPointGroup::holohedryString() const
{
  switch(_holohedry)
  {
  case Holohedry::none:
      return "None";
  case Holohedry::triclinic:
      return "Triclinic";
  case Holohedry::monoclinic:
      return "Monoclinic";
  case Holohedry::orthorhombic:
      return "Orthorhombic";
  case Holohedry::tetragonal:
      return "Tetragonal";
  case Holohedry::trigonal:
      return "Trigonal";
  case Holohedry::hexagonal:
      return "Hexagonal";
  case Holohedry::cubic:
      return "Cubic";
  }
}

QString SKPointGroup::LaueString() const
{
  switch(_laue)
  {
  case Laue::none:
      return "none";
  case Laue::laue_1:
      return "1";
  case Laue::laue_2m:
      return "2m";
  case Laue::laue_mmm:
      return "mmm";
  case Laue::laue_4m:
      return "4m";
  case Laue::laue_4mmm:
      return "4mmm";
  case Laue::laue_3:
      return "3";
  case Laue::laue_3m:
      return "3m";
  case Laue::laue_6m:
      return "6m";
  case Laue::laue_6mmm:
      return "6mmm";
  case Laue::laue_m3:
      return "m3";
  case Laue::laue_m3m:
      return "m3m";
  }
}

QDataStream &operator<<(QDataStream &stream, const SKPointGroup &pointGroup)
{
  stream << pointGroup._number;
  return stream;
}

QDataStream &operator>>(QDataStream &stream, SKPointGroup &pointGroup)
{
  qint64 pointGroupNumber;
  stream >> pointGroupNumber;
  pointGroup = SKPointGroup::pointGroupData[pointGroupNumber];
  return stream;
}


std::vector<SKPointGroup> SKPointGroup::pointGroupData = std::vector<SKPointGroup>{
  SKPointGroup(SKRotationalOccuranceTable(0, 0, 0, 0, 0, 0, 0, 0, 0, 0), 0, "", "", Holohedry::none, Laue::none, false, false),
  SKPointGroup(SKRotationalOccuranceTable(0, 0, 0, 0, 0, 1, 0, 0, 0, 0), 1, "1",      "C1",  Holohedry::triclinic,    Laue::laue_1,     false, true),
  SKPointGroup(SKRotationalOccuranceTable(0, 0, 0, 0, 1, 1, 0, 0, 0, 0), 2, "-1",     "Ci",  Holohedry::triclinic,    Laue::laue_1,     true,  false),
  SKPointGroup(SKRotationalOccuranceTable(0, 0, 0, 0, 0, 1, 1, 0, 0, 0), 3, "2",      "C2",  Holohedry::monoclinic,   Laue::laue_2m,    false, true),
  SKPointGroup(SKRotationalOccuranceTable(0, 0, 0, 1, 0, 1, 0, 0, 0, 0), 4, "m",      "Cs",  Holohedry::monoclinic,   Laue::laue_2m,    false, false),
  SKPointGroup(SKRotationalOccuranceTable(0, 0, 0, 1, 1, 1, 1, 0, 0, 0), 5, "2/m",    "C2h", Holohedry::monoclinic,   Laue::laue_2m,    true,  false),
  SKPointGroup(SKRotationalOccuranceTable(0, 0, 0, 0, 0, 1, 3, 0, 0, 0), 6, "222",    "D2",  Holohedry::orthorhombic, Laue::laue_mmm,   false, true),
  SKPointGroup(SKRotationalOccuranceTable(0, 0, 0, 2, 0, 1, 1, 0, 0, 0), 7, "mm2",    "C2v", Holohedry::orthorhombic, Laue::laue_mmm,   false, false),
  SKPointGroup(SKRotationalOccuranceTable(0, 0, 0, 3, 1, 1, 3, 0, 0, 0), 8, "mmm",    "D2h", Holohedry::orthorhombic, Laue::laue_mmm,   true,  false),
  SKPointGroup(SKRotationalOccuranceTable(0, 0, 0, 0, 0, 1, 1, 0, 2, 0), 9, "4",      "C4",  Holohedry::tetragonal,   Laue::laue_4m,    false, true),
  SKPointGroup(SKRotationalOccuranceTable(0, 2, 0, 0, 0, 1, 1, 0, 0, 0), 10, "-4",    "S4",  Holohedry::tetragonal,   Laue::laue_4m,    false, false),
  SKPointGroup(SKRotationalOccuranceTable(0, 2, 0, 1, 1, 1, 1, 0, 2, 0), 11, "4/m",   "C4h", Holohedry::tetragonal,   Laue::laue_4m,    true,  false),
  SKPointGroup(SKRotationalOccuranceTable(0, 0, 0, 0, 0, 1, 5, 0, 2, 0), 12, "422",   "D4",  Holohedry::tetragonal,   Laue::laue_4mmm,  false, true),
  SKPointGroup(SKRotationalOccuranceTable(0, 0, 0, 4, 0, 1, 1, 0, 2, 0), 13, "4mm",   "C4v", Holohedry::tetragonal,   Laue::laue_4mmm,  false, false),
  SKPointGroup(SKRotationalOccuranceTable(0, 2, 0, 2, 0, 1, 3, 0, 0, 0), 14, "-42m",  "D2d", Holohedry::tetragonal,   Laue::laue_4mmm,  false, false),
  SKPointGroup(SKRotationalOccuranceTable(0, 2, 0, 5, 1, 1, 5, 0, 2, 0), 15, "4/mmm", "D4h", Holohedry::tetragonal,   Laue::laue_4mmm,  true,  false),
  SKPointGroup(SKRotationalOccuranceTable(0, 0, 0, 0, 0, 1, 0, 2, 0, 0), 16, "3",     "C3",  Holohedry::trigonal,     Laue::laue_3,     false, true),
  SKPointGroup(SKRotationalOccuranceTable(0, 0, 2, 0, 1, 1, 0, 2, 0, 0), 17, "-3",    "C3i", Holohedry::trigonal,     Laue::laue_3,     true,  false),
  SKPointGroup(SKRotationalOccuranceTable(0, 0, 0, 0, 0, 1, 3, 2, 0, 0), 18, "32",    "D3",  Holohedry::trigonal,     Laue::laue_3m,    false, true),
  SKPointGroup(SKRotationalOccuranceTable(0, 0, 0, 3, 0, 1, 0, 2, 0, 0), 19, "3m",    "C3v", Holohedry::trigonal,     Laue::laue_3m,    false, false),
  SKPointGroup(SKRotationalOccuranceTable(0, 0, 2, 3, 1, 1, 3, 2, 0, 0), 20, "-3m",   "D3d", Holohedry::trigonal,     Laue::laue_3m,    true,  false),
  SKPointGroup(SKRotationalOccuranceTable(0, 0, 0, 0, 0, 1, 1, 2, 0, 2), 21, "6",     "C6",  Holohedry::hexagonal,    Laue::laue_6m,    false, true),
  SKPointGroup(SKRotationalOccuranceTable(2, 0, 0, 1, 0, 1, 0, 2, 0, 0), 22, "-6",    "C3h", Holohedry::hexagonal,    Laue::laue_6m,    false, false),
  SKPointGroup(SKRotationalOccuranceTable(2, 0, 2, 1, 1, 1, 1, 2, 0, 2), 23, "6/m",   "C6h", Holohedry::hexagonal,    Laue::laue_6m,    true,  false),
  SKPointGroup(SKRotationalOccuranceTable(0, 0, 0, 0, 0, 1, 7, 2, 0, 2), 24, "622",   "D6",  Holohedry::hexagonal,    Laue::laue_6mmm,  false, true),
  SKPointGroup(SKRotationalOccuranceTable(0, 0, 0, 6, 0, 1, 1, 2, 0, 2), 25, "6mm",   "C6v", Holohedry::hexagonal,    Laue::laue_6mmm,  false, false),
  SKPointGroup(SKRotationalOccuranceTable(2, 0, 0, 4, 0, 1, 3, 2, 0, 0), 26, "-6m",   "D3h", Holohedry::hexagonal,    Laue::laue_6mmm,  false, false),
  SKPointGroup(SKRotationalOccuranceTable(2, 0, 2, 7, 1, 1, 7, 2, 0, 2), 27, "6/mmm", "D6h", Holohedry::hexagonal,    Laue::laue_6mmm,  true,  false),
  SKPointGroup(SKRotationalOccuranceTable(0, 0, 0, 0, 0, 1, 3, 8, 0, 0), 28, "23",    "T",   Holohedry::cubic,        Laue::laue_m3,    false, true),
  SKPointGroup(SKRotationalOccuranceTable(0, 0, 8, 3, 1, 1, 3, 8, 0, 0), 29, "m-3",   "Th",  Holohedry::cubic,        Laue::laue_m3,    true,  false),
  SKPointGroup(SKRotationalOccuranceTable(0, 0, 0, 0, 0, 1, 9, 8, 6, 0), 30, "432",   "O",   Holohedry::cubic,        Laue::laue_m3m,   false, true),
  SKPointGroup(SKRotationalOccuranceTable(0, 6, 0, 6, 0, 1, 3, 8, 0, 0), 31, "-43m",  "Td",  Holohedry::cubic,        Laue::laue_m3m,   false, false),
  SKPointGroup(SKRotationalOccuranceTable(0, 6, 8, 9, 1, 1, 9, 8, 6, 0), 32, "m-3m",  "Oh",  Holohedry::cubic,        Laue::laue_m3m,   true,  false)
};
