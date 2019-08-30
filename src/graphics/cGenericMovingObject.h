#pragma once
#include "chai3d.h"

using namespace chai3d;
using namespace std;


/**
 * @file cGenericMovingObject.h
 *
 * @class cGenericMovingObject
 *
 * @brief A generic abstract class for any object that moves across the screen at regular time intervals.
 * 
 * All objects that move must inherit this class and override the graphicsLoopFunction. This
 * function is called in the graphics update loop for each moving object in the environment. 
 */
class cGenericMovingObject : public cGenericObject 
{
  public:
    cGenericMovingObject();
    ~cGenericMovingObject();
    virtual void graphicsLoopFunction(double dt, cVector3d toolPos, cVector3d toolVel) {};
};

