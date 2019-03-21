#include "chai3d.h"

using namespace chai3d;
using namespace std;

class cConstantForceFieldEffect : public cGenericEffect 
{
  public:
    double magnitude;
    double direction;

  public:
    cConstantForceFieldEffect(cWorld* worldPtr, double d, double m);
    bool computeForce(const cVector3d& a_toolPos, const cVector3d& a_toolVel, 
                      const unsigned int& a_toolID, cVector3d& a_reactionForce);
};
