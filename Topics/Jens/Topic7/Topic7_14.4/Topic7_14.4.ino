#include <krnl.h>

#define STK 150                         // Define stack size
char a[STK];                            // Stack for task a

struct k_t * pTask;                     // Pointer to the struct K_t
struct k_msg_t * floorQueue;            // Pointer to message struct
const int bufferSize = 25;              // Buffer size for queue, big enough for having each floor
int floorBuffer[bufferSize];            // Buffer for floors


////////////////////////////////////////////////////////////////////
//    ISR / Floor buttons
///////////////////////////////////////////////////////////////////

ISR(INT0_vect) {                             // ISR reacting to pin 2
      int someFloor = random(1,26);          // Get random floor numberfrom 1 to 25 
      ki_send(floorQueue, &someFloor);       // Send count to task
}


////////////////////////////////////////////////////////////////////
//    Elevator
///////////////////////////////////////////////////////////////////

void Elevator(){
  int OnFloor = 1;                           // Elevator starts on floor 1
  int GoTo;                                  // Variable to store which floor to go to
  int lost;                                  // Receives how many lost packages since last. ISN'T REALLY USED
  while(1){
    if (ki_receive(floorQueue, &GoTo, &lost) == -1)           // Receives from queue and checks whether there is something
      Serial.println("Please select floor");                  // Waiting for a button to be pushed
    else{
      if (GoTo == OnFloor){                                   // If floor is the same as the "go to" floor dont move.
        Serial.print("Elevator is already on floor: ");       
        Serial.println(OnFloor);                              
      }
      else{                                                   // If floor is not the same as "go to" floor then move!
        k_eat_msec(1500);
        Serial.println("_________________________");
        Serial.print("Going to floor: ");
        Serial.println(GoTo);
        delay(500);
        Serial.println("Closing doors..");
        delay(500);
        Serial.println("Elavating to floor..");
        OnFloor = GoTo;
        Serial.print("Reached Floor: ");
        Serial.println(OnFloor);
        Serial.println("_________________________");
        Serial.println("");
      }
    }
    k_sleep(1000);
  }
}


////////////////////////////////////////////////////////////////////
//    SETUP
///////////////////////////////////////////////////////////////////

void setup() {                                                          
  Serial.begin(115200);
  delay(2000);

  k_init(1,0,1);                                                      // Init kernel with 2 tasks and 2 message queues

  pTask = k_crt_task(Elevator, 3, a, STK);                            // Create ReceiveTask


  pinMode(2, INPUT_PULLUP);                                           // Listen to pin 2
  noInterrupts();                                                     // Disable arduino interrupts

  floorQueue =  k_crt_send_Q(bufferSize, sizeof(int), floorBuffer);   // Message queue for ISR to Task

  // eller fra krnl  DI();
 
  EICRA |= (1 << ISC01);                                              // Trigger INT0 on falling edge            
  EIMSK |= (1 << INT0);                                               // Enables external interrupt                      

  interrupts();                                                       // Enable arduino interrupts

  k_start(1); /* start krnl timer speed 1 milliseconds*/              // Start Kernel

}

void loop() {} // THE CAKE IS A LIE
