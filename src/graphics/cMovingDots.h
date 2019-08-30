#pragma once
#include "chai3d.h"
#include "cGenericMovingObject.h"
#include <random>
#include <ctime>

/**
 * @file cMovingDots.h
 *
 * @class cMovingDots
 * 
 * @brief A class for moving point clouds. 
 *
 * Creates a cloud of points that move in a given direction with a specified coherence. 
 * Essentially, the implementation of Movshon & Newsome's moving dot graphics.
 * Creates a cloud of moving points, where <em> c% </em> of points are moving according to the
 * direction and velocity given, while the remainder of the points move in random directions.
 */
class cMovingDots : public cGenericMovingObject
{
  private:
    int numDots;
    double coherence;
    double direction;
    double magnitude;
    cMultiPoint* movingPoints;
    cMultiPoint* randomPoints;
    default_random_engine generator;

  public:
    cMovingDots(int n, double c, double d, double m);
    virtual void graphicsLoopFunction(double dt, cVector3d toolPos, cVector3d toolVel); 
    cMultiPoint* getMovingPoints();
    cMultiPoint* getRandomPoints();
};
