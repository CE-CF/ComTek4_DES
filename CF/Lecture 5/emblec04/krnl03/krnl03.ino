#include <krnl.h>

#define STK 200
unsigned char s1[STK], s2[STK];

int nr = 0;
void t1(void)
{
  int n;
  DI(); n = nr;  nr++; EI();
  while (1) {
    Serial.print("12345678901234567890\n");
  }
}
void t2(void)
{
  int n;
  while (1) 
    Serial.print("abcdefghijklmnopqrst\n");
}

void t3(void)
{
  int n;
  while (1) 
    Serial.print("ABCDEFGHIJKLMNOPQRST\n"); 
}

void setup()
{
  Serial.begin(115200);  // for output from task 1
  pinMode(13, OUTPUT); // for blink on LED from task 2

  // init krnl so you can create 2 tasks, no semaphores and no message queues
  k_init(4, 0, 0);
  k_crt_task(t1, 11, s1, 200);
  k_crt_task(t2, 11, s1, 200);
  k_crt_task(t3, 11, s1, 200);

  k_start(1); // start kernel with tick speed 1 milli seconds
}

void loop() {
  /* loop will never be called */
}
