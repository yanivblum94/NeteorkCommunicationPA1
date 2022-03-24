#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include "ChannelHelper.h"


int main(int argc, char* argv[])
{
    bool to_continue = true;
    struct sockaddr_in sender_addr, receiver_addr;    
    fd_set sender_fds, receiver_fds;
    Channel_Params* channel_p =(Channel_Params*) malloc(sizeof(channel_p));
    int size;
    
    //validate args
    ValidateArgs(argc, 3, 4);
    InitChannelParams(argc, argv, channel_p);
    InitChannelSetup(channel_p, &sender_addr, &receiver_addr);
    //needs to generate noise
    while (to_continue) {
        channel_p->sender_accepted_sock = accpet(channel_p->sender_sock, NULL, NULL);
        assertion(INVALID_SOCKET == channel_p->sender_accepted_sock, "Accept failed on Sender's Socket", WSAGetLastError());
        channel_p->receiver_accepted_sock = accept(channel_p->receiver_sock, NULL, NULL);
        assertion(INVALID_SOCKET == channel_p->receiver_accepted_sock, "Accept failed on Receiver's Socket", WSAGetLastError());

    }


    free(channel_p);
    return 0;
}

