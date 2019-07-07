#include "graphics.h"

extern HapticData hapticsData;
extern ControlData controlData;
GraphicsData graphicsData;

void initDisplay(void)
{
  graphicsData.stereoMode = C_STEREO_DISABLED;
  graphicsData.fullscreen = false;
  graphicsData.mirroredDisplay = false;
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

  if (graphicsData.stereoMode == C_STEREO_ACTIVE) {
    glfwWindowHint(GLFW_STEREO, GL_TRUE);
  }
  else {
    glfwWindowHint(GLFW_STEREO, GL_FALSE);
  }
  
  graphicsData.width = w;
  graphicsData.height = h;
  graphicsData.xPos = x;
  graphicsData.yPos = y;
  graphicsData.swapInterval = 1;  

  graphicsData.window = glfwCreateWindow(w, h, "CHAI3D", NULL, NULL);
  if (!graphicsData.window) {
    cout << "Failed to create window" << endl;
    cSleepMs(1000);
    glfwTerminate();
    return;
  }
  
  glfwGetWindowSize(graphicsData.window, &graphicsData.width, &graphicsData.height);
  glfwSetWindowPos(graphicsData.window, graphicsData.xPos, graphicsData.yPos);
  glfwSetKeyCallback(graphicsData.window, keySelectCallback);
  glfwSetWindowSizeCallback(graphicsData.window, resizeWindowCallback);
  glfwMakeContextCurrent(graphicsData.window);
  glfwSwapInterval(graphicsData.swapInterval);

#ifdef GLEW_VERSION
  if(glewInit() != GLEW_OK) {
    cout << "Failed to initialize GLEW library" << endl;
    glfwTerminate();
    return;
  }
#endif
}

void initScene(void)
{
  graphicsData.world = new cWorld();
  graphicsData.world->m_backgroundColor.setBlack();
  graphicsData.camera = new cCamera(graphicsData.world);
  graphicsData.world->addChild(graphicsData.camera);
  graphicsData.camera->set(cVector3d(100.0, 0.0, 0.0),
                       cVector3d(0.0, 0.0, 0.0),
                       cVector3d(0.0, 0.0, 1.0));
  graphicsData.camera->setClippingPlanes(10.0, -10.0);
  graphicsData.camera->setStereoMode(graphicsData.stereoMode);
  graphicsData.camera->setStereoEyeSeparation(0.03);
  graphicsData.camera->setStereoFocalLength(50.0);
  graphicsData.camera->setMirrorVertical(graphicsData.mirroredDisplay);
  

  graphicsData.light = new cDirectionalLight(graphicsData.world);
  graphicsData.camera->addChild(graphicsData.light); 
  graphicsData.light->setEnabled(true);
  graphicsData.light->setLocalPos(0.0, 0.5, 0.0);
  graphicsData.light->setDir(-3.0, -0.5, 0.0);
}

void resizeWindowCallback(GLFWwindow* a_window, int a_width, int a_height)
{
  graphicsData.width = a_width;
  graphicsData.height = a_height;
}

void errorCallback(int error, const char* errorDescription)
{
  cout << "Error: " << errorDescription << endl;
}

void keySelectCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if ((action != GLFW_PRESS) && (action != GLFW_REPEAT)) {
    return;
  }
  else if ((key == GLFW_KEY_ESCAPE) || (key == GLFW_KEY_Q)) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
  else if(key == GLFW_KEY_F) {
    graphicsData.fullscreen = !graphicsData.fullscreen;
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    if (graphicsData.fullscreen) {
      glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
      glfwSwapInterval(graphicsData.swapInterval);
    }
    else {
      int w = 0.8 * mode->height;
      int h = 0.5 * mode->height;
      int x = 0.5 * (mode->width - w);
      int y = 0.5 * (mode->height - h);
      graphicsData.width = w;
      graphicsData.height = h;
      graphicsData.xPos = x;
      graphicsData.yPos = y;
      glfwSetWindowMonitor(window, NULL, x, y, w, h, mode->refreshRate);
      glfwSwapInterval(graphicsData.swapInterval);
    }
  }
  else {
    const char* key_name;
    if (key == 32) {
      key_name = "space";
    }
    else {
      key_name = glfwGetKeyName(key, 0);
    }
    M_KEYPRESS keypressEvent;
    memset(&keypressEvent, 0, sizeof(keypressEvent));
    auto packetNum = controlData.client->call("getMsgNum").as<int>();
    auto currTime = controlData.client->call("getTimestamp").as<double>();
    keypressEvent.header.serial_no = packetNum;
    keypressEvent.header.msg_type = KEYPRESS;
    keypressEvent.header.timestamp = currTime;
    memcpy(&(keypressEvent.keyname), key_name, sizeof(keypressEvent.keyname));
    char* packet[sizeof(keypressEvent)];
    memcpy(&packet, &keypressEvent, sizeof(keypressEvent));
    //auto res = controlData.client->call("sendMessage", (char* const) packet).as<int>();
    sendPacket((char *) packet, sizeof(packet), false);
    //if (res == 1) {
    cout << "Sent KEYPRESS message" << endl;
    //}
    //else {
    //  cout << "Error sending KEYPRESS message" << endl;
    //}
  }
}

void updateGraphics(void)
{
  graphicsData.world->updateShadowMaps(false, graphicsData.mirroredDisplay);
  graphicsData.camera->renderView(graphicsData.width, graphicsData.height);
  for(vector<cGenericMovingObject*>::iterator it = graphicsData.movingObjects.begin(); it != graphicsData.movingObjects.end(); it++)
  {
    double dt = (clock() - graphicsData.graphicsClock)/double(CLOCKS_PER_SEC);
    graphicsData.graphicsClock = clock();
    (*it)->graphicsLoopFunction(dt, hapticsData.tool->getDeviceGlobalPos(), hapticsData.tool->getDeviceGlobalLinVel());
  }
  glFinish();
  GLenum err = glGetError();
  if (err != GL_NO_ERROR) {
    cout << "Error: " << gluErrorString(err) << endl;
  }
}
