#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include "winsock2.h"
#include "../Common/Definitions.h"
#include "SenderHelper.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
	WSADATA  wsaData;
	struct sockaddr_in remote_addr;
	char userInput[MAX_FILE_NAME_LEN] = "";
	char msgBeforeHamm[MSG_SIZE];
	SOCKET s;

	//validate input arguments
	if (argc != 3) {
		fprintf(stderr, "Error  at  input args, should be 3 but there are %d args\n", argc);
		exit(-1);
	}

	//WSA Init
	int  iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		fprintf(stderr, "Error  at  WSAStartup()\n");
		exit(-1);
	}
	//Socket Init
	s = SocketInit();
	remote_addr = InitSockAddr(argv[2], atoi(argv[3]));

	//connect
	int status = connect(s, (SOCKADDR*)&remote_addr, sizeof(remote_addr));
	if (status == SOCKET_ERROR) {
	fprintf(stderr, "Error  in socket connect. Last error:%d\n", WSAGetLastError());
	exit(-1);
	}

	//main loop process
	while (1) {
		printf("enter file name:\n");
		gets(userInput);
		if (!strcmp(userInput, "quit"))
			break;
		FILE* file = fopen(userInput, "r");
		if (file == NULL) {
		fprintf(stderr, "Error  in file open\n");
		exit(-1);
		}
		int blocksOf26 = 0;
		while (Read26Bytes(file, msgBeforeHamm) == MSG_SIZE) {
			//TODO: add hamming code and add to send buffer
			blocksOf26++;
		}
		PrintOutput(blocksOf26);
		fclose(file);
	}

	CloseConnections(s);
	return 0;
}

/