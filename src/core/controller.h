#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_  

#include "chai3d.h"
#include "GLFW/glfw3.h"

struct ControlData
{
  bool simulationRunning;
  bool simulationFinished;
};
void close(void);
#endif
