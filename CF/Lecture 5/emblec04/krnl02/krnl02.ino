#include <krnl.h>

#define STK 200
unsigned char s1[STK], s2[STK], s3[STK], s4[STK];


int nr = 0;
void t1(void)
{
  int n;
  DI(); n = nr;  nr++; EI();
  while (1) {
    Serial.println(n);
  }
}

void setup()
{
  Serial.begin(115200);  // for output from task 1
  pinMode(13, OUTPUT); // for blink on LED from task 2

  // init krnl so you can create 2 tasks, no semaphores and no message queues
  k_init(4, 0, 0);
  k_crt_task(t1, 11, 200, s1);
  k_crt_task(t1, 11, 200, s2);
  k_crt_task(t1, 11, 200, s3);
  k_crt_task(t1, 11, 200, s4);

  k_start(1); // start kernel with tick speed 1 milli seconds
}

void loop() {
  /* loop will never be called */
}
