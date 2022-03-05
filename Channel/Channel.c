#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include "ChannelHelper.h"


int main(int argc, char* argv[])
{
    char recieved_buffer[1200];
    WSADATA wsaData;
    struct sockaddr_in my_addr, sender_addr, receiver_addr;
    char userInput[MAX_USER_INPUT_LEN_CHANNEL] = "";
    char ipAddrSender[MAX_IP_ADDRESS_LEN] = ""; char ipAddrReceiver[MAX_IP_ADDRESS_LEN] = "";
    int portSender, portRecevier;
    SOCKET sender_sock, receiver_sock;
    
    //validate args
    ValidateArgs(argc, 3, 4);

    //init winsock
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR)
        printf("Error at WSAStartup()\n");

    sender_sock = SocketInit();
    receiver_sock = SocketInit();

    while (1) {
        printf("enter sender details in this format: <ip_addr> <port>\n");
        gets(userInput);
        sscanf(userInput, "%s %d", ipAddrSender, &portSender);
        printf("enter receiver details in this format: <ip_addr> <port>\n");
        gets(userInput);
        sscanf(userInput, "%s %d", ipAddrReceiver, &portRecevier);
        InitSockAddr(&sender_addr, portSender, ipAddrSender);
        InitSockAddr(&receiver_addr, portRecevier, ipAddrReceiver);
    }

   
    ////init socket
    //SOCKET tcp_s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    //my_addr.sin_family = AF_INET;
    //my_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    //my_addr.sin_port = htons(6432);

    //int status = bind(tcp_s, (SOCKADDR*)&my_addr, sizeof(struct sockaddr));
    //status = listen(tcp_s, SOMAXCONN);
    ////int len = sizeof(peer_addr);

    //while (1)
    //{
    //    printf("hi\n");
    //    SOCKET s = accept(tcp_s, NULL, NULL);
    //    printf("hi2\n");
    //    int received = recv(s, recieved_buffer, MSG_SIZE, 0);
    //    if (received) printf("%s", recieved_buffer);
    //}
    return 0;
}

