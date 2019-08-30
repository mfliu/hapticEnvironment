#include "cArrow.h"

/** 
 * @param a_length Length of the arrow 
 * @param a_shaftRadius Radius of the arrow shaft
 * @param a_lengthTip Length of the tip of the arrow (the pointy part)
 * @param a_radiusTip Radius of the tip of the arrow. The tip of the arrow is conical.
 * @param a_bidir True to have arrow tips on both ends, False to have an arrow tip on one end
 * @param a_numSides Number of sides on radial sections. 
 * @param a_dir Direction of arrow 
 * @param a_pos Position to start arrow. Arrow starts at a_pos and points in a_dir, with the length
 * being the length of the a_dir vector.
 * @param a_color Color of vertices
 *
 */
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

/**
 * Returns the mesh object created by the arrow. 
 */
cMesh* cArrow::getArrowObj()
{
  return myMesh;
}
