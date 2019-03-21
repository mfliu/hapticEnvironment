#include "controller.h"
#include "network/network.h"
#include "network/streamer.h"
#include "network/messenger.h"
#include "haptics/haptics.h"
#include "graphics/graphics.h"


extern HapticData hapticsData;
extern GraphicsData graphicsData;
ControlData controlData;

int main(int argc, char* argv[])
{
  cout << endl;
  cout << "-----------------------------------" << endl;
  cout << "CHAI3D" << endl;
  cout << "-----------------------------------" << endl << endl << endl;
  cout << "Keyboard Options:" << endl << endl;
  cout << "[f] - Enable/Disable full screen mode" << endl;
  cout << "[q] - Exit application" << endl;
  cout << endl << endl;
  
  controlData.simulationRunning = false;
  controlData.simulationFinished = true;
  controlData.totalPackets = 0;

  // TODO: Set these IP addresses from a config file
  controlData.STREAMER_IP = "127.0.0.1";
  controlData.STREAMER_PORT = 10000;
  controlData.MESSENGER_IP = "127.0.0.1";
  controlData.MESSENGER_PORT = 2000;
  controlData.hapticsOnly = false;
  
  if (controlData.hapticsOnly == false) {
    initDisplay();
    initScene();
  }
  initHaptics();
  //initTask();
  startHapticsThread(); 
  streamStart(); 
  startMessenger();
  atexit(close);
  resizeWindowCallback(graphicsData.window, graphicsData.width, graphicsData.height);
  while (!glfwWindowShouldClose(graphicsData.window)) {
    glfwGetWindowSize(graphicsData.window, &graphicsData.width, &graphicsData.height);
    updateGraphics();
    glfwSwapBuffers(graphicsData.window);
    glfwPollEvents();
    graphicsData.freqCounterGraphics.signal(1);
  }
  glfwDestroyWindow(graphicsData.window);
  glfwTerminate();
  return(0);
}

void close()
{
  // TODO: Make thread exits more graceful, currently hangs
  controlData.simulationRunning = false;
  while (!controlData.simulationFinished) {
    cSleepMs(100);
  }
  hapticsData.tool->stop();
  delete hapticsData.hapticsThread;
  delete graphicsData.world;
  delete hapticsData.handler;
  closeAllConnections();
}

