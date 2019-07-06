#include "network.h"
#include "core/controller.h"

using namespace chai3d;
using namespace std;

extern ControlData controlData;

struct sockaddr_in listenerStruct;
int listenerLen = sizeof(listenerStruct);

vector<struct sockaddr_in> senderStructs; 

void openMessagingSockets()
{
  cout << "Opening listener socket..." << endl;
  controlData.listener_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (controlData.listener_socket < 0) {
    cout << "Opening listener socket failed" << endl;
    exit(1);
  }
  //fcntl(controlData.listener_socket, F_SETFL, O_NONBLOCK);

  memset((char *) &listenerStruct, 0, listenerLen);
  listenerStruct.sin_family = AF_INET;
  listenerStruct.sin_port = htons(controlData.LISTENER_PORT);
  listenerStruct.sin_addr.s_addr = inet_addr(controlData.LISTENER_IP);

  int opt = 1;
  int broadcast = setsockopt(controlData.listener_socket, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt)); 
  int reuseAddr = setsockopt(controlData.listener_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  int reusePort = setsockopt(controlData.listener_socket, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
  if (broadcast < 0 || reuseAddr < 0 || reusePort < 0)
  {
    cout << "Failed to set socket options" << endl;
    exit(1);
  }

  int bind_sock_in = bind(controlData.listener_socket, (struct sockaddr*) &listenerStruct, listenerLen);
  if (bind_sock_in < 0) {
    cout << "Error binding listener socket" << endl;
    exit(1);
  }

  cout << "Opening sender sockets..." << endl;
  for (int i = 0; i < controlData.SENDER_IPS.size(); i++)
  {
    struct sockaddr_in senderStruct;
    int sender_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sender_socket < 0) {
      cout << "Opening sender socket failed" << endl;
      exit(1);
    }
    //fcntl(controlData.listener_socket, F_SETFL, O_NONBLOCK);
    int senderLen = sizeof(senderStruct);
    
    char ipAddr[controlData.SENDER_IPS[i].length()];
    strcpy(ipAddr, controlData.SENDER_IPS[i].c_str());

    memset((char *) &senderStruct, 0, senderLen);
    senderStruct.sin_family = AF_INET;
    senderStruct.sin_port = htons(controlData.SENDER_PORTS[i]);
    senderStruct.sin_addr.s_addr = inet_addr(ipAddr);
    senderStructs.push_back(senderStruct);

    broadcast = setsockopt(sender_socket, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt)); 
    reuseAddr = setsockopt(sender_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    reusePort = setsockopt(sender_socket, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
    if (broadcast < 0 || reuseAddr < 0 || reusePort < 0)
    {
      cout << "Failed to set socket options" << endl;
      exit(1);
    }
    controlData.sender_sockets.push_back(sender_socket);
  }
}

void closeMessagingSocket()
{
  shutdown(controlData.listener_socket, 2);
}

int sendPacket(char* packet, uint16_t lengthPacket, bool isData)
{
  if (isData == true)
  {
    int senderLen = sizeof(senderStructs[0]);
    if (sendto(controlData.sender_sockets[0], packet, lengthPacket, 0, (struct sockaddr*) &(senderStructs[0]), senderLen) < 0)
    {
      cout << "Data Sending Error" << endl;
      return 0;
    }
  }
  else {
    for (int i = 1; i < controlData.sender_sockets.size(); i++)
    {
      int senderLen = sizeof(senderStructs[i]);
      if (sendto(controlData.sender_sockets[i], packet, lengthPacket, 0, (struct sockaddr*) &(senderStructs[i]), senderLen) < 0)
        {
          cout << "Sending error" << endl;
          return 0;
        }
    }
  }
  return 1;
}

int readPacket(char* packetPointer)
{
  int value = 0, bytesRead = 0;
  ioctl(controlData.listener_socket, FIONREAD, &value);
  if (value > 0) {
    bytesRead = recvfrom(controlData.listener_socket, packetPointer, MAX_PACKET_LENGTH, 0, (struct sockaddr*) &listenerStruct, (socklen_t *) &listenerLen);
    //cout << bytesRead << " bytes read from socket" << endl;
  }
  return bytesRead;
}

void closeAllConnections()
{
  close(controlData.listener_socket);

}
