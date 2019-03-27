#include "cConstantForceFieldEffect.h"

cConstantForceFieldEffect::cConstantForceFieldEffect(cWorld* worldPtr, double d, double m):cGenericEffect(worldPtr)
{
  direction = d;
  magnitude = m;
}
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


