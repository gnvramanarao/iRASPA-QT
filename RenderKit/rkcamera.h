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

#include <QRect>
#include <QSize>
#include <vector>
#include <math.h>
#include <cfloat>
#include <algorithm>
#include <symmetrykit.h>
#include <foundationkit.h>

enum class ResetDirectionType: qint64
{
  plus_X = 0, plus_Y = 1, plus_Z = 2, minus_X = 3, minus_Y = 4, minus_Z = 5
};

enum class FrustrumType: qint64
{
  perspective = 0, orthographic = 1
};


class RKCamera
{
public:
  RKCamera();
  SKBoundingBox cameraBoundingBox();
  double4x4 projectionMatrix();
  double3 position();
  simd_quatd referenceDirection();
  double3 EulerAngles();
  double4x4 modelMatrix();
  double4x4 modelViewMatrix();
  double3 myGluUnProject(double3 position, QRect viewPort);
  double3 myGluUnProject(double3 position, double4x4 modelMatrix, QRect viewPort);
  double3 myGluProject(double3 position, QRect viewPort);
  void pan(double panx, double pany);
  void increaseDistance(double deltaDistance);
  void updateFieldOfView();
  void updateCameraForWindowResize(double width, double height);

  void setCameraToOrthographic();
  bool isOrthographic() {return _frustrumType == FrustrumType::orthographic;}
  void setCameraToPerspective();
  bool isPerspective() {return _frustrumType == FrustrumType::perspective;}

  void resetForNewBoundingBox(SKBoundingBox box);
  void resetCameraToDirection();
  void resetCameraDistance();

  double resetFraction() {return _resetFraction;}
  void setResetFraction(double fraction) {_resetFraction = fraction;}
  ResetDirectionType resetCameraDirection() {return _resetDirectionType;}
  double3 centerOfScene() {return _centerOfScene;}
  double angleOfView() {return _angleOfView;}
  void setAngleOfView(double d);
  double rotationAngle() {return _rotationDelta;}
  void setRotationAngle(double angle) {_rotationDelta=angle;}

  double bloomLevel() const {return _bloomLevel;}
  void setBloomLevel(double level) {_bloomLevel = level;}
  double bloomPulse() const {return _bloomPulse;}
  void setBloomPulse(double pulse) {_bloomPulse = pulse;}

  void setBoundingBox(SKBoundingBox boundingBox);
  SKBoundingBox boundingBox();
  simd_quatd worldRotation() {return _worldRotation;}
  void setWorldRotation(simd_quatd rotation) {_worldRotation = rotation;}
  simd_quatd trackBallRotation() {return _trackBallRotation;}
  void setTrackBallRotation(simd_quatd trackBallRotation) {_trackBallRotation = trackBallRotation;}

  void setResetDirectionType(ResetDirectionType resetDirectionType) {_resetDirectionType = resetDirectionType;}

  static double4x4 GluLookAt(double3 eye, double3 center, double3 oldup);
  static double4x4 glFrustumfPerspective(double left, double right, double bottom, double top, double near, double far);
  static double4x4 glFrustumfPerspective(double fov, double aspectratio, double boundingBoxAspectRatio, double near, double far);
  static double4x4 glFrustumfOrthographic(double left, double right, double  bottom, double top, double near, double far);
  std::vector<int> selectPositionsInRectangle(std::vector<double3> &positions, QRect rect, double3 origin, QRect viewPortBounds);
private:
  qint64 _versionNumber{1};
  double _zNear=0.0;
  double _zFar=0.0;
  double _left = -10.0;
  double _right = 10.0;
  double _bottom = -10.0;
  double _top = 10.0;
  double _windowWidth = 100.0;
  double _windowHeight = 100.0;
  double _aspectRatio = 1.0;
  SKBoundingBox _boundingBox = SKBoundingBox();

  double _boundingBoxAspectRatio = 1.0;
  double3 _centerOfScene = double3(0,0,0);
  double3 _panning = double3(0.0,0.0,0.0);
  double3 _centerOfRotation = double3(0,0,0);
  double3 _eye = double3(0,0,0);
  double3 _distance = double3(0.0,0.0,50.0);
  double _orthoScale = 1.0;
  double _angleOfView = 60.0;
  FrustrumType _frustrumType = FrustrumType::orthographic;
  ResetDirectionType _resetDirectionType = ResetDirectionType::plus_Z;

  double _resetFraction = 0.85;
  bool _initialized = false;

  simd_quatd _worldRotation = simd_quatd(1.0,double3(0.0, 0.0, 0.0));
  simd_quatd _trackBallRotation = simd_quatd(1.0,double3(0.0, 0.0, 0.0));
  double _rotationDelta = 15.0;

  double _bloomLevel = 1.0;
  double _bloomPulse = 1.0;

  double4x4 _viewMatrix = double4x4();

  friend QDataStream &operator<<(QDataStream &, const std::shared_ptr<RKCamera> &);
  friend QDataStream &operator>>(QDataStream &, std::shared_ptr<RKCamera> &);
};
