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

#include "structure.h"
#include <iostream>
#include <unordered_set>
#include <symmetrykit.h>
#include <simulationkit.h>

Structure::Structure(): _atomsTreeController(std::make_shared<SKAtomTreeController>()),
                        _bondSetController(std::make_shared<SKBondSetController>(_atomsTreeController)), _spaceGroup(1), _cell(std::make_shared<SKCell>())
{

}

Structure::Structure(std::shared_ptr<SKAtomTreeController> atomTreeController): _atomsTreeController(atomTreeController),
                     _bondSetController(std::make_shared<SKBondSetController>(_atomsTreeController)), _spaceGroup(1), _cell(std::make_shared<SKCell>())
{

}

bool Structure::colorAtomsWithBondColor() const
{
  return _atomRepresentationType == RepresentationType::unity && _bondColorMode == RKBondColorMode::uniform;
}

void Structure::reComputeBoundingBox()
{
  SKBoundingBox b = boundingBox();
  _cell->setBoundingBox(b);
}

void Structure::setAtomScaleFactor(double size)
{
  _atomScaleFactor = size;
}

void Structure::setBondScaleFactor(double value)
{
  _bondScaleFactor = value;
  if(_atomRepresentationType == RepresentationType::unity)
  {
    std::vector<std::shared_ptr<SKAtomTreeNode>> asymmetricAtomNodes = _atomsTreeController->flattenedLeafNodes();

    for (std::shared_ptr<SKAtomTreeNode> node : asymmetricAtomNodes)
    {
      if (std::shared_ptr<SKAsymmetricAtom> atom = node->representedObject())
      {
        atom->setDrawRadius(value);
      }
    }
  }
}

double2 Structure::adsorptionSurfaceProbeParameters() const
{
  switch(_adsorptionSurfaceProbeMolecule)
  {
    case ProbeMolecule::helium:
      return double2(10.9, 2.64);
    case ProbeMolecule::nitrogen:
      return double2(36.0,3.31);
    case ProbeMolecule::methane:
      return double2(158.5,3.72);
    case ProbeMolecule::hydrogen:
      return double2(36.7,2.958);
    case ProbeMolecule::water:
      return double2(89.633,3.097);
    case ProbeMolecule::co2:
      // Y. Iwai, H. Higashi, H. Uchida, Y. Arai, Fluid Phase Equilibria 127 (1997) 251-261.
      return double2(236.1,3.72);
    case ProbeMolecule::xenon:
      // Gábor Rutkai, Monika Thol, Roland Span & Jadran Vrabec (2017), Molecular Physics, 115:9-12, 1104-1121
      return double2(226.14,3.949);
    case ProbeMolecule::krypton:
      // Gábor Rutkai, Monika Thol, Roland Span & Jadran Vrabec (2017), Molecular Physics, 115:9-12, 1104-1121
      return double2(162.58,3.6274);
    case ProbeMolecule::argon:
      return double2(119.8, 3.34);
    case ProbeMolecule::multiple_values:
      return double2();
  }
}

void Structure::setRepresentationStyle(RepresentationStyle style)
{
	std::cout << "SET STYLE TO: " << int(style) << std::endl;
	if (int(style) >= 0 && style < RepresentationStyle::multiple_values)
	{
		_atomRepresentationStyle = style;

		switch (style)
		{
		case RepresentationStyle::defaultStyle:
			_drawAtoms = true;
			_atomScaleFactor = 0.7;
			_atomHue = 1.0;
			_atomSaturation = 1.0;
			_atomValue = 1.0;
			_atomAmbientColor = QColor(255, 255, 255, 255);
			_atomSpecularColor = QColor(255, 255, 255, 255);
			_atomHDR = true;
			_atomHDRExposure = 1.5;
			_atomAmbientOcclusion = false;
			_bondAmbientOcclusion = false;
			_atomAmbientIntensity = 0.2;
			_atomDiffuseIntensity = 1.0;
			_atomSpecularIntensity = 1.0;
			_atomShininess = 6.0;

			_atomForceFieldIdentifier = QString("Default");
			_atomColorSchemeIdentifier = QString("Jmol");
			_atomColorSchemeOrder = ColorSchemeOrder::elementOnly;

			_drawBonds = true;
			_bondColorMode = RKBondColorMode::uniform;;
            _bondScaleFactor = 0.15;
			_bondAmbientColor = QColor(255, 255, 255, 255);
			_bondDiffuseColor = QColor(205, 205, 205, 255);
			_bondSpecularColor = QColor(255, 255, 255, 255);
			_bondAmbientIntensity = 0.35;
			_bondDiffuseIntensity = 1.0;
			_bondSpecularIntensity = 1.0;
			_bondShininess = 4.0;
			_bondHDR = true;
			_bondHDRExposure = 1.5;
			_bondHDRBloomLevel = 1.0;
			_bondHue = 1.0;
			_bondSaturation = 1.0;
			_bondValue = 1.0;
			_bondAmbientOcclusion = false;

			_atomSelectionStyle = RKSelectionStyle::WorleyNoise3D;
			_selectionScaling = 1.2;

			setRepresentationType(RepresentationType::sticks_and_balls);

			break;
		case RepresentationStyle::fancy:
			_drawAtoms = true;
			_atomScaleFactor = 1.0;
			_atomHue = 1.0;
			_atomSaturation = 0.5;
			_atomValue = 1.0;
			_atomAmbientColor = QColor(255, 255, 255, 255);
			_atomSpecularColor = QColor(255, 255, 255, 255);
			_atomHDR = false;
			_atomHDRExposure = 1.5;
			_atomAmbientOcclusion = true;
			_bondAmbientOcclusion = false;
			_atomAmbientIntensity = 1.0;
			_atomDiffuseIntensity = 0.0;
			_atomSpecularIntensity = 0.2;
			_atomShininess = 6.0;
			_atomScaleFactor = 1.0;

			_atomForceFieldIdentifier = QString("Default");
			_atomColorSchemeIdentifier = QString("Rasmol");
			_atomColorSchemeOrder = ColorSchemeOrder::elementOnly;

			_drawBonds = false;
			_bondAmbientOcclusion = false;

			_atomSelectionStyle = RKSelectionStyle::WorleyNoise3D;
			_selectionScaling = 1.0;

			setRepresentationType(RepresentationType::vdw);
			break;
		case RepresentationStyle::licorice:
			_drawAtoms = true;
			_atomScaleFactor = 1.0;
			_atomHue = 1.0;
			_atomSaturation = 1.0;
			_atomValue = 1.0;
			_atomAmbientColor = QColor(255, 255, 255, 255);
			_atomSpecularColor = QColor(255, 255, 255, 255);
			_atomHDR = true;
			_atomHDRExposure = 1.5;
			_atomAmbientOcclusion = false;
			_bondAmbientOcclusion = false;
			_atomAmbientIntensity = 0.2;
			_atomDiffuseIntensity = 1.0;
			_atomSpecularIntensity = 1.0;
			_atomShininess = 6.0;

			_atomForceFieldIdentifier = QString("Default");
			_atomColorSchemeIdentifier = QString("Jmol");
			_atomColorSchemeOrder = ColorSchemeOrder::elementOnly;

			_drawBonds = true;
			_bondColorMode = RKBondColorMode::split;;
            _bondScaleFactor = 0.25;
			_bondAmbientColor = QColor(255, 255, 255, 255);
			_bondDiffuseColor = QColor(205, 205, 205, 255);
			_bondSpecularColor = QColor(255, 255, 255, 255);
			_bondAmbientIntensity = 0.1;
			_bondDiffuseIntensity = 1.0;
			_bondSpecularIntensity = 1.0;
			_bondShininess = 4.0;
			_bondHDR = true;
			_bondHDRExposure = 1.5;
			_bondHDRBloomLevel = 1.0;
			_bondHue = 1.0;
			_bondSaturation = 1.0;
			_bondValue = 1.0;
			_bondAmbientOcclusion = false;

			_atomSelectionStyle = RKSelectionStyle::WorleyNoise3D;
			_selectionScaling = 1.5;

			setRepresentationType(RepresentationType::unity);
			break;
		case RepresentationStyle::multiple_values:
			break;
		default:
			break;
		}
	}

	std::vector<std::shared_ptr<SKAtomTreeNode>> asymmetricAtomNodes = _atomsTreeController->flattenedLeafNodes();

	for (std::shared_ptr<SKAtomTreeNode> node : asymmetricAtomNodes)
	{
		if (std::shared_ptr<SKAsymmetricAtom> atom = node->representedObject())
		{
			double radius = 0.0;
			switch (_atomRepresentationType)
			{
			case RepresentationType::sticks_and_balls:
				radius = PredefinedElements::predefinedElements[atom->elementIdentifier()]._covalentRadius;
				atom->setDrawRadius(radius);
				break;
			case RepresentationType::vdw:
				radius = PredefinedElements::predefinedElements[atom->elementIdentifier()]._VDWRadius;
				atom->setDrawRadius(radius);
				break;
			case RepresentationType::unity:
                atom->setDrawRadius(_bondScaleFactor);
				break;
			default:
				break;
			}
		}
	}
}

