#include "haptics.h"

HapticData hapticsData;
extern GraphicsData graphicsData;
extern ControlData controlData;
void initHaptics(void)
{
  hapticsData.handler = new cHapticDeviceHandler();
  hapticsData.handler->getDevice(hapticsData.hapticDevice, 0);
  hapticsData.hapticDeviceInfo = hapticsData.hapticDevice->getSpecifications();
  hapticsData.toolRadius = HAPTIC_TOOL_RADIUS;

  hapticsData.tool = new cToolCursor(graphicsData.world);
  graphicsData.world->addChild(hapticsData.tool);
  hapticsData.tool->setHapticDevice(hapticsData.hapticDevice);
  hapticsData.tool->setRadius(HAPTIC_TOOL_RADIUS);
  hapticsData.tool->setWorkspaceRadius(100.0);
  hapticsData.tool->setWorkspaceScaleFactor(WORKSPACE_SCALE_FACTOR);
  hapticsData.tool->setWaitForSmallForce(false);
  hapticsData.tool->start();
  cout << "Haptics tool initialized" << endl;
}

void startHapticsThread(void)
{
  hapticsData.hapticsThread = new cThread();
  hapticsData.hapticsThread->start(updateHaptics, CTHREAD_PRIORITY_HAPTICS);
  controlData.simulationRunning = true;
  controlData.simulationFinished = false;
  controlData.hapticsUp = true;
  cout << "Haptics thread started" << endl;
}

void updateHaptics(void)
{
  cPrecisionClock clock;
  clock.reset();
  cVector3d angVel(0.0, 0.0, 0.1);
  usleep(500); // give some time for other threads to start up
  while (controlData.simulationRunning){
    clock.stop();
    double timeInterval = clock.getCurrentTimeSeconds();
    clock.reset();
    clock.start();
    graphicsData.world->computeGlobalPositions(true);
    hapticsData.tool->updateFromDevice();
    hapticsData.tool->computeInteractionForces();
    hapticsData.tool->applyToDevice();
  }
  controlData.hapticsUp = false;
}
