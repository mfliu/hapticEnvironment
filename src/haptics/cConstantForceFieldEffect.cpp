#include "cConstantForceFieldEffect.h"

/**
 * @param worldPtr Pointer to the world. Force field effects are applied to the entire environment
 * @param d Direction of force field 
 * @param m Magnitude of force field 
 *
 * Constructor for constant force field effect. 
 */
cConstantForceFieldEffect::cConstantForceFieldEffect(cWorld* worldPtr, double d, double m):cGenericEffect(worldPtr)
{
  direction = d;
  magnitude = m;
}

/**
 * @param a_toolPos Position of the haptic tool in the workspace 
 * @param a_toolVel Velocity of the haptic tool 
 * @param a_toolID ID assigned to this particular haptic device
 * @param a_reactionForce A vector for storing the force to be applied to the haptic tool 
 *
 * This function is a virtual function from cGenericEffect that is implemented here.
 */
bool cConstantForceFieldEffect::computeForce(const cVector3d& a_toolPos, const cVector3d& a_toolVel, 
                                        const unsigned int& a_toolID, cVector3d& a_reactionForce)
{
  if (m_parent->getHapticEnabled() == true) {
    a_reactionForce.x(magnitude * cCosDeg(direction));
    a_reactionForce.y(magnitude * cSinDeg(direction));
    return true;
  }
  else {
    a_reactionForce.zero();
    return false;
  }
}


