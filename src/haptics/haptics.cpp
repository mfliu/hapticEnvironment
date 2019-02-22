#include "haptics.h"

HapticData hapticsData;
extern GraphicsData graphicsData;
extern ControlData controlData;
void initHaptics(void)
{
  hapticsData.handler = new cHapticDeviceHandler();
  hapticsData.handler->getDevice(hapticsData.hapticDevice, 0);
  hapticsData.hapticDeviceInfo = hapticsData.hapticDevice->getSpecifications();

  hapticsData.tool = new cToolCursor(graphicsData.world);
  graphicsData.world->addChild(hapticsData.tool);
  hapticsData.tool->setHapticDevice(hapticsData.hapticDevice);
  hapticsData.tool->setRadius(0.03);
  hapticsData.tool->setWorkspaceRadius(1.0);
  hapticsData.tool->setWaitForSmallForce(false);
  hapticsData.tool->start();
}

void updateHaptics(void)
{
  cPrecisionClock clock;
  clock.reset();
  controlData.simulationRunning = true;
  controlData.simulationFinished = false;
  cVector3d angVel(0.0, 0.0, 0.1);
  while (controlData.simulationRunning){
    clock.stop();
    double timeInterval = clock.getCurrentTimeSeconds();
    clock.reset();
    clock.start();
    graphicsData.world->computeGlobalPositions(true);
    hapticsData.tool->updateFromDevice();
    hapticsData.tool->computeInteractionForces();
    hapticsData.tool->applyToDevice();

    const double INERTIA = 0.4;
    const double MAX_ANGLE_VEL = 10.0;
    const double DAMPING = 0.1;

    cVector3d toolPos = hapticsData.tool->getDeviceGlobalPos();
    cVector3d objectPos = graphicsData.object->getGlobalPos();
    cVector3d v = cSub(toolPos, objectPos);
    cVector3d angAcc(0, 0, 0);
    if (v.length() > 0.0) {
      cVector3d toolForce = cNegate(hapticsData.tool->getDeviceGlobalForce());
      cVector3d force = toolForce - cProject(toolForce, v);
      cVector3d torque = cMul(v.length(), cCross(cNormalize(v), force));
      angAcc = (1.0 / INERTIA) * torque;
    }
    angVel.add(timeInterval * angAcc);
    double vel = angVel.length();
    if (vel > MAX_ANGLE_VEL) {
      angAcc.mul(MAX_ANGLE_VEL / vel);
    }
    angVel.mul(1.0-DAMPING * timeInterval);
    if (hapticsData.tool->getUserSwitch(0) == 1) {
      angVel.zero();
    }
    if (angVel.length() > C_SMALL) {
      graphicsData.object->rotateAboutGlobalAxisRad(cNormalize(angVel), timeInterval * angVel.length());
    }
    
  }
}
