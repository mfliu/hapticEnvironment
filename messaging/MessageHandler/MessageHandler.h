#pragma once

#ifndef _MESSAGEHANDLER_H_
#define _MESSAGEHANDLER_H_

#include <ctime>
#include <chrono>
#include <atomic>
#include <iostream>
#include "rpc/server.h"
#include <string>
#include <map>
#include <set>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include "messageDefinitions.h"

using namespace std::chrono;
using namespace std;

class MessageHandler 
{
  private:
    rpc::server* srv;
    atomic_int msgNum{0};
    high_resolution_clock::time_point startTime;
    char msg[MAX_PACKET_LENGTH]; 
    map<int, set<int>> moduleSubscribers;
    map<int, int> moduleSockets;
    map<int, struct sockaddr_in> socketStructs;

  public:
    MessageHandler(const char* address, int iPort);
    rpc::server* getServer();
    int getMsgNum();
    double getTimestamp();
    int addModule(int moduleID, int port, string ipAddr); //, const int subscriberList[10]);
    int subscribeTo(int myID, int subscribeID);
    int sendMessage(vector<char> packet, uint16_t lengthPacket, int module);
    //int getMessage(char* result);
};

#endif



