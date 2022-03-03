#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include "winsock2.h"
#include "../Common/Definitions.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
	WSADATA  wsaData;
	struct sockaddr_in remote_addr;

	//validate input arguments
	if (argc != 3) {
		fprintf(stderr, "Error  at  input args, should be 3 but there are %d args\n", argc);
	}  
	//WSA Init
	int  iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);     
	if (iResult != NO_ERROR)       
		fprintf(stderr, "Error  at  WSAStartup()\n");

	//Socket Init
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	remote_addr.sin_family = AF_INET;
	remote_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	remote_addr.sin_port = htons(6342);

	//connect
	int status = connect(s, (SOCKADDR*)&remote_addr, sizeof(struct sockaddr));
	if(status == SOCKET_ERROR)
		fprintf(stderr, "Error  in socket connect\n");

	//TODO: add input phase and hamming

	//send test message
	char buf[5];
	strcpy("tst", buf);
	send(s, buf, (int)strlen(buf), 0);
	int sent = send(s, buf, MSG_SIZE, 0);
	printf("sent %d bits", sent);
	return 0;
}

