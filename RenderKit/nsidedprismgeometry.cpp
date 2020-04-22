#include "nsidedprismgeometry.h"

NSidedPrismGeometry::NSidedPrismGeometry(double r, int s)
{
  _slices = s;
  _numberOfVertexes = 6 * _slices;
  _numberOfIndices = 6 * _slices;

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

    double cosTheta2 = cos(delta * double(i+1));
    double sinTheta2 = sin(delta * double(i+1));

    float4 position1 = float4(float(r * cosTheta), 1.0, float(r * sinTheta), 0.0);
    float4 position2 = float4(float(r * cosTheta), -1.0, float(r * sinTheta), 0.0);
    float4 position3 = float4(float(r * cosTheta2), 1.0, float(r * sinTheta2), 0.0);
    float4 v1 = position2 - position1;
    float4 w1 = position2 - position3;
    float3 normal1 = (float3::cross(float3(v1.x,v1.y,v1.z), float3(w1.x,w1.y,w1.z))).normalise();

    _vertexes[index] = RKVertex(position1, float4(normal1.x,normal1.y,normal1.z,0.0), float2());
    _indices[index] = short(index);
    index ++;

    _vertexes[index] = RKVertex(position3, float4(normal1.x,normal1.y,normal1.z,0.0), float2());
    _indices[index] = short(index);
    index++;

    _vertexes[index] = RKVertex(position2, float4(normal1.x,normal1.y,normal1.z,0.0), float2());
    _indices[index] = short(index);
    index++;



    float4 position4 = float4(float(r * cosTheta2), 1.0, float(r * sinTheta2), 0.0);
    float4 position5 = float4(float(r * cosTheta), -1.0, float(r * sinTheta),  0.0);
    float4 position6 = float4(float(r * cosTheta2), -1.0, float(r * sinTheta2), 0.0);
    float4 v2 = position5 - position4;
    float4 w2 = position5 - position6;
    float3 normal2 = (float3::cross(float3(v2.x,v2.y,v2.z), float3(w2.x,w2.y,w2.z))).normalise();

    _vertexes[index] = RKVertex(position4, float4(normal2.x,normal2.y,normal2.z,0.0), float2());
    _indices[index] = short(index);
    index++;

    _vertexes[index] = RKVertex(position6, float4(normal2.x,normal2.y,normal2.z,0.0), float2());
    _indices[index] = short(index);
    index++;

    _vertexes[index] = RKVertex(position5, float4(normal2.x,normal2.y,normal2.z,0.0), float2());
    _indices[index] = short(index);
    index++;


   }
}