void Structure::setRepresentationStyle(RepresentationStyle style, const SKColorSets &colorSets)
{
	setRepresentationStyle(style);

	switch (_atomRepresentationStyle)
	{
	case RepresentationStyle::defaultStyle:
		setRepresentationColorSchemeIdentifier("Jmol", colorSets);
		break;
	case RepresentationStyle::fancy:
		setRepresentationColorSchemeIdentifier("Rasmol", colorSets);
		break;
	case RepresentationStyle::licorice:
		setRepresentationColorSchemeIdentifier("Jmol", colorSets);
		break;
	case RepresentationStyle::multiple_values:
		break;
	default:
		break;
	}
}

void Structure::recheckRepresentationStyle()
{
  if (_drawAtoms == true &&
     (fabs(_atomHue - 1.0) < 1e-4) &&
     (fabs(_atomSaturation - 1.0) < 1e-4) &&
     (fabs(_atomValue - 1.0) < 1e-4) &&
     (_atomAmbientColor.red() == 255) &&
     (_atomAmbientColor.green() == 255) &&
     (_atomAmbientColor.blue() == 255) &&
     (_atomAmbientColor.alpha() == 255) &&
     (_atomSpecularColor.red() == 255) &&
     (_atomSpecularColor.green() == 255) &&
     (_atomSpecularColor.blue() == 255) &&
     (_atomSpecularColor.alpha() == 255) &&
     (_atomHDR == true) &&
     (fabs(_atomHDRExposure - 1.5) < 1e-4) &&
     (_atomAmbientOcclusion == false) &&
     (fabs(_atomAmbientIntensity - 0.2) < 1e-4) &&
     (fabs(_atomDiffuseIntensity - 1.0) < 1e-4) &&
     (fabs(_atomSpecularIntensity - 1.0)) < 1e-4 &&
     (fabs(_atomShininess - 6.0) < 1e-4) &&
     (_atomRepresentationType == RepresentationType::sticks_and_balls) &&
     (QString::compare(_atomForceFieldIdentifier, "Default", Qt::CaseInsensitive) == 0) &&
     (QString::compare(_atomColorSchemeIdentifier, "Jmol", Qt::CaseInsensitive) == 0) &&
     (_atomColorSchemeOrder == ColorSchemeOrder::elementOnly) &&
     (_drawBonds == true) &&
     (_bondColorMode == RKBondColorMode::uniform) &&
     (fabs(_bondScaleFactor - 0.15) < 1e-4) &&
     (_bondAmbientOcclusion == false) &&
     (_bondHDR == true) &&
     (fabs(_bondHDRExposure - 1.5) < 1e-4) &&
     (_bondAmbientColor.red() == 255) &&
     (_bondAmbientColor.green() == 255) &&
     (_bondAmbientColor.blue() == 255) &&
     (_bondAmbientColor.alpha() == 255) &&
     (_bondDiffuseColor.red() == 205) &&
     (_bondDiffuseColor.green() == 205) &&
     (_bondDiffuseColor.blue() == 205) &&
     (_bondDiffuseColor.alpha() == 255) &&
     (_bondSpecularColor.red() == 255) &&
     (_bondSpecularColor.green() == 255) &&
     (_bondSpecularColor.blue() == 255) &&
     (_bondSpecularColor.alpha() == 255) &&
     (fabs(_bondAmbientIntensity - 0.35) < 1e-4) &&
     (fabs(_bondDiffuseIntensity - 1.0) < 1e-4) &&
     (fabs(_bondSpecularIntensity - 1.0) < 1e-4) &&
     (fabs(_bondShininess - 4.0) < 1e-4) &&
     (fabs(_bondHue - 1.0) < 1e-4) &&
     (fabs(_bondSaturation - 1.0) < 1e-4) &&
     (fabs(_bondValue - 1.0) < 1e-4)  &&
     (_atomSelectionStyle == RKSelectionStyle::WorleyNoise3D) &&
     (fabs(_selectionScaling - 1.2) < 1e-4))
     {
       qDebug() << "FOUND DEFAULT";
       _atomRepresentationStyle = RepresentationStyle::defaultStyle;
     }
  else if ((fabs(_atomHue - 1.0) < 1e-4) &&
     (fabs(_atomSaturation - 0.5) < 1e-4) &&
     (fabs(_atomValue - 1.0) < 1e-4) &&
     (fabs(_atomAmbientColor.redF() - 1.0) < 1e-4) &&
     (fabs(_atomAmbientColor.greenF() - 1.0) < 1e-4) &&
     (fabs(_atomAmbientColor.blueF() - 1.0) < 1e-4) &&
     (fabs(_atomAmbientColor.alphaF() - 1.0) < 1e-4) &&
     (fabs(_atomSpecularColor.redF() - 1.0) < 1e-4) &&
     (fabs(_atomSpecularColor.greenF() - 1.0) < 1e-4) &&
     (fabs(_atomSpecularColor.blueF() - 1.0) < 1e-4) &&
     (fabs(_atomSpecularColor.alphaF() - 1.0) < 1e-4) &&
     (_drawAtoms == true) &&
     (_drawBonds == false) &&
     (_atomHDR == false) &&
     (_atomAmbientOcclusion == true) &&
     (fabs(_atomAmbientIntensity - 1.0) < 1e-4) &&
     (fabs(_atomDiffuseIntensity - 0.0) < 1e-4) &&
     (fabs(_atomSpecularIntensity - 0.2) < 1e-4) &&
     (fabs(_atomShininess - 6.0) < 1e-4) &&
     (fabs(_atomScaleFactor - 1.0) < 1e-4) &&
     (_atomRepresentationType == RepresentationType::vdw) &&
     (QString::compare(_atomForceFieldIdentifier, "Default", Qt::CaseInsensitive) == 0) &&
     (QString::compare(_atomColorSchemeIdentifier, "Rasmol", Qt::CaseInsensitive) == 0) &&
     (_atomColorSchemeOrder == ColorSchemeOrder::elementOnly) &&
     (_atomSelectionStyle == RKSelectionStyle::WorleyNoise3D) &&
     (fabs(_selectionScaling - 1.0) < 1e-4))
     {
       qDebug() << "FOUND FANCY";
       _atomRepresentationStyle = RepresentationStyle::fancy;
     }
  else if ((_drawAtoms == true) &&
     (_atomRepresentationType == RepresentationType::unity) &&
     (QString::compare(_atomForceFieldIdentifier, "Default", Qt::CaseInsensitive) == 0) &&
     (QString::compare(_atomColorSchemeIdentifier, "Jmol", Qt::CaseInsensitive) == 0) &&
     (_atomColorSchemeOrder == ColorSchemeOrder::elementOnly) &&
		(fabs(_atomScaleFactor - 1.0) < 1e-4) &&
     (_drawBonds == true) &&
     (_bondColorMode == RKBondColorMode::split) &&
     (fabs(_bondScaleFactor - 0.25) < 1e-4) &&
     (_bondAmbientOcclusion == false) &&
     (_bondHDR == true) &&
     ((_bondHDRExposure - 1.5) < 1e-4) &&
     (_bondAmbientColor.red() == 255) &&
     (_bondAmbientColor.green() == 255) &&
     (_bondAmbientColor.blue() == 255) &&
     (_bondAmbientColor.alpha() == 255) &&
     (_bondDiffuseColor.red() == 205) &&
     (_bondDiffuseColor.green() == 205) &&
     (_bondDiffuseColor.blue() == 205) &&
     (_bondDiffuseColor.alpha() == 255) &&
     (_bondSpecularColor.red() == 255) &&
     (_bondSpecularColor.green() == 255) &&
     (_bondSpecularColor.blue() == 255) &&
     (_bondSpecularColor.alpha() == 255) &&
     ((_bondAmbientIntensity - 0.1) < 1e-4) &&
     ((_bondDiffuseIntensity - 1.0) < 1e-4) &&
     ((_bondSpecularIntensity - 1.0) < 1e-4) &&
     (fabs(_bondShininess - 4.0) < 1e-4) &&
     (fabs(_bondHue - 1.0) < 1e-4) &&
     (fabs(_bondSaturation - 1.0) < 1e-4) &&
     (fabs(_bondValue - 1.0) < 1e-4) &&
     (_atomSelectionStyle == RKSelectionStyle::WorleyNoise3D) &&
     (fabs(_selectionScaling - 1.5) < 1e-4))
     {
       qDebug() << "FOUND LICORICE";
       _atomRepresentationStyle = RepresentationStyle::licorice;
     }
  else
  {
		qDebug() << "FOUND CUSTOM";
    _atomRepresentationStyle = RepresentationStyle::custom;
  }

  qDebug() << "FOUND: " << int(_atomRepresentationStyle);

	
}

