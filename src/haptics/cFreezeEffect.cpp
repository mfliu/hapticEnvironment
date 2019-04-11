#include "cFreezeEffect.h"

cFreezeEffect::cFreezeEffect(cWorld* worldPtr, double maxStiffness, cVector3d startPoint):cGenericEffect(worldPtr)
{
  stiffness = maxStiffness;
  freezePoint = startPoint;

}

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
