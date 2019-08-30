#include "cPositionForceFieldEffect.h"

/**
 * @param worldPtr Pointer to the world 
 * @param m Magnitude of the force field 
 * @param d Direciton of the force field 
 *
 * Constructor for a position-dependent force field
 */
cPositionForceFieldEffect::cPositionForceFieldEffect(cWorld* worldPtr, double m, double d):cGenericEffect(worldPtr)
{
  magnitude = m;
  direction = d;
}

/**
 * @param a_toolPos Position of the haptic tool 
 * @param a_toolVel Velocity of the haptic tool 
 * @param a_toolID ID number of the haptic tool 
 * @param a_reactionForce Vector that stores the force to be applied to the robot 
 *
 * The position of the haptic tool is multiplied by the magnitude and direction of the specified
 * force field
 */
bool cPositionForceFieldEffect::computeForce(const cVector3d& a_toolPos, const cVector3d& a_toolVel, 
                              const unsigned int& a_toolID, cVector3d& a_reactionForce)
{
  if (m_parent->getHapticEnabled() == true) {
    a_reactionForce.x(a_toolPos.x() * magnitude * cCosDeg(direction));
    a_reactionForce.y(a_toolPos.y() * magnitude * cSinDeg(direction));
    return true;
  }
  else {
    a_reactionForce.zero();
    return false;
  }
}