void Structure::setRepresentationType(RepresentationType type)
{
  if(int(type)>=0 && type < RepresentationType::multiple_values)
  {
    _atomRepresentationType = type;
    std::vector<std::shared_ptr<SKAtomTreeNode>> asymmetricAtomNodes = _atomsTreeController->flattenedLeafNodes();

    for(std::shared_ptr<SKAtomTreeNode> node: asymmetricAtomNodes)
    {
      if(std::shared_ptr<SKAsymmetricAtom> atom = node->representedObject())
      {
        double radius = 0.0;
        switch(type)
        {
          case RepresentationType::sticks_and_balls:
            radius = PredefinedElements::predefinedElements[atom->elementIdentifier()]._covalentRadius;
            atom->setDrawRadius(radius);
            _atomScaleFactor = 0.7;
            break;
          case RepresentationType::vdw:
            radius = PredefinedElements::predefinedElements[atom->elementIdentifier()]._VDWRadius;
            atom->setDrawRadius(radius);
            _atomScaleFactor = 1.0;
            break;
          case RepresentationType::unity:
            atom->setDrawRadius(_bondScaleFactor);
            break;
          default:
            break;
        }
      }
    }
  }
}

void Structure::setRepresentationColorSchemeIdentifier(const QString colorSchemeName, const SKColorSets &colorSets)
{

  if(const SKColorSet* colorSet = colorSets[colorSchemeName])
  {
    _atomColorSchemeIdentifier = colorSchemeName;
    std::vector<std::shared_ptr<SKAtomTreeNode>> asymmetricAtomNodes = _atomsTreeController->flattenedLeafNodes();

    for(std::shared_ptr<SKAtomTreeNode> node: asymmetricAtomNodes)
    {
      if(std::shared_ptr<SKAsymmetricAtom> atom = node->representedObject())
      {
         QString chemicalElement = PredefinedElements::predefinedElements[atom->elementIdentifier()]._chemicalSymbol;
         const QColor* color = (*colorSet)[chemicalElement];
         if(color)
         {
           atom->setColor(*color);
         }
      }
    }
  }
}


void Structure::setAtomForceFieldIdentifier(QString identifier, ForceFieldSets& forceFieldSets)
{
  _atomForceFieldIdentifier = identifier;
  updateForceField(forceFieldSets);
}


