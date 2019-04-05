#include "messenger.h"
#include "haptics/haptics.h"
#include "network.h"
#include "core/controller.h"

using namespace chai3d;
using namespace std;

extern ControlData controlData;
extern HapticData hapticsData;

void startMessenger()
{
  openMessengerSocket(controlData.MESSENGER_IP, controlData.MESSENGER_PORT);
  controlData.messengerThread = new cThread();
  controlData.messengerThread->start(updateMessenger, CTHREAD_PRIORITY_HAPTICS);
}

void updateMessenger()
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
 closeMessengerSocket();
}

void closeMessenger()
{
  closeMessengerSocket();
}
