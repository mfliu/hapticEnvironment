#include "network.h"
#include "core/controller.h"

using namespace chai3d;
using namespace std;

extern ControlData controlData;

//struct sockaddr_in listenerStruct;
//int listenerLen = sizeof(listenerStruct);

struct sockaddr_in msgStruct;
int msgLen = sizeof(msgStruct);

//vector<struct sockaddr_in> senderStructs; 

int addMessageHandlerModule()
{
  auto addMod = controlData.client->call("addModule", controlData.MODULE_NUM, controlData.IPADDR, controlData.PORT);
  //addMod.wait();
  return addMod.as<int>();
}

int subscribeToTrialControl() 
{
  bool subscribed = false;
  clock_t begin = clock();
  while (subscribed == false) {
    auto subscribe = controlData.client->async_call("subscribeTo", 1, 2);
    subscribe.wait();
    if (subscribe.get().as<int>() == 1) {
      subscribed = true;
      return 1;
    }
    clock_t now = clock();
    double elapsed = double(now - begin)/CLOCKS_PER_SEC;
    if (elapsed > 120) {
      cout << "Error subscribing to Trial Control, exiting." << endl;
      return 0;
    }
    sleep(5); // 1000 microseconds = 1 millisecond
  }
  return 1;
}

int openSocket()
{
  cout << "Opening messaging socket..." << endl;
  controlData.msg_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (controlData.msg_socket < 0) {
    cout << "Opening messaging socket failed" << endl;
    exit(1);
  }

  memset((char*) &msgStruct, 0, msgLen);
  msgStruct.sin_family = AF_INET;
  msgStruct.sin_port = htons(controlData.PORT);
  msgStruct.sin_addr.s_addr = inet_addr(controlData.IPADDR);
  int opt = 1;
  int broadcast = setsockopt(controlData.msg_socket, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt)); 
  int reuseAddr = setsockopt(controlData.msg_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  int reusePort = setsockopt(controlData.msg_socket, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
  if (broadcast < 0 || reuseAddr < 0 || reusePort < 0)
  {
    cout << "Failed to set socket options" << endl;
    exit(1);
  }

  int bind_sock_in = bind(controlData.msg_socket, (struct sockaddr*) &msgStruct, msgLen);
  if (bind_sock_in < 0) {
    cout << "Error binding messaging socket" << endl;
    exit(1);
  }
  return 1; 
}
/*
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
}*/

void closeMessagingSocket()
{
  shutdown(controlData.msg_socket, 2);
}
/*
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
}*/

int readPacket(char* packetPointer)
{
  int value = 0, bytesRead = 0;
  ioctl(controlData.msg_socket, FIONREAD, &value);
  if (value > 0) {
    bytesRead = recvfrom(controlData.msg_socket, packetPointer, MAX_PACKET_LENGTH, 0, (struct sockaddr*) &msgStruct, (socklen_t *) &msgLen);
    //cout << bytesRead << " bytes read from socket" << endl;
  }
  return bytesRead;
}

void closeAllConnections()
{
  close(controlData.msg_socket);

}
