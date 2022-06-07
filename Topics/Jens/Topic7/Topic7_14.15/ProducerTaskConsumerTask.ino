#include <krnl.h>
struct k_t *pTask1, *pTask2;

#define STK 350
unsigned char taskStak1[STK], taskStak2[STK];

struct k_msg_t *producerQueue, *consumerQueue;
const int buffersize = 25;
int producerBuffer[buffersize], consumerBuffer[buffersize];

void producer() {
  while (1) {
    Serial.println("Producer starts");
    int msg = random(0, 128);
    k_send(consumerQueue, &msg);
    int receiveMsg;
    int lostMsg;
    if (k_receive(producerQueue, &receiveMsg, 10, &lostMsg) == 0) {
      Serial.print("Message received: ");
      Serial.println(receiveMsg);
    }
    Serial.println("Producer sleeps");
    k_sleep(500);
  }
}


void consumer() {
  while (1) {
    Serial.println("Consumer starts");
    int receivedMsg;
    int lostedMsg;
    if (k_receive(consumerQueue, &receivedMsg, 10, &lostedMsg) == 0) {
      Serial.print("Message received: ");
      Serial.println(receivedMsg);
      int reply = 133769;
      k_send(producerQueue, &reply);
    }
    Serial.println("Consumer starts");
    k_sleep(1000);
  }
}



void setup() {
  // Standard Setup
  Serial.begin(115200);
  pinMode(2, INPUT_PULLUP);
  delay(500);

  noInterrupts();
  EICRA |= (1<<ISC01);
  EIMSK |= (1<<INT0);
  producerQueue = k_crt_send_Q(buffersize, sizeof(int), producerBuffer);
  consumerQueue = k_crt_send_Q(buffersize, sizeof(int), consumerBuffer);
  interrupts();

  // Code
  k_init(2, 0, 2);
  pTask1 = k_crt_task(producer, 12, taskStak1, STK);
  pTask2 = k_crt_task(consumer, 14, taskStak2, STK);
  k_start(1);
}

ISR(INT0_vect) {
  int msg = random(0, 128);
  k_send(consumerQueue, &msg);
}


void loop() {}
