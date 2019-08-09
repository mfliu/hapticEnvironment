#pragma once 
#include "GLFW/glfw3.h"
#include "chai3d.h"

using namespace chai3d;
using namespace std;

cWorld* world;
cCamera* camera;
cDirectionalLight* light;
GLFWwindow* window = NULL;
bool stereoMode = false;
bool mirroredDisplay = false;

int main(int argc, char* argv[])
{
  if (!glfwInit()) {
      cout << "Failed GLFW initialization" << endl;
      cSleepMs(1000);
      return;
    }
  
    glfwSetErrorCallback(errorCallback);

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    int w = 0.8 * mode->height;
    int h = 0.5 * mode->height;
    int x = 0.5 * (mode->width-w);
    int y = 0.5 * (mode->height-h);
  
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    if (stereoMode == C_STEREO_ACTIVE) {
     glfwWindowHint(GLFW_STEREO, GL_TRUE);
   }
    else {
      glfwWindowHint(GLFW_STEREO, GL_FALSE);
    }
  

    window = glfwCreateWindow(w, h, "CHAI3D", NULL, NULL);
    if (!window) {
      cout << "Failed to create window" << endl;
      cSleepMs(1000);
      glfwTerminate();
      return;
    }
  
    glfwGetWindowSize(window, w, h);
    glfwSetWindowPos(window, x, y);
    glfwSetKeyCallback(window, keySelectCallback);
    glfwSetWindowSizeCallback(window, resizeWindowCallback);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

#ifdef GLEW_VERSION
  if(glewInit() != GLEW_OK) {
    cout << "Failed to initialize GLEW library" << endl;
    glfwTerminate();
    return;
  }
#endif

  world = new cWorld();
  world->m_backgroundColor.setBlack();
  camera = new cCamera(graphicsData.world);
  world->addChild(graphicsData.camera);
  camera->set(cVector3d(400.0, 0.0, 0.0),
              cVector3d(0.0, 0.0, 0.0),
              cVector3d(0.0, 0.0, 1.0));
  camera->setClippingPlanes(10.0, -10.0);
  camera->setStereoMode(stereoMode);

  light = new cDirectionalLight(graphicsData.world);
  camera->addChild(light); 
  light->setEnabled(true);
  light->setLocalPos(0.0, 500.0, 0.0);
  light->setDir(0.0, -1.0, 0.0);

}
