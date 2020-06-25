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

#include "skspacegroupsetting.h"
#include <iostream>
#include <cassert>


SKSpaceGroupSetting::SKSpaceGroupSetting(qint64 number, qint64 spaceGroupNumber, qint64 order, char ext, QString qualifier, QString HM, QString oldHMString, QString Hall,
                                         bool inversionAtOrigin, int3 inversionCenter, Symmorphicity symmorphicity, bool standard, Centring centring,
                                         std::vector<int3> latticeTranslations, qint64 pointGroupNumber, std::string schoenflies, std::string generators,
                                         std::string encoding, int3x3 toPrimitiveTransformation)
{
  _HallNumber = number;
  _spaceGroupNumber = spaceGroupNumber;
  _order = order;
  _ext = ext;
  _qualifier = qualifier;
  _HMString = HM;
  _oldHMString = oldHMString;
  _HallString = Hall;
  _encodedGenerators = generators;
  _encodedSeitz =  encoding;
  _inversionAtOrigin = inversionAtOrigin;
  _inversionCenter = inversionCenter;
  _standard = standard;
  _symmorphicity = symmorphicity;
  _centring = centring;
  _latticeTranslations = latticeTranslations;
  _schoenflies = schoenflies;
  _pointGroupNumber = pointGroupNumber;
  _toPrimitiveTransformation = toPrimitiveTransformation;
}

QString SKSpaceGroupSetting::symmorphicityString() const
{
  switch(_symmorphicity)
  {
  case Symmorphicity::asymmorphic:
      return "asymmorphic";
  case Symmorphicity::symmorphic:
      return "symmorphic";
  case Symmorphicity::hemisymmorphic:
      return "hemisymmorphic";
  }
}

QString SKSpaceGroupSetting::centringString() const
{
  switch(_centring)
  {
  case Centring::none:
        return "none";
  case Centring::primitive:
      return "primitive";
  case Centring::body:
      return "body";
  case Centring::a_face:
      return "a";
  case Centring::b_face:
      return "b";
  case Centring::c_face:
      return "c";
  case Centring::face:
      return "face";
  case Centring::base:
      return "base";
  case Centring::r:
      return "r";
  case Centring::h:
      return "h";
  case Centring::d:
      return "d";
  }
}


std::ostream& operator<<(std::ostream& os, const SKSpaceGroupSetting& setting)
{
    os << "SpaceGroupSetting: " << setting._HallNumber << '/' << setting._spaceGroupNumber << '/' << setting._encodedSeitz;
    return os;
}

SKSymmetryOperationSet SKSpaceGroupSetting::fullSeitzMatrices()
{
  assert(_encodedSeitz.size() % 3 == 0);
  assert(_encodedSeitz.size()>0);

  bool centrosymmetric = SKPointGroup::pointGroupData[_pointGroupNumber].centrosymmetric();
  size_t m = _encodedSeitz.size()/3;

  size_t size = centrosymmetric ? 2 * m : m;
  std::vector<int3> translationVectors = _latticeTranslations;
  std::vector<SKSeitzMatrix> matrices = std::vector<SKSeitzMatrix>();
  matrices.resize(size * translationVectors.size());

  for(size_t i=0;i<m;i++)
  {
    char x = _encodedSeitz[3 * i];
    char y = _encodedSeitz[3 * i + 1];
    char z = _encodedSeitz[3 * i + 2];

    matrices[i] = SKSeitzMatrix(x,y,z);
  }

  if (centrosymmetric)
  {
    for(size_t i=0;i<m;i++)
    {
      char x = _encodedSeitz[3 * i];
      char y = _encodedSeitz[3 * i + 1];
      char z = _encodedSeitz[3 * i + 2];

      SKSeitzMatrix seitz = SKSeitzMatrix(x,y,z);

      int3 translation = seitz.translation() + seitz.rotation() * _inversionCenter;
      matrices[m+i] = SKSeitzMatrix(-seitz.rotation(), translation);
    }
  }

  // use the translation vectors on all Seitz matrices
  for(size_t k=1;k<translationVectors.size();k++)
  {
    for(size_t i=0;i<size;i++)
    {
      matrices[size * k + i] = matrices[i];
      matrices[size * k + i].setTranslation(matrices[size * k + i].translation() + translationVectors[k]);
    }
  }

  return SKSymmetryOperationSet(matrices);
}

