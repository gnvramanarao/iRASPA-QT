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

#include <cmath>
#include "cappedcylindersinglebondgeometry.h"

CappedCylinderSingleBondGeometry::CappedCylinderSingleBondGeometry(double r, int s)
{
  _slices = s;
  _numberOfVertexes = 4 * _slices + 2;
  _numberOfIndices = 12 * _slices;

  _vertexes = std::vector<RKVertex>();
  _indices = std::vector<short>();

  _vertexes.reserve(_numberOfVertexes);
  _vertexes.resize(_numberOfVertexes);
  _indices.reserve(_numberOfIndices);
  _indices.resize(_numberOfIndices);

  double delta = 2.0 * M_PI / double(_slices);

  int index = 0;
  for(int i=0;i<_slices;i++)
  {
    double cosTheta = cos(delta * double(i));
    double sinTheta = sin(delta * double(i));

    float4 position1 = float4(float(r * cosTheta), 0.0, float(r * sinTheta), 0.0);
    float4 normal1 = float4(float(cosTheta), 0.0, float(sinTheta), 0.0);
    _vertexes[index] = RKVertex(position1, normal1, float2());
    index += 1;

    float4 normal2 = float4(float(cosTheta), 0.0, float(sinTheta), 0.0);
    float4 position2 = float4(float(r * cosTheta), 1.0, float(r * sinTheta), 0.0);
    _vertexes[index] = RKVertex(position2, normal2, float2());
    index += 1;
  }

  // first cap
  // ==========================================================================

  float4 position_cap1 = float4(0.0, 0.0, 0.0, 0.0);
  float4 normal_cap1 = float4(0.0, -1.0, 0.0, 0.0);
  _vertexes[index] = RKVertex(position_cap1, normal_cap1, float2());
  int ref_cap_1 = index;
  index += 1;

  for(int i=0;i<_slices;i++)
  {
    double cosTheta = r * cos(delta * double(i));
    double sinTheta = r * sin(delta * double(i));
    float4 position_cap1 = float4(float(cosTheta), 0.0, float(sinTheta), 0.0);
    float4 normal_cap1 = float4(0.0, -1.0, 0.0, 0.0);
    _vertexes[index] = RKVertex(position_cap1, normal_cap1, float2());
    index += 1;
  }

  // second cap
  // ==========================================================================

  float4 position_cap2 = float4(0.0, 1.0, 0.0, 0.0);
  float4 normal_cap2 = float4(0.0, 1.0, 0.0, 0.0);
  _vertexes[index] = RKVertex(position_cap2, normal_cap2, float2());
  int ref_cap_2 = index;
  index += 1;

  for(int i=0;i<_slices;i++)
  {
    double cosTheta = r * cos(delta * double(i));
    double sinTheta = r * sin(delta * double(i));
    float4 position_cap2 = float4(float(cosTheta), 1.0, float(sinTheta), 0.0);
    float4 normal_cap2 = float4(0.0, 1.0, 0.0, 0.0);
    _vertexes[index] = RKVertex(position_cap2, normal_cap2, float2());
    index += 1;
  }

  index = 0;
  for(int i=0;i<_slices;i++)
  {
    _indices[index]=short((2 * i) % (2 * _slices));
    index += 1;
    _indices[index]=short((2 * i + 1) % (2 * _slices));
    index += 1;
    _indices[index]=short((2 * i + 2) % (2 * _slices));
    index += 1;
    _indices[index]=short((2 * i + 2) % (2 * _slices));
    index += 1;
    _indices[index]=short((2 * i + 1) % (2 * _slices));
    index += 1;
    _indices[index]=short((2 * i + 3) % (2 * _slices));
    index += 1;
  }

  // first cap
  // ==========================================================================

  for(int i=0;i<_slices;i++)
  {
    _indices[index]=short(ref_cap_1);
    index += 1;
    _indices[index]=short(ref_cap_1 + 1 + ((i) % _slices));
    index += 1;
    _indices[index]=short(ref_cap_1 + 1 + ((i + 1) % _slices));
    index += 1;
  }

  // second cap
  // ==========================================================================

  for(int i=0;i<_slices;i++)
  {
    _indices[index]=short(ref_cap_2);
    index += 1;
    _indices[index]=short(ref_cap_2 + 1 + ((i + 1) % _slices));
    index += 1;
    _indices[index]=short(ref_cap_2 + 1 + ((i) % _slices));
    index += 1;
  }
}
