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

#include "trackball.h"

TrackBall::TrackBall()
{

}

void TrackBall::start(double x, double y, double originX, double originY, double width, double height)
{
  double xxyy = 0;
  double nx = 0;
  double ny = 0;

  /* Start up the trackball.  The trackball works by pretending that a ball
     encloses the 3D view.  You roll this pretend ball with the mouse.  For
     example, if you click on the center of the ball and move the mouse straight
     to the right, you roll the ball around its Y-axis.  This produces a Y-axis
     rotation.  You can click on the "edge" of the ball and roll it around
     in a circle to get a Z-axis rotation.

     The math behind the trackball is simple: start with a vector from the first
     mouse-click on the ball to the center of the 3D view.  At the same time, set the radius
     of the ball to be the smaller dimension of the 3D view.  As you drag the mouse
     around in the 3D view, a second vector is computed from the surface of the ball
     to the center.  The axis of rotation is the cross product of these two vectors,
     and the angle of rotation is the angle between the two vectors.
   */

  nx = width;
  ny = height;
  if (nx > ny)
  {
    _gRadiusTrackball = ny * 0.5;
  }
  else
  {
    _gRadiusTrackball = nx * 0.5;
  }

  // Figure the center of the view.
  _gXCenterTrackball = originX + width * 0.5;
  _gYCenterTrackball = originY + height * 0.5;

  // Compute the starting vector from the surface of the ball to its center.
  _gStartPtTrackball.x = x - _gXCenterTrackball;
  _gStartPtTrackball.y = _gYCenterTrackball - y;
  xxyy = _gStartPtTrackball.x * _gStartPtTrackball.x + _gStartPtTrackball.y * _gStartPtTrackball.y;
  if (xxyy > _gRadiusTrackball * _gRadiusTrackball)
  {
    // Outside the sphere.
    _gStartPtTrackball.z = 0.0;
  }
  else
  {
    _gStartPtTrackball.z = sqrt (_gRadiusTrackball * _gRadiusTrackball - xxyy);
  }
}

simd_quatd TrackBall::rollToTrackball(double x, double y)
{
  double3 rot = double3();
  double xxyy;
  double cosAng;
  double sinAng;
  double ls, le;
  double rotationAngle;

  _gEndPtTrackball.x = x - _gXCenterTrackball;
  _gEndPtTrackball.y = _gYCenterTrackball - y;
  _gEndPtTrackball.z = _gYCenterTrackball - y;

  if (fabs(_gEndPtTrackball.x - _gStartPtTrackball.x) < _kTol && fabs(_gEndPtTrackball.y - _gStartPtTrackball.y) < _kTol)
  {
    return simd_quatd(1.0,double3(0.0, 0.0, 0.0));
  }

  // Compute the ending vector from the surface of the ball to its center.
  xxyy = _gEndPtTrackball.x * _gEndPtTrackball.x + _gEndPtTrackball.y * _gEndPtTrackball.y;
  if (xxyy > _gRadiusTrackball * _gRadiusTrackball)
  {
    // Outside the sphere.
    _gEndPtTrackball.z = 0.0;
  }
  else
  {
    _gEndPtTrackball.z = sqrt(_gRadiusTrackball * _gRadiusTrackball - xxyy);
  }

  // Take the cross product of the two vectors. r = s X e
  rot=double3(_gStartPtTrackball.y * _gEndPtTrackball.z - _gStartPtTrackball.z * _gEndPtTrackball.y,
              _gStartPtTrackball.z * _gEndPtTrackball.x - _gStartPtTrackball.x * _gEndPtTrackball.z,
              _gStartPtTrackball.x * _gEndPtTrackball.y - _gStartPtTrackball.y * _gEndPtTrackball.x);

   // Use atan for a better angle.  If you use only cos or sin, you only get
   // half the possible angles, and you can end up with rotations that flip around near
   // the poles.

   // cos(a) = (s . e) / (||s|| ||e||)
   cosAng = _gStartPtTrackball.x * _gEndPtTrackball.x + _gStartPtTrackball.y * _gEndPtTrackball.y + _gStartPtTrackball.z * _gEndPtTrackball.z; // (s . e)
   ls = sqrt(_gStartPtTrackball.x * _gStartPtTrackball.x + _gStartPtTrackball.y * _gStartPtTrackball.y + _gStartPtTrackball.z * _gStartPtTrackball.z);
   ls = 1.0 / ls; // 1 / ||s||
   le = sqrt(_gEndPtTrackball.x * _gEndPtTrackball.x + _gEndPtTrackball.y * _gEndPtTrackball.y + _gEndPtTrackball.z * _gEndPtTrackball.z);
   le = 1.0 / le; // 1 / ||e||
   cosAng = cosAng * ls * le;

   // sin(a) = ||(s X e)|| / (||s|| ||e||)
   sinAng = sqrt(rot.x * rot.x + rot.y * rot.y + rot.z * rot.z); // ||(s X e)||;

   // keep this length in lr for normalizing the rotation vector later.
   sinAng = sinAng * ls * le;
   rotationAngle = atan2(sinAng, cosAng);

   double s = sin(rotationAngle/2.0);
   // Return normalize the rotation axis.
   return simd_quatd(cos(rotationAngle/2.0), s * double3::normalize(rot));
}
