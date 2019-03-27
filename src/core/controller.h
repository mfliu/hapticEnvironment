#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_  

#include "chai3d.h"
#include "GLFW/glfw3.h"
#include <atomic>
#include <string>
#include <unordered_map>
#include <vector>
#include "network/network.h"
#include "network/streamer.h"
#include "network/messenger.h"
#include "haptics/haptics.h"
#include "graphics/graphics.h"

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
  unordered_map<string, vector<string>> objectEffects;
  unordered_map<string, cGenericEffect*> worldEffects;
  //vector<cGenericMovingObject*> movingObjects;
};
void close(void);
void parsePacket(char* packet);
#endif
