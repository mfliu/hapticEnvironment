#include "cPipe.h"

cPipe::cPipe(double a_height, double a_innerRadius, double a_outerRadius, unsigned int a_numSides, 
              unsigned int a_numHeightSegments, cVector3d* a_pos, cMatrix3d* a_rot, cColorf* a_color)
{
  height = a_height;
  innerRadius = a_innerRadius;
  outerRadius = a_outerRadius;
  numSides = a_numSides;
  a_numHeightSegments = a_numHeightSegments;
  pos = a_pos;
  rot = a_rot;
  color = a_color;

  myMesh = new cMesh();
  cCreatePipe(myMesh, height, innerRadius, outerRadius, numSides, numHeightSegments, *pos, *rot, *color);
}

cMesh* cPipe::getPipeObj()
{
  return myMesh;
}
