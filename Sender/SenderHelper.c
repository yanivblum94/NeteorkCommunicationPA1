#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include "SenderHelper.h"

int Read26Bytes(FILE* file, char* buf) {
	int bytesRead = fread(buf, 1, MSG_SIZE, file);
	if (bytesRead < 0) {
		fprintf(stderr, "read from file failed\n");
		exit(-1);
	}
	return bytesRead;
}

void PrintOutput(int blocks) {
	printf("file length: %d bytes\n", (blocks * MSG_SIZE));
	printf("sent: %d bytes\n", (blocks * HAMM_MSG_SIZE));
}

// converting 26 bytes (chars) to 208 binary chars representing 
void convertMsgToBinaryChars(char* originMsg, char* binaryMsg) {
	int size = sizeof(originMsg);
	char* temp;
	for (int i = 0; i < size; i++)
	{
		convertCharToBinary(originMsg[i], temp);
		for (int j = 0; j < 8; j++)
		{
			binaryMsg[i * 8 + j] = temp[j];
		}
	}
}


// converting char to 8 char array with it's binary represantation 
void convertCharToBinary(char c, char* binaryCharRep) {
	int temp;
	for (int i = 0; i < 8; i++)
	{
		temp = (c >> i) & 1;
		binaryCharRep[7 - i] = (temp) ? '1' :'0';
	}
}


void charsCopy(char* copyTo, char* copyFrom, int start, int length) {
	for (int i = 0; i < length; i++)
	{
		copyTo[start + i] = copyFrom[i];
	}
}

// func to encode 26bit msg to 31 bit msg with hamming code
// assuming it's ordered left to right 
// TODO: test with an example - might have problem with the msg order (little / big endian ) 
char* hammingDecode(char* originMsg) {   
	char result[32] = "";
	int i = 0, j = 0, p1 = 0, p2 = 0, p4 = 0, p8 = 0, p16 = 0, total = 0;
	while (i <= 32) {
		total = total + originMsg[i];
		if ((1 & originMsg[i]) == 1) {
			p1++;
		}
		if ((2 & originMsg[i]) == 2) {
			p2++;
		}
		if ((4 & originMsg[i]) == 4) {
			p4++;
		}
		if ((8 & originMsg[i]) == 8) {
			p8++;
		}
		if ((16 & originMsg[i]) == 16) {
			p16++;
		}
		i++;
	}
	i = 0;
	while (i <= 32) {
		switch (i)
		{
		case 0:
			result[i] = (total % 2) ? '1' : '0';
			break;
		case 1:
			result[i] = (p1 % 2) ? '1' : '0';
			break;
		case 2:
			result[i] = (p2 % 2) ? '1' : '0';
			break;
		case 4:
			result[i] = (p4 % 2) ? '1' : '0';
			break;
		case 8:
			result[i] = (p8 % 2) ? '1' : '0';
			break;
		case 16:
			result[i] = (p16 % 2) ? '1' : '0';
			break;
		default:
			result[i] = originMsg[j];
			j++;
		}
		i++;
	}
	return result;
}

char* hammingEncode(char* codedMsg) {
	int i = 1, xorResult = 0;
	int total = 0;
	char result[26] = "";
	//add function to copy from codedMsg to result on relevant indexes naively temporary 
	while (i <= 32) {
		total = total + (int)(codedMsg[i]);
		if (codedMsg[i]) {
			xorResult = xorResult ^ i;
		}
		i++;
	}
	if (xorResult) { //got a number 
		if ((total % 2) == 1 && codedMsg[0]) { //matching p0 and total 1's 
			result[xorResult] = (result[xorResult]) ? '0' : '1';
		}
		else {
			// got two mistakes - TODO ask on forum how to handle ? 

		}
	}
	else {
		if ((total % 2) == 1 && codedMsg[0]) { //matching p0 and total 1's 
			return result;
		}
		else { // miss match only on index 0 
			result[0] = (result[xorResult]) ? '0' : '1';
		}
	}

	return result;

}