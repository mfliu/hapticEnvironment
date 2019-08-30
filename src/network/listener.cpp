#include "listener.h"
#include "haptics/haptics.h"
#include "network.h"
#include "core/controller.h"

using namespace chai3d;
using namespace std;

/**
 * @file listener.h
 * @file listener.cpp
 * @brief Starts thread for listening to messages
 *
 * The Trial Control module should be the only module that sends messages to the robot
 * directly--every other module should interact with the Trial Control module. 
 */

extern ControlData controlData;
extern HapticData hapticsData;

/** 
 * Start the listening thread
 */
void startListener()
{
  controlData.listenerThread = new cThread();
  controlData.listenerThread->start(updateListener, CTHREAD_PRIORITY_HAPTICS);
  controlData.listenerUp = true;
}

/**
 * Upon receiving a packet, call the parsePacket function
 * @see parsePacket
 */
void updateListener()
{
  char rawPacket[MAX_PACKET_LENGTH];
  char* packetPointer = rawPacket;

  while (controlData.simulationRunning)
  {
    memset(rawPacket, 0, MAX_PACKET_LENGTH);
    int bytesRead = readPacket(packetPointer);
    if (bytesRead > 0) {
      //cout << "Bytes read " << bytesRead << endl;
      parsePacket(packetPointer);
    }
    usleep(100); // 1000 microseconds = 1 millisecond
  }
 closeMessagingSocket();
 controlData.listenerUp = false;
}

