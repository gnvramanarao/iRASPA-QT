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

#include <math.h>
#include "spheregeometry.h"

SphereGeometry::SphereGeometry(double r, int s)
{
  double theta, phi, sinTheta, sinPhi, cosTheta, cosPhi;
  float4 position, normal;

  _slices = s;
  _numberOfIndices = 2 * (s + 1) * s;
  _numberOfVertexes = (s + 1) * s;

  _vertexes = std::vector<RKVertex>();
  _indices = std::vector<short>();
  _vertexes.reserve(_numberOfVertexes);
  _vertexes.reserve(_numberOfIndices);

  for(int stackNumber=0;stackNumber<=_slices;stackNumber++)
  {
    for(int sliceNumber=0;sliceNumber<_slices;sliceNumber++)
    {
      theta = double(stackNumber) * M_PI / double(_slices);
      phi = double(sliceNumber) * 2.0 * M_PI / double(_slices);
      sinTheta = sin(theta);
      sinPhi = sin(phi);
      cosTheta = cos(theta);
      cosPhi = cos(phi);
      normal = float4(cosPhi * sinTheta, sinPhi * sinTheta, cosTheta, 0.0);
      position = float4(r * cosPhi * sinTheta, r * sinPhi * sinTheta, r * cosTheta, 0.0);
      _vertexes.push_back(RKVertex(position, normal, float2()));
    }
  }

  for(int stackNumber=0;stackNumber<_slices;stackNumber++)
  {
    for(int sliceNumber=0;sliceNumber<=_slices;sliceNumber++)
    {
      _indices.push_back(short((stackNumber * _slices) + (sliceNumber % _slices)));
      _indices.push_back(short(((stackNumber + 1) * _slices) + (sliceNumber % _slices)));
    }
  }
}
