#ifndef UTILS
#define UTILS

#include "Definitions.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/timeb.h>
#include <sys/types.h>
#include <winsock2.h>
#include <conio.h>
#include <errno.h>

int InitSockAddr(struct sockaddr_in* sa, int port, char* ip);
SOCKET SocketInit();
void CloseConnections(SOCKET s);
void ValidateArgs(int argc, int min, int max);
bool BindSocket(SOCKET s, struct sockaddr_in* addr);
int read_from_sock(SOCKET s, char* data, int len);
int write_to_sock(SOCKET s, char* data, int len);
void wsa_clean();

inline void assertion(int condition, char* message, int err_idx) {
	if (not(condition)) {
		fprintf(stderr, "Assertion Error: %s [%d]\n", message, err_idx);
		exit(-1);
	}
}
#endif