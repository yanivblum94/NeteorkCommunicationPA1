#ifndef SENDER_HELPER
#define SENDER_HELPER

#include "../Common/Utils.h"

int Read26Bytes(FILE* file, char* buf);

void PrintOutput(int blocks);

void hammingEncode(char* originMsg, char* encodedMsg);

// converting 26 bytes (chars) to 208 binary chars representing 
void convertMsgToBinaryChars(char* originMsg, char* binaryMsg) {


#endif