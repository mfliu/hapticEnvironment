#include "cCups.h"
extern ControlData controlData;
extern HapticData hapticsData;
extern GraphicsData graphicsData;

/**
 * @param worldPtr Pointer to the world 
 * 
 */
cCups::cCups(cWorld* worldPtr, double esc, double l, double bM, double cM):cGenericMovingObject(), cGenericEffect(worldPtr)
{
  world = worldPtr;
  escapeTheta = esc;
  pendulumLength = l;
  ballMass = bM;
  cartMass = cM;
  gravity = 9.8;

  startTarget = new cVector3d(0.0, -100.0, 0.0);
  stopTarget = new cVector3d(0.0, 100.0, 0.0);
  ballPos = 0.0;
  ballVel = 0.0;
  ballForce = 0.0;

  //Start and stop boxes
  start = new cShapeBox(0.0, 2*pendulumLength, pendulumLength);
  start->setLocalPos(*startTarget);
  start->m_material->setColorf(0.0, 1.0, 0.0);
  world->addChild(start);

  stop = new cShapeBox(0.0, 2*pendulumLength, pendulumLength);
  stop->setLocalPos(*stopTarget);
  stop->m_material->setColorf(0.0, 1.0, 0.0);
  world->addChild(stop);

  cartPos = -100.0;
  cartVel = 0.0;
  cartAcc = 0.0;

  // Ball
  ball = new cShapeSphere(2);
  ball->m_material->setColorf(0.0, 0.75, 1.0);
  ball->m_material->setStiffness(hapticsData.hapticDeviceInfo.m_maxLinearStiffness);
  ball->createEffectSurface();
  ball->setLocalPos(*startTarget);
  ball->setEnabled(true);
  world->addChild(ball);
  
  //Cup 
  cupMesh = new cMesh();
  cMatrix3d* rotationY = new cMatrix3d(0.0, 0.0, -1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0);
  cMatrix3d* rotationZ = new cMatrix3d(cCosDeg(45), -cSinDeg(45), 0.0, cSinDeg(45), cCosDeg(45), 0.0, 0, 0, 1);
  cMatrix3d rotation = cMul(*rotationY, *rotationZ);
  cVector3d cupPos = *startTarget - cVector3d(0.0, cartPos, -pendulumLength+2);
  cCreateRingSection(cupMesh, 1, 1, pendulumLength, 90, true, 10, 10, cupPos, rotation, cColorf(1.0, 1.0, 0.0, 1.0));
  cupMesh->setEnabled(true);
  cupMesh->createEffectSurface();
  cupMesh->m_material->setStiffness(hapticsData.hapticDeviceInfo.m_maxLinearStiffness);
  world->addChild(cupMesh);

  running = false;

  
  // Clock to prevent updates from happening too quickly
  cupsClock = new cPrecisionClock();
  lastUpdateTime = 0.0;
}

bool cCups::computeForce(const cVector3d& a_toolPos, const cVector3d& a_toolVel,
                      const unsigned int& a_toolID, cVector3d& a_reactionForce)
{
  if (running == true) {
    double cupsTime = cupsClock->getCurrentTimeSeconds();
    double dt = cupsTime - lastUpdateTime;
    if (dt >= 0.01) {
      lastUpdateTime = cupsTime;
      cartPos = a_toolPos.y();
      double newCartVel = a_toolVel.y();
      cartAcc = 2.0 * floor(((newCartVel - cartVel)/dt)*100/100);
      cartVel = newCartVel;
      updateNextBallPosition(dt);
    }
    double fBall = ballMass * pendulumLength * (computeBallAcceleration(ballPos) * cCosDeg(ballPos) - (ballVel * ballVel) * cSinDeg(ballPos));
    //ballForce = fmod(fBall, hapticsData.hapticDeviceInfo.m_maxLinearForce);
    a_reactionForce.y(0.0*fBall);
    return true;
  }
  else {
    a_reactionForce.zero();
  }
  return true;
}

