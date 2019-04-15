#pragma once 

#include "chai3d.h"

using namespace chai3d;
using namespace std;

class cArrow : public cGenericObject
{
  private:
    cMesh* myMesh;
    double myLength;
    double shaftRadius;
    double lengthTip;
    double radiusTip;
    bool bidirectional;
    unsigned int numSides;
    cVector3d* direction;
    cVector3d* position;
    cColorf* color;

  public:
    cArrow(double a_length, double a_shaftRadius, double a_lengthTip, double a_radiusTip, bool a_bidir,
          unsigned int a_numSides, cVector3d* a_dir, cVector3d* a_pos, cColorf* a_color);
    cMesh* getArrowObj();
};
