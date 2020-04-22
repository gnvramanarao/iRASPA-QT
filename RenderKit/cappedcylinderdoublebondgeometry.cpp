#include "cappedcylinderdoublebondgeometry.h"
#include "cappedcylindersinglebondgeometry.h"
#include <QDebug>

CappedCylinderDoubleBondGeometry::CappedCylinderDoubleBondGeometry(double r, int s): _singleBondGeometry(CappedCylinderSingleBondGeometry(r,s))
{
  _slices = s;
  _numberOfVertexes = 2*(4 * _slices + 2);
  _numberOfIndices = 2*(12 * _slices);

  _vertexes = std::vector<RKVertex>{};
  _indices = std::vector<short>{};

  std::transform(_singleBondGeometry.vertices().begin(), _singleBondGeometry.vertices().end(), std::back_inserter(_vertexes), [](const RKVertex &i) {return RKVertex(i.position + float4(-1.0,0.0,0.0,0.0), i.normal, i.st);});
  std::transform(_singleBondGeometry.vertices().begin(), _singleBondGeometry.vertices().end(), std::back_inserter(_vertexes), [](const RKVertex &i) {return RKVertex(i.position + float4(1.0,0.0,0.0,0.0), i.normal, i.st);});

  std::copy(_singleBondGeometry.indices().begin(), _singleBondGeometry.indices().end(), std::back_inserter(_indices));
  std::transform(_singleBondGeometry.indices().begin(), _singleBondGeometry.indices().end(), std::back_inserter(_indices), [=](const short &i) {return i + 4 * _slices + 2;});
}