void Structure::updateForceField(ForceFieldSets &forceFieldSets)
{
  if(ForceFieldSet* forceField = forceFieldSets[_atomForceFieldIdentifier])
  {
    std::vector<std::shared_ptr<SKAtomTreeNode>> asymmetricAtomNodes = _atomsTreeController->flattenedLeafNodes();

    switch(_atomForceFieldOrder)
    {
    case ForceFieldSchemeOrder::elementOnly:
      for(std::shared_ptr<SKAtomTreeNode> node: asymmetricAtomNodes)
      {
        if(std::shared_ptr<SKAsymmetricAtom> atom = node->representedObject())
        {
          SKElement& element = PredefinedElements::predefinedElements[atom->elementIdentifier()];
          const QString chemicalElement = element._chemicalSymbol;
          ForceFieldType* forceFieldType = (*forceField)[chemicalElement];
          if(forceFieldType)
          {
            atom->setPotentialParameters(forceFieldType->potentialParameters());
            atom->setBondDistanceCriteria(forceFieldType->userDefinedRadius());
          }
        }
      }
      break;
    case ForceFieldSchemeOrder::forceFieldFirst:
      for(std::shared_ptr<SKAtomTreeNode> node: asymmetricAtomNodes)
      {
        if(std::shared_ptr<SKAsymmetricAtom> atom = node->representedObject())
        {
          ForceFieldType* forceFieldType = (*forceField)[atom->uniqueForceFieldName()];
          if(forceFieldType)
          {
            atom->setPotentialParameters(forceFieldType->potentialParameters());
            atom->setBondDistanceCriteria(forceFieldType->userDefinedRadius());
          }
        }
      }
      break;
    case ForceFieldSchemeOrder::forceFieldOnly:
      for(std::shared_ptr<SKAtomTreeNode> node: asymmetricAtomNodes)
      {
        if(std::shared_ptr<SKAsymmetricAtom> atom = node->representedObject())
        {
          ForceFieldType* forceFieldType = (*forceField)[atom->uniqueForceFieldName()];
          if(forceFieldType)
          {
            atom->setPotentialParameters(forceFieldType->potentialParameters());
            atom->setBondDistanceCriteria(forceFieldType->userDefinedRadius());
          }
        }
      }
      break;
    }
  }
  else
  {
      std::cout << "No force field present" << std::endl;
  }
}

double Structure::renderSelectionFrequency() const
{
  switch(_atomSelectionStyle)
  {
  case RKSelectionStyle::WorleyNoise3D:
    return _atomSelectionWorleyNoise3DFrequency;
  case RKSelectionStyle::striped:
      return _atomSelectionStripesFrequency;
  case RKSelectionStyle::glow:
  case RKSelectionStyle::multiple_values:
     return 0.0;
  }
}

void Structure::setSelectionFrequency(double value)
{
  switch(_atomSelectionStyle)
  {
  case RKSelectionStyle::WorleyNoise3D:
    _atomSelectionWorleyNoise3DFrequency = value;
    break;
  case RKSelectionStyle::striped:
    _atomSelectionStripesFrequency = value;
    break;
  case RKSelectionStyle::glow:
  case RKSelectionStyle::multiple_values:
    break;
  }
}

double Structure::renderSelectionDensity() const
{
  switch(_atomSelectionStyle)
  {
  case RKSelectionStyle::WorleyNoise3D:
    return _atomSelectionWorleyNoise3DJitter;
  case RKSelectionStyle::striped:
    return _atomSelectionStripesDensity;
  case RKSelectionStyle::glow:
  case RKSelectionStyle::multiple_values:
    return 0.0;
  }
}
void Structure::setSelectionDensity(double value)
{
  switch(_atomSelectionStyle)
  {
  case RKSelectionStyle::WorleyNoise3D:
    _atomSelectionWorleyNoise3DJitter = value;
    break;
  case RKSelectionStyle::striped:
    _atomSelectionStripesDensity = value;
    break;
  case RKSelectionStyle::glow:
  case RKSelectionStyle::multiple_values:
    break;
  }
}

void  Structure::clearSelection()
{
  _atomsTreeController->selectedTreeNodes().clear();
}

void Structure::setAtomSelection(int atomId)
{
  _atomsTreeController->selectedTreeNodes().clear();
  addAtomToSelection(atomId);
}

void Structure::addAtomToSelection(int atomId)
{
  std::vector<std::shared_ptr<SKAtomTreeNode>> atomNodes = _atomsTreeController->flattenedLeafNodes();

  std::vector<std::shared_ptr<SKAtomCopy>> atomCopies = _atomsTreeController->atomCopies();

  std::shared_ptr<SKAtomCopy> selectedAtomCopy = atomCopies[atomId / _cell->numberOfReplicas()];
  std::shared_ptr<SKAtomTreeNode> selectedAtom = atomNodes[selectedAtomCopy->asymmetricIndex()];

  _atomsTreeController->selectedTreeNodes().insert(selectedAtom);
}



void Structure::toggleAtomSelection(int atomId)
{
  std::vector<std::shared_ptr<SKAtomTreeNode>> atomNodes = _atomsTreeController->flattenedLeafNodes();

  std::vector<std::shared_ptr<SKAtomCopy>> atomCopies = _atomsTreeController->atomCopies();

  std::shared_ptr<SKAtomCopy> selectedAtomCopy = atomCopies[atomId / _cell->numberOfReplicas()];
  std::shared_ptr<SKAtomTreeNode> selectedAtom = atomNodes[selectedAtomCopy->asymmetricIndex()];

  std::unordered_set<std::shared_ptr<SKAtomTreeNode>>::const_iterator search = _atomsTreeController->selectedTreeNodes().find(selectedAtom);
  if (search != _atomsTreeController->selectedTreeNodes().end())
  {
    _atomsTreeController->selectedTreeNodes().erase(*search);
  }
  else
  {
    _atomsTreeController->selectedTreeNodes().insert(selectedAtom);
  }
}

void Structure::setAtomSelection(std::vector<int>& atomIds)
{
  _atomsTreeController->selectedTreeNodes().clear();
  addToAtomSelection(atomIds);
}

void Structure::addToAtomSelection(std::vector<int>& atomIds)
{
  std::vector<std::shared_ptr<SKAtomTreeNode>> atomNodes = _atomsTreeController->flattenedLeafNodes();

  std::vector<std::shared_ptr<SKAtomCopy>> atomCopies = _atomsTreeController->atomCopies();

  for(int atomId: atomIds)
  {
    std::shared_ptr<SKAtomCopy> selectedAtomCopy = atomCopies[atomId / _cell->numberOfReplicas()];
    std::shared_ptr<SKAtomTreeNode> selectedAtom = atomNodes[selectedAtomCopy->asymmetricIndex()];

    _atomsTreeController->selectedTreeNodes().insert(selectedAtom);
  }
}

