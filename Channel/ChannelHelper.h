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
	SOCKET receiver_sock;
	SOCKET sender_sock;
	SOCKET sender_accepted_sock;
	SOCKET receiver_accepted_sock;
	Noise_Type noise_type;
	char* ip;
	int sender_port;
	int recevier_port;
	bool quit;
	int flipped_bits;
	int prob;
	int seed;
	int cycle_length;
}Channel_Params;


void InitChannelParams(int argc, char* argv[], Channel_Params* ch_p);
void InitChannelSetup(Channel_Params* ch_p, struct sockaddr_in* sender_addr, struct sockaddr_in* receiver_addr);
void AddNoise();
void ApplyRandom();
void ApplyDet();

#endif
