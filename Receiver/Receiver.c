#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include "ReceiverHelper.h"

int main(int argc, char* argv[])
{
	WSADATA  wsaData;
	struct sockaddr_in remote_addr;
	char userInput[MAX_FILE_NAME_LEN] = "";
	char msgWithHamm[HAMM_MSG_SIZE];
	SOCKET s;

	//validate input arguments
	ValidateArgs(argc, 3, 3);

	//WSA Init
	int  iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		fprintf(stderr, "Error  at  WSAStartup()\n");
		exit(-1);
	}
	//Socket Init
	s = SocketInit();
	InitSockAddr(&remote_addr, atoi(argv[3]), argv[2]);
	if ((bind(s, (struct sockaddr *)&remote_addr, sizeof(remote_addr))) < 0) {
		fprintf(stderr, "bind failed with error %d \n", WSAGetLastError());
		exit(-1);
	}

	//main loop process
	while (1) {
		printf("enter file name:\n");
		gets(userInput);
		if (!strcmp(userInput, "quit"))
			break;
		FILE* file = fopen(userInput, "wb");
		if (file == NULL) {
			fprintf(stderr, "Error  in file open\n");
			exit(-1);
		}
		int blocksOf31 = 0, errorsCorrected=0;
		while (1) {
			//TODO:read from channel, add hamming decoding and error correction and write to file
			blocksOf31++;
		}
		PrintOutput(blocksOf31, errorsCorrected);
		fclose(file);
	}

	CloseConnections(s);
	return 0;
}