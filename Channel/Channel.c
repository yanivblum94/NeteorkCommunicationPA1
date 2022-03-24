#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include "ChannelHelper.h"


int main(int argc, char* argv[])
{
    char recieved_buffer[1200];
    struct sockaddr_in sender_addr, receiver_addr;    
    int portSender, portRecevier;
    Channel_Params* channel_p =(Channel_Params*) malloc(sizeof(channel_p));
    
    //validate args
    ValidateArgs(argc, 3, 4);
    InitChannelParams(argc, argv, channel_p);
    InitChannelSetup(channel_p, &sender_addr, &receiver_addr);



    free(channel_p);
    return 0;
}

