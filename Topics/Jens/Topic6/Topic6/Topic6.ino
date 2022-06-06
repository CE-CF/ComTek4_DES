#include <krnl.h>

#define STK 150                         // Define stack size
char a[STK];                            // Stack for task a
char b[STK];                            // Stack for task b

struct k_t * pTask1;                    // Pointer to the struct K_t
struct k_t * pTask2;                    // Also pointer for K_t... this might be redundant could just use the one above.
struct k_msg_t * msgQ;                  // Pointer to message struct
struct k_msg_t * mPoint;                // Pointer to message struct 
const int BUFSZ = 6;                    // Buffer size for ISR to task
const int BufferSize = 7;               // Buffer size for Task to ISR

int bufForQ[BUFSZ];                     // Buffer
int messageBuffer[BufferSize];          // Buffer

int number;                             // Variable for the random number in range [122;124]
int resultISR;                          // Varible for ISR to save result
int isrLost;                            // Variable for ISR to count lost messages

//volatile int ISRoverflow = 0;           // ISR overflow thing 
volatile int nrISR = 0;                 // Counter counting how many times isr has received 123 
//volatile unsigned long last = 0;        

////////////////////////////////////////////////////////////////////
//    ISR
///////////////////////////////////////////////////////////////////

ISR(INT0_vect, ISR_NAKED) {                         // ISR reacting to pin 2
  PUSHREGS();                                       //  Pushes to registers
  if (!k_running)                                   // If Kernel isn't running go to exit
    goto exitt;
   ki_receive(mPoint, &resultISR, &isrLost);        // Receive from message queue
   Serial.print("ISR Received: ");                  // Print
   Serial.println(resultISR);                       // Print
   if (resultISR == 123){                           // Checks if message is 123
    nrISR++;                                        // Increments counter
    ki_send(msgQ, &nrISR);                          // Send count to task
   }
  K_CHG_STAK();                                     // Something with stack???

exitt:
  POPREGS();                                        // Pops from registers
  RETI();                                           // RETURN????
}

////////////////////////////////////////////////////////////////////
//    Receive Task
///////////////////////////////////////////////////////////////////

void receiveTask(){                                       // Task for the receiver
  int result;                                             // Variable for result
  int counter;                                            // Variable for the counter;
  int lost;                                               // Variable for how many messages lost 

  while(1){
    if(-1 == ki_receive(msgQ, &result, &lost)){           // Checks whether receive function failed
      counter++;                                          // Counts how many times it has failed
//      Serial.print("Failed to receive: "); 
//      Serial.println(counter);
    }
    else {                                                // Prints results of the message received
      Serial.println("_________________");
      Serial.print("Got message: "); 
      Serial.println(result);

      Serial.print("Lost since last: ");
      Serial.println(lost);
      Serial.println("_________________");
      Serial.println("");
    }
    k_sleep(50);                                          // Runs every 50 milliseconds
  }
}

////////////////////////////////////////////////////////////////////
//    Send Task
///////////////////////////////////////////////////////////////////

void sendTask(){                                        // Send Task
  while(1){
    number = random(122,125);                           // Generates random number that can be 122, 123 or 124
    if (ki_send(mPoint, &number) == -1){                // Send the random number and check whether it failed
//      Serial.println("Message not sent");
    }
    else{
      Serial.println("Message sent");
    }
    k_sleep(1000);                                      // Run every second
  }
}

////////////////////////////////////////////////////////////////////
//    SETUP
///////////////////////////////////////////////////////////////////

void setup() {                                                          
  Serial.begin(115200);
  delay(2000);

  k_init(2,0,2);                                                      // Init kernel with 2 tasks and 2 message queues

  pTask1 = k_crt_task(receiveTask, 10, a, STK);                       // Create ReceiveTask
  pTask2 = k_crt_task(sendTask, 5, b, STK);                           // Create sendTask

  pinMode(2, INPUT_PULLUP);                                           // Listen to pin 2
  noInterrupts();                                                     // Disable arduino interrupts

  msgQ =  k_crt_send_Q(BUFSZ, sizeof(int), bufForQ);                  // Message queue for ISR to Task
  mPoint = k_crt_send_Q(BufferSize, sizeof(int), messageBuffer);      // Message queue for Task to ISR

  // eller fra krnl  DI();
 
  EICRA |= (1 << ISC01);                                              // Trigger INT0 on falling edge            
  EIMSK |= (1 << INT0);                                               // Enables external interrupt                      

  interrupts();                                                       // Enable arduino interrupts

  k_start(1); /* start krnl timer speed 1 milliseconds*/              // Start Kernel

}

void loop() {} // THE CAKE IS A LIE
