#include "network.h"
#include "core/controller.h"

using namespace chai3d;
using namespace std;

extern ControlData controlData;


struct sockaddr_in messengerStruct, streamerStruct;
int streamLen = sizeof(streamerStruct);
int messengerLen = sizeof(messengerStruct);

void openStreamingSocket(const char* ipAddr, int port) 
{
  cout << "Opening streaming socket..." << endl;
  controlData.streamer_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (controlData.streamer_socket < 0) {
    cout << "Opening streaming socket failed." << endl;
    exit(1);
  }

  memset((char *) &streamerStruct, 0, streamLen);
  streamerStruct.sin_family = AF_INET;
  streamerStruct.sin_port = htons(port);
  if (inet_aton(ipAddr, &streamerStruct.sin_addr) == 0) {
    cout << "inet_aton failed" << endl;
    exit(1);
  }

  int broadcast = 1;
  if (setsockopt(controlData.streamer_socket, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0) {
    cout << "setsockopt failed" << endl;
    exit(1);
  }
}
void closeStreamingSocket()
{
  close(controlData.streamer_socket);
}

void openMessengerSocket(const char* ipAddr, int port)
{
  cout << "Opening listening socket..." << endl;
  controlData.messenger_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (controlData.messenger_socket < 0) {
    cout << "Opening listening socket failed" << endl;
    exit(1);
  }

  memset((char *) &messengerStruct, 0, messengerLen);
  messengerStruct.sin_family = AF_INET;
  messengerStruct.sin_port = htons(port);
  messengerStruct.sin_addr.s_addr = inet_addr(ipAddr);
  int bind_sock_in = bind(controlData.messenger_socket, (struct sockaddr*) &messengerStruct, messengerLen);
  if (bind_sock_in < 0) {
    cout << "Error binding listener socket" << endl;
    exit(1);
  }
}

void closeMessengerSocket()
{
  close(controlData.messenger_socket);
}

int sendPacket(char* packet, uint16_t lengthPacket, bool isData)
{
  if (isData == true) {
    if (sendto(controlData.streamer_socket, packet, lengthPacket, 0, (struct sockaddr*) &streamerStruct, streamLen) < 0)
    {
      cout << "Data streaming error" << endl;
      return 0;
    }
  }
  else if (isData == false) {
    if (sendto(controlData.messenger_socket, packet, lengthPacket, 0, (struct sockaddr*) &messengerStruct, messengerLen) < 0)
    {
      cout << "Message sending error" << endl;
      return 0;
    }
  }
  //controlData.totalPackets++;
  return 1;
}

int readPacket(char* packetPointer)
{
  int value = 0, bytesRead = 0;
  ioctl(controlData.messenger_socket, FIONREAD, &value);
  if (value > 0) {
    bytesRead = recvfrom(controlData.messenger_socket, packetPointer, MAX_PACKET_LENGTH, 0, (struct sockaddr*) &messengerStruct, (socklen_t *) &messengerLen);
    // cout << bytesRead << " bytes read from socket" << endl;
  }
  //controlData.totalPackets++;
  return bytesRead;
}

void closeAllConnections()
{
  close(controlData.streamer_socket);
  close(controlData.messenger_socket);
}
