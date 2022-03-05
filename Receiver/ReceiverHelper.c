#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include "ReceiverHelper.h"


void PrintOutput(int blocks, int errors) {
	printf("recevid: %d bytes\n", (blocks * HAMM_MSG_SIZE));
	printf("wrote: %d bytes\n", (blocks * MSG_SIZE));
	printf("corrected %d errors\n", errors);
}
