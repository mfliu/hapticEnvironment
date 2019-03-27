#include "chai3d.h"

using namespace chai3d;
using namespace std;

class cViscosityEffect : public cGenericEffect
{
  public:
    cMatrix3d* viscosityMatrix;

  public:
    cViscosityEffect(cWorld* worldPtr, cMatrix3d* B);
    bool computeForce(const cVector3d& a_toolPos, const cVector3d& a_toolVel,
                      const unsigned int& a_toolID, cVector3d& a_reactionForce);
};
