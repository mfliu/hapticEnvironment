#pragma once 

#ifndef _NETWORK_H_
#define _NETWORK_H_  

#include "messageDefinitions.h"
#include <stdio.h>
#include <string.h>
#include <vector>
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

int addMessageHandlerModule();
int subscribeToTrialControl();
int openMessagingSocket();
void closeMessagingSocket();
int readPacket(char* packet);
#endif
