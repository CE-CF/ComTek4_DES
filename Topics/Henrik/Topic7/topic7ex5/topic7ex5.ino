#include <krnl.h>
#define STKSIZE 100
#define TASKPRIO 10

int sem1;
int sem2;
int period1 = 50, period2 = 80;
int execTime = 20;
char stakA[STKSIZE], stakB[STKSIZE];

void t1(void) 
{
  k_set_sem_timer(sem1, period1);         // timer to the semaphore so KRNL will signal the semaphore with regular intervals
  while (1) {
    k_wait(sem1, 0);                      // wait until semaphore signal
    digitalWrite(6,HIGH);
    digitalWrite(7,LOW);
    k_eat_msec(execTime);                 // execution time
    digitalWrite(6,LOW);
  }
}
void t2(void)
{
  k_set_sem_timer(sem2, period2);
  while (1) {
    k_wait(sem2, 0);
    digitalWrite(7,HIGH);
    digitalWrite(6,LOW);
    k_eat_msec(execTime);
    digitalWrite(7,LOW);
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  delay(3000);
  k_init(2, 2, 0);                                                // 2 tasks and 2 semaphore
  k_crt_task(t1, TASKPRIO, stakA, STKSIZE);                       // The tasks have the same priority
  k_crt_task(t2, TASKPRIO, stakB, STKSIZE);                       // ------------||--------------
  sem1 = k_crt_sem(1, 10);                                        // 1 == 1 key hang at the door !
  sem2 = k_crt_sem(1, 10);                                        // 1 == 1 key hang at the door !
  k_start(1);                                                     // start kernel with tick speed 1 milli seconds
}

void loop() {
  /* loop will never be called */
}
