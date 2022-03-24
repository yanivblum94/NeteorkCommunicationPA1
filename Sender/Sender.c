#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include "SenderHelper.h"
#include "Sender.h"

int main(int argc, char* argv[])
{
	int fileSizeBytes;
	int encodedMsgSize;
	int blocksOf26;

	WSADATA  wsaData;
	struct sockaddr_in remote_addr;
	char userInput[MAX_FILE_NAME_LEN] = "";
	char msgAfterRead[MSG_SIZE];
	char msgRepBinary[MSG_SIZE * 8];
	char msgAfterHamming[HAMM_MSG_SIZE];
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

		FILE* file = fopen(userInput, "rb");
		if (file == NULL) {
		fprintf(stderr, "Error  in file open\n");
		exit(-1);
		}

		fileSizeBytes = getFileSize(file);
		encodedMsgSize = fileSizeBytes * 8 / MSG_SIZE * HAMM_MSG_SIZE;
		sendFileSize(encodedMsgSize, s);
		BINARY_MESSAGE = (char*)malloc((fileSizeBytes * 8) * sizeof(char));
		SENDER_BUFFER = (char*)malloc(encodedMsgSize * sizeof(char));
		blocksOf26 = 0;
		while (Read26Bytes(file, msgAfterRead) == MSG_SIZE) {
			convertMsgToBinaryChars(msgAfterRead, msgRepBinary);
			charsCopy(BINARY_MESSAGE, msgRepBinary, blocksOf26 * 26, MSG_SIZE);
			blocksOf26++;
		}

		for (int i = 0; i < (fileSizeBytes*8/MSG_SIZE); i++)
		{
			charsCopy(msgRepBinary, BINARY_MESSAGE, i * MSG_SIZE, MSG_SIZE);
			hammingEncode(msgRepBinary, msgAfterHamming);
			charsCopy(SENDER_BUFFER, msgAfterHamming, i * HAMM_MSG_SIZE, HAMM_MSG_SIZE);
		}
		write_to_sock(s, SENDER_BUFFER, encodedMsgSize);
		PrintOutput(blocksOf26);
		fclose(file);
	}

	CloseConnections(s);
	return 0;
}

// return the number of BYTES in the file 
int getFileSize(FILE* file) {
	int count = 0; 
	char* c[26];
	while (fread(c, 1, MSG_SIZE, file) > 0) {
		count++;
	}
	rewind(file);
	return count * MSG_SIZE;
}

// function to send the file's size given with int
// TODO: change 10 to the MACRO Yaniv added
void sendFileSize(int size, SOCKET s) {
	char* filesSizeInString[10];
	itoa(size, filesSizeInString, 10);
	// SEND with Yaniv's function 
}
