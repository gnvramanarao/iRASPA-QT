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
#include "skxyzwriter.h"

SKXYZWriter::SKXYZWriter(QString displayName, SKSpaceGroup &spacegroup, std::shared_ptr<SKCell> cell, double3 origin, std::vector<std::shared_ptr<SKAsymmetricAtom>> atoms):
   _displayName(displayName), _spacegroup(spacegroup), _cell(cell), _origin(origin), _atoms(atoms)
{
}

QString SKXYZWriter::string()
{
  QString outputString = QString::number(_atoms.size()) + "\n";

  QString commentString = "# " + _displayName + "\n";
  if(_cell)
  {
    double3x3 unitCell = _cell->unitCell();
    commentString = QString("Lattice=\"%1 %2 %3 %4 %5 %6 %7 %8 %9\"\n").
        arg(unitCell.ax).arg(unitCell.ay).arg(unitCell.az).
        arg(unitCell.bx).arg(unitCell.by).arg(unitCell.bz).
        arg(unitCell.cx).arg(unitCell.cy).arg(unitCell.cz);
  }
  outputString += commentString;

  for(std::shared_ptr<SKAsymmetricAtom> atom : _atoms)
  {
    int atomicNumber = atom->elementIdentifier();
    SKElement element = PredefinedElements::predefinedElements[atomicNumber];
    QString elementSymbol =  element._chemicalSymbol.rightJustified(4, ' ', true);

    QString orthogonalCoordinatesForX = QString("%1 ").arg(atom->position().x - _origin.x, 12, 'f', 8, ' ');
    QString orthogonalCoordinatesForY = QString("%1 ").arg(atom->position().y - _origin.y, 12, 'f', 8, ' ');
    QString orthogonalCoordinatesForZ = QString("%1\n").arg(atom->position().z - _origin.z, 12, 'f', 8, ' ');

    QString line = elementSymbol + orthogonalCoordinatesForX + orthogonalCoordinatesForY + orthogonalCoordinatesForZ;

    outputString += line;
  }

  return outputString;
}
