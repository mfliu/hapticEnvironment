#pragma once

#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <ctime>
#include <fstream>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>
#include "messageDefinitions.h"
#include <thread>

using namespace std;

class Logger
{
  private:
    const char* ipAddr;
    int port;
    int data_port;
    const char* saveDir;
    const char* saveFile;
    int listener_socket;
    int data_socket;
    struct sockaddr_in listenerStruct;
    struct sockaddr_in dataStruct;
    int listenerLen = sizeof(listenerStruct);
    int dataLen = sizeof(dataStruct);
    fstream logFile;
    fstream logDataFile;
    bool running;

  public:
    Logger(const char* address, int port, int dataPort, const char* logDirectory, const char* logFilePrefix);
    void startLogger();
    void updateLogger();
    void updateDataLogger();
    void stopMsgLogger();
    void stopDataLogger();
    int readPacket(char* packetPointer);
    int readData(char* packetPointer);
};

#endif
