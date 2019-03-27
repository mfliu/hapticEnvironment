#pragma once
#include "chai3d.h"
#include "cGenericMovingObject.h"
#include <random>
#include <ctime>

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
    virtual void graphicsLoopFunction(double dt); 
    cMultiPoint* getMovingPoints();
    cMultiPoint* getRandomPoints();
};