std::vector<RKInPerInstanceAttributesAtoms> Structure::renderSelectedAtoms() const
{
    int numberOfReplicas = _cell->numberOfReplicas();

    int minimumReplicaX = _cell->minimumReplicaX();
    int minimumReplicaY = _cell->minimumReplicaY();
    int minimumReplicaZ = _cell->minimumReplicaZ();

    int maximumReplicaX = _cell->maximumReplicaX();
    int maximumReplicaY = _cell->maximumReplicaY();
    int maximumReplicaZ = _cell->maximumReplicaZ();

    std::unordered_set<std::shared_ptr<SKAtomTreeNode>> asymmetricAtomNodes = _atomsTreeController->selectedTreeNodes();
    //std::vector<std::shared_ptr<SKAtomTreeNode>> asymmetricAtomNodes = _atomsTreeController->flattenedLeafNodes();

    std::cout << "Number of selected atomas: " << asymmetricAtomNodes.size() << std::endl;

    std::vector<RKInPerInstanceAttributesAtoms> atomData = std::vector<RKInPerInstanceAttributesAtoms>();


    for(std::shared_ptr<SKAtomTreeNode> node: asymmetricAtomNodes)
    {
       if(std::shared_ptr<SKAsymmetricAtom> atom = node->representedObject())
       {
         for(std::shared_ptr<SKAtomCopy> copy : atom->copies())
         {
           if(copy->type() == SKAtomCopy::AtomCopyType::copy)
           {
             QColor color = atom->color();
             double w = atom->isVisible() ? 1.0 : -1.0;
             //std::cout << "here" << std::endl;
             for(int k1=minimumReplicaX;k1<=maximumReplicaX;k1++)
             {
               for(int k2=minimumReplicaY;k2<=maximumReplicaY;k2++)
               {
                 for(int k3=minimumReplicaZ;k3<=maximumReplicaZ;k3++)
                 {
                   //std::cout << "position: " << copy->position() .x << "," << copy->position().y << ", " << copy->position().z << std::endl;

                   float4 position = float4(_cell->unitCell() * (copy->position() + double3(k1,k2,k3)), w);

                   float4 ambient = float4(color.redF(),color.greenF(),color.blueF(),color.alphaF());
                   float4 diffuse = float4(color.redF(),color.greenF(),color.blueF(),color.alphaF());

                   float4 specular = float4(1.0,1.0,1.0,1.0);
                   double radius = atom->drawRadius() * atom->occupancy();
                   float4 scale = float4(radius,radius,radius,1.0);

                   //if(atom->occupancy()<1.0)
                   //{
                   //  diffuse = float4(1.0,1.0,1.0,1.0);
                   //}

                   RKInPerInstanceAttributesAtoms atom1 = RKInPerInstanceAttributesAtoms(position, ambient, diffuse, specular, scale);
                   atomData.push_back(atom1);
                 }
               }
             }
           }
         }
       }
    }

    return atomData;
}

void Structure::setStructureNitrogenSurfaceArea(double value)
{
  std::cout << "Surface area: " << value << std::endl;
  _structureGravimetricNitrogenSurfaceArea = value * Constants::AvogadroConstantPerAngstromSquared / _structureMass;
  _structureVolumetricNitrogenSurfaceArea = value * 1e4 / cell()->volume();
}

void Structure::recomputeDensityProperties()
{
  std::vector<std::shared_ptr<SKAtomCopy>> atomCopies = _atomsTreeController->atomCopies();

  _structureMass = 0.0;
  for(std::shared_ptr<SKAtomCopy> atom : atomCopies)
  {
    int elementId = atom->parent()->elementIdentifier();
    _structureMass +=  PredefinedElements::predefinedElements[elementId]._mass;
  }

  _structureDensity = 1.0e-3 * _structureMass / (Constants::AvogadroConstantPerAngstromCubed * _cell->volume());
  _structureSpecificVolume = 1.0e3 / _structureDensity;
  _structureAccessiblePoreVolume = _structureHeliumVoidFraction * _structureSpecificVolume;
}

