#pragma once
#include "chai3d.h"

using namespace chai3d;
using namespace std;

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
