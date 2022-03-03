#ifndef SENDER_HELPER
#define SENDER_HELPER

#include "winsock2.h"
#include "../Common/Definitions.h"
#include <stdio.h>
#include <string.h>

int Read26Bytes(FILE* file, char* buf);

void PrintOutput(int blocks);

void CloseConnections(SOCKET s);

SOCKET SocketInit();

struct sockaddr_in InitSockAddr(char* ipAddr, int port);

#endif