QDataStream &operator<<(QDataStream &stream, const std::shared_ptr<Structure> &structure)
{
  stream << structure->_versionNumber;
  stream << structure->_displayName;
  stream << structure->_isVisible;

  stream << structure->_spaceGroup;
  stream << structure->_cell;
  stream << structure->_periodic;
  stream << structure->_origin;
  stream << structure->_scaling;
  stream << structure->_orientation;
  stream << structure->_rotationDelta;

  stream << structure->_primitiveTransformationMatrix;
  stream << structure->_primitiveOrientation;
  stream << structure->_primitiveRotationDelta;

  stream << structure->_primitiveOpacity;
  stream << structure->_primitiveIsCapped;
  stream << structure->_primitiveIsFractional;
  stream << structure->_primitiveNumberOfSides;
  stream << structure->_primitiveThickness;

  stream << structure->_primitiveFrontSideHDR;
  stream << structure->_primitiveFrontSideHDRExposure;
  stream << structure->_primitiveFrontSideAmbientColor;
  stream << structure->_primitiveFrontSideDiffuseColor;
  stream << structure->_primitiveFrontSideSpecularColor;
  stream << structure->_primitiveFrontSideDiffuseIntensity;
  stream << structure->_primitiveFrontSideAmbientIntensity;
  stream << structure->_primitiveFrontSideSpecularIntensity;
  stream << structure->_primitiveFrontSideShininess;

  stream << structure->_primitiveBackSideHDR;
  stream << structure->_primitiveBackSideHDRExposure;
  stream << structure->_primitiveBackSideAmbientColor;
  stream << structure->_primitiveBackSideDiffuseColor;
  stream << structure->_primitiveBackSideSpecularColor;
  stream << structure->_primitiveBackSideDiffuseIntensity;
  stream << structure->_primitiveBackSideAmbientIntensity;
  stream << structure->_primitiveBackSideSpecularIntensity;
  stream << structure->_primitiveBackSideShininess;

  stream << structure->_minimumGridEnergyValue;

  stream << structure->_atomsTreeController;
  stream << structure->_drawAtoms;
  stream << structure->_atomRepresentationType;
  stream << structure->_atomRepresentationStyle;
  stream << structure->_atomForceFieldIdentifier;
  stream << structure->_atomForceFieldOrder;
  stream << structure->_atomColorSchemeIdentifier;
  stream << structure->_atomColorSchemeOrder;

  stream << structure->_atomSelectionStyle;
  stream << structure->_atomSelectionStripesDensity;
  stream << structure->_atomSelectionStripesFrequency;
  stream << structure->_atomSelectionWorleyNoise3DFrequency;
  stream << structure->_atomSelectionWorleyNoise3DJitter;
  stream << structure->_selectionScaling;
  stream << structure->_selectionIntensity;

  stream << structure->_atomHue;
  stream << structure->_atomSaturation;
  stream << structure->_atomValue;
  stream << structure->_atomScaleFactor;

  stream << structure->_atomAmbientOcclusion;
  stream << structure->_atomAmbientOcclusionPatchNumber;
  stream << structure->_atomAmbientOcclusionTextureSize;
  stream << structure->_atomAmbientOcclusionPatchSize;

  stream << structure->_atomHDR;
  stream << structure->_atomHDRExposure;
  stream << structure->_atomHDRBloomLevel;

  stream << structure->_atomAmbientColor;
  stream << structure->_atomDiffuseColor;
  stream << structure->_atomSpecularColor;
  stream << structure->_atomAmbientIntensity;
  stream << structure->_atomDiffuseIntensity;
  stream << structure->_atomSpecularIntensity;
  stream << structure->_atomShininess;

  stream << structure->_atomTextType;
  stream << structure->_atomTextFont;
  stream << structure->_atomTextScaling;
  stream << structure->_atomTextColor;
  stream << structure->_atomTextGlowColor;
  stream << structure->_atomTextStyle;
  stream << structure->_atomTextEffect;
  stream << structure->_atomTextAlignment;
  stream << structure->_atomTextOffset;


  stream << structure->_bondSetController;

  stream << structure->_drawBonds;
  stream << structure->_bondScaleFactor;
  stream << structure->_bondColorMode;

  stream << structure->_bondAmbientColor;
  stream << structure->_bondDiffuseColor;
  stream << structure->_bondSpecularColor;
  stream << structure->_bondAmbientIntensity;
  stream << structure->_bondDiffuseIntensity;
  stream << structure->_bondSpecularIntensity;
  stream << structure->_bondShininess;

  stream << structure->_bondHDR;
  stream << structure->_bondHDRExposure;
  stream << structure->_bondHDRBloomLevel;

  stream << structure->_bondHue;
  stream << structure->_bondSaturation;
  stream << structure->_bondValue;

  stream << structure->_bondAmbientOcclusion;


  stream << structure->_drawUnitCell;
  stream << structure->_unitCellScaleFactor;
  stream << structure->_unitCellDiffuseColor;
  stream << structure->_unitCellDiffuseIntensity;

  stream << structure->_drawAdsorptionSurface;
  stream << structure->_adsorptionSurfaceOpacity;
  stream << structure->_adsorptionSurfaceIsoValue;
  stream << structure->_adsorptionSurfaceMinimumValue;

  stream << structure->_adsorptionSurfaceSize;
  stream << structure->_adsorptionSurfaceNumberOfTriangles;

  stream << structure->_adsorptionSurfaceProbeMolecule;

  stream << structure->_adsorptionSurfaceFrontSideHDR;
  stream << structure->_adsorptionSurfaceFrontSideHDRExposure;
  stream << structure->_adsorptionSurfaceFrontSideAmbientColor;
  stream << structure->_adsorptionSurfaceFrontSideDiffuseColor;
  stream << structure->_adsorptionSurfaceFrontSideSpecularColor;
  stream << structure->_adsorptionSurfaceFrontSideAmbientIntensity;
  stream << structure->_adsorptionSurfaceFrontSideDiffuseIntensity;
  stream << structure->_adsorptionSurfaceFrontSideSpecularIntensity;
  stream << structure->_adsorptionSurfaceFrontSideShininess;

  stream << structure->_adsorptionSurfaceBackSideHDR;
  stream << structure->_adsorptionSurfaceBackSideHDRExposure;
  stream << structure->_adsorptionSurfaceBackSideAmbientColor;
  stream << structure->_adsorptionSurfaceBackSideDiffuseColor;
  stream << structure->_adsorptionSurfaceBackSideSpecularColor;
  stream << structure->_adsorptionSurfaceBackSideAmbientIntensity;
  stream << structure->_adsorptionSurfaceBackSideDiffuseIntensity;
  stream << structure->_adsorptionSurfaceBackSideSpecularIntensity;
  stream << structure->_adsorptionSurfaceBackSideShininess;

  stream << structure->_structureType;
  stream << structure->_structureMaterialType;
  stream << structure->_structureMass;
  stream << structure->_structureDensity;
  stream << structure->_structureHeliumVoidFraction;
  stream << structure->_structureSpecificVolume;
  stream << structure->_structureAccessiblePoreVolume;
  stream << structure->_structureVolumetricNitrogenSurfaceArea;
  stream << structure->_structureGravimetricNitrogenSurfaceArea;
  stream << structure->_structureNumberOfChannelSystems;
  stream << structure->_structureNumberOfInaccessiblePockets;
  stream << structure->_structureDimensionalityOfPoreSystem;
  stream << structure->_structureLargestCavityDiameter;
  stream << structure->_structureRestrictingPoreLimitingDiameter;
  stream << structure->_structureLargestCavityDiameterAlongAviablePath;

  stream << structure->_authorFirstName;
  stream << structure->_authorMiddleName;
  stream << structure->_authorLastName;
  stream << structure->_authorOrchidID;
  stream << structure->_authorResearcherID;
  stream << structure->_authorAffiliationUniversityName;
  stream << structure->_authorAffiliationFacultyName;
  stream << structure->_authorAffiliationInstituteName;
  stream << structure->_authorAffiliationCityName;
  stream << structure->_authorAffiliationCountryName;

  stream << uint16_t(structure->_creationDate.day());
  stream << uint16_t(structure->_creationDate.month());
  stream << uint32_t(structure->_creationDate.year());
  stream << structure->_creationTemperature;
  stream << structure->_creationTemperatureScale;
  stream << structure->_creationPressure;
  stream << structure->_creationPressureScale;
  stream << structure->_creationMethod;
  stream << structure->_creationUnitCellRelaxationMethod;
  stream << structure->_creationAtomicPositionsSoftwarePackage;
  stream << structure->_creationAtomicPositionsIonsRelaxationAlgorithm;
  stream << structure->_creationAtomicPositionsIonsRelaxationCheck;
  stream << structure->_creationAtomicPositionsForcefield;
  stream << structure->_creationAtomicPositionsForcefieldDetails;
  stream << structure->_creationAtomicChargesSoftwarePackage;
  stream << structure->_creationAtomicChargesAlgorithms;
  stream << structure->_creationAtomicChargesForcefield;
  stream << structure->_creationAtomicChargesForcefieldDetails;

  stream << structure->_experimentalMeasurementRadiation;
  stream << structure->_experimentalMeasurementWaveLength;
  stream << structure->_experimentalMeasurementThetaMin;
  stream << structure->_experimentalMeasurementThetaMax;
  stream << structure->_experimentalMeasurementIndexLimitsHmin;
  stream << structure->_experimentalMeasurementIndexLimitsHmax;
  stream << structure->_experimentalMeasurementIndexLimitsKmin;
  stream << structure->_experimentalMeasurementIndexLimitsKmax;
  stream << structure->_experimentalMeasurementIndexLimitsLmin;
  stream << structure->_experimentalMeasurementIndexLimitsLmax;
  stream << structure->_experimentalMeasurementNumberOfSymmetryIndependentReflections;
  stream << structure->_experimentalMeasurementSoftware;
  stream << structure->_experimentalMeasurementRefinementDetails;
  stream << structure->_experimentalMeasurementGoodnessOfFit;
  stream << structure->_experimentalMeasurementRFactorGt;
  stream << structure->_experimentalMeasurementRFactorAll;

  stream << structure->_chemicalFormulaMoiety;
  stream << structure->_chemicalFormulaSum;
  stream << structure->_chemicalNameSystematic;
  stream << structure->_cellFormulaUnitsZ;

  stream << structure->_citationArticleTitle;
  stream << structure->_citationJournalTitle;
  stream << structure->_citationAuthors;
  stream << structure->_citationJournalVolume;
  stream << structure->_citationJournalNumber;
  stream << structure->_citationJournalPageNumbers;
  stream << structure->_citationDOI;
  stream << uint16_t(structure->_citationPublicationDate.day());
  stream << uint16_t(structure->_citationPublicationDate.month());
  stream << uint32_t(structure->_citationPublicationDate.year());
  stream << structure->_citationDatebaseCodes;

  return stream;
}

