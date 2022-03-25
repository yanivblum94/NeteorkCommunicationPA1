#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include "ReceiverHelper.h"

int main(int argc, char* argv[])
{
	int size_temp;
	char msg_size_str[SIZE_MSG_LEN];
	char curr_hamming_msg[HAMM_MSG_SIZE + 1], partial_decoded_msg[MSG_SIZE];
	curr_hamming_msg[0] = '0';

	ValidateArgs(argc, 3, 3);
	Receiver_Params* receiver_p = (Receiver_Params*)malloc(sizeof(Receiver_Params));
	InitServerParams(argv, receiver_p);
	OpenOutputFile(receiver_p);
	struct sockaddr_in channel_addr;
	memset(&channel_addr, 0, sizeof(channel_addr));
	InitSockAddr(&channel_addr, receiver_p->ip, receiver_p->port);
	assertion(connect(receiver_p->socket, (SOCKADDR*)&channel_addr, sizeof(struct sockaddr)) != SOCKET_ERROR, "connection falied", WSAGetLastError());
	
	//main loop process
	while (!receiver_p->quit) {
		//read message size and data from channel
		size_temp = read_from_sock(receiver_p->socket, msg_size_str, SIZE_MSG_LEN);
		AllocateBuffersSizes(receiver_p, msg_size_str);
		size_temp = read_from_sock(receiver_p->socket, receiver_p->encoded_message, receiver_p->encoded_message_size);

		for (int i = 0; i < (receiver_p->encoded_message_size / 31 ); i++)
		{
			cuttingHamming(curr_hamming_msg, receiver_p->encoded_message, i * HAMM_MSG_SIZE, HAMM_MSG_SIZE);
			hammingDecode(curr_hamming_msg, partial_decoded_msg);
			charsCopy(receiver_p->fixed_message, curr_hamming_msg, i * MSG_SIZE, MSG_SIZE);
		}
		fromFixedToOrigin(receiver_p->fixed_message, receiver_p->origin_message, receiver_p->decoded_message_size);

		//write decoded message to file
		fwrite(receiver_p->origin_message,sizeof(char), (receiver_p->decoded_message_size / 8), receiver_p->file);
		PrintOutput(receiver_p);

		//clean current run
		CleanReceiverRound(receiver_p);
		//get ready for a new round
		receiver_p->socket = SocketInit();
		assertion(connect(receiver_p->socket, (SOCKADDR*)&channel_addr, sizeof(struct sockaddr)) != SOCKET_ERROR, "connection falied", WSAGetLastError());
		OpenOutputFile(receiver_p);
	}

	shutdown(receiver_p->socket, SD_BOTH);
	CloseConnections(receiver_p->socket);
	wsa_clean();
	free(receiver_p);
	return 0;
}