#pragma once
#include "chai3d.h"

using namespace chai3d;
using namespace std;

/**
 * @file cPositionForceFieldEffect.h
 * @class cPositionForceFieldEffect
 *
 * @brief Creates a force field that varies over the space of the haptic environment.
 *
 * This effect depends on the position of the haptic cursor. Forces applied are in the specified
 * direction and magnitude, multiplied by the position of the cursor. 
 * @see cCST
 */
class cPositionForceFieldEffect : public cGenericEffect
{
  private:
    double magnitude;
    double direction;

  public:
    cPositionForceFieldEffect(cWorld* worldPtr, double m, double d);
    bool computeForce(const cVector3d& a_toolPos, const cVector3d& a_toolVel,
                      const unsigned int& a_toolID, cVector3d& a_reactionForce);
};
