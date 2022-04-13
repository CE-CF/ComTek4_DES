#include <krnl.h>


k_t * semWC;

void t1(void)
{

  while (1) {
    k_wait(semWC, 0); // wait 0: no timeout == forever
    Serial.print("12345678901234567890\n");
    k_signal(semWC);
  }
}
void t2(void)
{
  int n;
  while (1) {
    k_wait(semWC, 0); // wait 0: no timeout == forever
    Serial.print("abcdefghijklmnopqrst\n");
    k_signal(semWC);
  }
}

void t3(void)
{
  int n;
  while (1) {
    k_wait(semWC, 0); // wait 0: no timeout == forever
    Serial.print("ABCDEFGHIJKLMNOPQRST\n");
    k_signal(semWC);
  }
}

void setup()
{
  Serial.begin(115200);  // for output from task 1
  delay(3000);
  k_init(3, 1, 0);
  semWC = k_crt_sem(1, 10); // 1 == 1 key hang at the door !
  k_crt_task(t1, 11, 200);
  k_crt_task(t2, 11, 200);
  k_crt_task(t3, 11, 200);
  k_start(1); // start kernel with tick speed 1 milli seconds
}

void loop() {
  /* loop will never be called */
}
