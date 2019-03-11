#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_  

#include "chai3d.h"
#include "GLFW/glfw3.h"
#include <atomic>
#include <string>
#include <unordered_map>

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
  // TODO: Make the hapticsOnly = true mode actually work
  bool hapticsOnly;
  cThread* streamerThread;
  cThread* messengerThread;
  unordered_map<string, cGenericObject*> objectMap;
};
void close(void);
void parsePacket(char* packet);
#endif
