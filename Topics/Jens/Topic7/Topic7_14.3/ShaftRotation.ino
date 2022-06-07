#include <krnl.h>
struct k_t *pTask;

#define STK 350
unsigned char taskStak[STK];
const int calibration_cost = 12;
int rotation_count = 0;

void determine_distance() {
  /* Dummy code */
  while(1) {
  Serial.println("Calculating cumulative distance");
  Serial.println(rotation_count);
  k_eat_msec(500);
  k_sleep(1000);
  }
}

void setup() {
  // Standard Setup
  Serial.begin(115200);
  pinMode(2, INPUT_PULLUP);

  noInterrupts();
  EICRA |= (1<<ISC01);
  EIMSK |= (1<<INT0);

  interrupts();
  
  // Code
  k_init(1, 0, 0);
  pTask = k_crt_task(determine_distance, 10, taskStak, STK);
  k_start(1);
}

ISR(INT0_vect) {
  rotation_count++;
}

void loop() {}
