#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
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
	char* temp[26];
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
/*
	function to create a 32 long char array from the origin array with integers 
	and each redundant bit has 0 initially
*/

void createIntArray(char* originMsg, int* originMsgInt) {
	int size = sizeof(originMsg);
	int j = 0, i=0;
	originMsgInt[0] = 0;
	while (i <= 32) {
		if (i == 1 || i == 2 || i == 4 || i == 8 || i == 16) {
			originMsgInt[i] = 0;
		}
		else {
			originMsgInt[i] = (originMsg[j] == '1') ? 1 : 0;
			j++;
		}
		i++;
	}
}

void createCharArray(char* targetArray, int* sourceArray) {
	int size = sizeof(sourceArray);
	for (int i = 0; i < size; i++)
	{
		targetArray[i] = (sourceArray[i]) ? '1' : '0';
	}
}


// func to encode 26bit msg to 31 bit msg with hamming code
// TODO: test with an example - might have problem with the msg order (little / big endian ) 
void hammingEncode(char* originMsg, char* encodedMsg) {   
	int i = 1,  p1 = 0, p2 = 0, p4 = 0, p8 = 0, p16 = 0;
	int* originMsgInt[32];
	char* temp[32];
	createIntArray(originMsg, originMsgInt);
	while (i <= 32) {
		if ((1 & i) == 1 && originMsgInt[i]) {
			p1++;
		}
		if ((2 & i) == 2 && originMsgInt[i]) {
			p2++;
		}
		if ((4 & i) == 4 && originMsgInt[i]) {
			p4++;
		}
		if ((8 & i) == 8 && originMsgInt[i]) {
			p8++;
		}
		if ((16 & i) == 16 && originMsgInt[i]) {
			p16++;
		}
		i++;
	}
	originMsgInt[1] = (p1 % 2) ? 1 : 0;
	originMsgInt[2] = (p2 % 2) ? 1 : 0;
	originMsgInt[4] = (p4 % 2) ? 1 : 0;
	originMsgInt[8] = (p8 % 2) ? 1 : 0;
	originMsgInt[16] = (p16 % 2) ? 1 : 0;

	createCharArray(temp, originMsgInt);
	charsCopy(encodedMsg, temp, 1, 31);

}


// should be in the reciver + didnt check it yet 
char* hammingDecode(char* codedMsg) {
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

// return the number of BYTES in the file 
int getFileSize(FILE* file) {
	int count = 0;
	char* c[26];
	while (fread(c, 1, MSG_SIZE, file) > 0) {
		count++;
	}
	rewind(file);
	return count * MSG_SIZE;
}

// function to send the file's size as string given with int
void sendFileSize(int size, SOCKET s) {
	char* filesSizeInString[SIZE_MSG_LEN];
	itoa(size, filesSizeInString, SIZE_MSG_LEN);
	write_to_sock(s, filesSizeInString, SIZE_MSG_LEN);
}

void FinishOneRound(SOCKET s, FILE* file) {
	CloseConnections(s);
	free(SENDER_BUFFER);
	free(BINARY_MESSAGE);
	wsa_clean();
	fclose(file);
}