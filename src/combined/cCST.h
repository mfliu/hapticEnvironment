#pragma once
#include "chai3d.h"
#include "core/controller.h"
#include "network/network.h"
#include "haptics/haptics.h"
#include "graphics/graphics.h"
#include "graphics/cGenericMovingObject.h"

using namespace chai3d;
using namespace std;

/**
 * @file cCST.h
 * @class cCST
 * @brief Instance of a critical stability task paradigm
 *
 * This class instantiates a Critical Stability Task (CST) object. The CST involves stabilizing a
 * cursor with the haptic robot. Each instantiation of the CST creates a CST cursor. The CST can be
 * instantiated with haptic feedback only, visual feedback only, or both. The haptic feedback uses a
 * position-dependent force field to render state information to the user. For more information on
 * the CST, see Quick et al. (https://www.physiology.org/doi/full/10.1152/jn.00300.2017)
 * 
 * The CST system is governed by the equation:
 * \f[
 * \frac{dx}{dt} = \lambda (x(t)-u(t)) \f]
 * where \f$x(t)\f$ is the CST cursor position and \f$u(t)\f$ is the hand position, and \f$\lambda\f$ 
 * is the degree of instability of the system.
 * 
 */
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
