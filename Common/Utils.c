#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include "Utils.h"

void CloseConnections(SOCKET s) {
	if (closesocket(s) == SOCKET_ERROR) {
		fprintf(stderr, "Error  in socket closure. Last error:%d\n", WSAGetLastError());
		exit(-1);
	}
	if (WSACleanup() == SOCKET_ERROR) {
		fprintf(stderr, "Error  in WAS cleanup. Last error:%d\n", WSAGetLastError());
		exit(-1);
	}
}

SOCKET SocketInit() {
	WSADATA wsaData;
	//init winsock
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR)
		printf("Error at WSAStartup()\n");
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET) {
		fprintf(stderr, "socket() generated invalid socket, Last error:%d\n", WSAGetLastError());
		exit(-1);
	}
	return s;
}

int InitSockAddr(struct sockaddr_in* sa, int port, char* ip) {
	sa->sin_family = AF_INET;
	sa->sin_port = port;
	if (ip == NULL) {
		sa->sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else {
		sa->sin_addr.s_addr = inet_addr(ip);
	}
	return 0;
}

void ValidateArgs(int argc, int min, int max) {
	if (argc < min || argc > max) {
		fprintf(stderr, "Error  at  input args, should be < %d or > %d but there are %d args\n", min, max, argc);
		exit(-1);
	}
}

bool BindSocket(SOCKET s, struct sockaddr_in* addr) {
	int cond = ((bind(s, (SOCKADDR*)addr, sizeof(struct sockaddr))) > 0);
	assert(cond == 0, "Binding Failed");
	return true;
}