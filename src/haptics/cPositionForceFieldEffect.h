#pragma once
#include "chai3d.h"

using namespace chai3d;
using namespace std;

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