void cCups::graphicsLoopFunction(double dt, cVector3d toolPos, cVector3d toolVel)
{
  if (running == true) {
    // Update cart graphics
    cupMesh->setLocalPos(0.0, toolPos.y(), 0.0);

    // Update ball graphics

    double ballX = cartPos - pendulumLength * cSinDeg(ballPos);
    double ballY = pendulumLength - pendulumLength * cCosDeg(ballPos);
    cVector3d* newBallPos = new cVector3d(0.0, floor(ballX*100)/100, floor(ballY*100)/100);
    ball->setLocalPos(*newBallPos); //cVector3d(toolPos.x(), floor(ballX*100)/100, floor(ballY*100)/100));
  }
}

double cCups::computeBallAcceleration(double ballP) 
{
  double ballAcc = fmod((cartAcc/pendulumLength) * cCosDeg(ballP) - (gravity/pendulumLength) * cSinDeg(ballP), 360);
  return ballAcc;
}

void cCups::updateNextBallPosition(double dt)
{
  double ballPos1 = fmod(ballPos, 360);
  double ballVel1 = fmod(ballVel, 360);
  double ballAcc1 = computeBallAcceleration(ballPos1);

  double ballPos2 = fmod(ballPos + 0.5 * ballVel1 * dt, 360);
  double ballVel2 = fmod(ballVel + 0.5 * ballAcc1 * dt, 360);
  double ballAcc2 = computeBallAcceleration(ballPos2);

  double ballPos3 = fmod(ballPos2 + 0.5 * ballVel2 * dt, 360);
  double ballVel3 = fmod(ballVel2 + 0.5 * ballAcc2 * dt, 360);
  double ballAcc3 = computeBallAcceleration(ballPos3);

  double ballPos4 = fmod(ballPos + ballVel3 * dt, 360);
  double ballVel4 = fmod(ballVel + ballAcc3 * dt, 360);
  double ballAcc4 = computeBallAcceleration(ballPos4);

  double ballPosFinal = fmod(ballPos + (dt/6.0) * (ballVel1 + 2*ballVel2 + 2*ballVel3 + ballVel4), 360);
  double ballVelFinal = fmod(ballVel + (dt/6.0) * (ballAcc1 + 2*ballAcc2 + 2*ballAcc3 + ballAcc4), 360);
  double ballAccFinal = computeBallAcceleration(ballPosFinal);
  
  
  ballPos = ballPosFinal;
  ballVel = ballVelFinal;
  
  M_CUPS_DATA cupsData;
  memset(&cupsData, 0, sizeof(cupsData));
  auto packetIdx = controlData.client->async_call("getMsgNum");
  auto timestamp = controlData.client->async_call("getTimestamp");
  packetIdx.wait();
  timestamp.wait();
  int packetNum = packetIdx.get().as<int>();
  double currTime = timestamp.get().as<double>();
  cupsData.header.serial_no = packetNum;
  cupsData.header.msg_type = CUPS_DATA;
  cupsData.header.timestamp = currTime;
  cupsData.ballPos = ballPos;
  cupsData.cartPos = cartPos;
  char packet[sizeof(cupsData)];
  memcpy(&packet, &cupsData, sizeof(cupsData));
    vector<char> packetData(packet, packet+sizeof(packet) / sizeof(char));

  auto cupsInt = controlData.client->async_call("sendMessage", packetData, sizeof(cupsData), controlData.MODULE_NUM);    

}

void cCups::startCups()
{
  running = true;
  cupsClock->start();
  lastUpdateTime = cupsClock->getCurrentTimeSeconds();
}

void cCups::stopCups()
{
  running = false;
  ballPos = 0.0;
  ballVel = 0.0;
  ballForce = 0.0;
  cartPos = -100;
  cartVel = 0.0;
  cartAcc = 0.0;
  cupsClock->stop();
  cupsClock->reset();
}

void cCups::destructCups()
{
  world->deleteChild(ball);
  world->deleteChild(cupMesh);
}
