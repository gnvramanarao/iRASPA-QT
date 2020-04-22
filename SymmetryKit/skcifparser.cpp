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

#include <QDebug>
#include <QRegExp>
#include "skcifparser.h"
#include "symmetrykitprotocols.h"
#include "skasymmetricatom.h"
#include "skelement.h"
#include "skspacegroup.h"

#define _USE_MATH_DEFINES
#include "math.h"

//std::map<QString, double> SKCIFParser::SKCIFParser::scene()
//{
//  return std::map<QString, double>();
//}

void SKCIFParser::startParsing()
{
  while(!_scanner.isAtEnd())
  {
    QString tempString;

    // scan to first keyword
    _previousScanLocation = _scanner.scanLocation();
    if (_scanner.scanUpToCharacters(CharacterSet::whitespaceAndNewlineCharacterSet(), tempString))
    {
      QString keyword = tempString.toLower();

      if (keyword.startsWith(QString("_audit")))
      {
        parseAudit(keyword);
      }
      else if(keyword.startsWith(QString("_iraspa")))
      {

        parseiRASPA(keyword);
      }
      else if(keyword.startsWith(QString("_chemical")))
      {
        parseChemical(keyword);
      }
      else if(keyword.startsWith(QString("_cell")))
      {
        parseCell(keyword);
      }
      else if(keyword.startsWith(QString("_symmetry")))
      {
        parseSymmetry(keyword);
      }
      else if(keyword.startsWith(QString("_symmetry_space_group")))
      {
        parseSymmetry(keyword);
      }
      else if(keyword.startsWith(QString("data_")))
      {
        parseName(keyword);
      }
      else if(keyword.startsWith(QString("loop_")))
      {
        parseLoop(keyword);
      }
      else if(keyword.startsWith(QString("#")))
      {
        skipComment();
      };
    }
  }

  std::vector<std::tuple<std::shared_ptr<SKAtomTreeController>,std::shared_ptr<SKCell>, int>> movieFrames{};
  std::shared_ptr<SKCell> _cell = std::make_shared<SKCell>(_a, _b, _c, _alpha * M_PI/180.0, _beta*M_PI/180.0, _gamma*M_PI/180.0);
  movieFrames.push_back(std::make_tuple(_atomTreeController,_cell, _spaceGroupHallNumber ? *_spaceGroupHallNumber : 1));
  _movies.push_back(movieFrames);
}

void SKCIFParser::parseAudit(QString& string)
{
  Q_UNUSED(string);
}

void SKCIFParser::parseiRASPA(QString& string)
{
  Q_UNUSED(string);
}

void SKCIFParser::parseChemical(QString& string)
{
  Q_UNUSED(string);
}

void SKCIFParser::parseCell(QString& string)
{
  if (string == QString("_cell_length_a") || string == QString("_cell.length_a"))
  {
    _a = scanDouble();
  }
  if (string == QString("_cell_length_b") || string == QString("_cell.length_b"))
  {
    _b = scanDouble();
  }
  if (string == QString("_cell_length_c") || string == QString("_cell.length_c"))
  {
    _c = scanDouble();
  }

  if (string == QString("_cell_angle_alpha") || string == QString("_cell.angle_alpha"))
  {
    _alpha = scanDouble();
  }
  if (string == QString("_cell_angle_beta") || string == QString("_cell.angle_beta"))
  {
    _beta = scanDouble();
  }
  if (string == QString("_cell_angle_gamma") || string == QString("_cell.angle_gamma"))
  {
    _gamma = scanDouble();
  }

}

void SKCIFParser::parseSymmetry(QString& string)
{
  if(string == QString("_symmetry_cell_settings").toLower())
  {
    return;
  }

  // prefer setting spacegroup based on Hall-symbol
  if((string == QString("_symmetry_space_group_name_Hall").toLower()) ||
     (string == QString("_symmetry.space_group_name_Hall").toLower()))
  {
    std::optional<QString> string = scanString();

    if(string)
    {
      _spaceGroupHallNumber = SKSpaceGroup::HallNumber(*string);
    }
  }

  if(!_spaceGroupHallNumber)
  {
    if((string == QString("_space_group_name_H-M_alt").toLower()) ||
       (string == QString("_symmetry_space_group_name_H-M").toLower()) ||
       (string == QString("_symmetry.pdbx_full_space_group_name_H-M").toLower()))
    {
      std::optional<QString> string = scanString();
      if(string)
      {
        _spaceGroupHallNumber = SKSpaceGroup::HallNumberFromHMString(*string);
      }
    }
  }

  if(!_spaceGroupHallNumber)
  {
    if((string == QString("_space_group_IT_number").toLower()) ||
       (string == QString("_symmetry_Int_Tables_number").toLower()) ||
       (string == QString("_symmetry.Int_Tables_number").toLower()))
    {
      int spaceGroupNumber = scanInt();
      //_spaceGroupHallNumber = SKSpaceGroup::HallNumberFromSpaceGrouprNumber(spaceGroupNumber);
    }
  }
}

void SKCIFParser::parseName(QString& string)
{
  Q_UNUSED(string);
}

std::optional<QString> SKCIFParser::parseValue()
{
  if (_scanner.isAtEnd())
  {
    return std::nullopt;
  }

  QString::const_iterator previousScanLocation = _scanner.scanLocation();

  QString tempString;
  while (_scanner.scanUpToCharacters(CharacterSet::whitespaceAndNewlineCharacterSet(),tempString) && tempString.startsWith(QString("#")))
  {
    skipComment();
  };

  QString keyword = tempString.toLower();

  if ((keyword.startsWith( QString("_")) || keyword.startsWith( QString("loop_"))))
  {
    _scanner.setScanLocation(previousScanLocation);

    return std::nullopt;
  }
  else
  {
     return keyword;
  }
}

