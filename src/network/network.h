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


void openStreamingSocket(const char* ipAddr, int port);
void openMessengerSocket(const char* ipAddr, int port);
void closeStreamingSocket(void);
void closeMessengerSocket(void);
int sendPacket(char* packet, uint16_t lengthPacket, bool isData);
int readPacket(char* packet);
void closeAllConnections(void);
#endif
