#include "cArrow.h"

cArrow::cArrow(double a_length, double a_shaftRadius, double a_lengthTip, double a_radiusTip, bool a_bidir,
                unsigned int a_numSides, cVector3d* a_dir, cVector3d* a_pos, cColorf* a_color)
{
  myLength = a_length;
  shaftRadius = a_shaftRadius;
  lengthTip = a_lengthTip;
  radiusTip = a_radiusTip;
  bidirectional = a_bidir;
  numSides = a_numSides;
  direction = a_dir;
  position = a_pos;
  color = a_color;

  myMesh = new cMesh();
  cCreateArrow(myMesh, myLength, shaftRadius, lengthTip, radiusTip, bidirectional, 
                numSides, *direction, *position, *color);
}

cMesh* cArrow::getArrowObj()
{
  return myMesh;
}
