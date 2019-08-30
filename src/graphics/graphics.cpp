#include "graphics.h"

/**
 * @file graphics.cpp
 * @brief Functions for setting up and starting the graphics loop.
 *
 * The graphics loop is the main loop of the program. Haptics runs in its own loop and messaging is
 * handled by a separate thread. The functions here are responsible for using GLFW libraries to
 * initialize and update the display.
 */

extern HapticData hapticsData;
extern ControlData controlData;
GraphicsData graphicsData;

/**
 * Creates and initializes a GLFW window, and stores a pointer to this window in the graphicsData
 * struct. This does not initialize the Chai3D graphics information. For that, @see initScene
 */

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

/**
 * Initializes the world for Chai3D. Also creates and sets the camera viewing angle and lighting.
 */
void initScene(void)
{
  graphicsData.world = new cWorld();
  graphicsData.world->m_backgroundColor.setBlack();
  graphicsData.camera = new cCamera(graphicsData.world);
  graphicsData.world->addChild(graphicsData.camera);
  graphicsData.camera->set(cVector3d(400.0, 0.0, 0.0),
                       cVector3d(0.0, 0.0, 0.0),
                       cVector3d(0.0, 0.0, 1.0));
  //graphicsData.camera->setClippingPlanes(10.0, -10.0);
  //graphicsData.camera->setStereoMode(graphicsData.stereoMode);
  //graphicsData.camera->setStereoEyeSeparation(0.03);
  //graphicsData.camera->setStereoFocalLength(50.0);
  graphicsData.camera->setMirrorVertical(graphicsData.mirroredDisplay);
  

  graphicsData.light = new cDirectionalLight(graphicsData.world);
  graphicsData.camera->addChild(graphicsData.light); 
  graphicsData.light->setEnabled(true);
  graphicsData.light->setLocalPos(0.0, 500.0, 0.0);
  graphicsData.light->setDir(0.0, -1.0, 0.0);
}

/**
 * @param a_window Pointer to GLFW window.
 * @param a_width Initial window width 
 * @param a_height Initial window height
 *
 * This function is called when the user resizes the window.
 */
void resizeWindowCallback(GLFWwindow* a_window, int a_width, int a_height)
{
  graphicsData.width = a_width;
  graphicsData.height = a_height;
}

/**
 * @param error Error code
 * @param errorDescription Error message
 *
 * This function is automatically called when a GLFW error is thrown
 */
void errorCallback(int error, const char* errorDescription)
{
  cout << "Error: " << errorDescription << endl;
}

/**
 * @param window Pointer to GLFW Window object 
 * @param key Name of the key that was pressed. Space key causes errors over message passing
 * @param scancode The scancode assigned to the key that was pressed 
 * @param action Type of action--should be GLFW_PRESS 
 * @param mods Modifier bits
 *
 * Callback function if a keyboard key is pressed. When a key is pressed, a message is sent to the
 * Trial Control module with a string representing the name of the key. 
 */
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
    auto res = controlData.client->call("sendMessage", (char* const) packet, sizeof(packet), controlData.MODULE_NUM).as<int>();
    //sendPacket((char *) packet, sizeof(packet), false);
    if (res == 1) {
      cout << "Sent KEYPRESS message" << endl;
    }
    else {
      cout << "Error sending KEYPRESS message" << endl;
    }
  }
}

/**
 * updateGraphics is called from the main loop and updates the graphics at each time step. Each
 * update involves updating the shadows and camera view, as well as rendering the updated position
 * of any moving objects. All moving objects must override the graphicsLoopFunction method.
 */
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
