#include "chai3d.h"

using namespace chai3d;
using namespace std;

/**
 * @file cBoundingPlane.h
 * @class cBoundingPlane
 * @brief Creates bounding planes that constrict movement to a 2D plane near the center of the
 * workspace.
 *
 * Restricts movement of the haptic device to a vertical 2D plane. The stiffness of the bounds can
 * be specified. 
 *
 * @deprecated Do not use this class until issues with colllision detection are resolved
 */

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
    cBoundingPlane(int stiffness, double toolRadius, double bWidth, double bHeight);
    cMesh* getLowerBoundingPlane() { return lowerMesh; }
    cMesh* getUpperBoundingPlane() { return upperMesh; }
    cMesh* getTopBoundingPlane() { return topMesh; }
    cMesh* getBottomBoundingPlane() { return bottomMesh; }
    cMesh* getLeftBoundingPlane() { return leftMesh; }
    cMesh* getRightBoundingPlane() { return rightMesh; }
};