void parsePacket(char* packet)
{
  MSG_HEADER header;
  memcpy(&header, packet, sizeof(header));
  int msgType = header.msg_type;
  switch (msgType)
  {
    case HAPTICS_SET_ENABLED:
    {
      cout << "Received HAPTICS_SET_ENABLED Message" << endl;
      M_HAPTICS_SET_ENABLED hapticsEnabled;
      memcpy(&hapticsEnabled, packet, sizeof(hapticsEnabled));
      char* objectName;
      objectName = hapticsEnabled.objectName;
      if (controlData.objectMap.find(objectName) == controlData.objectMap.end()) {
        cout << objectName << " not found" << endl;
      }
      else {
        if (hapticsEnabled.enabled == 1) {
          controlData.objectMap[objectName]->setHapticEnabled(true);
        }
        else if (hapticsEnabled.enabled == 0) {
          controlData.objectMap[objectName]->setHapticEnabled(false);
        }
      }
      break;
    }
    case HAPTICS_SET_STIFFNESS:
    {
      cout << "Received HAPTICS_SET_STIFFNESS Message" << endl;
      M_HAPTICS_SET_STIFFNESS stiffness;
      memcpy(&stiffness, packet, sizeof(stiffness));
      char* objectName;
      objectName = stiffness.objectName;
      if (controlData.objectMap.find(objectName) == controlData.objectMap.end()) {
        cout << objectName << " not found" << endl;
      }
      else {
        controlData.objectMap[objectName]->m_material->setStiffness(stiffness.stiffness);
      }
      break;
    }

    case HAPTICS_BOUNDING_PLANE:
    {
      cout << "Received HAPTICS_BOUNDING_PLANE Message" << endl;
      int stiffness = hapticsData.hapticDeviceInfo.m_maxLinearStiffness;
      double toolRadius = hapticsData.toolRadius;
      cBoundingPlane* bp = new cBoundingPlane(stiffness, toolRadius);
      graphicsData.world->addChild(bp->getLowerBoundingPlane());
      graphicsData.world->addChild(bp->getUpperBoundingPlane());
      graphicsData.world->addChild(bp->getTopBoundingPlane());
      graphicsData.world->addChild(bp->getBottomBoundingPlane());
      graphicsData.world->addChild(bp->getLeftBoundingPlane());
      graphicsData.world->addChild(bp->getRightBoundingPlane());
      break;
    }
    
    case HAPTICS_CONSTANT_FORCE_FIELD:
    {
      cout << "Received HAPTICS_CONSTANT_FORCE_FIELD Message" << endl;
      M_HAPTICS_CONSTANT_FORCE_FIELD cffInfo;
      memcpy(&cffInfo, packet, sizeof(cffInfo));
      double d = cffInfo.direction;
      double m = cffInfo.magnitude;
      cConstantForceFieldEffect* cFF = new cConstantForceFieldEffect(graphicsData.world, d, m);
      graphicsData.world->addEffect(cFF);
      break;
    }
    case GRAPHICS_SET_ENABLED:
    {
      cout << "Received GRAPHICS_SET_ENABLED Message" << endl;
      M_GRAPHICS_SET_ENABLED graphicsEnabled;
      memcpy(&graphicsEnabled, packet, sizeof(graphicsEnabled));
      char* objectName;
      objectName = graphicsEnabled.objectName;
      int enabled = graphicsEnabled.enabled;
      if (controlData.objectMap.find(objectName) == controlData.objectMap.end()) {
        cout << objectName << " not found" << endl;
      }
      else {
        if (graphicsEnabled.enabled == 1) {
          controlData.objectMap[objectName]->setShowEnabled(true);
        }
        else if (graphicsEnabled.enabled == 0) {
          controlData.objectMap[objectName]->setShowEnabled(false);
        }
      }
      break;
    }

    case GRAPHICS_SHAPE_SPHERE: 
    {
      cout << "Received GRAPHICS_SHAPE_SPHERE Message" << endl;
      M_GRAPHICS_SHAPE_SPHERE sphere;
      memcpy(&sphere, packet, sizeof(sphere));
      cShapeSphere* sphereObj = new cShapeSphere(sphere.radius);
      graphicsData.world->addChild(sphereObj);
      sphereObj->setLocalPos(sphere.localPosition[0], sphere.localPosition[1], sphere.localPosition[2]);
      sphereObj->m_material->setColorf(sphere.color[0], sphere.color[1], sphere.color[2], sphere.color[3]);
      cEffectSurface* sphereEffect = new cEffectSurface(sphereObj);
      sphereObj->addEffect(sphereEffect);
      controlData.objectMap[sphere.objectName] = sphereObj;
      break;
    }
    case GRAPHICS_SHAPE_TORUS:
    {
      cout << "Received GRAPHICS_SHAPE_TORUS Message" << endl;
      M_GRAPHICS_SHAPE_TORUS torus;
      memcpy(&torus, packet, sizeof(torus));
      cShapeTorus* torusObj = new cShapeTorus(torus.innerRadius, torus.outerRadius);
      graphicsData.world->addChild(torusObj);
      torusObj->setLocalPos(0.0, 0.0, 0.0);
      torusObj->m_material->setStiffness(1.0);
      torusObj->m_material->setColorf(255.0, 255.0, 255.0, 1.0);
      cEffectSurface* torusEffect = new cEffectSurface(torusObj);
      torusObj->addEffect(torusEffect);
      controlData.objectMap[torus.objectName] = torusObj;
      break; 
    }
  }
}