void SKCIFParser::parseLoop(QString& string)
{
  QString tempString;
  QString::const_iterator previousScanLocation;
  std::vector<QString> tags;


  // part 1: read the 'tags'
  previousScanLocation = _scanner.scanLocation();
  while(_scanner.scanUpToCharacters(CharacterSet::whitespaceAndNewlineCharacterSet(), tempString) && (tempString.size() > 0)  && (tempString.startsWith(QString("_")) || (tempString.startsWith(QString("#")))))
  {
    QString tag = tempString.toLower();

    if(tag.startsWith( QString("_")))
    {
       tags.push_back(tag);
       previousScanLocation=_scanner.scanLocation();
    }
  }

  // set scanner back to the first <value>
  _scanner.setScanLocation(previousScanLocation);

  std::optional<QString> value = std::nullopt;
  do
  {
    std::map<QString,QString> dictionary{};

    for(QString tag : tags)
    {
      if ((value = parseValue()))
      {
        dictionary[tag] = *value;
      }
    }

    if (value)
    {
      if (std::map<QString,QString>::iterator index = dictionary.find(QString("_atom_site_type_symbol")); (index != dictionary.end()))
      {
        // at least _atom_site_type_symbol is present
        QString chemicalElement = index->second;

        chemicalElement.remove(QRegExp("[0123456789+-]."));
        chemicalElement.replace(0, 1, chemicalElement[0].toUpper());

       std::shared_ptr<SKAsymmetricAtom> atom = std::make_shared<SKAsymmetricAtom>();

       if (std::map<QString,QString>::iterator index = dictionary.find(QString("_atom_site_label")); (index != dictionary.end()))
       {
         // First character to uppercase

         atom->setDisplayName(chemicalElement);
       }

       std::map<QString,QString>::iterator atom_site_x = dictionary.find(QString("_atom_site_fract_x"));
       std::map<QString,QString>::iterator atom_site_y = dictionary.find(QString("_atom_site_fract_y"));
       std::map<QString,QString>::iterator atom_site_z = dictionary.find(QString("_atom_site_fract_z"));
       if ((atom_site_x != dictionary.end()) && (atom_site_y != dictionary.end()) && (atom_site_z != dictionary.end()))
       {
         double3 position;
         bool succes = false;
         position.x = atom_site_x->second.split('(').at(0).toDouble(&succes);
         position.y = atom_site_y->second.split('(').at(0).toDouble(&succes);
         position.z = atom_site_z->second.split('(').at(0).toDouble(&succes);
         atom->setPosition(position);
       }

       if (std::map<QString,int>::iterator index = PredefinedElements::atomicNumberData.find(chemicalElement); index != PredefinedElements::atomicNumberData.end())
       {
          atom->setElementIdentifier(index->second);
       }


       std::shared_ptr<SKAtomTreeNode> atomTreeNode = std::make_shared<SKAtomTreeNode>(atom);
       _atomTreeController->appendToRootnodes(atomTreeNode);
      }
      else if (std::map<QString,QString>::iterator index = dictionary.find(QString("_atom_site.type_symbol")); (index != dictionary.end()))
      {
        std::shared_ptr<SKAsymmetricAtom> atom = std::make_shared<SKAsymmetricAtom>();

        if (std::map<QString,QString>::iterator index = dictionary.find(QString("_atom_site.id")); (index != dictionary.end()))
        {
          atom->setDisplayName(index->second);
        }

        std::map<QString,QString>::iterator atom_site_x = dictionary.find(QString("_atom_site_fract_x"));
        std::map<QString,QString>::iterator atom_site_y = dictionary.find(QString("_atom_site_fract_y"));
        std::map<QString,QString>::iterator atom_site_z = dictionary.find(QString("_atom_site_fract_z"));
        if ((atom_site_x != dictionary.end()) && (atom_site_y != dictionary.end()) && (dictionary.find(QString("atom_site_z")) != dictionary.end()))
        {
          double3 position;
          bool succes = false;
          position.x = atom_site_x->second.split('(').at(0).toDouble(&succes);
          position.y = atom_site_y->second.split('(').at(0).toDouble(&succes);
          position.z = atom_site_z->second.split('(').at(0).toDouble(&succes);
          atom->setPosition(position);
        }

        std::shared_ptr<SKAtomTreeNode> atomTreeNode = std::make_shared<SKAtomTreeNode>(atom);
        _atomTreeController->appendToRootnodes(atomTreeNode);
      }
    }
  }
  while (value);
}

void SKCIFParser::skipComment()
{
  QString tempString;
  _scanner.scanUpToCharacters(CharacterSet::newlineCharacterSet(),tempString);
}

qint64 SKCIFParser::scanInt()
{
  return 0;
}

double SKCIFParser::scanDouble()
{
  QString tempString;
  if (_scanner.scanUpToCharacters(CharacterSet::whitespaceAndNewlineCharacterSet(),tempString))
  {
    bool success = false;
    return tempString.split('(').at(0).toDouble(&success);
  }
  return 0.0;
}

std::optional<QString> SKCIFParser::scanString()
{
  QString tempString;
  if (_scanner.scanUpToCharacters(CharacterSet::newlineCharacterSet(),tempString))
  {
    return tempString;
  }

  return std::nullopt;
}

