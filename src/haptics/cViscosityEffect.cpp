#include "cViscosityEffect.h"

cViscosityEffect::cViscosityEffect(cWorld* worldPtr, cMatrix3d* B):cGenericEffect(worldPtr)
{
  viscosityMatrix = B;  
}

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
