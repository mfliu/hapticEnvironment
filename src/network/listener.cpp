#include "listener.h"
#include "haptics/haptics.h"
#include "network.h"
#include "core/controller.h"

using namespace chai3d;
using namespace std;

extern ControlData controlData;
extern HapticData hapticsData;

void startListener()
{
  openMessageHandlerListenSocket(controlData.LISTENER_IP, controlData.LISTENER_PORT);
  controlData.listenerThread = new cThread();
  controlData.listenerThread->start(updateListener, CTHREAD_PRIORITY_HAPTICS);
}

void updateListener()
{
  //double currTime;
  //cPrecisionClock clock;
  char rawPacket[MAX_PACKET_LENGTH];
  char* packetPointer = rawPacket;

  while (controlData.simulationRunning)
  {
    //currTime = clock.getCurrentTimeSeconds();
    memset(rawPacket, 0, MAX_PACKET_LENGTH);
    int bytesRead = readPacket(packetPointer);
    if (bytesRead > 0) {
      //cout << "Bytes read " << bytesRead << endl;
      parsePacket(packetPointer);
    }
    usleep(1000); // 1000 microseconds = 1 millisecond
  }
 closeListenSocket();
}

void closeListener()
{
  closeListenSocket();
}
