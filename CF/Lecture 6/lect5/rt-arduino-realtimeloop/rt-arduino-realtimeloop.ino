/* critical region - by semaphores - aka mutex
    JDN
 *  */

#include <krnl.h>

#define STK 150
char a1[STK];

#define TASKPRIO 10


struct k_t * pTask1;

 
struct k_t *timedSem1; 
 
void task1()
{
    k_set_sem_timer(timedSem1, 100);  // 100 tick aka 100 msec realtime signal from krnl

  while (1) {
    k_wait(timedSem1, 0); // wait until kick
  
    k_eat_msec(50);  // simulate realtime code
    Serial.println(k_millis());  // NB k_millis does NOT insert leap msec as std millis

   }
}

 
void setup() {
 
  Serial.begin(115200);
  delay(2000);

  Serial.println("just bef init part");

  k_init(1,1, 0); // 2 task, 1 semaphores, 0 messaegQueues */

  pTask1 = k_crt_task(task1, 15,  a1,STK);
 
  timedSem1 = k_crt_sem(0, 1); // 1: start value, 10: max value (clipping)
  
  Serial.println("just bef k_start");

  k_start(1); /* start krnl timer speed 1 milliseconds*/

  Serial.println("If you see this then krnl didnt start :-( ");
}

void loop() {}

 
