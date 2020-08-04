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

#pragma  once

#include <symmetrykit.h>
#include "structure.h"
#include "iraspakitprotocols.h"

class CrystalPolygonalPrismPrimitive: public Structure, public RKRenderCrystalPrimitivePolygonalPrimsObjectsSource
{
public:
  CrystalPolygonalPrismPrimitive();
  CrystalPolygonalPrismPrimitive(const CrystalPolygonalPrismPrimitive &crystalPolygonalPrismPrimitive);
  CrystalPolygonalPrismPrimitive(std::shared_ptr<Structure> s);

  bool hasSymmetry() override final {return true;}
  std::shared_ptr<Structure> superCell() const override final;

  iRASPAStructureType structureType() override final { return iRASPAStructureType::crystalPolygonalPrismPrimitive; }

  std::vector<RKInPerInstanceAttributesAtoms> renderCrystalPrimitivePolygonalPrismObjects() const override;

  SKBoundingBox boundingBox() const override final;
  void expandSymmetry() override final;
  std::optional<std::pair<std::shared_ptr<SKCell>, double3>> cellForFractionalPositions() override final;
  std::optional<std::pair<std::shared_ptr<SKCell>, double3>> cellForCartesianPositions() override final;
  std::vector<std::shared_ptr<SKAsymmetricAtom>> asymmetricAtomsCopiedAndTransformedToCartesianPositions() override final;
  std::vector<std::shared_ptr<SKAsymmetricAtom>> asymmetricAtomsCopiedAndTransformedToFractionalPositions() override final;
  std::vector<std::shared_ptr<SKAsymmetricAtom>> atomsCopiedAndTransformedToCartesianPositions() override final;
  std::vector<std::shared_ptr<SKAsymmetricAtom>> atomsCopiedAndTransformedToFractionalPositions() override final;

  simd_quatd primitiveOrientation() const override {return _primitiveOrientation;}
  double3x3 primitiveTransformationMatrix() const override  {return _primitiveTransformationMatrix;}
  double primitiveOpacity() const override  {return _primitiveOpacity;}
  bool primitiveIsCapped() const override  {return _primitiveIsCapped;}
  bool primitiveIsFractional() const override  {return _primitiveIsFractional;}
  int primitiveNumberOfSides() const override  {return _primitiveNumberOfSides;}
  double primitiveThickness() const override  {return _primitiveThickness;}

  bool primitiveFrontSideHDR() const override  {return _primitiveFrontSideHDR;}
  double primitiveFrontSideHDRExposure() const override  {return _primitiveFrontSideHDRExposure;}
  QColor primitiveFrontSideAmbientColor() const override  {return _primitiveFrontSideAmbientColor;}
  QColor primitiveFrontSideDiffuseColor() const override  {return _primitiveFrontSideDiffuseColor;}
  QColor primitiveFrontSideSpecularColor() const override  {return _primitiveFrontSideSpecularColor;}
  double primitiveFrontSideAmbientIntensity() const override  {return _primitiveFrontSideAmbientIntensity;}
  double primitiveFrontSideDiffuseIntensity() const override  {return _primitiveFrontSideDiffuseIntensity;}
  double primitiveFrontSideSpecularIntensity() const override  {return _primitiveFrontSideSpecularIntensity;}
  double primitiveFrontSideShininess() const override  {return _primitiveFrontSideShininess;}

  bool primitiveBackSideHDR() const override  {return _primitiveBackSideHDR;}
  double primitiveBackSideHDRExposure() const override  {return _primitiveBackSideHDRExposure;}
  QColor primitiveBackSideAmbientColor() const override  {return _primitiveBackSideAmbientColor;}
  QColor primitiveBackSideDiffuseColor() const override  {return _primitiveBackSideDiffuseColor;}
  QColor primitiveBackSideSpecularColor() const override  {return _primitiveBackSideSpecularColor;}
  double primitiveBackSideAmbientIntensity() const override  {return _primitiveBackSideAmbientIntensity;}
  double primitiveBackSideDiffuseIntensity() const override  {return _primitiveBackSideDiffuseIntensity;}
  double primitiveBackSideSpecularIntensity() const override  {return _primitiveBackSideSpecularIntensity;}
  double primitiveBackSideShininess() const override  {return _primitiveBackSideShininess;}
private:
  qint64 _versionNumber{1};
  friend QDataStream &operator<<(QDataStream &, const std::shared_ptr<CrystalPolygonalPrismPrimitive> &);
  friend QDataStream &operator>>(QDataStream &, std::shared_ptr<CrystalPolygonalPrismPrimitive> &);
};

