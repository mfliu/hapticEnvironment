#pragma once
#include "chai3d.h"

using namespace chai3d;
using namespace std;

/**
 * @file cConstrainToLine.h
 * @class cConstrainToLine
 *
 * @brief Constrains movement of the haptic device to a line between two points 
 *
 * Creates a line between two points that has a strong magnetic effect. Can be enabled or disabled.
 * As with other haptic effects, this class inherits from Chai3D's cGenericEFfect. 
 */
class cConstrainToLine : public cGenericEffect
{
  private:
    cVector3d* point1;
    cVector3d* point2;
    bool enabled;
    double stiffness; 

  public:
    cConstrainToLine(cWorld* worldPtr, cVector3d* p1, cVector3d* p2, double s);
    void setEnabled(bool e);
    bool getEnabled();
    bool computeForce(const cVector3d& a_toolPos, const cVector3d& a_toolVel, 
                      const unsigned int& a_toolID, cVector3d& a_reactionForce);
};
