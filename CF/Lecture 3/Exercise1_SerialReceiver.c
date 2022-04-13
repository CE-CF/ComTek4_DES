#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c \n"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

struct Rules {
	char startByte;
	char stopByte;
	char escapeChar;
	char escapeable[4];
} rules;

struct Data {
	int RSSI;
	int TxID_low;
	int TxID_med;
	int TxID_hig;
	int TxID_time_low;
	int TxID_time_high;
} data;

struct Type {
	int message;
	int receiverID_low;
	int receiverID_mid;
	int receiverID_hig;
	int transducerLevel;
	int quietTime;
	int mastersInRange;
	struct Data* data;
} type;

int testPacket[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42}; // For at simulere en datapakke på 42 byte
int testFrame[] = {0x02,43,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42, }; // Indeholder: Hej<0x10>?


void decToBinary(int n){
	int decNumber = n;
	int binaryNum[32];
	int i = 0;
	while (n > 0) {
		if (i<8){
			binaryNum[i] = n%2;
			n = n/2;
			i++;	
		}
		else {
			n = n/2;
		}	
	}
	printf("Binary number of %i is: ",decNumber);
	for (int j=i-1;j>=0; j--) {
		printf("%i",binaryNum[j]);
	}
	printf("\n\n");
	//return returnBin;
}
int Fletcher_8bit(int framePacket[], int packetLen){
	int CK_A = 0, CK_B = 0;
	for (int i = 0; i < packetLen; i++){
		CK_A += framePacket[i];
		CK_B += CK_A;
	}
	CK_A = CK_A%256;
	CK_B = CK_B%256;
	//printf("Checkum CK_A | %i \n",CK_A);
	//printf("Checkum CK_B | %i \n",CK_B);
	return 0;

}

char ESC(int inputString[], int dataFrameEntry){
	char ch = inputString[dataFrameEntry];
	
}
void assignment1(int inputString[], struct Rules rules){
	
	for (int i = 0; i<sizeof(testFrame); i++){
		if (testFrame[i]==rules.escapeChar){
			char nextCh = inputString[i+1];
			printf("%c",nextCh ^ 0x10);	;
			i++;
		}
		else {
			printf("%c",testFrame[i]);
		}
	}
	printf("\n\n");
}

void assignment4(int frame[], struct Type type){
	int arrayCounter = 7;
	for (int i = 0; i<testPacket[6];i++){
		// printf("I er : %i \n", i);
		for (int j = 0; j<6; j++){
			// printf("J er : %i \n", j);
			if (j==0){
				type.data[i].RSSI = testPacket[arrayCounter];
				arrayCounter++;
			}
			if (j==1){
				type.data[i].TxID_low = testPacket[arrayCounter];
				arrayCounter++;
			}
			if (j==2){
				type.data[i].TxID_med = testPacket[arrayCounter];
				arrayCounter++;	
			}
			if (j==3){
				type.data[i].TxID_hig = testPacket[arrayCounter];
				arrayCounter++;
			}
			if (j==4){
				type.data[i].TxID_time_low = testPacket[arrayCounter];
				arrayCounter++;
			}
			if (j==5){
				type.data[i].TxID_time_high = testPacket[arrayCounter];
				arrayCounter++;	
			}
		}
	}
}

void assignment5(int arr[]){
	printf("Number of masters in range: %i\n\n", arr[6]);
}

void assignment6(struct Type type) {
	for (int i = 0; i<type.mastersInRange; i++){
		printf("Data for master #%i:\n",(i+1));
		printf("---------------------\n");
		printf("TxID_%i_Low: %i\n",i, type.data[i].TxID_low);
		printf("TxID_%i_Middle: %i\n",i,  type.data[i].TxID_med);
		printf("TxID_%i_High: %i\n",i,  type.data[i].TxID_hig);
		printf("TxID_%i_time_Low: %i\n",i,  type.data[i].TxID_time_low);
		printf("TxID_%i_time_High: %i\n\n",i,  type.data[i].TxID_time_high);
	}
}



int main(int argc, char *argv[]) {
	struct Rules rules = {0x02, 0x03, 0x10, {0x02, 0x03, 0x10, 0x20}};
	struct Type type = {testPacket[0], testPacket[1], testPacket[2], testPacket[3], testPacket[4], testPacket[5], testPacket[6]};
	type.data = malloc(testPacket[6] * sizeof(struct Data));
	//Fletcher_8bit(testPacket, testFrame[1]);
	//assignment1(testPacket, rules);
	assignment4(testPacket, type);
	assignment5(testPacket);
	assignment6(type);
	

	/*
	printf("Message type: %i\n", type.message);
	printf("3 masters RSSI: %i\n", type.data[2].RSSI);
	printf("3 masters RSSI i binary: " BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(type.data[2].RSSI));
	*/
	/*
	printf("0x3F ASCII: %c \n",0x3F);
	printf("0x3F BINARY : "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(0x3F));
	printf("0x10 ASCII: %c \n",0x10);
	printf("0x10 BINARY : "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(0x10));
	printf("XOR  ASCII: %c \n",0x3F^0x10);
	char fisk = 0x3F^0x10;
	printf("XOR  BINARY : "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(fisk));
	//printf("%c",0x3F^0x10) 
	*/
}



