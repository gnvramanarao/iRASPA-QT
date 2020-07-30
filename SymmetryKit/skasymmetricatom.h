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
#include <vector>
#include <unordered_set>
#include <mathkit.h>
#include <type_traits>
#include <foundationkit.h>
#include "skbond.h"
#include "skatomcopy.h"

class SKAsymmetricAtom
{
public:
    SKAsymmetricAtom();
    SKAsymmetricAtom(QString displayName, int elementIdentifier);
    virtual ~SKAsymmetricAtom();
    enum class SKAsymmetricAtomType: qint64
    {
      container = 0, asymmetric = 1
    };

    enum class Hybridization: qint64
    {
      untyped = 0, sp_linear = 1, sp2_trigonal = 2, sp3_tetrahedral = 3, square_planar = 4, trigonal_bipyramidal = 5, square_pyramidal = 6, octahedral = 7
    };

    qint64 asymmetricIndex() {return _asymmetricIndex;}
    void setAsymmetricIndex(qint64 value) {_asymmetricIndex = value;}


    QString displayName() const  {return _displayName;}
    void setDisplayName(QString newValue)  {_displayName = newValue;}
    double3 position() const  {return _position;}
    void setPosition(double3 newValue)  {_position = newValue;}
    double charge() const  {return _charge;}
    void setCharge(double newValue)  {_charge = newValue;}

    qint64 tag() {return _tag;}
    void setTag(qint64 tag) {_tag = tag;}
    bool isVisible() {return _isVisible;}
    void setVisibility(bool visibility) {_isVisible = visibility;}

    QColor color() {return _color;}
    void setColor(QColor color) {_color = color;}
    double drawRadius() {return _drawRadius;}
    void setDrawRadius(double radius) {_drawRadius = radius;}

    double2 potentialParameters() {return _potentialParameters;}
    void setPotentialParameters(double2 value) {_potentialParameters = value;}

    QString uniqueForceFieldName() const  {return _uniqueForceFieldName;}
    void setUniqueForceFieldName(QString newValue)  {_uniqueForceFieldName = newValue;}
    qint64 elementIdentifier() const  {return _elementIdentifier;}
    void setElementIdentifier(qint64 newValue)  {_elementIdentifier = newValue;}

    double bondDistanceCriteria() const {return _bondDistanceCriteria;}
    void setBondDistanceCriteria(double bondDistanceCriteria) {_bondDistanceCriteria = bondDistanceCriteria;}

    bool3 isFixed() const  {return _isFixed;}
    void setIsFixed(bool3 newValue)  {_isFixed = newValue;}
    qint64 serialNumber() const  {return _serialNumber;}
    void setSerialNumber(qint64 newValue)  {_serialNumber = newValue;}
    QChar remotenessIndicator() const  {return _remotenessIndicator;}
    void setRemotenessIndicator(char newValue)  {_remotenessIndicator = newValue;}
    QChar branchDesignator() const  {return _branchDesignator;}
    void setBranchDesignator(char newValue)  {_branchDesignator = newValue;}
    QChar alternateLocationIndicator() const  {return _alternateLocationIndicator;}
    void setAlternateLocationIndicator(char newValue)  {_alternateLocationIndicator = newValue;}
    QString residueName() const  {return _residueName;}
    void setResidueName(QString newValue)  {_residueName = newValue;}
    QChar chainIdentifier() const  {return _chainIdentifier;}
    void setChainIdentifier(char newValue)  {_chainIdentifier = newValue;}
    qint64 residueSequenceNumber() const  {return _residueSequenceNumber;}
    void setResidueSequenceNumber(qint64 newValue)  {_residueSequenceNumber = newValue;}
    QChar codeForInsertionOfResidues() const  {return _codeForInsertionOfResidues;}
    void setCodeForInsertionOfResidues(char newValue)  {_codeForInsertionOfResidues = newValue;}
    double occupancy() const  {return _occupancy;}
    void setOccupancy(double newValue)  {_occupancy = newValue;}
    double temperaturefactor() const  {return _temperaturefactor;}
    void setTemperaturefactor(double newValue)  {_temperaturefactor = newValue;}
    QString segmentIdentifier() const  {return _segmentIdentifier;}
    void setSegmentIdentifier(QString newValue)  {_segmentIdentifier = newValue;}
    qint64 asymetricID() const  {return _asymetricID;}
    //void setAsymetricID(int newValue)  {_asymetricID = newValue;}

    bool ligandAtom() const  {return _ligandAtom;}
    void setLigandAtom(bool newValue)  {_ligandAtom = newValue;}
    bool backBoneAtom() const  {return _backBoneAtom;}
    void backBoneAtom(bool newValue)  {_backBoneAtom = newValue;}
    bool fractional() const  {return _fractional;}
    void fractional(bool newValue)  {_fractional = newValue;}
    bool solvent() const  {return _solvent;}
    void setSolvent(bool newValue)  {_solvent = newValue;}

    std::vector<std::shared_ptr<SKAtomCopy>>& copies()  {return _copies;}
    void setCopies(std::vector<std::shared_ptr<SKAtomCopy>> copies) {_copies = copies;}
private:
    qint64 _versionNumber{2};
    qint64 _asymmetricIndex;
    QString _displayName = QString("Default");
    double3 _position = double3(0,0,0);
    double _charge = 0;

    QString _uniqueForceFieldName;
    qint64 _elementIdentifier = 0;
    QColor _color = QColor(0,255,0,255);
    double _drawRadius = 1.0;
    double _bondDistanceCriteria = 1.0;
    double2 _potentialParameters = double2(0.0,0.0);

    qint64 _tag = 0;
    SKAsymmetricAtomType _symmetryType = SKAsymmetricAtomType::asymmetric;
    Hybridization _hybridization = Hybridization::untyped;

    // atom properties (bonds are visible depending on whether the atoms of the bonds are visible)
    bool3 _isFixed = bool3(false, false, false);
    bool _isVisible = true;
    bool _isVisibleEnabled = true;

    qint64 _serialNumber = 0;
    QChar _remotenessIndicator = ' ';         // character 'A','B','C','D',...
    QChar _branchDesignator = ' ';            // character '1','2','3',...
    qint64 _asymetricID = 0;                    // positive integer
    QChar _alternateLocationIndicator = ' ';  // character ' ' or 'A','B',...
    QString _residueName = QString("");           // empty or 3 characters
    QChar _chainIdentifier = ' ';             // empty or 'A','B','C',...
    qint64 _residueSequenceNumber = 0;          // positive integer
    QChar _codeForInsertionOfResidues = ' ';  // empty or 'A','B','C',...
    double _occupancy = 1.0;
    double _temperaturefactor = 0.0;
    QString _segmentIdentifier = QString("");     // empty or 4 characters

    bool _ligandAtom = false;
    bool _backBoneAtom = false;
    bool _fractional = false;
    bool _solvent = false;

    double3 _displacement = double3();

    // the crystallographic copies of the atom
    std::vector<std::shared_ptr<SKAtomCopy>> _copies;

    friend QDataStream &operator<<(QDataStream &, const std::shared_ptr<SKAsymmetricAtom> &);
    friend QDataStream &operator>>(QDataStream &, std::shared_ptr<SKAsymmetricAtom> &);
};


