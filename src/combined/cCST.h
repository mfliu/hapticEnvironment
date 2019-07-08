#pragma once
#include "chai3d.h"
#include "core/controller.h"
#include "network/network.h"
#include "haptics/haptics.h"
#include "graphics/graphics.h"
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
    cVector3d* computeNextPosition(cVector3d toolPos);
    cPrecisionClock* cstClock;
    double lastUpdateTime;
    //ControlData controlData;

  public:
    cCST(cWorld* worldPtr, double l, double f, bool v, bool h);
    bool computeForce(const cVector3d& a_toolPos, const cVector3d& a_toolVel, 
                      const unsigned int& a_toolID, cVector3d& a_reactionForce);
    void graphicsLoopFunction(double dt, cVector3d toolPos, cVector3d toolVel);
    void setVisionEnabled(bool v);
    void setHapticEnabled(bool h);
    bool setLambda(double l);
    void startCST();
    void stopCST();
    void destructCST();
};
