#pragma once

#ifndef _MESSAGEHANDLER_H_
#define _MESSAGEHANDLER_H_

#include <ctime>
#include <chrono>
#include <iostream>
#include <atomic>
#include "rpc/server.h"

using namespace std::chrono;
using namespace std;

class MessageHandler 
{
  private:
    rpc::server* srv;
    atomic_int msgNum{0};
    high_resolution_clock::time_point startTime;

  public:
    MessageHandler(const string &address, uint16_t port);
    rpc::server* getServer();
    int getMsgNum();
    double getTimestamp();
};

#endif