QDataStream &operator>>(QDataStream &stream, std::shared_ptr<Structure> &structure)
{
  uint16_t day,month;
  uint32_t year;
  qint64 versionNumber;
  stream >> versionNumber;
  if(versionNumber > structure->_versionNumber)
  {
    throw InvalidArchiveVersionException(__FILE__, __LINE__, "Structure");
  }

  stream >> structure->_displayName;
  stream >> structure->_isVisible;

  stream >> structure->_spaceGroup;
  stream >> structure->_cell;
  stream >> structure->_periodic;
  stream >> structure->_origin;
  stream >> structure->_scaling;
  stream >> structure->_orientation;
  stream >> structure->_rotationDelta;

  if(versionNumber >= 2)
  {
    stream >> structure->_primitiveTransformationMatrix;
    stream >> structure->_primitiveOrientation;
    stream >> structure->_primitiveRotationDelta;

    stream >> structure->_primitiveOpacity;
    stream >> structure->_primitiveIsCapped;
    stream >> structure->_primitiveIsFractional;
    stream >> structure->_primitiveNumberOfSides;
    stream >> structure->_primitiveThickness;

    stream >> structure->_primitiveFrontSideHDR;
    stream >> structure->_primitiveFrontSideHDRExposure;
    stream >> structure->_primitiveFrontSideAmbientColor;
    stream >> structure->_primitiveFrontSideDiffuseColor;
    stream >> structure->_primitiveFrontSideSpecularColor;
    stream >> structure->_primitiveFrontSideDiffuseIntensity;
    stream >> structure->_primitiveFrontSideAmbientIntensity;
    stream >> structure->_primitiveFrontSideSpecularIntensity;
    stream >> structure->_primitiveFrontSideShininess;

    stream >> structure->_primitiveBackSideHDR;
    stream >> structure->_primitiveBackSideHDRExposure;
    stream >> structure->_primitiveBackSideAmbientColor;
    stream >> structure->_primitiveBackSideDiffuseColor;
    stream >> structure->_primitiveBackSideSpecularColor;
    stream >> structure->_primitiveBackSideDiffuseIntensity;
    stream >> structure->_primitiveBackSideAmbientIntensity;
    stream >> structure->_primitiveBackSideSpecularIntensity;
    stream >> structure->_primitiveBackSideShininess;
  }

  stream >> structure->_minimumGridEnergyValue;

  stream >> structure->_atomsTreeController;
  stream >> structure->_drawAtoms;

  stream >> structure->_atomRepresentationType;
  stream >> structure->_atomRepresentationStyle;
  stream >> structure->_atomForceFieldIdentifier;
  stream >> structure->_atomForceFieldOrder;
  stream >> structure->_atomColorSchemeIdentifier;
  stream >> structure->_atomColorSchemeOrder;

  stream >> structure->_atomSelectionStyle;
  stream >> structure->_atomSelectionStripesDensity;
  stream >> structure->_atomSelectionStripesFrequency;
  stream >> structure->_atomSelectionWorleyNoise3DFrequency;
  stream >> structure->_atomSelectionWorleyNoise3DJitter;
  stream >> structure->_selectionScaling;
  stream >> structure->_selectionIntensity;

  stream >> structure->_atomHue;
  stream >> structure->_atomSaturation;
  stream >> structure->_atomValue;
  stream >> structure->_atomScaleFactor;

  stream >> structure->_atomAmbientOcclusion;
  stream >> structure->_atomAmbientOcclusionPatchNumber;
  stream >> structure->_atomAmbientOcclusionTextureSize;
  stream >> structure->_atomAmbientOcclusionPatchSize;

  stream >> structure->_atomHDR;
  stream >> structure->_atomHDRExposure;
  stream >> structure->_atomHDRBloomLevel;

  stream >> structure->_atomAmbientColor;
  stream >> structure->_atomDiffuseColor;
  stream >> structure->_atomSpecularColor;
  stream >> structure->_atomAmbientIntensity;
  stream >> structure->_atomDiffuseIntensity;
  stream >> structure->_atomSpecularIntensity;
  stream >> structure->_atomShininess;

  stream >> structure->_atomTextType;
  stream >> structure->_atomTextFont;
  stream >> structure->_atomTextScaling;
  stream >> structure->_atomTextColor;
  stream >> structure->_atomTextGlowColor;
  stream >> structure->_atomTextStyle;
  stream >> structure->_atomTextEffect;
  stream >> structure->_atomTextAlignment;
  stream >> structure->_atomTextOffset;

  stream >> structure->_bondSetController;

  stream >> structure->_drawBonds;
  stream >> structure->_bondScaleFactor;
  stream >> structure->_bondColorMode;

  stream >> structure->_bondAmbientColor;
  stream >> structure->_bondDiffuseColor;
  stream >> structure->_bondSpecularColor;
  stream >> structure->_bondAmbientIntensity;
  stream >> structure->_bondDiffuseIntensity;
  stream >> structure->_bondSpecularIntensity;
  stream >> structure->_bondShininess;

  stream >> structure->_bondHDR;
  stream >> structure->_bondHDRExposure;
  stream >> structure->_bondHDRBloomLevel;

  stream >> structure->_bondHue;
  stream >> structure->_bondSaturation;
  stream >> structure->_bondValue;

  stream >> structure->_bondAmbientOcclusion;

  stream >> structure->_drawUnitCell;
  stream >> structure->_unitCellScaleFactor;
  stream >> structure->_unitCellDiffuseColor;
  stream >> structure->_unitCellDiffuseIntensity;

  stream >> structure->_drawAdsorptionSurface;
  stream >> structure->_adsorptionSurfaceOpacity;
  stream >> structure->_adsorptionSurfaceIsoValue;
  stream >> structure->_adsorptionSurfaceMinimumValue;

  stream >> structure->_adsorptionSurfaceSize;
  stream >> structure->_adsorptionSurfaceNumberOfTriangles;

  stream >> structure->_adsorptionSurfaceProbeMolecule;

  stream >> structure->_adsorptionSurfaceFrontSideHDR;
  stream >> structure->_adsorptionSurfaceFrontSideHDRExposure;
  stream >> structure->_adsorptionSurfaceFrontSideAmbientColor;
  stream >> structure->_adsorptionSurfaceFrontSideDiffuseColor;
  stream >> structure->_adsorptionSurfaceFrontSideSpecularColor;
  stream >> structure->_adsorptionSurfaceFrontSideAmbientIntensity;
  stream >> structure->_adsorptionSurfaceFrontSideDiffuseIntensity;
  stream >> structure->_adsorptionSurfaceFrontSideSpecularIntensity;
  stream >> structure->_adsorptionSurfaceFrontSideShininess;

  stream >> structure->_adsorptionSurfaceBackSideHDR;
  stream >> structure->_adsorptionSurfaceBackSideHDRExposure;
  stream >> structure->_adsorptionSurfaceBackSideAmbientColor;
  stream >> structure->_adsorptionSurfaceBackSideDiffuseColor;
  stream >> structure->_adsorptionSurfaceBackSideSpecularColor;
  stream >> structure->_adsorptionSurfaceBackSideAmbientIntensity;
  stream >> structure->_adsorptionSurfaceBackSideDiffuseIntensity;
  stream >> structure->_adsorptionSurfaceBackSideSpecularIntensity;
  stream >> structure->_adsorptionSurfaceBackSideShininess;

  stream >> structure->_structureType;
  stream >> structure->_structureMaterialType;
  stream >> structure->_structureMass;
  stream >> structure->_structureDensity;
  stream >> structure->_structureHeliumVoidFraction;
  stream >> structure->_structureSpecificVolume;
  stream >> structure->_structureAccessiblePoreVolume;
  stream >> structure->_structureVolumetricNitrogenSurfaceArea;
  stream >> structure->_structureGravimetricNitrogenSurfaceArea;
  stream >> structure->_structureNumberOfChannelSystems;
  stream >> structure->_structureNumberOfInaccessiblePockets;
  stream >> structure->_structureDimensionalityOfPoreSystem;
  stream >> structure->_structureLargestCavityDiameter;
  stream >> structure->_structureRestrictingPoreLimitingDiameter;
  stream >> structure->_structureLargestCavityDiameterAlongAviablePath;

  stream >> structure->_authorFirstName;
  stream >> structure->_authorMiddleName;
  stream >> structure->_authorLastName;
  stream >> structure->_authorOrchidID;
  stream >> structure->_authorResearcherID;
  stream >> structure->_authorAffiliationUniversityName;
  stream >> structure->_authorAffiliationFacultyName;
  stream >> structure->_authorAffiliationInstituteName;
  stream >> structure->_authorAffiliationCityName;
  stream >> structure->_authorAffiliationCountryName;



  stream >> day;
  stream >> month;
  stream >> year;
  structure->_creationDate = QDate(int(year),int(month),int(day));
  stream >> structure->_creationTemperature;
  stream >> structure->_creationTemperatureScale;
  stream >> structure->_creationPressure;
  stream >> structure->_creationPressureScale;
  stream >> structure->_creationMethod;
  stream >> structure->_creationUnitCellRelaxationMethod;
  stream >> structure->_creationAtomicPositionsSoftwarePackage;
  stream >> structure->_creationAtomicPositionsIonsRelaxationAlgorithm;
  stream >> structure->_creationAtomicPositionsIonsRelaxationCheck;
  stream >> structure->_creationAtomicPositionsForcefield;
  stream >> structure->_creationAtomicPositionsForcefieldDetails;
  stream >> structure->_creationAtomicChargesSoftwarePackage;
  stream >> structure->_creationAtomicChargesAlgorithms;
  stream >> structure->_creationAtomicChargesForcefield;
  stream >> structure->_creationAtomicChargesForcefieldDetails;

  stream >> structure->_experimentalMeasurementRadiation;
  stream >> structure->_experimentalMeasurementWaveLength;
  stream >> structure->_experimentalMeasurementThetaMin;
  stream >> structure->_experimentalMeasurementThetaMax;
  stream >> structure->_experimentalMeasurementIndexLimitsHmin;
  stream >> structure->_experimentalMeasurementIndexLimitsHmax;
  stream >> structure->_experimentalMeasurementIndexLimitsKmin;
  stream >> structure->_experimentalMeasurementIndexLimitsKmax;
  stream >> structure->_experimentalMeasurementIndexLimitsLmin;
  stream >> structure->_experimentalMeasurementIndexLimitsLmax;
  stream >> structure->_experimentalMeasurementNumberOfSymmetryIndependentReflections;
  stream >> structure->_experimentalMeasurementSoftware;
  stream >> structure->_experimentalMeasurementRefinementDetails;
  stream >> structure->_experimentalMeasurementGoodnessOfFit;
  stream >> structure->_experimentalMeasurementRFactorGt;
  stream >> structure->_experimentalMeasurementRFactorAll;

  stream >> structure->_chemicalFormulaMoiety;
  stream >> structure->_chemicalFormulaSum;
  stream >> structure->_chemicalNameSystematic;
  stream >> structure->_cellFormulaUnitsZ;

  stream >> structure->_citationArticleTitle;
  stream >> structure->_citationJournalTitle;
  stream >> structure->_citationAuthors;
  stream >> structure->_citationJournalVolume;
  stream >> structure->_citationJournalNumber;
  stream >> structure->_citationJournalPageNumbers;
  stream >> structure->_citationDOI;
  stream >> day;
  stream >> month;
  stream >> year;
  structure->_citationPublicationDate = QDate(int(year),int(month),int(day));
  stream >> structure->_citationDatebaseCodes;

  return stream;
}
