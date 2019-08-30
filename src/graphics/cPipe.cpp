#include "cPipe.h"

/**
 * @param a_height Height of the pipe 
 * @param a_innerRadius Radius of the inner portion of the pipe (can be hollow)
 * @param a_outerRadius Radius of the outer portion of the pipe 
 * @param numSides Number of sides that make up the pipe. If 6, then the pipe is hexagonal. 
 * @param a_numHeightSegments Number of segments in the pipe 
 * @param a_pos Position of the center of the pipe 
 * @param a_rot Rotation of the pipe, 0 is perfectly horizontal
 * @param a_color Color of the pipe
 */
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

/**
 * Returns the mesh object associated with this pipe object
 */
cMesh* cPipe::getPipeObj()
{
  return myMesh;
}
