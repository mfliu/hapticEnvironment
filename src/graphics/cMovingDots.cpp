#include "cMovingDots.h"

/**
 * @param n Number of points 
 * @param c Coherence of points, must be between 0 and 1
 * @param d Direction in degrees that dots are moving in 
 * @param m Magnitude or speed at which the dots move
 *
 * @brief Constructor for moving dot object 
 *
 * This function generates a dot at a random position in the field of view and adds a pointer to the
 * dots using Chai3D's cMultiPoint object. For each set of moving dots, \a c*n points are chosen to
 * be the "movingDots", or dots that move in the direction specified. The remainder of the points
 * <em> (n-(c*n)) </em> are dots that move in random directions.
 */
cMovingDots::cMovingDots(int n, double c, double d, double m) : cGenericMovingObject()
{
  generator.seed(time(0));
  uniform_real_distribution<double> distribution(-1.0, 1.0);

  movingPoints = new cMultiPoint();
  randomPoints = new cMultiPoint();
  
  movingPoints->setPointSize(5.0);
  movingPoints->setLocalPos(0.0, 0.0, 0.0);
  movingPoints->setEnabled(true);
  movingPoints->setUseDisplayList(true);

  randomPoints->setPointSize(5.0);
  randomPoints->setLocalPos(0.0, 0.0, 0.0);
  randomPoints->setEnabled(true);
  randomPoints->setUseDisplayList(true);

  numDots = n;
  coherence = c;
  direction = d;
  magnitude = m;
 
  int numMove = (int) (coherence * numDots);
  int numRand = numDots - numMove;
  
  for (int n=0; n<numMove; n++)
  {
    double x = distribution(generator);
    double y = distribution(generator);
    int vId = movingPoints->newVertex(0.0, x, y);
    int pId = movingPoints->newPoint(vId);
    movingPoints->setPointColor(cColorf(255.0, 255.0, 255.0, 1.0));
  }
   
  for (int n=0; n<numRand; n++)
  {
    double x = distribution(generator);
    double y = distribution(generator);
    int vId = randomPoints->newVertex(0.0, x, y);
    randomPoints->newPoint(vId);
    randomPoints->setPointColor(cColorf(255.0, 255.0, 255.0, 1.0));
  }
}

/**
 * @param dt The amount of time that has passed since this function was last called
 * @param toolPos Position of the haptic robot 
 * @param toolVel Velocity of the haptic robot 
 *
 * @brief Update function for this moving object 
 *
 * This function is called for the moving object in each iteration of the graphics loop. For moving
 * dots, this function computes the next position of each true moving dot based on the specified direction and
 * velocity, and re-renders the dot accordingly. Each random dot is assigned a 
 * random velocity and direction to move. If the dot moves out of the field of view, it's
 * position is randomly selected to be within the field of view. 
 */
void cMovingDots::graphicsLoopFunction(double dt, cVector3d toolPos, cVector3d toolVel)
{
  uniform_real_distribution<double> distribution(-1.0, 1.0);
  uniform_real_distribution<double> vDist(0.0, 360.0);

  double xOffset = dt * magnitude * cCosDeg(direction);
  double yOffset = dt * magnitude * cSinDeg(direction);
  cVector3d* vOffset = new cVector3d(0.0, xOffset, yOffset);
  movingPoints->offsetVertices(*vOffset);
  
  int numVertices = movingPoints->getNumVertices();
  cVertexArrayPtr vertices = movingPoints->m_vertices;
  for (int i = 0; i < numVertices; i++)
  {
    double xCoord = vertices->getLocalPos(i).y();
    double yCoord = vertices->getLocalPos(i).z();
    if (xCoord > 1.0 || xCoord < -1.0 || yCoord > 1.0 || yCoord < -1.0)
    {
      double newX = distribution(generator);
      double newY = distribution(generator);
      vertices->setLocalPos(i, 0.0, newX, newY);
    }
  }
  
  cVertexArrayPtr randVert = randomPoints->m_vertices;
  for (int i = 0; i < randomPoints->getNumVertices(); i++)
  {
    double randomDeg = vDist(generator);
    double xRand = dt * magnitude * cCosDeg(randomDeg);
    double yRand = dt * magnitude * cSinDeg(randomDeg);
    double newX = randVert->getLocalPos(i).y() + xRand;
    double newY = randVert->getLocalPos(i).z() + yRand;
    randVert->setLocalPos(i, 0.0, newX, newY);

    if (newX > 1.0 || newX < -1.0 || newY > 1.0 || newY < -1.0)
    {
      double newX = distribution(generator);
      double newY = distribution(generator);
      randVert->setLocalPos(i, 0.0, newX, newY);
    }
  }
  movingPoints->markForUpdate();
  randomPoints->markForUpdate();
}

/**
 * Returns the dots moving in the specified direction
 */
cMultiPoint* cMovingDots::getMovingPoints()
{
  return movingPoints; 
}

/**
 * Returns the dots moving in random directions. If coherence is 1, the number of random dots is 0.
 */
cMultiPoint* cMovingDots::getRandomPoints()
{
  return randomPoints;
}
