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

// func to encode 26bit msg to 31 bit msg with hamming code 
// assuming it's ordered left to right 
// TODO: test with an example - might have problem with the msg order (little / big endian ) 
const char* hammingDecode(char* originMsg) {
	char result[32] = "";
	int i = 0, j = 0, p1 = 0, p2 = 0, p4 = 0, p8 =0, p16 = 0 , total = 0;
	while (i <= 32) {
		total = total + originMsg[i];
		if ((1 & originMsg[i]) == 1) {
			p1++;
		}
		if ((2 & originMsg[i]) == 2) {
			p2++;
		}
		if ((4 & originMsg[i]) == 4) {
			p4++;
		}
		if ((8 & originMsg[i]) == 8) {
			p8++;
		}
		if ((16 & originMsg[i]) == 16) {
			p16++;
		}
		i++;
	}
	i = 0;
	while (i <= 32) {
		switch (i)
		{
		case 0:
			result[i] = (total % 2) ? '1' : '0';
			break;
		case 1:
			result[i] = (p1 % 2) ? '1' : '0';
			break;
		case 2:
			result[i] = (p2 % 2) ? '1' : '0';
			break;
		case 4:
			result[i] = (p4 % 2) ? '1' : '0';
			break;
		case 8:
			result[i] = (p8 % 2) ? '1' : '0';
			break;
		case 16:
			result[i] = (p16 % 2) ? '1' : '0';
			break;
		default:
			result[i] = originMsg[j];
			j++;
		}
		i++;
	}
	return result;
}

const char* hammingEncode(char* codedMsg) {
	int i = 1, xorResult = 0;
	int total = 0;
	char result[26] = "";
	//add function to copy from codedMsg to result on relevant indexes naively temporary 
	while (i <= 32) {
		total = total + (int)(codedMsg[i]);
		if (codedMsg[i]) {
			xorResult = xorResult ^ i ;
		}
		i++;
	}
	if (xorResult) { //got a number 
		if ((total % 2) == 1 && codedMsg[0]) { //matching p0 and total 1's 
			result[xorResult] = (result[xorResult]) ? '0' : '1';
		}
		else {
			// got two mistakes - TODO ask on forum how to handle ? 

		}
	}
	else {
		if ((total % 2) == 1 && codedMsg[0]) { //matching p0 and total 1's 
			return result;
		}
		else { // miss match only on index 0 
			result[0] = (result[xorResult]) ? '0' : '1';
		}
	}
	
	return result;

}