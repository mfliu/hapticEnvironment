#include "cConstrainToLine.h"

cConstrainToLine::cConstrainToLine(cWorld* worldPtr, cVector3d* p1, cVector3d* p2, double s):cGenericEffect(worldPtr)
{
  point1 = p1;
  point2 = p2;
  stiffness = s;
}

void cConstrainToLine::setEnabled(bool e)
{
  enabled = e;
}

bool cConstrainToLine::getEnabled()
{
  return enabled;
}

bool cConstrainToLine::computeForce(const cVector3d& a_toolPos, const cVector3d& a_toolVel, 
                      const unsigned int& a_toolID, cVector3d& a_reactionForce)
{
  if (enabled == true) {
    cVector3d distance = cProjectPointOnLine(a_toolPos, *point1, cSub(*point2, *point1)); //distance to line 
    a_reactionForce = cSub(distance, a_toolPos);
    a_reactionForce.mul(stiffness);
    return true;
  }
  else {
    a_reactionForce.zero();
    return true;
  }
}
