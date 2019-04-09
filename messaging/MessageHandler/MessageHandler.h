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
#include <thread>

using namespace std::chrono;
using namespace std;

class MessageHandler 
{
  private:
    atomic_int msgNum{0};
    high_resolution_clock::time_point startTime;
    struct sockaddr_in inputStruct;
    struct sockaddr_in outputStruct;
    struct sockaddr_in dataInStruct;
    struct sockaddr_in dataOutStruct;
    int input_socket;
    int output_socket;
    int dataIn_socket;
    int dataOut_socket;
    int inputLen = sizeof(inputStruct);
    int outputLen = sizeof(outputStruct);
    int dataInLen = sizeof(dataInStruct);
    int dataOutLen = sizeof(dataOutStruct);
    const char* ipAddr;
    int inputPort;
    int outputPort;
    int dataInPort;
    int dataOutPort;
    bool running;

  public:
    MessageHandler(const char* address, int iPort, int oPort, int diPort, int doPort);
    int getMsgNum();
    double getTimestamp();
    void openInputSocket();
    void openOutputSocket();
    void openDataSockets();
    void closeInputSocket();
    void closeOutputSocket();
    void closeDataSockets();
    int readPacket(char* packetPointer);
    int sendPacket(char* packet, uint16_t lengthPacket);
    int readData(char* packetPointer);
    int sendData(char*packet, uint16_t lengthPacket);
    void updateMessageHandler();
    void updateDataHandler();
};

#endif



