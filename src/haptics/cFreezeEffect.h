#include "chai3d.h"

using namespace chai3d;
using namespace std;

/**
 * @file cFreezeEffect.h
 * @class cFreezeEffect
 *
 * @brief Freezes the haptic tool and prevents it from moving
 *
 * Freezes the haptic tool in place essentially by creating a spring between the haptic point and
 * its freeze-time location with a high stiffness. Usage would be to create a freeze effect the
 * moment the haptic cursor needs to be frozen. However, this effect can be used to create "sink"
 * essentially to trap the haptic cursor to a different location.
 */
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
