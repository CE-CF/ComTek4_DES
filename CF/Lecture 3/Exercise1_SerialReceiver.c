#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Used to convert bytes to their decimal representation

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c \n"
#define BYTE_TO_BINARY(byte)  \
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
struct Frame {
	int message;
	int receiverID_low;
	int receiverID_mid;
	int receiverID_hig;
	int transducerLevel;
	int quietTime;
	int mastersInRange;
	struct Data* data;
} frame;


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
		printf("Binary number of %c is: ",decNumber);
	}
	else {
		printf("Last 8 bit representation of %c is: ",decNumber);
	}
	for (int j=i-1;j>=0; j--) {
		printf("%c",binaryNum[j]);
	}
	printf("\n\n");
	//return returnBin;
}

// Flectcher 8-bit is a checksum that can be used to verify the packet that is send
int Fletcher_7bit(char *framePacket, int packetLen, char *checksum){
	int CK_A = 0, CK_B = 0;
	for (int i = 1; i < packetLen; i++){
		CK_A += (int)(framePacket[i]);
		CK_B += CK_A;
		//printf("framePacket[%i]: %d\nCK_A: %d\nCK_B: %d\n\n", i, framePacket[i], CK_A, CK_B);
	}

	CK_A = CK_A%128;	
	CK_B = CK_B%128;
	//printf("Checkum CK_A | %i \n",CK_A);
	//printf("Checkum CK_B | %i \n",CK_B);
	checksum[0] = CK_A;
	checksum[1] = CK_B;
	return 0;
}



void assignment1_escape(size_t size, char *inputString, struct Rules rules, int *escaped){
	printf("Escaping the escapable chars where |B: Before| and |A: 0x10 & B+0x20|:\n\n");
	for (int i = 1; i<size; i++){
		int escapeable = 0;
		for (int j = 0; j<3; j++){
			if (inputString[i] == rules.escapeable[j]){
				escapeable = 1;
			}	
		}
		if (escapeable != 0){
			size++;
			// shift elements forward to fit an escape character
	    for (int j = size-1; j >= i; j--)
        inputString[j] = inputString[j-1];
      inputString[i+1] = inputString[i+1]+0x20;
      inputString[i] = 0x10;
      printf("| B: %c | A: %c %d | ", inputString[i+1]-0x20,inputString[i], inputString[i+1]);
      *escaped+=1;
			i++;
		}
		else {
			printf("%d ",inputString[i]);
		}
	}
	printf("\n\n");
}

void assignment1_restore(size_t size, char *inputString, struct Rules rules, int *restored){
	printf("\n\n#########################################################\n\n");

	printf("Restoring the escaped chars where |B: 0x10 & Escaped| and |A: Escaped-0x20|:\n\n");
	for (int i = 0; i<size; i++){
		if (inputString[i] == rules.escapeChar){
			inputString[i] = inputString[i+1]-0x20;

			for (int j = i+1; j <size+1; j++){
        inputString[j] = inputString[j+1];
			}
			printf("| B: %c %d | A:  %c | ", 0x10, inputString[i]+0x20, inputString[i]);
    	*restored+=1;  
		}
		else {
			printf("%d ",inputString[i]);
		}
	}
}

void assignment4(char *testFrame, struct Frame frame){
	int arrayCounter = 9;
	for (int i = 0; i<testFrame[7];i++){
		//printf("I er : %i \n", i);
		for (int j = 0; j<6; j++){
			//printf("J er : %i \n", j);
			if (j==0){
				frame.data[i].RSSI = testFrame[arrayCounter];
				arrayCounter++;
			}
			if (j==1){
				frame.data[i].TxID_low = testFrame[arrayCounter];
				arrayCounter++;
			}
			if (j==2){
				frame.data[i].TxID_med = testFrame[arrayCounter];
				arrayCounter++;	
			}
			if (j==3){
				frame.data[i].TxID_hig = testFrame[arrayCounter];
				arrayCounter++;
			}
			if (j==4){
				frame.data[i].TxID_time_low = testFrame[arrayCounter];
				arrayCounter++;
			}
			if (j==5){
				frame.data[i].TxID_time_high = testFrame[arrayCounter];
				arrayCounter++;	
			}
		}
	}
}

void assignment5(char arr[]){
	printf("\n\n#########################################################\n\n");
	printf("Number of masters in range: %i\n\n", arr[8]);
}

void assignment6(struct Frame frame) {
	for (int i = 0; i<frame.mastersInRange; i++){
		printf("Data for master #%i:\n",(i+1));
		printf("---------------------\n");
		printf("RSSI_%i: %i\n",i+1, frame.data[i].RSSI);
		printf("TxID_%i_Low: %i\n",i+1, frame.data[i].TxID_low);
		printf("TxID_%i_Middle: %i\n",i+1,  frame.data[i].TxID_med);
		printf("TxID_%i_High: %i\n",i+1,  frame.data[i].TxID_hig);
		printf("TxID_%i_time_Low: %i\n",i+1,  frame.data[i].TxID_time_low);
		printf("TxID_%i_time_High: %i",i+1,  frame.data[i].TxID_time_high);
		printf("\n\n#########################################################\n\n");
	}
}

