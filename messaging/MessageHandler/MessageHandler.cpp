#include "MessageHandler.h"
#include <typeinfo>
#include <fcntl.h>
#include <string>

MessageHandler::MessageHandler(const char* address, int port)
{
  srv = new rpc::server(address, port);
  startTime = high_resolution_clock::now();
}

rpc::server* MessageHandler::getServer()
{
  return srv;  
}

int MessageHandler::getMsgNum()
{
  return msgNum++;
}

double MessageHandler::getTimestamp()
{
  high_resolution_clock::time_point currTime = high_resolution_clock::now();
  duration<double> timeNow = duration_cast<duration<double>> (currTime-startTime);
  return timeNow.count();
}

//int MessageHandler::sendMessage(char* const packet, int module)
//{
//  memset(msg, 0, MAX_PACKET_LENGTH);
//  memcpy(msg, packet, sizeof(&packet));
//  return 1;
//}

int MessageHandler::addModule(int moduleID, int port, string ipAddr) //, const int subscriberList[10])
{
  struct sockaddr_in sockStruct;
  int sockLen = sizeof(sockStruct);
  int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (sock < 0) {
    cout << "Opening socket for module number " << moduleID << " failed." << endl;
    return 0;
  }
  memset((char *) &sockStruct, 0, sockLen);
  sockStruct.sin_family = AF_INET;
  sockStruct.sin_port = htons(port);
  sockStruct.sin_addr.s_addr = inet_addr(ipAddr.c_str());
  
  int opt = 1;
  int broadcast = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt));
  int reuseAddr = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  int reusePort = setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
  if (broadcast < 0 || reuseAddr < 0 || reusePort < 0) {
    cout << "Failed to set socket options for module " << moduleID << "." << endl;
    return 0;
  }

  int bind_sock = bind(sock, (struct sockaddr*) &sockStruct, sockLen);
  if (bind_sock < 0) {
    cout << "Error binding socket for module " << moduleID << "." << endl;
    return 0;
  }
  cout << "Module " << moduleID << ":\t" << inet_ntoa(sockStruct.sin_addr) << ":" << sockStruct.sin_port << endl;
  moduleSubscribers[moduleID] = {};
  moduleSockets[moduleID] = sock;
  socketStructs[sock] = sockStruct;
  cout << "Socket opened for module " << moduleID << "." << endl;
  return 1;
  /*try {
    map<int, set<int>>::iterator it = moduleSubscribers.find(moduleID);
    if (it != moduleSubscribers.end()) {
      moduleSubscribers = set<int>();
      //for(int i = 0; i < 10; i++){
      //  moduleSubscribers[moduleID].insert(subscriberList[i]);
      //} 
    }
    else {
      for (int i = 0; i < 10; i++) {
        moduleSubscribers[moduleID].insert(subscriberList[i]);
      }
    }
    return 1;
  }
  catch (...) {
    cout << "Failed to subscribe module " << moduleID << endl;
    return 0;
  }*/
}

int MessageHandler::subscribeTo(int myID, int subscribeID) 
{
  map<int, set<int>>::iterator it = moduleSubscribers.find(subscribeID);
  if (it == moduleSubscribers.end()) {
    cout << "Could not find module ID " << subscribeID << "." << endl;
    return 0;
  }
  moduleSubscribers[subscribeID].insert(myID);
  return 1;
}

int MessageHandler::sendMessage(vector<char> packet, uint16_t lengthPacket, int sendingModule)
{ 
  map<int, set<int>>::iterator it = moduleSubscribers.find(sendingModule);
  set<int> receivingModules;
  if (it != moduleSubscribers.end()) {
    receivingModules = moduleSubscribers[sendingModule];
    for (set<int>::iterator setIt = receivingModules.begin(); setIt != receivingModules.end(); ++setIt) {
      int socketNum = moduleSockets[*setIt];
      struct sockaddr_in sockStruct = socketStructs[socketNum];
      int socketLen = sizeof(sockStruct);
      if (sendto(socketNum, reinterpret_cast<char*>(packet.data()), lengthPacket, 0, (struct sockaddr*) &(socketStructs[socketNum]), socketLen) < 0) {
        cout << "Data sending error for module " << sendingModule << " sending to module " << socketNum << "." << endl;
        return 0;
      }
    }
    return 1; 
  }
  
  cout << "Could not find module" << endl;
  return 0;
  
}

int main()
{
  //TODO: Read Ports and IP address from config file
  MessageHandler* mh = new MessageHandler("127.0.0.1", 8080);
  mh->getServer()->bind("getMsgNum", [&mh](){return mh->getMsgNum();});
  mh->getServer()->bind("getTimestamp", [&mh](){return mh->getTimestamp();});
  mh->getServer()->bind("addModule", [&mh](int moduleID, int port, string ipAddr){return mh->addModule(moduleID, port, ipAddr);});
  mh->getServer()->bind("subscribeTo", [&mh](int myID, int subscribeID){return mh->subscribeTo(myID, subscribeID);});
  mh->getServer()->bind("sendMessage", [&mh](vector<char> packet, uint16_t lengthPacket, int sendingModule){return mh->sendMessage(packet, lengthPacket, sendingModule);});
  mh->getServer()->run();
}
