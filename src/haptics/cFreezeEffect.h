#include "chai3d.h"

using namespace chai3d;
using namespace std;

class cFreezeEffect : public cGenericEffect
{
  
  private:
    cVector3d freezePoint;
    double stiffness;

  public:
    cFreezeEffect(cWorld* worldPtr, double maxStiffness, cVector3d startPoint);
    bool computeForce(const cVector3d& a_toolPos, const cVector3d& a_toolVel, 
                      const unsigned int& a_toolID, cVector3d& a_reactionForce);
};
