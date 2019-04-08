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
    const char* saveDir;
    const char* saveFile;
    int listener_socket;
    struct sockaddr_in listenerStruct;
    int listenerLen = sizeof(listenerStruct);
    fstream logFile;
  
  public:
    Logger(const char* address, int port, const char* logDirectory, const char* logFilePrefix);
    void startLogger();
    void updateLogger();
    void stopLogger();
    int readPacket(char* packetPointer);
};

#endif
