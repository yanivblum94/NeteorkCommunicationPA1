#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE

#include "ReceiverHelper.h"

void InitServerParams(char* argv[], Receiver_Params* receiver_p) {
	receiver_p->ip = argv[1];
	receiver_p->port = atoi(argv[2]);
	printf("ip=%s, port=%d\n", receiver_p->ip, receiver_p->port);
	receiver_p->socket = SocketInit();
	receiver_p->quit = false;
}

void OpenOutputFile(Receiver_Params* receiver_p) {
	printf("Enter file name:\n");
	assertion(scanf("%s", receiver_p->filename) == 1, "Error in response in server", WSAGetLastError());
	if (strcmp(receiver_p->filename, "quit"))
	{
		receiver_p->file = fopen(receiver_p->filename, "w");
		assertion(receiver_p->file != NULL, "Error in open file in server.", WSAGetLastError());
		return;
	}
	receiver_p->quit = true;
}

void AllocateBuffersSizes(Receiver_Params* receiver_p, char msg_str_size[10]) {
	receiver_p->encoded_message_size = atoi(msg_str_size);
	receiver_p->decoded_message_size = (receiver_p->encoded_message_size / HAMM_MSG_SIZE) * MSG_SIZE;
	receiver_p->encoded_message = (char*)malloc(receiver_p->encoded_message_size * sizeof(char));
	receiver_p->origin_message = (char*)malloc((receiver_p->decoded_message_size/8 + 1) * sizeof(char));
	receiver_p->fixed_message = (char*)malloc(receiver_p->decoded_message_size * sizeof(char));
}

void PrintOutput(Receiver_Params* receiver_p){
	printf("recevid: %d bytes\n", (receiver_p->encoded_message_size/8));
	printf("wrote: %d bytes\n", (receiver_p->decoded_message_size / 8));
	printf("corrected %d errors\n", (receiver_p->bits_fixed / 8));
}

void CleanReceiverRound(Receiver_Params* receiver_p) {
	free(receiver_p->origin_message);
	free(receiver_p->encoded_message);
	free(receiver_p->fixed_message);
	CloseConnections(receiver_p->socket);
	wsa_clean();
}

void fromFixedToOrigin(char* fixed, char* origin, int decodedSize) {
	int temp; 
	char c=' ', buffer[9];
	for (int i = 0; i < (decodedSize/8); i++)
	{
		temp = (fixed[i * 8] == '1') ? 1 : 0;
		for (int j = 1; j < 8; j++)
		{
			temp << 1; 
			if (fixed[i * 8 + j] == '1') {
				temp += 1;
			}
		}
		itoa(c, buffer, 2);
		origin[i] = strtol(buffer, 0, 2);
	}
}

void charsCopy(char* copyTo, char* copyFrom, int start, int length) {
	for (int i = 0; i < length; i++)
	{
		copyTo[start + i] = copyFrom[i];
	}
}

/* 
	Function to cut from big encoded msg a 31 bits encoded msg 
	currEncoded size should be 32 for comfort reasons 
*/
void cuttingHamming(char* currEncoded, char* wholeMsg, int start, int len) {
	for (int i = 1; i < len; i++)
	{
		currEncoded[i] = wholeMsg[start + i];
	}
}

void hammingDecode(char* codedMsg, char* fixed_message) {
	int i = 1, p1 = 0, p2 = 0, p4 = 0, p8 = 0, p16 = 0, badSum = 0 ;
	while (i <= 32)
	{
		if ((1 & i) == 1 && codedMsg[i] == '1') {
			p1++;
		}
		if ((2 & i) == 2 && codedMsg[i] == '1') {
			p2++;
		}
		if ((4 & i) == 4 && codedMsg[i] == '1') {
			p4++;
		}
		if ((8 & i) == 8 && codedMsg[i] == '1') {
			p8++;
		}
		if ((16 & i) == 16 && codedMsg[i] == '1') {
			p16++;
		}
		i++;
	}
	if (((p1 % 2) && codedMsg[1] == '0') || (!(p1 % 2) && codedMsg[1] == '1')) {
		badSum += 1;
	}
	if (((p2 % 2) && codedMsg[2] == '0') || (!(p2 % 2) && codedMsg[2] == '1')) {
		badSum += 2;
	}
	if (((p4 % 2) && codedMsg[4] == '0') || (!(p4 % 2) && codedMsg[4] == '1')) {
		badSum += 4;
	}
	if (((p1 % 8) && codedMsg[8] == '0') || (!(p8 % 2) && codedMsg[8] == '1')) {
		badSum += 8;
	}
	if (((p1 % 16) && codedMsg[16] == '0') || (!(p16 % 2) && codedMsg[16] == '1')) {
		badSum += 16;
	}
	codedMsg[badSum] = (codedMsg[badSum] == '1') ? '0' : '1';
	hammingNarrowMsg(codedMsg, fixed_message);
}

void hammingNarrowMsg(char* encoded, char* result) {
	int j = 0;
	for (int i = 0; i < HAMM_MSG_SIZE+1; i++)
	{
		if (i == 0 || i == 1 || i == 2 || i == 4 || i == 8 || i == 16) {
			continue;
		}
		result[j] = encoded[i];
		j++;
	}
}