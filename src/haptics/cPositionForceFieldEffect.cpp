#include "cPositionForceFieldEffect.h"

cPositionForceFieldEffect::cPositionForceFieldEffect(cWorld* worldPtr, double m, double d):cGenericEffect(worldPtr)
{
  magnitude = m;
  direction = d;
}

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
