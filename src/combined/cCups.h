#pragma once 
#include "chai3d.h"
#include "core/controller.h"
#include "network/network.h"
#include "haptics/haptics.h"
#include "graphics/graphics.h"
#include "graphics/cGenericMovingObject.h"
#include "math.h"

using namespace chai3d;
using namespace std;

/**
 * @file cCups.h
 * @class cCups
 * @brief Instance of cups task. See Hasson et al., 2012: https://www.ncbi.nlm.nih.gov/pmc/articles/PMC3544966/
 *
 * This class instantiates a Cups task tobject. 
*/

class cCups: public cGenericMovingObject, public cGenericEffect
{
  private:
    double escapeTheta;
    double pendulumLength;
    double ballMass;
    double cartMass;
    double gravity;
    cWorld* world;
    cShapeSphere* ball;
    cShapeBox* start;
    cShapeBox* stop;
    cMesh* cupMesh;
    cVector3d* startTarget;
    cVector3d* stopTarget;
    double ballPos;
    double ballVel;
    double ballForce;
    double cartPos;
    double cartVel;
    double cartAcc;
    double lastUpdateTime;
    bool running;
    cPrecisionClock* cupsClock;

  public:
    cCups(cWorld* worldPtr, double esc, double l, double bM, double cM);
    bool computeForce(const cVector3d& a_toolPos, const cVector3d& a_toolVel,
                      const unsigned int& a_toolID, cVector3d& a_reactionForce);
    void startCups();
    void stopCups();
    void destructCups();
    void graphicsLoopFunction(double dt, cVector3d toolPos, cVector3d toolVel);
    double computeBallAcceleration(double ballP);
    void updateNextBallPosition(double dt);
};
