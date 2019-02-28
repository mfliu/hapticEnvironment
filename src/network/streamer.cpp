#include "streamer.h"

#include "haptics/haptics.h"
#include "network.h"

using namespace chai3d;
using namespace std;

extern ControlData controlData;
extern HapticData hapticsData;

void streamStart(void)
{
  openStreamingSocket(controlData.STREAMER_IP, controlData.STREAMER_PORT);
  controlData.streamerThread = new cThread();
  controlData.streamerThread->start(streamUpdate, CTHREAD_PRIORITY_HAPTICS);
}

void streamClose()
{
 closeStreamingSocket(); 
}

void streamUpdate(void)
{
  double currTime;
  cVector3d pos;
  cVector3d vel;
  cVector3d force;

  double posX = 0, posY = 0, posZ = 0;
  double velX = 0, velY = 0, velZ = 0;
  double forceX = 0, forceY = 0, forceZ = 0;

  cPrecisionClock clock;
  while (controlData.simulationRunning)
  {
    currTime = clock.getCurrentTimeSeconds();  
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
    
    M_HAPTIC_DATA_STREAM emgData;
    memset(&emgData, 0, sizeof(emgData));  
    emgData.header.serial_no = (int) controlData.totalPackets;
    emgData.header.msg_type = HAPTIC_DATA_STREAM;
    emgData.header.timestamp = currTime;
    emgData.posX = posX;
    emgData.posY = posY;
    emgData.posZ = posZ;
    emgData.velX = velX;
    emgData.velY = velY;
    emgData.velZ = velZ;
    emgData.forceX = forceX;
    emgData.forceY = forceY;
    emgData.forceZ = forceZ;
    
    char* packet[sizeof(emgData)];
    memcpy(&packet, &emgData, sizeof(emgData));
   
    sendPacket((char *) packet, sizeof(packet), true);
    usleep(1000); // 1000 microseconds = 1 millisecond
  }
  closeStreamingSocket();
}
