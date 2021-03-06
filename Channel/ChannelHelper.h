#ifndef CHANNEL_HELPER
#define CHANNEL_HELPER

#include "../Common/Utils.h"

typedef enum
{
	RAND,
	DET
}Noise_Type;

typedef struct
{
	uint32_t message_size;
	char* message;
	char* message_sent;
	int msg_size;
	char msg_size_str[SIZE_MSG_LEN];
	SOCKET receiver_sock;
	SOCKET sender_sock;
	SOCKET sender_accepted_sock;
	SOCKET receiver_accepted_sock;
	Noise_Type noise_type;
	char* ip;
	int sender_port;
	int recevier_port;
	int flipped_bits;
	int prob;
	int seed;
	int cycle_length;
	int det_counter;
}Channel_Params;


void InitChannelParams(int argc, char* argv[], Channel_Params* ch_p);
void InitChannelSetup(Channel_Params* ch_p, struct sockaddr_in* sender_addr, struct sockaddr_in* receiver_addr);
bool GetUserOutput();
void ApplyRandom(Channel_Params* channel_p);
void ApplyDet(Channel_Params* channel_p);

#endif
