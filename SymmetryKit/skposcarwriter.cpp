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

#include <cmath>
#include "skelement.h"
#include "skposcarwriter.h"

SKPOSCARWriter::SKPOSCARWriter(QString displayName, SKSpaceGroup &spaceGroup, std::shared_ptr<SKCell> cell, double3 origin, std::vector<std::shared_ptr<SKAsymmetricAtom>> atoms):
   _displayName(displayName), _spaceGroup(spaceGroup), _cell(cell), _origin(origin), _atoms(atoms)
{

}

QString SKPOSCARWriter::string()
{
  QString outputString  = "# " + _displayName + "\n";
  outputString += "1.00000000000000\n";

  double3x3 unitCell = _cell->unitCell();
  QString cellAX = QString("%1 ").arg(unitCell.ax, 12, 'f', 8, ' ');
  QString cellAY = QString("%1 ").arg(unitCell.ay, 12, 'f', 8, ' ');
  QString cellAZ = QString("%1\n").arg(unitCell.az, 12, 'f', 8, ' ');
  outputString += cellAX + cellAY + cellAZ;

  QString cellBX = QString("%1 ").arg(unitCell.bx, 12, 'f', 8, ' ');
  QString cellBY = QString("%1 ").arg(unitCell.by, 12, 'f', 8, ' ');
  QString cellBZ = QString("%1\n").arg(unitCell.bz, 12, 'f', 8, ' ');
  outputString += cellBX + cellBY + cellBZ;

  QString cellCX = QString("%1 ").arg(unitCell.cx, 12, 'f', 8, ' ');
  QString cellCY = QString("%1 ").arg(unitCell.cy, 12, 'f', 8, ' ');
  QString cellCZ = QString("%1\n").arg(unitCell.cz, 12, 'f', 8, ' ');
  outputString += cellCX + cellCY + cellCZ;

  std::vector<std::shared_ptr<SKAsymmetricAtom>> sortedAtoms{_atoms};
  std::sort(sortedAtoms.begin(), sortedAtoms.end(),
            [](const std::shared_ptr<SKAsymmetricAtom> & a, const std::shared_ptr<SKAsymmetricAtom> & b) -> bool
            {
                return a->elementIdentifier() < b->elementIdentifier();
            });

  std::map<int,int> histogram;

  for (const auto& e : sortedAtoms)
    histogram[e->elementIdentifier()] += 1;

  QString elementString{};
  QString countString{};
  for (const auto& x : histogram)
  {
    SKElement element = PredefinedElements::predefinedElements[x.first];
    elementString += " " + element._chemicalSymbol.leftJustified(4, ' ');
    countString += " " + QString::number(x.second).leftJustified(4, ' ');
  }
  outputString += elementString + "\n";
  outputString += countString + "\n";

  outputString += QString("Direct\n");
  for(std::shared_ptr<SKAsymmetricAtom> atom : sortedAtoms)
  {
    QString positionX = QString("%1").arg(atom->position().x - _origin.x, 12, 'f', 8, ' ');
    QString positionY = QString("%1").arg(atom->position().y - _origin.y, 12, 'f', 8, ' ');
    QString positionZ = QString("%1").arg(atom->position().z - _origin.z, 12, 'f', 8, ' ');
    QString charge = QString("%1").arg(atom->charge(), 12, 'f', 8, ' ');

    QString line = positionX + " " + positionY + " " + positionZ + "\n";
    outputString +=  line;
  }

  return outputString;
}
