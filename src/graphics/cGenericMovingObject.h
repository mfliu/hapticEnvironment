#pragma once
#include "chai3d.h"

using namespace chai3d;
using namespace std;

class cGenericMovingObject : public cGenericObject 
{
  public:
    cGenericMovingObject();
    ~cGenericMovingObject();
    virtual void graphicsLoopFunction(double dt, cVector3d toolPos, cVector3d toolVel) {};
};

