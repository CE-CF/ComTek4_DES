#include <krnl.h>

#define STK 150

struct k_t *pTask;                      // Pointer to task descriptor

unsigned char blinkerStak[STK];         // Stack for the blinker
unsigned char writerStak[STK];          // Stack for the writer  
unsigned long counter;                  // Counter long

void blinker(){
  while(1){
    Serial.print("Blinker: ");
    Serial.println(k_millis()); // Uncomment to check blinker interval.. stays on and off for 50
    digitalWrite(13, !digitalRead(13)); // toggle the LED
    k_sleep(50);                        // Kernel sleep, this doesn't eat time from other tasks
  }
}

void writer(){
  while(1){
    Serial.print("Writer: ");
    Serial.println(k_millis());       // Uncomment to check writer interval
    counter += 1;                       // Counter
    Serial.print("Count: ");            
    Serial.println(counter);
    k_sleep(400);                       // Kernel sleep, this doesn't eat time from other tasks
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial);                      // Wait for Serial to begin
  Serial.println("Waiting on startup");
  
  pinMode(13, OUTPUT);

  k_init (2,0,0);                      // INIT kernel 2 tasks, 0 Semaphores, 0 msg queues.

  pTask = k_crt_task (blinker, 10, blinkerStak, STK);    // Setup blinker task with priority 10
  pTask = k_crt_task (writer, 8, writerStak, STK);       // Setup writer task with priority 8

  k_start(1);                         // Start
  
}

void loop() {} // THE CAKE IS A LIE
