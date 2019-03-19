#include "chai3d.h"

using namespace chai3d;
using namespace std;

class cBoundingPlane : public cGenericObject 
{
  private:
    cMesh* lowerMesh;
    cMesh* upperMesh;
    cMesh* topMesh;
    cMesh* bottomMesh;
    cMesh* rightMesh;
    cMesh* leftMesh;

  public:
    cBoundingPlane(int stiffness, double toolRadius);
    cMesh* getLowerBoundingPlane() { return lowerMesh; }
    cMesh* getUpperBoundingPlane() { return upperMesh; }
    cMesh* getTopBoundingPlane() { return topMesh; }
    cMesh* getBottomBoundingPlane() { return bottomMesh; }
    cMesh* getLeftBoundingPlane() { return leftMesh; }
    cMesh* getRightBoundingPlane() { return rightMesh; }
};
