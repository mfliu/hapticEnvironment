#include "controller.h"
#include "haptics/haptics.h"
#include "graphics/graphics.h"

using namespace chai3d;
using namespace std;

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

  initDisplay();
  initScene();
  initHaptics();
  initTask();
  
    
  hapticsData.hapticsThread = new cThread();
  hapticsData.hapticsThread->start(updateHaptics, CTHREAD_PRIORITY_HAPTICS);

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
}
