#ifndef RECEIVER_HELPER
#define RECEIVER_HELPER

#include "../Common/Utils.h"

typedef struct {
	SOCKET socket;
	int port;
	char* ip;
	FILE* file;
	int encoded_message_size;
	int decoded_message_size;
	uint32_t messageHamming;
	char filename[MAX_FILE_NAME_LEN];
	bool quit;
	char* encoded_message;
	char* origin_message;
	char* fixed_message;
	int bits_fixed;
}Receiver_Params;

void InitServerParams(char* argv[], Receiver_Params* receiver_p);
void OpenOutputFile(Receiver_Params* receiver_p);
void PrintOutput(Receiver_Params* receiver_p);
void AllocateBuffersSizes(Receiver_Params* receiver_p, char msg_str_size[10]);
void CleanReceiverRound(Receiver_Params* receiver_p);

#endif