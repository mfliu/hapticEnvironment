#include "cMovingDots.h"

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

void cMovingDots::graphicsLoopFunction(double dt)
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

cMultiPoint* cMovingDots::getMovingPoints()
{
  return movingPoints; 
}

cMultiPoint* cMovingDots::getRandomPoints()
{
  return randomPoints;
}
