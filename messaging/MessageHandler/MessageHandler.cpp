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

int MessageHandler::addModule(int moduleID, string ipAddr, int port) //, const int subscriberList[10])
{
  cout << "Adding module " << moduleID << " with IP " << ipAddr << ":" << port << endl;
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

  /*int bind_sock = bind(sock, (struct sockaddr*) &sockStruct, sockLen);
  if (bind_sock < 0) {
    cout << "Error binding socket for module " << moduleID << "." << endl;
    return 0;
  }*/

  moduleSubscribers[moduleID] = {};
  moduleSockets[moduleID] = sock;
  socketStructs[sock] = sockStruct;
  cout << "Added module " << moduleID << ":\t" << inet_ntoa(sockStruct.sin_addr) << ":" << ntohs(sockStruct.sin_port) << endl;
  return 1;
}

int MessageHandler::subscribeTo(int myID, int subscribeID) 
{
  map<int, set<int>>::iterator it = moduleSubscribers.find(subscribeID);
  if (it == moduleSubscribers.end() and subscribeID != 999) {
    cout << "Could not find module ID " << subscribeID << "." << endl;
    return 0;
  }
  if (subscribeID == 999) {
    for (map<int, set<int>>::iterator modIt = moduleSubscribers.begin(); modIt != moduleSubscribers.end(); ++modIt) {
      moduleSubscribers[modIt->first].insert(myID);
    }
    return 1;
  }
  moduleSubscribers[subscribeID].insert(myID);
  return 1;
}

int MessageHandler::sendMessage(vector<char> packet, uint16_t lengthPacket, int sendingModule)
{ 
  /*MSG_HEADER header;
  memcpy(&header, reinterpret_cast<char*> (&packet[0]), sizeof(header));
  if (header.msg_type == CST_CREATE) {
    cout << "RECEIVED CST_CREATE MESSAGE" << endl;
  }*/
  map<int, set<int>>::iterator it = moduleSubscribers.find(sendingModule);
  set<int> receivingModules;
  if (it != moduleSubscribers.end()) {
    receivingModules = moduleSubscribers[sendingModule];
    for (set<int>::iterator setIt = receivingModules.begin(); setIt != receivingModules.end(); ++setIt) {
      int socketNum = moduleSockets[*setIt];
      struct sockaddr_in sockStruct = socketStructs[socketNum];
      int socketLen = sizeof(sockStruct);
      if (sendto(socketNum, &packet[0], lengthPacket, 0, (struct sockaddr*) &(socketStructs[socketNum]), socketLen) < 0) {
        cout << "Data sending error for module " << sendingModule << " sending to module " << *setIt << "." << endl;
        return 0;
      }
    }
    return 1; 
  }
  
  cout << "Could not find module" << endl;
  return 0;
  
}

int MessageHandler::testMessage(int val)
{
  cout << "Test message received with value " << val << endl;
  return 1;
}

int main(int argc, char *argv[])
{
  //TODO: Read Ports and IP address from config file
  const char* IP;
  int PORT;
  if (argc <= 2) {
    IP = "127.0.0.1";
    PORT = 8080;
  }
  else {
    IP = argv[1];
    PORT = atoi(argv[2]);
  }
  MessageHandler* mh = new MessageHandler(IP, PORT);
  cout << "Made Message Handler with IP " << IP << " and PORT " << PORT << endl;
  mh->getServer()->bind("getMsgNum", [&mh](){return mh->getMsgNum();});
  mh->getServer()->bind("getTimestamp", [&mh](){return mh->getTimestamp();});
  mh->getServer()->bind("addModule", [&mh](int moduleID, string ipAddr, int port){return mh->addModule(moduleID, ipAddr, port);});
  mh->getServer()->bind("subscribeTo", [&mh](int myID, int subscribeID){return mh->subscribeTo(myID, subscribeID);});
  mh->getServer()->bind("sendMessage", [&mh](vector<char> packet, uint16_t lengthPacket, int sendingModule){return mh->sendMessage(packet, lengthPacket, sendingModule);});
  mh->getServer()->bind("testMessage", [&mh](int val){return mh->testMessage(val);});
  mh->getServer()->run(); 
}
