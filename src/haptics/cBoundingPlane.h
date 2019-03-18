#include "chai3d.h"

using namespace chai3d;

class cBoundingPlane : public cGenericObject 
{
  private:
    cMesh *topEdge;
    cMesh *bottomEdge;
    cMesh *leftEdge;
    cMesh *rightEdge;

  public:
    cBoundingPlane(cVector3d* topLeft, cVector3d* topRight, cVector3d* botLeft, cVector3d* botRight, int stiffness);
    cMesh* getTopEdge() { return topEdge; }
    cMesh* getBottomEdge() { return bottomEdge; }
    cMesh* getLeftEdge() { return leftEdge; }
    cMesh* getRightEdge() { return rightEdge; }

};
