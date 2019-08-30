#pragma once
#include "chai3d.h"

using namespace chai3d;
using namespace std;

/**
 * @file cConstantForceFieldEffect.h
 * @class cConstantForceFieldEffect
 *
 * @brief Create a constant force field pervasive in the entire workspace
 *
 * Force fields created with this class have a magnitude and direction, and are exprienced
 * regardless of the kinematics of the haptic tool. All haptic effects must override the
 * cGenericEffect class, which contains the function computeForce. 
 *
 * @see cGenericEffect
 */
class cConstantForceFieldEffect : public cGenericEffect 
{
  public:
    double magnitude;
    double direction;

  public:
    cConstantForceFieldEffect(cWorld* worldPtr, double d, double m);
    bool computeForce(const cVector3d& a_toolPos, const cVector3d& a_toolVel, 
                      const unsigned int& a_toolID, cVector3d& a_reactionForce);
};
