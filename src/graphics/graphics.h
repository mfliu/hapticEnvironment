#ifndef _GRAPHICS_H_INCLUDED_
#define _GRAPHICS_H_INCLUDED_ 


#include <vector>
#include "chai3d.h"
#include <GLFW/glfw3.h>
#include "haptics/haptics.h"

using namespace chai3d; 
using namespace std; 

struct GraphicsData {
  cStereoMode stereoMode;
  bool fullscreen;
  bool mirroredDisplay;
  cWorld* world;
  cCamera* camera;
  cSpotLight* light;
  GLFWwindow* window;
  int width;
  int height;
  int xPos;
  int yPos;
  int swapInterval;
  cShapeTorus* object;
  cFrequencyCounter freqCounterGraphics;
};

void initDisplay(void);
void initScene(void);
void errorCallback(int error, const char* errorDescription);
void resizeWindowCallback(GLFWwindow* window, int w, int h);
void keySelectCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void updateGraphics(void);

#endif
