#include "cCST.h"
extern ControlData controlData;
extern HapticData hapticsData;
extern GraphicsData graphicsData;

/**
 * @param worldPtr Pointer to the world 
 * @param l Lambda, or the instability parameter for CST 
 * @param f Magnitude of the force 
 * @param v Set whether visual feedback is enabled
 * @param h Set whether haptic feedback is enabled
 * 
 * Constructor for a CST object. A CST object inherits both from cGenericMovingObject and
 * cGenericEffect because the CST cursor is a graphical object whose position must be updated at
 * each iteration of the graphical loop. Similarly, cGenericEffect enables haptic feedback
 * rendering.  
 */
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

/**
 * @param toolPos Position of the haptic tool 
 * 
 * Given the position of the haptic tool, (the hand position \f$u(t)\f$, this function computes the
 * next position of the CST cursor \f$x(t)\f$. 
 */
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
    auto packetIdx = controlData.client->async_call("getMsgNum");
    auto timestamp = controlData.client->async_call("getTimestamp");
    packetIdx.wait();
    timestamp.wait();
    int packetNum = packetIdx.get().as<int>();
    double currTime = timestamp.get().as<double>();
    cstData.header.serial_no = packetNum;
    cstData.header.msg_type = CST_DATA;
    cstData.header.timestamp = currTime;
    cstData.cursorX = currPos->x();
    cstData.cursorY = currPos->y();
    cstData.cursorZ = currPos->z();
    char packet[sizeof(cstData)];
    memcpy(&packet, &cstData, sizeof(cstData));
    vector<char> packetData(packet, packet+sizeof(packet) / sizeof(char));
    auto cstInt = controlData.client->async_call("sendMessage", packetData, sizeof(cstData), controlData.MODULE_NUM);    
    //sendPacket((char *) packet, sizeof(cstData), false);
    usleep(1000);
    cstInt.wait();
    auto cstNum = cstInt.get().as<int>();
    return nextPos;
  }
  else {
    usleep(500);
    return currPos;
  }
}

/**
 * @param a_toolPos Position of the haptic tool 
 * @param a_toolVel Velocity of the haptic tool 
 * @param a_toolID ID number of the haptic tool 
 * @param a_reactionForce Vector for storing forces to be applied the haptic tool 
 *
 * This function takes the position of the CST cursor computed by computeNextPosition and determines
 * the force needed to be rendered to the user. 
 */
bool cCST::computeForce(const cVector3d& a_toolPos, const cVector3d& a_toolVel,
                  const unsigned int& a_toolID, cVector3d& a_reactionForce)
{
  usleep(1000);
  if (hapticEnabled == true && running == true) {
    cVector3d* nextPos = cCST::computeNextPosition(a_toolPos);
    double forceMark = (forceMagnitude * (hapticsData.maxForce) * (nextPos->y()/200) + 0.0);
    //double forceMark = forceMagnitude * 8.0 * (nextPos->y()/100);
    if (forceMark > 8.0) {
      a_reactionForce.y(8.0);
    }
    else {
      a_reactionForce.y(forceMark);
    }
    return true;
  }
  else {
    a_reactionForce.zero();
    return false;
  }
}

/**
 * @param dt Time increment since this function was last run 
 * @param toolPos Position of the haptic tool 
 * @param toolVel Velocity of the haptic tool 
 *
 * Since the CST cursor is a moving object and inherits from cGenericMovingObject, it must override
 * this function. This function updates the graphical rendering of the CST cursor based on the
 * position computed by the computeNextPosition function.
 */
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

/**
 * @param v Set whether visual feedback should be given. 
 *
 * True to enable visual feedback, false to disable visual feedback.
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
}

/**
 * @param h Set whether haptic feedback should be given.
 *
 * True to enable haptic feedback, false to disable haptic feedback.
 */
void cCST::setHapticEnabled(bool h)
{
  hapticEnabled = h;
}

/**
 * @param l Lambda 
 *
 * Set the value of the instability parameter \f$\lambda\f$
 */
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

/**
 * The CST object is instantiated for all trials of CST. This function enables the start of a new
 * trial of CST
 */
void cCST::startCST()
{
  running = true;
  cstClock->start();
  lastUpdateTime = cstClock->getCurrentTimeSeconds();
}

/**
 * Stops a CST trial.
 */
void cCST::stopCST()
{
  running = false;
  cstClock->stop();
  cstClock->reset();
  currPos->x(0.0);
  currPos->y(0.0);
  currPos->z(0.0);
}

/**
 * When a session of CST is complete, this is called to clear up memory allocated to the CST object
 * and remove it from the Chai3d world.
 */
void cCST::destructCST()
{
  world->deleteChild(visualCursor);
}
