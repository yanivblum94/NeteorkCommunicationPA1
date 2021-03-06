#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE

#include "SenderHelper.h"


int main(int argc, char* argv[])
{
	int fileSizeBytes, encodedMsgSize, blocksOf26;
	
	WSADATA  wsaData;
	struct sockaddr_in remote_addr;
	char userInput[MAX_FILE_NAME_LEN];
	char msgAfterRead[MSG_SIZE];
	char msgRepBinary[BLOCK_SIZE_BITS];
	char msgAfterHamming[HAMM_MSG_SIZE];
	char preHamming[MSG_SIZE];
	char* BINARY_MESSAGE;
	char* SENDER_BUFFER;
	SOCKET s;

	//validate input arguments
	ValidateArgs(argc, 3, 3);

	//Socket Init
	s = SocketInit();
	memset(&remote_addr, 0, sizeof(remote_addr));
	InitSockAddr(&remote_addr, atoi(argv[2]), argv[1]);

	//connect
	assertion(connect(s, (SOCKADDR*)&remote_addr, sizeof(struct sockaddr)) != SOCKET_ERROR, "connection falied", WSAGetLastError());

	printf("ip=%s, port=%s\n", argv[1], argv[2]);
	//main loop process
	while (1) {
		printf("enter file name:\n");
		assertion(scanf("%s", userInput) == 1, "Error in response in server", WSAGetLastError());
		if (!strcmp(userInput, "quit"))
			break;

		FILE* file = fopen(userInput, "r");
		assertion(file != NULL, "Error in open file in server.", WSAGetLastError());
		fileSizeBytes = getFileSize(file);
		encodedMsgSize = fileSizeBytes * 8 / MSG_SIZE * HAMM_MSG_SIZE;
		sendFileSize(encodedMsgSize, s);
		BINARY_MESSAGE = (char*)malloc((fileSizeBytes * 8) * sizeof(char));
		SENDER_BUFFER = (char*)malloc(encodedMsgSize * sizeof(char));
		blocksOf26 = 0;
		while (Read26Bytes(file, msgAfterRead) == MSG_SIZE) {
			convertMsgToBinaryChars(msgAfterRead, msgRepBinary);
			charsCopy(BINARY_MESSAGE, msgRepBinary, blocksOf26 * BLOCK_SIZE_BITS, BLOCK_SIZE_BITS);
			blocksOf26++;
		}

		for (int i = 0; i < (fileSizeBytes*8/MSG_SIZE); i++)
		{
			charsCopyHamm(preHamming, BINARY_MESSAGE, i * MSG_SIZE, MSG_SIZE);
			hammingEncode(preHamming, msgAfterHamming);
			charsCopy(SENDER_BUFFER, msgAfterHamming, i * HAMM_MSG_SIZE, HAMM_MSG_SIZE);
		}
		write_to_sock(s, SENDER_BUFFER, encodedMsgSize);
		PrintOutput(blocksOf26);

		FinishOneRound(s, file);
		free(SENDER_BUFFER);
		free(BINARY_MESSAGE);

		//reconnect to socket
		s = SocketInit();
		assertion(connect(s, (SOCKADDR*)&remote_addr, sizeof(struct sockaddr)) != SOCKET_ERROR, "connection falied", WSAGetLastError());
	}

	shutdown(s, SD_BOTH);
	CloseConnections(s);
	wsa_clean();
	return 0;
}


