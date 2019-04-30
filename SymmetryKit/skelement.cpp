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

#include "skelement.h"

SKElement::SKElement()
{

}

SKElement::SKElement(QString string, qint64 atomicNumber, qint64 group, qint64 period, QString name, double mass, double atomRadius, double covalentRadius, double singleBondCovalentRadius,
          double doubleBondCovalentRadius, double tripleBondCovalentRadius, double vDWRadius, std::vector<int> possibleOxidationStates):
          _chemicalSymbol(string), _atomicNumber(atomicNumber), _group(group), _period(period), _name(name), _mass(mass), _atomRadius(atomRadius), _covalentRadius(covalentRadius),
          _singleBondCovalentRadius(singleBondCovalentRadius), _doubleBondCovalentRadius(doubleBondCovalentRadius), _tripleBondCovalentRadius(tripleBondCovalentRadius),
          _VDWRadius(vDWRadius), _possibleOxidationStates(possibleOxidationStates)
{

}

std::vector<SKElement> PredefinedElements::predefinedElements =
{
    SKElement(QString(""), 0, 0, 0, QString(""), 0.0,0.0, 0.0, 0.0, 0.0, 0.0, 0.0,std::vector<int>{{}}),
    SKElement(QString("H"), 1, 1, 1, QString("Hydrogen"), 1.00794, 0.53, 0.32, 0.0, 0.0, 0.0, 1.2, std::vector<int>{{-1,1}}),
    SKElement(QString("He"), 2, 18, 1, QString("Helium"), 4.002602, 0.31, 0.28, 0.0, 0.0, 0.0, 1.4, std::vector<int>{{0}}),
    SKElement(QString("Li"), 3, 1, 2, QString("Lithium"), 6.9421, 1.67, 1.28, 0.0, 0.0, 0.0, 1.82, std::vector<int>{{1}}),
    SKElement(QString("Be"), 4, 2, 3, QString("Beryllium"), 9.012182, 1.12, 0.96, 0.0, 0.0, 0.85, 1.86, std::vector<int>{{1,2}}),
    SKElement(QString("B"), 5, 13, 2, QString("Boron"), 10.881, 0.90, 0.84, 0.0, 0.0, 0.73, 1.92, std::vector<int>{{-5,-1,1,2,3}}),
    SKElement(QString("C"), 6, 14, 2, QString("Carbon"), 12.0107, 0.67, 0.77, 0.0, 0.0, 0.69, 1.7, std::vector<int>{{-4,-3,-2,-1,0,1,2,3,4}}),
    SKElement(QString("N"), 7, 15, 2, QString("Nitrogen"), 14.0067, 0.56, 0.71, 0.0, 0.0, 0.54, 1.55, std::vector<int>{{-3,-2,-1,1,2,3,4,5}}),
    SKElement(QString("O"), 8, 16, 2, QString("Oxygen"), 15.9994, 0.48, 0.66, 0.0, 0.0, 0.53, 1.52, std::vector<int>{{-2,1,1,2}}),
    SKElement(QString("F"), 9, 17, 2, QString("Fluorine"), 18.9984032, 0.42, 0.64, 0.0, 0.0, 0.53, 1.35, std::vector<int>{{-1}}),
    SKElement(QString("Ne"), 10, 18, 2, QString("Neon"), 20.1797, 0.38, 0.58, 0.0, 0.0, 0.0, 1.54, std::vector<int>{{0}}),
    SKElement(QString("Na"), 11, 1, 3, QString("Sodium"), 22.98976928, 1.86, 1.66, 0.0, 0.0, 0.0, 2.27, std::vector<int>{{-1,1}}),
    SKElement(QString("Mg"), 12, 2, 3, QString("Magnesium"), 24.305, 1.60, 1.41, 0.0, 0.0, 1.27, 1.73, std::vector<int>{{1,2}}),
    SKElement(QString("AL"), 13, 13, 3, QString("Aluminum"), 26.9815386, 1.43, 1.21, 0.0, 0.0, 1.11, 1.84, std::vector<int>{{-2,-1,1,2,3}}),
    SKElement(QString("Si"), 14, 14, 3, QString("Silicon"), 28.0855, 1.11, 1.11, 0.0, 0.0, 1.02, 2.1, std::vector<int>{{-4,-3,-2,-1,1,2,3,4}}),
    SKElement(QString("P"), 15, 15, 3, QString("Phosphorus"), 30.973762, 0.98, 1.07, 0.0, 0.0, 0.94, 1.8, std::vector<int>{{-3,-2,-1,1,2,3,4,5}}),
    SKElement(QString("S"), 16, 16, 3, QString("Sulfur"), 32.065, 0.88, 1.05, 0.0, 0.0, 0.95, 1.8, std::vector<int>{{-2,-1,1,2,3,4,5,6}}),
    SKElement(QString("Cl") ,17, 17, 3, QString("Chlorine"), 35.453, 0.79, 1.02, 0.0, 0.0, 0.93, 1.75, std::vector<int>{{-1,1,2,3,4,5,6,7}}),
    SKElement(QString("Ar") ,18, 18, 3, QString("Argon"), 39.948, 0.71, 1.06, 0.0, 0.0, 0.96, 1.88, std::vector<int>{{0}}),
    SKElement(QString("K") ,19, 1, 4, QString("Potassium"), 39.0983, 2.27, 2.03, 0.0, 0.0, 0.0, 2.75, std::vector<int>{{-1,1}}),
    SKElement(QString("Ca"), 20, 2, 4, QString("Calcium"), 40.078, 1.97, 1.76, 0.0, 0.0, 1.33, 2.31, std::vector<int>{{1,2}}),
    SKElement(QString("Sc"), 21, 3, 4, QString("Scandium"), 44.955912, 1.62, 1.7, 0.0, 0.0, 1.14, 2.11, std::vector<int>{{1,2,3}}),
    SKElement(QString("Ti"), 22, 4, 4, QString("Titanium"), 47.867, 1.47, 1.6, 0.0, 0.0, 1.08, 2.14, std::vector<int>{{-2,-1,1,2,3,4}}),
    SKElement(QString("V"), 23, 5, 4, QString("Vanadium"), 50.9415, 1.34, 1.53, 0.0, 0.0, 1.06, 2.12, std::vector<int>{{-3,-1,1,2,3,4,5}}),
    SKElement(QString("Cr"), 24, 6, 4, QString("Chromium"), 51.9961, 1.28, 1.39, 0.0, 0.0, 1.03, 1.97, std::vector<int>{{-4,-2,-1,1,2,3,4,5,6}}),
    SKElement(QString("Mn"), 25, 7, 4, QString("Manganese"), 54.939045, 1.27, 1.39, 0.0, 0.0, 1.03, 1.96, std::vector<int>{{-3,-2,1,1,2,3,4,5,6,7}}),
    SKElement(QString("Fe"), 26, 8, 4, QString("Iron"), 55.845, 1.26, 1.32, 0.0, 0.0, 1.02, 1.96, std::vector<int>{{-4,-2,-1,1,2,3,4,5,6,7}}),
    SKElement(QString("Co"), 27, 9, 4, QString("Cobalt"), 58.933195, 1.25, 1.26, 0.0, 0.0, 0.96, 1.95, std::vector<int>{{-3,-1,1,2,3,4,5}}),
    SKElement(QString("Ni"), 28, 10, 4, QString("NickeQString("), 58.6934 , 1.24, 1.24, 0.0, 0.0, 1.01, 1.63, std::vector<int>{{-2,-1,1,2,3,4}}),
    SKElement(QString("Cu"), 29, 11, 4, QString("Copper"), 63.546,1.28, 1.32, 0.0, 0.0, 1.2, 1.4, std::vector<int>{{-2,1,2,3,4}}),
    SKElement(QString("Zn"), 30, 12, 4, QString("Zinc"), 65.38, 1.34, 1.22, 0.0, 0.0, 0.0, 1.39, std::vector<int>{{-2,0,1,2}}),
    SKElement(QString("Ga"), 31, 13, 4, QString("Gallium"), 69.723, 1.35, 1.22, 0.0, 0.0, 1.21, 1.87, std::vector<int>{{-5,-4,-2,-1,1,2,3}}),
    SKElement(QString("Ge"), 32, 14, 4, QString("Germanium"), 72.64, 1.22, 1.22, 0.0, 0.0, 1.14, 2.11, std::vector<int>{{-4,-3,-2,-1,0,1,2,3,4}}),
    SKElement(QString("As"), 33, 15, 4, QString("arsenic"), 74.9216, 1.19, 1.19, 0.0, 0.0, 1.06, 1.85, std::vector<int>{{-3,-2,-1,1,2,3,4,5}}),
    SKElement(QString("Se"), 34, 16, 3, QString("Selenium"), 78.96, 1.20, 1.2, 0.0, 0.0, 1.07, 1.9, std::vector<int>{{-2,-1,1,2,3,4,5,6}}),
    SKElement(QString("Br"), 35, 17, 4, QString("Bromine"), 79.904, 1.20, 1.2, 0.0, 0.0, 1.1, 1.85, std::vector<int>{{-1,1,3,4,5,7}}),
    SKElement(QString("Kr"), 36, 18, 4, QString("Krypton"), 83.798, 0.88, 1.16, 0.0, 0.0, 1.08, 2.02, std::vector<int>{{0,1,2}}),
    SKElement(QString("Rb"), 37, 1, 5, QString("Rubidium"), 85.4678, 2.48, 2.2, 0.0, 0.0, 0.0, 3.03, std::vector<int>{{-1,1}}),
    SKElement(QString("Sr"), 38, 2, 5, QString("Strontium"), 87.62, 2.15, 1.95, 0.0, 0.0, 1.39, 2.49, std::vector<int>{{1,2}}),
    SKElement(QString("Y"), 39, 3, 5, QString("Yttrium"), 88.90585, 1.80, 1.9, 0.0, 0.0, 1.24, 2.45, std::vector<int>{{1,2,3}}),
    SKElement(QString("Zr"), 40, 4, 5, QString("Zirconium"), 91.224, 1.60, 1.75, 0.0, 0.0, 1.21, 0.0, std::vector<int>{{-2,1,2,3,4}}),
    SKElement(QString("Nb"), 41, 5, 5, QString("Niobium"), 92.90638, 1.46, 1.64, 0.0, 0.0, 1.16, 0.0, std::vector<int>{{-3,-1,1,2,3,4,5}}),
    SKElement(QString("Mo"), 42, 6, 5, QString("Molybdenum"), 95.96, 1.39, 1.54, 0.0, 0.0, 1.13, 2.06, std::vector<int>{{-4,-2,-1,1,2,3,4,5,6}}),
    SKElement(QString("Tc"), 43, 7, 5, QString("Technetium"), 98, 1.36, 1.47, 0.0, 0.0, 1.10, 0.0, std::vector<int>{{-3,-1,1,2,3,4,5,6,7}}),
    SKElement(QString("Ru"), 44, 8, 5, QString("Ruthenium"), 101.07, 1.34, 1.46, 0.0, 0.0, 1.03, 2.02, std::vector<int>{{-4,-2,1,2,3,4,5,6,7,8}}),
    SKElement(QString("Rh"), 45, 9, 5, QString("Rhodium"), 102.59055, 1.34, 1.42, 0.0, 0.0, 1.06, 2.02, std::vector<int>{{-3,-1,1,2,3,4,5,6}}),
    SKElement(QString("Pd"), 46, 10, 5, QString("Palladium"), 106.42, 1.37, 1.39, 0.0, 0.0, 1.12, 1.63, std::vector<int>{{0,1,2,3,4}}),
    SKElement(QString("Ag"), 47, 11, 5, QString("Silver"), 107.8682, 1.44, 1.45, 0.0, 0.0, 1.37, 1.72, std::vector<int>{{-2,-1,1,2,3}}),
    SKElement(QString("Cd"), 48, 12, 5, QString("Cadmium"), 112.411, 1.51, 1.44, 0.0, 0.0, 0.0, 1.58, std::vector<int>{{-2,1,2}}),
    SKElement(QString("In"), 49, 13, 5, QString("Indium"), 114.818, 1.67, 1.42, 0.0, 0.0, 1.46, 1.93, std::vector<int>{{-5,-2,-1,1,2,3}}),
    SKElement(QString("Sn"), 50, 14, 5, QString("Tin"), 118.71, 1.40, 1.39, 0.0, 0.0, 1.32, 2.17, std::vector<int>{{-4,-3,-2,-1,1,2,3,4}}),
    SKElement(QString("Sb"), 51, 15, 5, QString("Antimony"), 121.76, 1.40, 1.39, 0.0, 0.0, 1.27, 2.06, std::vector<int>{{-3,-2,-1,1,2,3,4,5}}),
    SKElement(QString("Te"), 52, 16, 5, QString("Tellurium"), 127.6, 1.40, 1.38, 0.0, 0.0, 1.21, 2.06, std::vector<int>{{-2,-1,1,2,3,4,5,6}}),
    SKElement(QString("I"), 53, 17, 5, QString("Iodine"), 126.90447, 1.40, 1.39, 0.0, 0.0, 1.25, 1.98, std::vector<int>{{-1,1,3,4,5,6,7}}),
    SKElement(QString("Xe"), 54, 18, 5, QString("Xenon"), 131.293, 1.08, 1.4, 0.0, 0.0, 1.22, 2.16, std::vector<int>{{0,1,2,4,6,8}}),
    SKElement(QString("Cs"), 55, 1, 6, QString("Cesium"), 132.9054519, 2.65, 2.44, 0.0, 0.0, 0.0, 3.43, std::vector<int>{{-1,1}}),
    SKElement(QString("Ba"), 56, 2, 6, QString("Barium"), 137.327, 2.22, 2.15, 0.0, 0.0, 1.49, 2.68, std::vector<int>{{1,2}}),
    SKElement(QString("La"), 57, -1, 6, QString("Lanthanum"), 138.90547, 1.87, 2.07, 0.0, 0.0, 1.39, 2.51, std::vector<int>{{1,2,3}}),
    SKElement(QString("Ce"), 58, -1, 6, QString("Cerium"), 140.116, 1.818, 2.04, 0.0, 0.0, 1.31, 0.0, std::vector<int>{{1,2,3,4}}),
    SKElement(QString("Pr"), 59, -1, 6, QString("Praseodymium"), 140.90765, 1.82, 2.03, 0.0, 0.0, 1.28,0.0, std::vector<int>{{2,3,4,5}}),
    SKElement(QString("Nd"), 60, -1, 6, QString("Neodymium"), 144.242, 1.81, 2.01, 0.0, 0.0, 0.0, 0.0, std::vector<int>{{2,3,4}}),
    SKElement(QString("Pm"), 61, -1, 6, QString("Promethium"), 145, 1.83, 1.99, 0.0, 0.0, 0.0, 0.0, std::vector<int>{{2,3}}),
    SKElement(QString("Sm"), 62, -1, 6, QString("Samarium"), 150.36, 1.80, 1.98, 0.0, 0.0, 0.0, 0.0, std::vector<int>{{1,2,3,4}}),
    SKElement(QString("Eu"), 63, -1, 6, QString("Europium"), 151.964, 1.80, 1.98, 0.0, 0.0, 0.0, 0.0, std::vector<int>{{1,2,3}}),
    SKElement(QString("Gd"), 64, -1, 6, QString("Gadolinium"), 157.25, 1.80, 1.96, 0.0, 0.0, 1.32, 0.0, std::vector<int>{{1,2,3}}),
    SKElement(QString("Tb"), 65, -1, 6, QString("Terbium"), 158.92535, 1.77, 1.94, 0.0, 0.0, 0.0, 0.0, std::vector<int>{{1,2,3,4}}),
    SKElement(QString("Dy"), 66, -1, 6, QString("Dysprodium"), 162.5, 1.78, 1.92, 0.0, 0.0, 0.0, 0.0, std::vector<int>{{1,2,3,4}}),
    SKElement(QString("Ho"), 67, -1, 6, QString("Holmium"), 164.93032, 1.76, 1.92, 0.0, 0.0, 0.0, 0.0, std::vector<int>{{1,2,3}}),
    SKElement(QString("Er"), 68, -1, 6, QString("Erbium"), 167.259, 1.76, 1.89, 0.0, 0.0, 0.0, 0.0, std::vector<int>{{1,2,3}}),
    SKElement(QString("Tm"), 69, -1, 6, QString("Thulium"), 168.93421, 1.76, 1.9, 0.0, 0.0, 0.0, 0.0, std::vector<int>{{2,3}}),
    SKElement(QString("Yb"), 70, -1, 6, QString("Ytterbium"), 173.054, 1.76, 1.87, 0.0, 0.0, 0.0, 0.0, std::vector<int>{{1,2,3}}),
    SKElement(QString("Lu"), 71, 3, 6, QString("Lutetium"), 174.9668, 1.74, 1.87, 0.0, 0.0, 1.31, 0.0, std::vector<int>{{1,2,3}}),
    SKElement(QString("Hf"), 72, 4, 6, QString("Hafnium"), 178.49, 1.59, 1.75, 0.0, 0.0, 1.22, 0.0, std::vector<int>{{-2,1,2,3,4}}),
    SKElement(QString("Ta"), 73, 5, 6, QString("Tantalum"), 180.94788, 1.46, 1.7, 0.0, 0.0, 1.19, 0.0, std::vector<int>{{-3,-1,1,2,3,4,5}}),
    SKElement(QString("W"), 74, 6, 6, QString("Tungsten"), 183.84, 1.39, 1.62, 0.0, 0.0, 1.15, 2.07, std::vector<int>{{-4,-2,-1,0,1,2,3,4,5,6}}),
    SKElement(QString("Re"), 75, 7, 6, QString("Rhenium"), 186.207, 1.37, 1.51, 0.0, 0.0, 1.1, 2.05, std::vector<int>{{-3,-1,0,1,2,3,4,5,6,7}}),
    SKElement(QString("Os"), 76, 8, 6, QString("Osmium"), 190.23, 1.35, 1.44, 0.0, 0.0, 1.09, 2.03, std::vector<int>{{-4,-2,0,1,2,3,4,5,6,7,8}}),
    SKElement(QString("Ir"), 77, 9, 6, QString("Iridium"), 192.217, 1.36, 1.41, 0.0, 0.0, 1.07, 2.03, std::vector<int>{{-3,-1,0,1,2,3,4,5,6,7,8,9}}),
    SKElement(QString("Pt"), 78, 10, 6, QString("Platinum"), 195.084, 1.39, 1.36, 0.0, 0.0, 1.1, 1.75, std::vector<int>{{-3,-2,-1,2,3,4,5,6}}),
    SKElement(QString("Au"), 79, 11, 6, QString("Gold"), 196.966569, 1.44, 1.36, 0.0, 0.0, 1.23, 1.66, std::vector<int>{{-3,-2,-1,1,2,3,5}}),
    SKElement(QString("Hg"), 80, 12, 6, QString("Mercury"), 200.59, 1.51, 1.32, 0.0, 0.0, 1.55, 0.0, std::vector<int>{{-2,1,2}}),
    SKElement(QString("Th"), 81, 13, 6, QString("Thallium"), 204.3833, 1.70, 1.45, 0.0, 0.0, 0.15, 1.96, std::vector<int>{{-5,-2,-1,1,2,3}}),
    SKElement(QString("Pb"), 82, 14, 6, QString("Lead"), 207.2, 1.75, 1.46, 0.0, 0.0, 1.37, 2.02, std::vector<int>{{-4,-2,-1,2,3,4}}),
    SKElement(QString("Bi"), 83, 15, 6, QString("Bismuth"), 208.9804, 1.56, 1.48, 0.0, 0.0, 1.35, 2.07, std::vector<int>{{-3,-2,-1,1,2,3,4,5}}),
    SKElement(QString("Po"), 84, 16, 6, QString("Polonium"), 210, 1.68, 1.4, 0.0, 0.0, 1.29, 1.97, std::vector<int>{{-2,2,4,5,6}}),
    SKElement(QString("At"), 85, 17, 6, QString("Astatine"), 210.8, 0.0, 1.5, 0.0, 0.0, 1.38, 2.02, std::vector<int>{{-1,1,3,5,7}}),
    SKElement(QString("Rn"), 86, 18, 6, QString("Radon"), 222, 1.2, 1.5, 0.0, 0.0, 1.33, 2.20, std::vector<int>{{0,2,6}}),
    SKElement(QString("Fr"), 87, 1, 7, QString("Francium"), 223, 0.0, 2.6, 0.0, 0.0, 0.0, 3.48, std::vector<int>{{1}}),
    SKElement(QString("Ra"), 88, 2, 7, QString("Radium"), 226, 0.0, 2.21, 0.0, 0.0, 1.59, 2.83, std::vector<int>{{2}}),
    SKElement(QString("Ac"), 89, -1, 7, QString("Actinium"), 227, 0.0, 2.15, 0.0, 0.0, 1.4, 0.0, std::vector<int>{{2,3}}),
    SKElement(QString("Th"), 90, -1, 7, QString("Thorium"), 232.03806, 1.798, 2.06, 0.0, 0.0, 1.36, 2.43, std::vector<int>{{1,2,3,4}}),
    SKElement(QString("Pa"), 91, -1, 7, QString("Protactinium"), 231.03588, 1.63, 2.0, 0.0, 0.0, 1.29, 0.0, std::vector<int>{{2,3,4,5}}),
    SKElement(QString("U"), 92, -1, 7, QString("Uranium"), 238.02891, 1.56, 1.96, 0.0, 0.0, 1.18, 1.86, std::vector<int>{{1,2,3,4,5,6}}),
    SKElement(QString("Np"), 93, -1, 7, QString("Neptunium"), 237, 1.55, 1.9, 0.0, 0.0, 1.16, 0.0, std::vector<int>{{2,3,4,5,6,7}}),
    SKElement(QString("Pu"), 94, -1, 7, QString("Plutonium"), 244, 1.59, 1.87, 0.0, 0.0, 0.0, 0.0, std::vector<int>{{1,2,3,4,5,6,7,8}}),
    SKElement(QString("Am"), 95, -1, 7, QString("Americium"), 243, 1.73, 1.8, 0.0, 0.0, 0.0, 0.0, std::vector<int>{{2,3,4,5,6,7,8}}),
    SKElement(QString("Cm"), 96, -1, 7, QString("Curium"), 247, 1.74, 1.69, 0.0, 0.0, 0.0, 0.0, std::vector<int>{{2,3,4,6}}),
    SKElement(QString("Bk"), 97, -1, 7, QString("Berkelium"), 247, 1.7, 0.0, 0.0, 0.0, 0.0, 0.0, std::vector<int>{{2,3,4}}),
    SKElement(QString("Cf"), 98, -1, 7, QString("Californium"), 251, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, std::vector<int>{{2,3,4}}),
    SKElement(QString("Es"), 99, -1, 7, QString("Einsteinium"), 252, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, std::vector<int>{{2,3,4}}),
    SKElement(QString("Fm"), 100, -1, 7, QString("Fermium"), 257, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, std::vector<int>{{2,3}}),
    SKElement(QString("Md"), 101, -1, 7, QString("Mendelevium"), 258, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, std::vector<int>{{2,3}}),
    SKElement(QString("No"), 102, -1, 7, QString("Nobelium"), 259, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, std::vector<int>{{2,3}}),
    SKElement(QString("Lr"), 103, 3, 7, QString("Lawrencium"), 262, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, std::vector<int>{{3}}),
    SKElement(QString("Rf"), 104, 4, 7, QString("Rutherfordium"), 261, 0.0, 0.0, 0.0, 0.0, 1.31, 0.0, std::vector<int>{{2,3,4}}),
    SKElement(QString("Db"), 105, 5, 7, QString("Dubnium"), 268, 1.39, 1.49, 0.0, 0.0, 1.26, 0.0, std::vector<int>{{3,4,5}}),
    SKElement(QString("Sg"), 106, 6, 7, QString("Seaborgium"), 269, 1.32, 1.43, 0.0, 0.0, 1.21, 0.0, std::vector<int>{{3,4,5,6}}),
    SKElement(QString("Bh"), 107, 7, 7, QString("Bohrium"), 270, 1.28, 1.41, 0.0, 0.0, 1.19, 0.0, std::vector<int>{{3,4,5,7}}),
    SKElement(QString("Hs"), 108, 8, 7, QString("Hassium"), 269, 1.26, 1.34, 0.0, 0.0, 1.18, 0.0, std::vector<int>{{2,3,4,5,6,8}}),
    SKElement(QString("Mt"), 109, 9, 7, QString("Meitnerium"), 278, 1.28, 1.29, 0.0, 0.0, 1.13, 0.0, std::vector<int>{{1,3,4,6,8,9}}),
    SKElement(QString("Ds"), 110, 10, 7, QString("Darmstadtium"), 281, 1.32, 1.28, 0.0, 0.0, 1.12, 0.0, std::vector<int>{{0,2,4,6,8}}),
    SKElement(QString("Rg"), 111, 11, 7, QString("Roentgenium"), 281, 1.38, 1.21, 0.0, 0.0, 1.18, 0.0, std::vector<int>{{-1,1,3,5}}),
    SKElement(QString("Cn"), 112, 12, 7, QString("Copernicium"), 285, 1.47, 1.22, 0.0, 0.0, 1.30, 0.0, std::vector<int>{{0,1,2}}),
    SKElement(QString("Nh"), 113, 13, 7, QString("Nihonium"), 286, 1.70, 1.76, 0.0, 0.0, 0.0, 0.0, std::vector<int>{{-1,1,3,5}}),
    SKElement(QString("Fl"), 114, 14, 7, QString("Flerovium"), 289, 1.80, 1.74, 0.0, 0.0, 0.0, 0.0, std::vector<int>{{0,1,2,4,6}}),
    SKElement(QString("Mc"), 115, 15, 7, QString("Moscovium"), 288, 1.87, 1.57, 0.0, 0.0, 0.0, 0.0, std::vector<int>{{1,3}}),
    SKElement(QString("Lv"), 116, 16, 7, QString("Livermorium"), 293, 1.83, 1.64, 0.0, 0.0, 0.0, 0.0, std::vector<int>{{-2,2,4}}),
    SKElement(QString("Ts"), 117, 17, 7, QString("Tennessine"), 294, 1.38, 1.56, 0.0, 0.0, 0.0, 0.0, std::vector<int>{{-1,1,3,5}}),
    SKElement(QString("Og"), 118, 18, 7, QString("Oganesson"), 294, 1.52, 1.57, 0.0, 0.0, 0.0, 0.0, std::vector<int>{{-1,0,1,2,4,6}})
};

