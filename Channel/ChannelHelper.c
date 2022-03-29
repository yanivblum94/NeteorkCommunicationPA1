#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include "ChannelHelper.h"


void InitChannelParams(int argc, char* argv[], Channel_Params* ch_p) {

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
    ch_p->flipped_bits = 0;
    
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
    assertion(getsockname(ch_p->sender_sock, (SOCKADDR*)sender_addr, &addlen) == 0, "couldn't get port for sender", WSAGetLastError());
    printf("Sender's Socket:  IP=%s, port=%d\n", inet_ntoa(sender_addr->sin_addr), sender_addr->sin_port);
    assertion(getsockname(ch_p->receiver_sock, (SOCKADDR*)receiver_addr, &addlen) == 0, "couldn't get port for receiver", WSAGetLastError());
    printf("Receiver's Socket:  IP=%s, port=%d\n", inet_ntoa(receiver_addr->sin_addr), receiver_addr->sin_port);
    assertion(!listen(ch_p->sender_sock, SOMAXCONN), "Listening to sender socket failed", WSAGetLastError());
    assertion(!listen(ch_p->receiver_sock, SOMAXCONN), "Listening to receiver socket failed", WSAGetLastError());
}

bool GetUserOutput() {
    char input[100] = { 0 };
    printf("continue? (yes/no)\n");
    int r = scanf("%99s", input);
    return strcmp(input, "no");
}

void ApplyDet(Channel_Params* channel_p) {
    channel_p->det_counter = channel_p->cycle_length-1;
    for (int i = 0; i < channel_p->msg_size; i++) {
        if (channel_p->det_counter == 0) {
            channel_p->message_sent[i] = (channel_p->message[i] == '1') ? '0' : '1';
            channel_p->det_counter = channel_p->cycle_length-1;
            channel_p->flipped_bits++;
        }
        else {
            channel_p->message_sent[i] = channel_p->message[i];
            channel_p->det_counter--;
        }
    }
}

void ApplyRandom(Channel_Params* channel_p) {
    unsigned rand_num;
    for (int i = 0; i < channel_p->msg_size; i++) {
           rand_s(&rand_num);
           // we use one rand to get 16 bits integer instead of 2 - efficient random
           bool even = (rand_num % 2 == 0);
           rand_num = rand_num << 1;
           if (!even)
               rand_num++;
           if (((float)rand_num / (float)RAND_MAX) <= channel_p->prob)
           {
               channel_p->message_sent[i] = (channel_p->message[i] == '1') ? '0' : '1';
               channel_p->flipped_bits++;
           }
           else {
               channel_p->message_sent[i] = channel_p->message[i];
           }
    }
}
