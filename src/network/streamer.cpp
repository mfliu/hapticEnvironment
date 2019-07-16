#include "streamer.h"

#include "haptics/haptics.h"
#include "network.h"

using namespace chai3d;
using namespace std;

extern ControlData controlData;
extern HapticData hapticsData;

void startStreamer(void)
{
  controlData.streamerThread = new cThread();
  controlData.streamerThread->start(updateStreamer, CTHREAD_PRIORITY_HAPTICS);
  controlData.streamerUp = true;
}

void updateStreamer(void)
{
  cVector3d pos;
  cVector3d vel;
  cVector3d force;

  double posX = 0, posY = 0, posZ = 0;
  double velX = 0, velY = 0, velZ = 0;
  double forceX = 0, forceY = 0, forceZ = 0;

  cPrecisionClock clock;
  while (controlData.simulationRunning)
  {
    pos = hapticsData.tool->getDeviceGlobalPos();
    posX = pos.x();
    posY = pos.y();
    posZ = pos.z();

    vel = hapticsData.tool->getDeviceGlobalLinVel();
    velX = vel.x();
    velY = vel.y();
    velZ = vel.z();

    force = hapticsData.tool->getDeviceGlobalForce();
    forceX = force.x();
    forceY = force.y();
    forceZ = force.z();
    
    M_HAPTIC_DATA_STREAM toolData;
    memset(&toolData, 0, sizeof(toolData)); 
    auto packetIdx = controlData.client->async_call("getMsgNum");
    auto timestamp = controlData.client->async_call("getTimestamp");
    packetIdx.wait();
    timestamp.wait();
    auto packetNum = packetIdx.get().as<int>();
    auto currTime = timestamp.get().as<double>();
    toolData.header.serial_no = packetNum;
    toolData.header.msg_type = HAPTIC_DATA_STREAM;
    toolData.header.timestamp = currTime;
    toolData.posX = posX;
    toolData.posY = posY;
    toolData.posZ = posZ;
    toolData.velX = velX;
    toolData.velY = velY;
    toolData.velZ = velZ;
    toolData.forceX = forceX;
    toolData.forceY = forceY;
    toolData.forceZ = forceZ;
    
    char collisions[4][MAX_STRING_LENGTH];
    memset(&collisions, 0, sizeof(collisions));
    int collisionIdx = 0;
    unordered_map<string, cGenericObject*>::iterator objectItr;
    for (objectItr = controlData.objectMap.begin(); objectItr != controlData.objectMap.end(); objectItr++)
    {
      if (hapticsData.tool->isInContact(objectItr->second)) {
        int n = objectItr->first.length();
        char objectName[n+1];
        strcpy(objectName, objectItr->first.c_str());
        memcpy(&(collisions[collisionIdx]), objectName, sizeof(collisions[collisionIdx]));
        collisionIdx++;
      }
    }
    memcpy(&(toolData.collisions), collisions, sizeof(toolData.collisions));
    char* packet[sizeof(toolData)];
    memcpy(&packet, &toolData, sizeof(toolData));
   
    sendPacket((char *) packet, sizeof(packet), true);
    if (controlData.loggingData == true)
    {
      controlData.dataFile.write((const char*) packet, sizeof(toolData));
    }
    usleep(500); // 1000 microseconds = 1 millisecond
  }
  closeMessagingSocket();
  controlData.streamerUp = false;
}


