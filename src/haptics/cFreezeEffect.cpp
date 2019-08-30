#include "cFreezeEffect.h"

/**
 * @param worldPtr Pointer to the world 
 * @param maxStiffness Maximum stiffness of the world to ensure that the haptic tool is maximally
 * freezed
 * @param startPoint The position at which the haptic cursor is frozen--typically the instantaneous
 * position of the cursor.
 */
cFreezeEffect::cFreezeEffect(cWorld* worldPtr, double maxStiffness, cVector3d startPoint):cGenericEffect(worldPtr)
{
  stiffness = maxStiffness;
  freezePoint = startPoint;

}

/**
 * @param a_toolPos Position of the haptic tool 
 * @param a_toolVel Velocity of the haptic tool 
 * @param a_toolID ID number of the haptic tool 
 * @param a_reactionForce Vector to store the computed reaction force 
 *
 * Produces a force in the direction between the haptic tool and the position at which the tool
 * should be frozen.
 */
bool cFreezeEffect::computeForce(const cVector3d& a_toolPos, const cVector3d& a_toolVel,
                                  const unsigned int& a_toolID, cVector3d& a_reactionForce)
{
  if (m_parent->getHapticEnabled() == true)
  {
    double ptDistance = cDistance(freezePoint, a_toolVel);
    double forceMag = ptDistance * stiffness;
    a_reactionForce = (forceMag/ptDistance) * cSub(freezePoint, a_toolPos);
    return true;
  }
  else {
    a_reactionForce.zero();
    return false;
  }
}
