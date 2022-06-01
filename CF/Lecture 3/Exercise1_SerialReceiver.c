#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Used to convert bytes to their decimal representation

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

// Rules defined in the assignment written as a struct
struct Rules {
	char startByte;
	char stopByte;
	char escapeChar;
	char escapeable[4];
} rules;

// Data for each master defined as a struct
struct Data {
	int RSSI;
	int TxID_low;
	int TxID_med;
	int TxID_hig;
	int TxID_time_low;
	int TxID_time_high;
} data;

// Define the type of packet with contents
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


void intToBinary(int n){
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
	if (decNumber<255) {
		printf("Binary number of %i is: ",decNumber);
	}
	else {
		printf("Last 8 bit representation of %i is: ",decNumber);
	}
	for (int j=i-1;j>=0; j--) {
		printf("%i",binaryNum[j]);
	}
	printf("\n\n");
	//return returnBin;
}

// Flectcher 8-bit is a checksum that can be used to verify the packet that is send
int Fletcher_8bit(int *framePacket, int packetLen, int *checksum){
	int CK_A = 0, CK_B = 0;
	for (int i = 0; i < packetLen; i++){
		CK_A += framePacket[i];
		CK_B += CK_A;
	}
	CK_A = CK_A%256;
	CK_B = CK_B%256;
	//printf("Checkum CK_A | %i \n",CK_A);
	//printf("Checkum CK_B | %i \n",CK_B);
	checksum[0] = CK_A;
	checksum[1] = CK_B;
	return 0;
}



void assignment1(size_t size, int *inputString, struct Rules rules){
	for (int i = 0; i<size; i++){
		char nextCh = inputString[i+1];

		if (inputString[i]!=rules.escapeChar){
			
			printf("%d ",inputString[i]);
		

			
		}
		else {
			int escapeable = 0;
			for (int i = 0; i<sizeof(rules.escapeable); i++){
				if (nextCh == rules.escapeable[i]){
					escapeable = 1;
				}	
			}
			if (escapeable != 0){
				printf("%d ",nextCh ^ 0x20);
				i++;
			}
		}
	}
	printf("\n\n");
}

void assignment4(int *frame, struct Type type){
	int arrayCounter = 8;
	for (int i = 0; i<frame[7];i++){
		//printf("I er : %i \n", i);
		for (int j = 0; j<6; j++){
			//printf("J er : %i \n", j);
			if (j==0){
				type.data[i].RSSI = frame[arrayCounter];
				arrayCounter++;
			}
			if (j==1){
				type.data[i].TxID_low = frame[arrayCounter];
				arrayCounter++;
			}
			if (j==2){
				type.data[i].TxID_med = frame[arrayCounter];
				arrayCounter++;	
			}
			if (j==3){
				type.data[i].TxID_hig = frame[arrayCounter];
				arrayCounter++;
			}
			if (j==4){
				type.data[i].TxID_time_low = frame[arrayCounter];
				arrayCounter++;
			}
			if (j==5){
				type.data[i].TxID_time_high = frame[arrayCounter];
				arrayCounter++;	
			}
		}
	}
}

void assignment5(int arr[]){
	printf("Number of masters in range: %i\n\n", arr[7]);
}

void assignment6(struct Type type) {
	for (int i = 0; i<type.mastersInRange; i++){
		printf("Data for master #%i:\n",(i+1));
		printf("---------------------\n");
		printf("RSSI_%i: %i\n",i, type.data[i].RSSI);
		printf("TxID_%i_Low: %i\n",i, type.data[i].TxID_low);
		printf("TxID_%i_Middle: %i\n",i,  type.data[i].TxID_med);
		printf("TxID_%i_High: %i\n",i,  type.data[i].TxID_hig);
		printf("TxID_%i_time_Low: %i\n",i,  type.data[i].TxID_time_low);
		printf("TxID_%i_time_High: %i\n\n",i,  type.data[i].TxID_time_high);
	}
}



int main(int argc, char *argv[]) {
	
	int checksum[2]; 	
	
										// start,msg,length																																																																	Chck_low, Chck_up, stop
	int testFrame[53] = {0x02,111,2,3,4,5,6,7,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,23,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,0,0,0x03}; // For at simulere en datapakke på 43 byte
	Fletcher_8bit(testFrame,48, checksum);
	printf("Checksum lower: %i\nChecksum upper: %i \n", checksum[0], checksum[1]);
	testFrame[50] = checksum[0];
	testFrame[51] = checksum[1];

	// Defining the rules used in the assignment
	struct Rules rules = {0x02, 0x03, 0x10, {0x02, 0x03, 0x10, 0x20}};
	struct Type type = {testFrame[1], testFrame[0], testFrame[3], testFrame[4], testFrame[5], testFrame[6], testFrame[7]};
	type.data = malloc(testFrame[7] * sizeof(struct Data));

	int testFrame_size = sizeof(testFrame)/sizeof(*testFrame);
	assignment1(testFrame_size, testFrame, rules);
	assignment4(testFrame, type);
	assignment5(testFrame);
	assignment6(type);
	
	/*
	printf("Message type: %i\n", type.message);
	printf("3 masters RSSI: %i\n", type.data[2].RSSI);
	printf("3 masters RSSI i binary: " BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(type.data[2].RSSI));
	*/

	printf("0x03 ASCII: %c \n",0x03);
	printf("0x03 DECIMAL: %i \n",0x03);
	printf("0x03 BINARY : "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(0x03));
	printf("\nXOR  ASCII: %c \n",(0x03^0x20));
	printf("XOR  DECIMAL: %i \n",(0x03^0x20));
	char 	xor = 0x03^0x20;
	printf("XOR  BINARY : "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(xor));
	char revxor = xor^0x20;
	printf("\nREVXOR  ASCII: %c \n",revxor);
	printf("REVXOR  DECIMAL: %i \n",revxor);
	printf("REVXOR  BINARY : "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(revxor));
}



