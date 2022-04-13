#include <krnl.h>


k_t * semSampling;

void t1(void)
{
int i = 0;
  k_set_sem_timer(semSampling, 20);
  while (1) {
    k_wait(semSampling, 0); // wait 0: no timeout == forever
    k_eat_ticks(34);
    Serial.println(i++);  
  }
}
void setup()
{
  Serial.begin(115200);  // for output from task 1
  delay(3000);
  k_init(1, 1, 0);
  semSampling = k_crt_sem(0, 2); // 0== 1 key hang at the door !
  k_crt_task(t1, 11, 200);
  k_start(1); // start kernel with tick speed 1 milli seconds
}

void loop() {
  /* loop will never be called */
}
