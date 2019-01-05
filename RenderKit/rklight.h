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

#include <foundationkit.h>
#include <symmetrykit.h>
#include <mathkit.h>
#include <QColor>

class RKLight
{
public:
  RKLight();
  double4 position() const {return _position;}
  void setPosition(double4 position) {_position = position;}
  QColor ambientColor()  const {return _ambientColor;}
  void setAmbientColor(QColor color) {_ambientColor = color;}
  QColor diffuseColor()  const {return _diffuseColor;}
  void setDiffuseColor(QColor color) {_diffuseColor = color;}
  QColor specularColor()  const {return _specularColor;}
  void setSpecularColor(QColor color) {_specularColor = color;}
  double ambientIntensity()  const {return _ambientIntensity;}
  void setAmbientIntensity(double intensity) {_ambientIntensity = intensity;}
  double diffuseIntensity()  const {return _diffuseIntensity;}
  void setDiffuseIntensity(double intensity) {_diffuseIntensity = intensity;}
  double specularIntensity()  const {return _specularIntensity;}
  void setSpecularIntensity(double intensity) {_specularIntensity = intensity;}
  double shininess() const {return _shininess;}
  void setShininess(double shininess) {_shininess = shininess;}
  double constantAttenuation()  const {return _constantAttenuation;}
  void setConstantAttenuation(double attenuation) {_constantAttenuation = attenuation;}
  double linearAttenuation() const {return _linearAttenuation;}
  void setLinearAttenuation(double attenuation) {_linearAttenuation = attenuation;}
  double quadraticAttenuation() const {return _quadraticAttenuation;}
  void setQuadraticAttenuation(double attenuation) {_quadraticAttenuation = attenuation;}
  double3 spotDirection() const {return _spotDirection;}
  void setSpotDirection(double3 direction) {_spotDirection = direction;}
  double spotCutoff() const {return _spotCutoff;}
  void setSpotCutoff(double cutoff) {_spotCutoff = cutoff;}
  double spotExponent() const {return _spotExponent;}
  void setSpotExponent(double exponent) {_spotExponent = exponent;}
private:
  int _versionNumber = 1;
  double4 _position = double4(0, 0, 100.0, 0.0);
  QColor _ambientColor = QColor(255, 255, 255, 255);
  QColor _diffuseColor = QColor(255, 255, 255, 255);
  QColor _specularColor = QColor(255, 255, 255, 255);
  double _ambientIntensity = 1.0;
  double _diffuseIntensity = 1.0;
  double _specularIntensity = 1.0;
  double _shininess = 4.0;
  double _constantAttenuation = 1.0;
  double _linearAttenuation = 1.0;
  double _quadraticAttenuation = 1.0;
  double3 _spotDirection = double3(1.0, 1.0, 1.0);
  double _spotCutoff = 1.0;
  double _spotExponent = 1.0;
};
