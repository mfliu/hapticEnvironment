#include "cCST.h"

cCST::cCST(cWorld* worldPtr, double l, double f, bool v, bool h):cGenericMovingObject(), cGenericEffect(worldPtr)
{
  world = worldPtr;
  lambda = l;
  forceMagnitude = f;
  visionEnabled = v;
  hapticEnabled = h;
  running = false;
  currPos = new cVector3d(0.0, 0.0, 0.0);
  
  // Visual Cursor
  visualCursor = new cShapeSphere(0.5);
  visualCursor->m_material->setColorf(0.0, 0.75, 1.0);
  visualCursor->setLocalPos(*currPos);
  visualCursor->setEnabled(false);
  world->addChild(visualCursor);
  
}

cVector3d* cCST::computeNextPosition(cVector3d toolPos)
{
  double cursorY = currPos->y();
  double toolY = toolPos.y();
  double nextY = (lambda * cursorY) + ((lambda-1) * toolY);
  cVector3d* nextPos = new cVector3d(0.0, nextY, 0.0);
  currPos->y(nextPos->y());
  return nextPos;
}

bool cCST::computeForce(const cVector3d& a_toolPos, const cVector3d& a_toolVel,
                  const unsigned int& a_toolID, cVector3d& a_reactionForce)
{
  if (hapticEnabled == true && running == true) {
    cVector3d* nextPos = cCST::computeNextPosition(a_toolPos);
    a_reactionForce.y(forceMagnitude * nextPos->y());
    return true;
  }
  else {
    a_reactionForce.zero();
    return false;
  }
}

void cCST::graphicsLoopFunction(double dt, cVector3d toolPos, cVector3d toolVel)
{
  if (visionEnabled == true && running == true) {
    if (visualCursor->getEnabled() == false) {
      visualCursor->setEnabled(true);
    }
    cVector3d* nextPos = cCST::computeNextPosition(toolPos);
    visualCursor->setLocalPos(0.0, currPos->y(), 0.0);
  }
  else {
    visualCursor->setEnabled(false);
  }
}

/*
cShapeSphere* cCST::getVisualCursor()
{
  return visualCursor;
}

cPositionForceFieldEffect* cCST::getHapticCursor()
{
  return hapticCursor;
}
*/
void cCST::setVisionEnabled(bool v)
{
  visionEnabled = v;
  if (visionEnabled == true) {
    visualCursor->setEnabled(true);
  }
  else {
    visualCursor->setEnabled(false);
  }
 // visualCursor->setEnabled(visionEnabled);
}

void cCST::setHapticEnabled(bool h)
{
  hapticEnabled = h;
 // hapticCursor->setEnabled(hapticEnabled);
}

void cCST::startCST()
{
  running = true;
}

void cCST::stopCST()
{
  running = false;
}

void cCST::destructCST()
{
  world->deleteChild(visualCursor);
}
