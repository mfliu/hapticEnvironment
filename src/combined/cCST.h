#pragma once
#include "chai3d.h"
#include "haptics/haptics.h"
#include "graphics/graphics.h"
#include "haptics/cPositionForceFieldEffect.h"
#include "graphics/cGenericMovingObject.h"

using namespace chai3d;
using namespace std;

class cCST: public cGenericMovingObject, public cGenericEffect
{
  private: 
    double lambda;
    double forceMagnitude;
    bool visionEnabled;
    bool hapticEnabled;
    bool running;
    cWorld* world;
    cVector3d* currPos;
    cShapeSphere* visualCursor;
    //cPositionForceFieldEffect* hapticCursor;
    cVector3d* computeNextPosition(cVector3d toolPos);

  public:
    cCST(cWorld* worldPtr, double l, double f, bool v, bool h);
    bool computeForce(const cVector3d& a_toolPos, const cVector3d& a_toolVel, 
                      const unsigned int& a_toolID, cVector3d& a_reactionForce);
    void graphicsLoopFunction(double dt, cVector3d toolPos, cVector3d toolVel);
    //cShapeSphere* getVisualCursor();
    //cPositionForceFieldEffect* getHapticCursor();
    void setVisionEnabled(bool v);
    void setHapticEnabled(bool h);
    void startCST();
    void stopCST();
    void destructCST();
};
