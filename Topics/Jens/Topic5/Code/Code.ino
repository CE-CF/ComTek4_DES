#include <krnl.h>
struct k_t *pTask1, *pTask2;

#define STK 350
unsigned char taskStak1[STK], taskStak2[STK];
int v = 0;

void printVar() {
  while(1) {
  Serial.println(v);
  Serial.println(k_millis());
  k_sleep(400);
  }
}

void incrVar() {
  while(1) {
    v++;
    Serial.println(k_millis());
    k_sleep(2000);
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(13, OUTPUT);

  k_init (2, 0, 0);

  pTask1 = k_crt_task(printVar, 10, taskStak1, STK);
  pTask2 = k_crt_task(incrVar, 12, taskStak2, STK);
  k_start(1);
 
}

void loop() {}
