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
  // cout << "Packet size " << sizeof(&packet) << endl;
  // cout << "Header size " << sizeof(header) << endl;
  // cout << "Message type " << msgType << endl;
  switch (msgType)
  {
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
        cout << objectName << " found" << endl;
        controlData.objectMap[objectName]->m_material->setStiffness(stiffness.stiffness);
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
      sphereObj->m_material->setStiffness(0);
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
