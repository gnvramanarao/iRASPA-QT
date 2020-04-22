#include "cappedcylindertriplebondgeometry.h"
#include "cappedcylindersinglebondgeometry.h"
#include <algorithm>
#include <float4.h>

CappedCylinderTripleBondGeometry::CappedCylinderTripleBondGeometry(double r, int s)
{
  _slices = s;
  _numberOfVertexes = 3*(4 * _slices + 2);
  _numberOfIndices = 3*(12 * _slices);

  _vertexes = std::vector<RKVertex>();
  _indices = std::vector<short>();

  CappedCylinderSingleBondGeometry singleBondGeometry = CappedCylinderSingleBondGeometry(r,s);

  std::transform(singleBondGeometry.vertices().begin(), singleBondGeometry.vertices().end(), std::back_inserter(_vertexes), [](RKVertex i) {return RKVertex(i.position + float4(-1.0,0.0,-0.5*sqrt(3.0),0.0), i.normal, i.st);});
  std::transform(singleBondGeometry.vertices().begin(), singleBondGeometry.vertices().end(), std::back_inserter(_vertexes), [](RKVertex i) {return RKVertex(i.position + float4(1.0,0.0,-0.5*sqrt(3.0),0.0), i.normal, i.st);});
  std::transform(singleBondGeometry.vertices().begin(), singleBondGeometry.vertices().end(), std::back_inserter(_vertexes), [](RKVertex i) {return RKVertex(i.position + float4(0.0,0.0,0.5*sqrt(3.0),0.0), i.normal, i.st);});

  std::copy(singleBondGeometry.indices().begin(), singleBondGeometry.indices().end(), std::back_inserter(_indices));
  std::transform(singleBondGeometry.indices().begin(), singleBondGeometry.indices().end(), std::back_inserter(_indices), [=](uint16_t i) {return i + 4 * _slices + 2;});
  std::transform(singleBondGeometry.indices().begin(), singleBondGeometry.indices().end(), std::back_inserter(_indices), [=](uint16_t i) {return i + 8 * _slices + 4;});
}
