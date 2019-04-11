#pragma once 

#ifndef _NETWORK_H_
#define _NETWORK_H_  

#include "messageDefinitions.h"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <ctime>
#include <assert.h>
#include <math.h>
#include <sstream>
#include <iostream>
#include <fcntl.h>

void openMessagingSocket(const char* ipAddr, int listenerPort, int senderPort);
void closeMessagingSocket();

//void openMessageHandlerSendSocket(const char* ipAddr, int port);
//void openMessageHandlerListenSocket(const char* ipAddr, int port);
//void openDataSocket(const char* ipAddr, int port);
//void closeSendSocket(void);
//void closeListenSocket(void);
//void closeDataSocket();
int sendPacket(char* packet, uint16_t lengthPacket); //, bool isData);
int readPacket(char* packet);
//int sendData(char* packet, uint16_t lengthPacket);
//void openDataSavingSocket(const char* ipAddr, int port);
//void closeDataSavingSocket();
//int readData(char* packet);
//void closeAllConnections(void);
#endif
