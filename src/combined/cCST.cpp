#include "cCST.h"
extern ControlData controlData;
extern HapticData hapticsData;
extern GraphicsData graphicsData;

cCST::cCST(cWorld* worldPtr, double l, double f, bool v, bool h):cGenericMovingObject(), cGenericEffect(worldPtr)
{
  world = worldPtr;
  lambda = l/60 + 1;
  forceMagnitude = f;
  visionEnabled = v;
  hapticEnabled = h;
  running = false;
  currPos = new cVector3d(0.0, 0.0, 0.0);
  
  // Visual Cursor
  visualCursor = new cShapeSphere(2);
  visualCursor->m_material->setColorf(0.0, 0.75, 1.0);
  visualCursor->setLocalPos(*currPos);
  visualCursor->setEnabled(false);
  world->addChild(visualCursor);
  
  // Clock to prevent updates from happening too quickly
  cstClock = new cPrecisionClock();
  lastUpdateTime = 0.0;
}

cVector3d* cCST::computeNextPosition(cVector3d toolPos)
{
  double cstTime = cstClock->getCurrentTimeSeconds();
  if (cstTime - lastUpdateTime >= 0.01)
  {
    double cursorY = currPos->y();
    double toolY = toolPos.y();
    double nextY = (lambda * cursorY) + ((lambda-1) * toolY);
    //cout << cursorY << ", " << nextY << endl;
    cVector3d* nextPos = new cVector3d(0.0, nextY, 0.0);
    currPos->y(nextPos->y());
    lastUpdateTime = cstTime;
    
    M_CST_DATA cstData;
    memset(&cstData, 0, sizeof(cstData));
    auto packetNum = controlData.client->call("getMsgNum").as<int>();
    auto currTime = controlData.client->call("getTimestamp").as<double>();
    cstData.header.serial_no = packetNum;
    cstData.header.msg_type = CST_DATA;
    cstData.header.timestamp = currTime;
    cstData.cursorX = currPos->x();
    cstData.cursorY = currPos->y();
    cstData.cursorZ = currPos->z();
    char* packet[sizeof(cstData)];
    memcpy(&packet, &cstData, sizeof(cstData));
    sendPacket((char *) packet, sizeof(cstData), false);
    usleep(500);
    return nextPos;
  }
  else {
    usleep(500);
    return currPos;
  }
}

bool cCST::computeForce(const cVector3d& a_toolPos, const cVector3d& a_toolVel,
                  const unsigned int& a_toolID, cVector3d& a_reactionForce)
{
  usleep(1000);
  if (hapticEnabled == true && running == true) {
    cVector3d* nextPos = cCST::computeNextPosition(a_toolPos);
    double forceMark = forceMagnitude * (nextPos->y()/120);
    if (forceMark > 8.0) {
      a_reactionForce.y(8.0);
    }
    else {
      a_reactionForce.y(forceMagnitude * (nextPos->y()/120));
    }
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
void cCST::setVisionEnabled(bool v)
{
  visionEnabled = v;
  if (visionEnabled == true) {
    visualCursor->setEnabled(true);
  }
  else {
    visualCursor->setEnabled(false);
  }
}

void cCST::setHapticEnabled(bool h)
{
  hapticEnabled = h;
}

bool cCST::setLambda(double l)
{
  if (running == true) {
    return false;
  }
  else {
    lambda = l/60 + 1;
    return true;
  }
}

void cCST::startCST()
{
  running = true;
  cstClock->start();
  lastUpdateTime = cstClock->getCurrentTimeSeconds();
}

void cCST::stopCST()
{
  running = false;
  cstClock->stop();
  cstClock->reset();
  currPos->x(0.0);
  currPos->y(0.0);
  currPos->z(0.0);
}

void cCST::destructCST()
{
  world->deleteChild(visualCursor);
}
