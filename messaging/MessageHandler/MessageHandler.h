#pragma once

#ifndef _MESSAGEHANDLER_H_
#define _MESSAGEHANDLER_H_

#include <ctime>
#include <chrono>
#include <atomic>
#include <iostream>
#include "rpc/server.h"
#include <vector>
#include <string>
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

  public:
    MessageHandler(const char* address, int iPort);
    rpc::server* getServer();
    int getMsgNum();
    double getTimestamp();
    //int sendMessage((char* const) packet);
    //int getMessage(char* result);
};

#endif



