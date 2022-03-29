
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include "ChannelHelper.h"


int main(int argc, char* argv[])
{
    bool to_continue = true;
    struct sockaddr_in sender_addr, receiver_addr;    
    Channel_Params* channel_p =(Channel_Params*) malloc(sizeof(channel_p));
    int size_temp;
    //validate args
    ValidateArgs(argc, 3, 4);

    InitChannelParams(argc, argv, channel_p);
    InitChannelSetup(channel_p, &sender_addr, &receiver_addr);
    if (channel_p->noise_type == RAND)
        srand(channel_p->seed);
    while (to_continue) {
        channel_p->sender_accepted_sock = accept(channel_p->sender_sock, NULL, NULL);
        assertion(INVALID_SOCKET != channel_p->sender_accepted_sock, "Accept failed on Sender's Socket", WSAGetLastError());
        channel_p->receiver_accepted_sock = accept(channel_p->receiver_sock, NULL, NULL);
        assertion(INVALID_SOCKET != channel_p->receiver_accepted_sock, "Accept failed on Receiver's Socket", WSAGetLastError());


        //read size and message
        size_temp = read_from_sock(channel_p->sender_accepted_sock, channel_p->msg_size_str, SIZE_MSG_LEN);
        channel_p->msg_size = atoi(channel_p->msg_size_str);
        channel_p->message = (char*)calloc(channel_p->msg_size, sizeof(char));
        size_temp = read_from_sock(channel_p->sender_accepted_sock, channel_p->message, channel_p->msg_size);
        printf("Received message from Sender [%dB]\n", (channel_p->msg_size / 8));

        //needs to add noise
        channel_p->message_sent = (char*)calloc(channel_p->msg_size, sizeof(char));
        if (channel_p->noise_type == RAND)
            ApplyRandom(channel_p);
        else
            ApplyDet(channel_p);

        //send size and message
        size_temp = write_to_sock(channel_p->receiver_accepted_sock, channel_p->msg_size_str, SIZE_MSG_LEN);
        size_temp = write_to_sock(channel_p->receiver_accepted_sock, channel_p->message_sent, channel_p->msg_size);
        printf("Sent message to sender [%dB], flipped %d bits\n", (channel_p->msg_size / 8), channel_p->flipped_bits);

        CloseConnections(channel_p->sender_accepted_sock);
        CloseConnections(channel_p->receiver_accepted_sock);
        free(channel_p->message);
        free(channel_p->message_sent);

        //print channel output
        to_continue = GetUserOutput();
    }

    CloseConnections(channel_p->sender_sock);
    CloseConnections(channel_p->receiver_sock);
    wsa_clean();
    //free(channel_p);
    return 0;
}

