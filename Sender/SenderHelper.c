#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include "SenderHelper.h"

int Read26Bytes(FILE* file, char* buf) {
	int bytesRead = fread(buf, 1, MSG_SIZE, file);
	if (bytesRead < 0) {
		fprintf(stderr, "read from file failed\n");
		exit(-1);
	}
	return bytesRead;
}

void PrintOutput(int blocks) {
	printf("file length: %d bytes\n", (blocks * MSG_SIZE));
	printf("sent: %d bytes\n", (blocks * HAMM_MSG_SIZE));
}

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
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET) {
		fprintf(stderr, "socket() generated invalid socket, Last error:%d\n", WSAGetLastError());
		exit(-1);
	}
	return s;
}

struct sockaddr_in InitSockAddr(char* ipAddr, int port) {
	struct sockaddr_in remote_addr;
	remote_addr.sin_family = AF_INET;
	remote_addr.sin_addr.s_addr = inet_addr(ipAddr);
	remote_addr.sin_port = htons(port);
	return remote_addr;
}