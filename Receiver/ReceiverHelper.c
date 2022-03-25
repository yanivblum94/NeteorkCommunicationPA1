#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include "ReceiverHelper.h"

void InitServerParams(char* argv[], Receiver_Params* receiver_p) {
	receiver_p->ip = argv[1];
	receiver_p->port = atoi(argv[2]);
	printf("ip=%s, port=%d\n", receiver_p->ip, receiver_p->port);
	receiver_p->socket = SocketInit();
	receiver_p->quit = false;
}

void OpenInputFile(Receiver_Params* receiver_p) {
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
