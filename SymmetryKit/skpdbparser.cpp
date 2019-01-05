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

#include "skpdbparser.h"
#include <QDebug>
#include "symmetrykitprotocols.h"
#include "skasymmetricatom.h"
#include "skelement.h"

#define _USE_MATH_DEFINES
#include <math.h>



void SKPDBParser::addFrameToStructure(size_t currentMovie, size_t currentFrame)
{
  if (std::get<0>(_frame)->rootNodes().size() > 0)
  {
    if (currentMovie >= _movies.size())
    {
      std::vector<std::tuple<std::shared_ptr<SKAtomTreeController>,std::shared_ptr<SKCell>, int>> movie = std::vector<std::tuple<std::shared_ptr<SKAtomTreeController>,std::shared_ptr<SKCell>,int>>();
      _movies.push_back(movie);
    }

    if (currentFrame >= _movies[currentMovie].size())
    {
      _movies[currentMovie].push_back(_frame);
      _frame = std::make_tuple(std::make_shared<SKAtomTreeController>(),std::make_shared<SKCell>(),1);
    }
  }
}

void SKPDBParser::startParsing()
{

  int lineNumber = 0;
  int modelNumber = 0;
  size_t currentMovie = 0;
  size_t currentFrame = 0;

  while(!_scanner.isAtEnd())
  {
    QString scannedLine;

    // scan to first keyword
    _previousScanLocation = _scanner.scanLocation();
    if (_scanner.scanUpToCharacters(CharacterSet::newlineCharacterSet(), scannedLine) && !scannedLine.isEmpty())
    {
      lineNumber += 1;

      int length = scannedLine.size();

      if(length < 3) continue;
      QStringRef shortKeyword(&scannedLine, 0, 3);

      if(shortKeyword == "END")
      {
        addFrameToStructure(currentMovie,currentFrame);
        currentFrame += 1;
        continue;
      }

      if(shortKeyword == "TER")
      {
        addFrameToStructure(currentMovie,currentFrame);
        currentMovie += 1;
        continue;
      }

      if(length < 6) continue;
      QStringRef keyword(&scannedLine, 0, 6);


      if(keyword == "HEADER")
      {
        continue;
      }

      if(keyword == "AUTHOR")
      {
        continue;
      }

      if(keyword == "REVDAT")
      {
        continue;
      }

      if(keyword == "JRNL  ")
      {
        continue;
      }

      if(keyword == "REMARK")
      {
        continue;
      }

      if(keyword == "MODEL")
      {
        currentMovie = 0;

        if(length <= 10) continue;
        QStringRef modelString(&scannedLine, 6, length - 6);

        bool success = false;
        int integerValue = modelString.toInt(&success);
        if(success)
        {
          _frame = std::make_tuple(std::make_shared<SKAtomTreeController>(),std::make_shared<SKCell>(), 1);
          currentFrame = std::max(0, integerValue-1);
          currentFrame = modelNumber;
          modelNumber += 1;
        }
        continue;
      }

      if(keyword == "SCALE1")
      {
        continue;
      }

      if(keyword == "SCALE2")
      {
        continue;
      }

      if(keyword == "SCALE3")
      {
        continue;
      }

      if(keyword == "CRYST1")
      {
        QStringRef lengthAString(&scannedLine, 6, 9);
        QStringRef lengthBString(&scannedLine, 15, 9);
        QStringRef lengthCString(&scannedLine, 24, 9);
        bool succes = false;
        _a = lengthAString.toDouble(&succes);
        _b = lengthBString.toDouble(&succes);
        _c = lengthCString.toDouble(&succes);

        _alpha = 90.0;
        _beta = 90.0;
        _gamma = 90.0;
        if(scannedLine.size()>=54)
        {
          QStringRef alphaAngleString(&scannedLine, 33, 7);
          QStringRef betaAngleString(&scannedLine, 40, 7);
          QStringRef gammaAngleString(&scannedLine, 47, 7);
          _alpha = alphaAngleString.toDouble(&succes);
          _beta = betaAngleString.toDouble(&succes);
          _gamma = gammaAngleString.toDouble(&succes);
        }
        std::get<1>(_frame) = std::make_shared<SKCell>(_a, _b, _c, _alpha * M_PI/180.0, _beta*M_PI/180.0, _gamma*M_PI/180.0);

        if(scannedLine.size()>=66)
        {
          QStringRef spaceGroupString(&scannedLine, 55, 11);

          if(stdx::optional<int> spaceGroupHallNumber = SKSpaceGroup::HallNumberFromHMString(spaceGroupString.toString()))
          {
            std::get<2>(_frame) = *spaceGroupHallNumber;
          }
        }
        continue;
      }

      if(keyword == "ORIGX1")
      {
        continue;
      }

      if(keyword == "ORIGX2")
      {
        continue;
      }

      if(keyword == "ORIGX3")
      {
        continue;
      }


      if(keyword == "ATOM  " || keyword == "HETATM")
      {
        std::shared_ptr<SKAsymmetricAtom> atom = std::make_shared<SKAsymmetricAtom>();

        QStringRef positionsX(&scannedLine, 30, 8);
        QStringRef positionsY(&scannedLine, 38, 8);
        QStringRef positionsZ(&scannedLine, 46, 8);
        QStringRef chemicalElementString(&scannedLine, 76, 2);

        double occupancy = 1.0;
        bool succes = false;
        QStringRef occupancyString(&scannedLine, 54, 6);

        occupancy = occupancyString.toDouble(&succes);
        if(succes)
        {
          atom->setOccupancy(occupancy);
        }

        double3 position;
        succes = false;
        position.x = positionsX.toDouble(&succes);
        position.y = positionsY.toDouble(&succes);
        position.z = positionsZ.toDouble(&succes);
        atom->setPosition(std::get<1>(_frame)->convertToFractional(position));

        QString chemicalElement = chemicalElementString.toString().simplified().toLower();
        chemicalElement.replace(0, 1, chemicalElement[0].toUpper());
        if (std::map<QString,int>::iterator index = PredefinedElements::atomicNumberData.find(chemicalElement); index != PredefinedElements::atomicNumberData.end())
        {
           atom->setElementIdentifier(index->second);
           atom->setDisplayName(chemicalElement);
        }

        std::shared_ptr<SKAtomTreeNode> atomTreeNode = std::make_shared<SKAtomTreeNode>(atom);
        std::get<0>(_frame)->appendToRootnodes(atomTreeNode);
        continue;
      }

    }
  }

  // add current frame in case last TER, ENDMDL, or END is missing
  addFrameToStructure(currentMovie,currentFrame);
}