std::map<QString, int> PredefinedElements::atomicNumberData =
{
    {QString("H"), 1}, {QString("He"), 2}, {QString("Li"), 3}, {QString("Be"), 4}, {QString("B"), 5}, {QString("C"), 6}, {QString("N"), 7}, {QString("O"), 8}, {QString("F"), 9},
    {QString("Ne"), 10}, {QString("Na"), 11}, {QString("Mg"), 12}, {QString("Al"), 13}, {QString("Si"), 14}, {QString("P"), 15}, {QString("S"), 16,}, {QString("Cl"), 17}, {QString("Ar"), 18}, {QString("K"), 19},
    {QString("Ca"), 20}, {QString("Sc"), 21}, {QString("Ti"), 22}, {QString("V"), 23}, {QString("Cr"), 24}, {QString("Mn"), 25}, {QString("Fe"), 26}, {QString("Co"), 27}, {QString("Ni"), 28}, {QString("Cu"), 29},
    {QString("Zn"), 30}, {QString("Ga"), 31}, {QString("Ge"), 32}, {QString("As"), 33}, {QString("Se"), 34,}, {QString("Br"), 35}, {QString("Kr"), 36}, {QString("Rb"), 37}, {QString("Sr"), 38}, {QString("Y"), 39},
    {QString("Zr"), 40}, {QString("Nb"), 41}, {QString("Mo"), 42}, {QString("Tc"), 43}, {QString("Ru"), 44}, {QString("Rh"), 45}, {QString("Pd"), 46}, {QString("Ag"), 47}, {QString("Cd"), 48}, {QString("In"), 49},
    {QString("Sn"), 50}, {QString("Sb"), 51}, {QString("Te"), 52}, {QString("I"), 53}, {QString("Xe"), 54}, {QString("Cs"), 55}, {QString("Ba"), 56}, {QString("La"), 57}, {QString("Ce"), 58}, {QString("Pr"), 59},
    {QString("Nd"), 60}, {QString("Pm"), 61}, {QString("Sm"), 62}, {QString("Eu"), 63}, {QString("Gd"), 64}, {QString("Tb"), 65}, {QString("Dy"), 66}, {QString("Ho"), 67}, {QString("Er"), 68}, {QString("Tm"), 69},
    {QString("Yb"), 70}, {QString("Lu"), 71}, {QString("Hf"), 72}, {QString("Ta"), 73}, {QString("W"), 74}, {QString("Re"), 75}, {QString("Os"), 76}, {QString("Ir"), 77}, {QString("Pt"), 78}, {QString("Au"), 79},
    {QString("Hg"), 80}, {QString("Th"), 81}, {QString("Pb"), 82}, {QString("Bi"), 83}, {QString("Po"), 84}, {QString("At"), 85}, {QString("Rn"), 86}, {QString("Fr"), 87}, {QString("Ra"), 88}, {QString("Ac"), 89},
    {QString("Th"), 90}, {QString("Pa"), 91}, {QString("U"), 92}, {QString("Np"), 93}, {QString("Pu"), 94}, {QString("Am"), 95}, {QString("Cm"), 96}, {QString("Bk"), 97}, {QString("Cf"), 98}, {QString("Es"), 99},
    {QString("Fm"), 100}, {QString("Md"), 101}, {QString("No"), 102}, {QString("Lr"), 103}, {QString("Rf"), 104}, {QString("Db"), 105}, {QString("Sg"), 106}, {QString("Bh"), 107}, {QString("Hs"), 108}, {QString("Mt"), 109},
    {QString("Ds"), 110}, {QString("Rg"), 111}, {QString("Cn"), 112}, {QString("Nh"), 113}, {QString("Fl("), 114}, {QString("Mc"), 115}, {QString("Lv"), 116}, {QString("Ts"), 117}, {QString("Og"), 118}
};
