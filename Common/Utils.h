#ifndef UTILS
#define UTILS

#include "winsock2.h"
#include "Definitions.h"
#include <stdio.h>
#include <string.h>

int InitSockAddr(struct sockaddr_in* sa, int port, char* ip);
SOCKET SocketInit();
void CloseConnections(SOCKET s);
void ValidateArgs(int argc, int min, int max);

#endif