void generateFrame(int mastersInRange, char *resultFrame, struct Rules rules, int *finalSize){
	printf("Generating a test frame with %i masters in range", mastersInRange);
	printf("\n\n#########################################################\n\n");
	int layer3_test_size = 6*mastersInRange;	// Simulating 18 byte layer 3 data 
	char layer3[layer3_test_size]; 					// Char array at size to be able to insert escape chars
	
	for (int i = 0; i<layer3_test_size;i++){
		layer3[i] = rand() % 128; // Fill data with ASCII from 33-127
	}
	int staticDataCounter = 9;
	size_t myLayer3Size = (layer3_test_size+staticDataCounter);  // Length of the layer 3 data

	// Generating random values for static layer 3
	char Type = rand() % 5;
	char ReceiverID_Low = rand() % 3;
	char ReceiverID_Middle = (rand() % (5-3))+3;
	char ReceiverID_High = (rand() % (10-5))+5;
	char TransducerLevel = rand() % 10;
	char QuietTime = rand() % 5;
	char MIR = layer3_test_size/6;



	struct Frame frame = {Type, ReceiverID_Low, ReceiverID_Middle, ReceiverID_High, TransducerLevel, QuietTime, MIR};
	printf("Type: %i\n", frame.message);
	printf("ReceiverID_Low: %i\n", frame.receiverID_low);
	printf("ReceiverID_Middle: %i\n", frame.receiverID_mid);
	printf("ReceiverID_High: %i\n", frame.receiverID_hig);
	printf("TransducerLevel: %i\n", frame.transducerLevel);
	printf("Quiet Time: %i\n", frame.quietTime);
	printf("# Masters in range: %i", frame.mastersInRange);

	printf("\n\n#########################################################\n\n");

	size_t frame_test_size = layer3_test_size+12;

	// Generating the length + initial layer 3 data

	resultFrame[1] = myLayer3Size;
	resultFrame[2] = frame.message;
	resultFrame[3] = frame.receiverID_low;
	resultFrame[4] = frame.receiverID_mid;
	resultFrame[5] = frame.receiverID_hig;
	resultFrame[6] = frame.transducerLevel;
	resultFrame[7] = frame.quietTime;
	resultFrame[8] = frame.mastersInRange;

	// Generating the layer 3 master data
	
	for (int i = 0; i < layer3_test_size; i++){
		resultFrame[i+staticDataCounter] = layer3[i];
	}
	staticDataCounter += layer3_test_size;
	
	printf("Values used to calculate the Fletcher_7bit checksum:\n\n");
	for (int i = 1; i<(9+6*mastersInRange);i++){
		printf("%d ",resultFrame[i]);	
	}
	printf("\n\n#########################################################\n\n");

	char checksum[2] = {0,0};
	Fletcher_7bit(resultFrame, staticDataCounter, checksum);

	resultFrame[staticDataCounter] = checksum[0];
	resultFrame[staticDataCounter+1] = checksum[1];


	printf("Checksum lower(DEC): %d\nChecksum lower(CHAR): %c\n\nChecksum upper(DEC): %d\nChecksum upper(CHAR): %c \n\n", checksum[0], checksum[0], checksum[1], checksum[1]);
	printf("Resulting in a testFrame with checksum:\n\n");

	for (int i = 1; i<(11+6*mastersInRange);i++){
		printf("%d ",resultFrame[i]);	
	}
	printf("\n\n#########################################################\n\n");
	int escaped = 0;
	assignment1_escape((staticDataCounter+2), resultFrame, rules, &escaped);
	printf("escaped: %d",escaped);
	printf("\n\n#########################################################\n\n");
	resultFrame[0] = 0x02;
	resultFrame[staticDataCounter+2+escaped] =	0x03;
	printf("Then startByte and stopByte are added to the frame:\n");
	printf("Frame [StartByte, length, layer3, checksum_low, checksum_upper, stopByte]:\n\n");
	for (int i = 0; i<(12+6*mastersInRange+escaped);i++){
		printf("%d ",resultFrame[i]);	
	}
	*finalSize = (12+6*mastersInRange);

}


int main(int argc, char *argv[]) {
	char testFrame[100];
	// Defining the rules used in the assignment
	struct Rules rules = {0x02, 0x03, 0x10, {0x02, 0x03, 0x10}};
	
	// Generate a test frame with <mir> number of masters in range
	int mir = 3;
	int testFrame_size = 0; 
	generateFrame(mir, testFrame, rules, &testFrame_size);
	int restored =0;
	assignment1_restore(testFrame_size, testFrame, rules, &restored);
	struct Frame frame = {testFrame[2], testFrame[3], testFrame[4], testFrame[5], testFrame[6], testFrame[7], testFrame[8]};
	frame.data = malloc(testFrame[7] * sizeof(struct Data));
	assignment4(testFrame, frame);
	assignment5(testFrame);
	assignment6(frame);
	
	
	printf("0x03 ASCII       : %c \n",0x03);
	printf("0x03 DECIMAL     : %i \n",0x03);
	printf("0x03 BINARY      : "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(0x03));
	printf("\n+0x20  ASCII     : %c \n",(0x03+0x20));
	printf("+0x20  DECIMAL   : %i \n",(0x03+0x20));
	char 	xor = 0x03+0x20;
	printf("+0x20  BINARY    : "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(xor));
	char revxor = xor-0x20;
	printf("\nRESTORED  ASCII  : %c \n",revxor);
	printf("RESTORED  DECIMAL: %i \n",revxor);
	printf("RESTORED  BINARY : "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(revxor));
	
}



