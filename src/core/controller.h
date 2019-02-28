#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_  

#include "chai3d.h"
#include "GLFW/glfw3.h"
#include <atomic>

using namespace chai3d;
using namespace std;

struct ControlData
{
  bool simulationRunning;
  bool simulationFinished;
  atomic_int totalPackets;
  const char* STREAMER_IP;
  const char* MESSENGER_IP;
  int MESSENGER_PORT;
  int STREAMER_PORT;
  int streamer_socket;
  int messenger_socket;
  cThread* streamerThread;
  cThread* messengerThread;
};
void close(void);
void parsePacket(char* packet);
#endif
