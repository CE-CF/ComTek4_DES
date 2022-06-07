#include <krnl.h>

#define STK 150                            // Define stack size
char a[STK];                               // Stack for task consumerTask
char b[STK];                               // Stack for task producerTask

struct k_t * pTask;                        // Pointer to the struct K_t
struct k_msg_t * consumerQueue;            // Consumer Queue
struct k_t * pTask2;                       // Pointer to the struct K_t
struct k_msg_t * producerQueue;            // Producer Queue
const int bufferSize = 3;                  // Buffer size
const int bufferSize2 = 3;                 // Buffer size 
int consumerBuffer[bufferSize];            // Buffer for consumer queue
int producerBuffer[bufferSize];            // Buffer for producer queue

////////////////////////////////////////////////////////////////////
//    Consumer
///////////////////////////////////////////////////////////////////

void consumerTask(){
  int msgForConsumer;                                                  // Variable to store message
  int lost;                                                            // Receives how many lost packages since last. ISN'T REALLY USED
  int message = 2;                                                     // Variable to store message to send
  while(1){
    if (k_receive(consumerQueue, &msgForConsumer, 0,&lost) == -1)      // Receives from queue and checks whether there is something
      Serial.println("Consumer: No message :(");                       
    else{
      Serial.print("Consumer got message: ");
      Serial.println(msgForConsumer);
      k_send(producerQueue, &message);                                // Sends message to producer queue
    }
    k_sleep(1000);                                                    // Runs every second
  }
}

////////////////////////////////////////////////////////////////////
//    Producer
///////////////////////////////////////////////////////////////////

void producerTask(){
  int msgForProducer;                                                   // Variable to store which floor to go to
  int lost;                                                             // Receives how many lost packages since last. ISN'T REALLY USED
  int message = 1;                                                      // Variable to store message to send
  while(1){
    k_send(consumerQueue, &message);                                    // Sends message to consumer queue
    if (k_receive(producerQueue, &msgForProducer, 0,&lost) == -1)      // Receives from queue and checks whether there is something
      Serial.println("Producer: Got nothing");
    else{
      Serial.print("Producer Got Message: ");
      Serial.println(msgForProducer);
    }
    k_sleep(1000);                                                     // Runs every second
  }
}


////////////////////////////////////////////////////////////////////
//    SETUP
///////////////////////////////////////////////////////////////////

void setup() {                                                          
  Serial.begin(115200);
  delay(2000);

  k_init(2,0,2);                                                      // Init kernel with 2 tasks and 2 message queues

  pTask = k_crt_task(consumerTask, 3, a, STK);                        // Create consumerTask
  pTask2 = k_crt_task(producerTask, 2, b, STK);                       // Create producerTask

  pinMode(2, INPUT_PULLUP);                                           // Listen to pin 2
  noInterrupts();                                                     // Disable arduino interrupts

  consumerQueue =  k_crt_send_Q(bufferSize, sizeof(int), consumerBuffer);   // Message queue for consumer
  producerQueue =  k_crt_send_Q(bufferSize, sizeof(int), producerBuffer);   // Message queue for producer
 
  EICRA |= (1 << ISC01);                                              // Trigger INT0 on falling edge            
  EIMSK |= (1 << INT0);                                               // Enables external interrupt                      

  interrupts();                                                       // Enable arduino interrupts

  k_start(1); /* start krnl timer speed 1 milliseconds*/              // Start Kernel

}

void loop() {} // THE CAKE IS A LIE
