#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include "ChannelHelper.h"


InitChannelParams(int argc, char* argv[], Channel_Params* ch_p) {

    if (argc == 4)
    {
        ch_p->noise_type = RAND;
        ch_p->prob = atoi(argv[2]);
        ch_p->seed = atoi(argv[3]);
        srand(ch_p->seed);
        ch_p->prob = ch_p->prob / ERROR_PROB_CONSTANT;
    }
    else
    {
        ch_p->cycle_length = atoi(argv[2]);
        ch_p->noise_type = DET;
    }

    ch_p->ip = IP_LISTEN_ALL;
    ch_p->sender_port = 0;
    ch_p->recevier_port = 0;

    ch_p->receiver_sock = SocketInit();
    ch_p->sender_sock = SocketInit();
}

void InitChannelSetup(Channel_Params* ch_p, struct sockaddr_in* sender_addr, struct sockaddr_in* receiver_addr) {
    InitSockAddr(sender_addr, ch_p->sender_port, ch_p->ip);
    InitSockAddr(receiver_addr, ch_p->recevier_port, ch_p->ip);
    BindSocket(ch_p->sender_sock, sender_addr);
    BindSocket(ch_p->receiver_sock, receiver_addr);
    int addlen = sizeof(SOCKADDR);

}
