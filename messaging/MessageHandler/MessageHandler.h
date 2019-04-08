#pragma once

#ifndef _MESSAGEHANDLER_H_
#define _MESSAGEHANDLER_H_

#include <stdio.h>
#include <string.h>
#include <ctime>
#include <chrono>
#include <atomic>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <assert.h>
#include <sstream>
#include <iostream>
#include "messageDefinitions.h"

using namespace std::chrono;
using namespace std;

class MessageHandler 
{
  private:
    atomic_int msgNum{0};
    high_resolution_clock::time_point startTime;
    struct sockaddr_in inputStruct;
    struct sockaddr_in outputStruct;
    int input_socket;
    int output_socket;
    int inputLen = sizeof(inputStruct);
    int outputLen = sizeof(outputStruct);
    const char* ipAddr;
    int inputPort;
    int outputPort; 

  public:
    MessageHandler(const char* address, int iPort, int oPort);
    int getMsgNum();
    double getTimestamp();
    void openInputSocket();
    void openOutputSocket();
    void closeInputSocket();
    void closeOutputSocket();
    int readPacket(char* packetPointer);
    int sendPacket(char* packet, uint16_t lengthPacket);
    
};

#endif



