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
  controlData.STREAMER_IP = "127.0.0.1";
  controlData.STREAMER_PORT = 10000;
  controlData.MESSENGER_IP = "127.0.0.1";
  controlData.MESSENGER_PORT = 2000;
  controlData.totalPackets = 0;

  initDisplay();
  initScene();
  initHaptics();
  initTask();
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
    case GRAPHICS_SHAPE_SPHERE:
      cout << "RECEIVED GRAPHICS_SHAPE_SPHERE Message" << endl;
      M_GRAPHICS_SHAPE_SPHERE sphere;
      memcpy(&sphere, packet, sizeof(sphere));
      cShapeSphere* sphereObj = new cShapeSphere(sphere.radius);
      graphicsData.world->addChild(sphereObj);
      sphereObj->setLocalPos(sphere.localPosition[0], sphere.localPosition[1], sphere.localPosition[2]);
      sphereObj->m_material->setColorf(sphere.color[0], sphere.color[1], sphere.color[2], sphere.color[3]);
      sphereObj->m_material->setStiffness(0);
      cEffectSurface* newEffect = new cEffectSurface(sphereObj);
      sphereObj->addEffect(newEffect);
      break;
  }
}
