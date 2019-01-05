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

#include "skboundingbox.h"
#include <mathkit.h>


SKBoundingBox::SKBoundingBox()
{

}

SKBoundingBox::SKBoundingBox(double3 minimum, double3 maximum)
{
  this->_minimum = minimum;
  this->_maximum = maximum;
}

SKBoundingBox::SKBoundingBox(const double3 center, const double3 width, const double scale)
{
  this->_minimum = center - (0.5 * scale) * width;
  this->_maximum = center + (0.5 * scale) * width;
}



double3 SKBoundingBox::widths()
{
  return double3(_maximum.x-_minimum.x, _maximum.y-_minimum.y, _maximum.z-_minimum.z);
}

std::array<double3,8> SKBoundingBox::corners()
{
   std::array<double3,8> temp = {{
      double3(_minimum.x, _minimum.y, _minimum.z),
      double3(_maximum.x, _minimum.y, _minimum.z),
      double3(_maximum.x, _maximum.y, _minimum.z),
      double3(_minimum.x, _maximum.y, _minimum.z),
      double3(_minimum.x, _minimum.y, _maximum.z),
      double3(_maximum.x, _minimum.y, _maximum.z),
      double3(_maximum.x, _maximum.y, _maximum.z),
      double3(_minimum.x, _maximum.y, _maximum.z)
   }};
   return temp;
}

double3 SKBoundingBox::center()
{
  return _minimum + (_maximum - _minimum) * 0.5;
}


double SKBoundingBox::boundingSphereRadius()
{
  std::array<double3,8> corners = {{
     double3(_minimum.x, _minimum.y, _minimum.z),
     double3(_maximum.x, _minimum.y, _minimum.z),
     double3(_maximum.x, _maximum.y, _minimum.z),
     double3(_minimum.x, _maximum.y, _minimum.z),
     double3(_minimum.x, _minimum.y, _maximum.z),
     double3(_maximum.x, _minimum.y, _maximum.z),
     double3(_maximum.x, _maximum.y, _maximum.z),
     double3(_minimum.x, _maximum.y, _maximum.z)
  }};

  double3 centerOfScene  = _minimum + (_maximum - _minimum) * 0.5;
  double radius = 0.0;
  for(double3 corner : corners)
  {
    double3 distanceVector = centerOfScene-corner;
    double cornerRadius = distanceVector.length();
    if (cornerRadius > radius)
    {
      radius = cornerRadius;
    }
  }
  return radius;
}

SKBoundingBox SKBoundingBox::adjustForTransformation(double4x4 transformation)
{
  double3 centerOfScene  = _minimum + (_maximum - _minimum) * 0.5;
  double3 min = double3();
  double3 max = double3();

  if (transformation[0][0] > 0.0)
  {
    min.x += transformation[0][0] * (this->_minimum.x - centerOfScene.x);
    max.x += transformation[0][0] * (this->_maximum.x - centerOfScene.x);
  }
  else
  {
    min.x += transformation[0][0] * (this->_maximum.x - centerOfScene.x);
    max.x += transformation[0][0] * (this->_minimum.x - centerOfScene.x);
  }

  if (transformation[0][1] > 0.0)
  {
    min.y += transformation[0][1] * (this->_minimum.x - centerOfScene.x);
    max.y += transformation[0][1] * (this->_maximum.x - centerOfScene.x);
  }
  else
  {
    min.y += transformation[0][1] * (this->_maximum.x - centerOfScene.x);
    max.y += transformation[0][1] * (this->_minimum.x - centerOfScene.x);
  }

  if (transformation[0][2] > 0.0)
  {
    min.z += transformation[0][2] * (this->_minimum.x - centerOfScene.x);
    max.z += transformation[0][2] * (this->_maximum.x - centerOfScene.x);
  }
  else
  {
    min.z += transformation[0][2] * (this->_maximum.x - centerOfScene.x);
    max.z += transformation[0][2] * (this->_minimum.x - centerOfScene.x);
  }

  if (transformation[1][0] > 0.0)
  {
    min.x += transformation[1][0] * (this->_minimum.y - centerOfScene.y);
    max.x += transformation[1][0] * (this->_maximum.y - centerOfScene.y);
  }
  else
  {
    min.x += transformation[1][0] * (this->_maximum.y - centerOfScene.y);
    max.x += transformation[1][0] * (this->_minimum.y - centerOfScene.y);
  }

  if (transformation[1][1] > 0.0)
  {
    min.y += transformation[1][1] * (this->_minimum.y - centerOfScene.y);
    max.y += transformation[1][1] * (this->_maximum.y - centerOfScene.y);
  }
  else
  {
    min.y += transformation[1][1] * (this->_maximum.y - centerOfScene.y);
    max.y += transformation[1][1] * (this->_minimum.y - centerOfScene.y);
  }

  if (transformation[1][2] > 0.0)
  {
    min.z += transformation[1][2] * (this->_minimum.y - centerOfScene.y);
    max.z += transformation[1][2] * (this->_maximum.y - centerOfScene.y);
  }
  else
  {
    min.z += transformation[1][2] * (this->_maximum.y - centerOfScene.y);
    max.z += transformation[1][2] * (this->_minimum.y - centerOfScene.y);
  }

  if (transformation[2][0] > 0.0)
  {
    min.x += transformation[2][0] * (this->_minimum.z - centerOfScene.z);
    max.x += transformation[2][0] * (this->_maximum.z - centerOfScene.z);
  }
  else
  {
    min.x += transformation[2][0] * (this->_maximum.z - centerOfScene.z);
    max.x += transformation[2][0] * (this->_minimum.z - centerOfScene.z);
  }

  if (transformation[2][1] > 0.0)
  {
    min.y += transformation[2][1] * (this->_minimum.z - centerOfScene.z);
    max.y += transformation[2][1] * (this->_maximum.z - centerOfScene.z);
  }
  else
  {
    min.y += transformation[2][1] * (this->_maximum.z - centerOfScene.z);
    max.y += transformation[2][1] * (this->_minimum.z - centerOfScene.z);
  }

  if (transformation[2][2] > 0.0)
  {
    min.z += transformation[2][2] * (this->_minimum.z - centerOfScene.z);
    max.z += transformation[2][2] * (this->_maximum.z - centerOfScene.z);
  }
  else
  {
    min.z += transformation[2][2] * (this->_maximum.z - centerOfScene.z);
    max.z += transformation[2][2] * (this->_minimum.z - centerOfScene.z);
  }
  return SKBoundingBox(min + centerOfScene, max + centerOfScene);
}

SKBoundingBox operator+(const SKBoundingBox left, double3 right)
{
  return SKBoundingBox(left._minimum + right, left._maximum + right);
}

SKBoundingBox operator-(const SKBoundingBox left, double3 right)
{
  return SKBoundingBox(left._minimum - right, left._maximum - right);
}

QDataStream &operator<<(QDataStream &stream, const SKBoundingBox &box)
{
  stream << box._versionNumber;
  stream << box._minimum;
  stream << box._maximum;
  return stream;
}

QDataStream &operator>>(QDataStream &stream, SKBoundingBox &box)
{
  qint64 versionNumber;
  stream >> versionNumber;
  if(versionNumber > box._versionNumber)
  {
    throw InvalidArchiveVersionException(__FILE__, __LINE__, "SKBoundingBox");
  }
  stream >> box._minimum;
  stream >> box._maximum;
  return stream;
}
