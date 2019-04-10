#pragma once 

#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_  

#include "chai3d.h"
#include "GLFW/glfw3.h"
#include <atomic>
#include <string>
#include <unordered_map>
#include <vector>
#include "network/network.h"
#include "network/sender.h"
#include "network/listener.h"
#include "haptics/haptics.h"
#include "graphics/graphics.h"
#include <fstream>

using namespace chai3d;
using namespace std;

struct ControlData
{
  // State variables
  bool simulationRunning;
  bool simulationFinished;
  bool hapticsUp;
  bool listenerUp;
  bool dataSenderUp;
  bool dataLoggerUp;
  
  // Messaging and Data Logging Variables
  const char* SENDER_IP;
  const char* LISTENER_IP;
  int DATA_PORT;
  int DATA_LOG_PORT;
  int SENDER_PORT;
  int LISTENER_PORT;
  int sender_socket;
  int listener_socket;
  int data_socket;
  int dataLog_socket;
  cThread* dataThread; // for streaming haptic data only
  cThread* dataLogThread;
  cThread* listenerThread;
  ofstream dataFile;

  // TODO: Make the hapticsOnly = true mode actually work
  bool hapticsOnly;
  
  //Object Tracking
  unordered_map<string, cGenericObject*> objectMap;
  unordered_map<string, vector<string>> objectEffects;
  unordered_map<string, cGenericEffect*> worldEffects;
};

bool allThreadsDown(void);
void close(void);
void parsePacket(char* packet);
#endif
