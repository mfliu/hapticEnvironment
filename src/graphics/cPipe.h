#pragma once 
#include "chai3d.h"

using namespace chai3d;
using namespace std;

/**
 * @brief Pipe class for generating cylindrical pipes.
 */
class cPipe : public cGenericObject 
{
  private:
    cMesh* myMesh;
    double height;
    double innerRadius;
    double outerRadius;
    int numSides;
    int numHeightSegments;
    cVector3d* pos;
    cMatrix3d* rot;
    cColorf* color;

  public:
    cPipe(double height, double innerRadius, double outerRadius, unsigned int numSides, 
          unsigned int numHeightSegments, cVector3d* pos, cMatrix3d* rot, cColorf* color);
    cMesh* getPipeObj();
};
