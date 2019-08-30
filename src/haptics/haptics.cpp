#include "haptics.h"

/**
 * @file haptics.h 
 * @file haptics.cpp
 *
 * @brief Functions for haptic control.
 *
 * This file contains the initialization of the haptic device, as well as the update function that
 * is called in the haptics thread. 
 */

HapticData hapticsData;
extern GraphicsData graphicsData;
extern ControlData controlData;

/**
 * @brief Initializes the haptic thread. 
 *
 * Contains some custom scale factors depending on which device is
 * used (Falcon or delta.3)
 */
void initHaptics(void)
{
  hapticsData.handler = new cHapticDeviceHandler();
  hapticsData.handler->getDevice(hapticsData.hapticDevice, 0);
  hapticsData.hapticDeviceInfo = hapticsData.hapticDevice->getSpecifications();
  
  double workspaceScaleFactor;
  double forceScaleFactor;

  if (hapticsData.hapticDeviceInfo.m_model == C_HAPTIC_DEVICE_FALCON) {
    workspaceScaleFactor = 3000;
    forceScaleFactor = 3000;
    cout << "Falcon Detected." << endl;
  }
  else if (hapticsData.hapticDeviceInfo.m_model == C_HAPTIC_DEVICE_DELTA_3) {
    workspaceScaleFactor = 1000;
    forceScaleFactor = 1000;
    cout << "Delta Detected." << endl;
  }
  else {
    workspaceScaleFactor = 1000;
    forceScaleFactor = 1000;
    //hapticsData.hapticDevice->close();
    cout << "Device not recognized." << endl;
  }
  
  hapticsData.tool = new cToolCursor(graphicsData.world);
  hapticsData.tool->m_hapticPoint->m_sphereProxy->m_material->setRed();
  graphicsData.world->addChild(hapticsData.tool);
  hapticsData.tool->setHapticDevice(hapticsData.hapticDevice);
  hapticsData.tool->setRadius(HAPTIC_TOOL_RADIUS);
  hapticsData.tool->setWorkspaceScaleFactor(workspaceScaleFactor);
  hapticsData.tool->setWaitForSmallForce(false);
  hapticsData.tool->start();
  
  hapticsData.maxForce = hapticsData.hapticDeviceInfo.m_maxLinearForce;
  cout << "Haptics tool initialized" << endl;
}

/**
 * @brief Starts the haptic thread. 
 *
 * Stores a pointer to the haptic thread in the ControlData struct
 */
void startHapticsThread(void)
{
  hapticsData.hapticsThread = new cThread();
  hapticsData.hapticsThread->start(updateHaptics, CTHREAD_PRIORITY_HAPTICS);
  controlData.simulationRunning = true;
  controlData.simulationFinished = false;
  controlData.hapticsUp = true;
  cout << "Haptics thread started" << endl;
}

/**
 * @brief Haptic update function 
 *
 * This function is called on each iteration of the haptic loop. It computes the global and local
 * positions of the device and renders any forces based on objects in the Chai3d world
 */
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
    cVector3d pos = hapticsData.tool->getDeviceLocalPos();
    //cout << pos.x() << ", " << pos.y() << ", " << pos.z() << endl;
    hapticsData.tool->updateFromDevice();
    hapticsData.tool->computeInteractionForces();
    hapticsData.tool->applyToDevice();
  }
  controlData.hapticsUp = false;
}
