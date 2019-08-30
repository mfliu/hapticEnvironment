#include "cViscosityEffect.h"

/**
 * @param worldPtr Pointer to the world 
 * @param B 3x3 matrix of multipliers 
 *
 * Constructor for the viscous field.
 */
cViscosityEffect::cViscosityEffect(cWorld* worldPtr, cMatrix3d* B):cGenericEffect(worldPtr)
{
  viscosityMatrix = B;  
}

/**
 * @param a_toolPos Position of the haptic tool in the workspace 
 * @param a_toolVel Velocity of the haptic tool 
 * @param a_toolID ID assigned to this particular haptic device
 * @param a_reactionForce A vector for storing the force to be applied to the haptic tool 
 *
 * Computes B*toolVel at each timepoint at which this function is called (in the haptic loop)
 */
bool cViscosityEffect::computeForce(const cVector3d& a_toolPos, const cVector3d& a_toolVel, 
                                        const unsigned int& a_toolID, cVector3d& a_reactionForce)
{
  if (m_parent->getHapticEnabled() == true) {
    a_reactionForce = cMul(*viscosityMatrix, a_toolVel);
    return true;
  }
  else {
    a_reactionForce.zero();
    return false;
  }
}
