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
#include <string>
#include <vector>
#include <mathkit.h>
#include "sksymmetryoperationset.h"
#include "skdefinitions.h"
#include "skpointgroup.h"


class SKSpaceGroupSetting
{
public:
  SKSpaceGroupSetting(qint64 number, qint64 spaceGroupNumber, qint64 order, char ext, QString qualifier, QString HM, QString oldHM, QString Hall,
                      bool inversionAtOrigin, int3 inversionCenter, Symmorphicity symmorphicity, bool standard, Centring centring,
                      std::vector<int3> latticeTranslations, qint64 pointGroupNumber, std::string schoenflies, std::string generators,
                      std::string encoding, int3x3 toPrimitiveTransformation);
  SKSymmetryOperationSet fullSeitzMatrices();

  qint64 number() const {return _spaceGroupNumber;}
  qint64 HallNumber() const {return _HallNumber;}
  QString HallString() const {return _HallString;}
  QString HMString() const {return _HMString;}
  qint64 pointGroup() const {return _pointGroupNumber;}
  QString qualifier() const {return _qualifier;}
  Symmorphicity symmorphicity() const {return _symmorphicity;}
  QString symmorphicityString() const;
  QString centringString() const;

  bool inversionAtOrigin() const {return _inversionAtOrigin;}
  int3 inversionCenter() const {return _inversionCenter;}

  std::vector<int3> latticeTranslations() {return _latticeTranslations;}

  friend std::ostream& operator<<(std::ostream& os, const SKSpaceGroupSetting& setting);
private:
  qint64 _HallNumber = 1;
  qint64 _spaceGroupNumber = 1;      // space group number (1-230)
  qint64 _order;
  char _ext;                         // '1', '2', 'H', 'R' or '\0'
  QString _qualifier;                // e.g. "-cba" or "b1"
  QString _HMString;                 // H-M symbol; nul-terminated string
  QString _oldHMString;              // H-M symbol; nul-terminated string
  QString _HallString;               // Hall symbol; nul-terminated string
  std::string _encodedGenerators;    // encoded seitz matrix-generators
  std::string _encodedSeitz;         // encoded seitz matrix
  bool _inversionAtOrigin;
  int3 _inversionCenter;
  bool _standard = false;
  Symmorphicity _symmorphicity = Symmorphicity::asymmorphic;
  Centring _centring = Centring::primitive;
  std::vector<int3> _latticeTranslations;
  std::string _schoenflies;
  qint64 _pointGroupNumber;
  int3x3 _toPrimitiveTransformation;
};
