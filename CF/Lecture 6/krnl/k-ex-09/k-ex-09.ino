
/* k-ex-01
    Two task's
    preemptive schedule

    a : busy waiting using CPU time for delat
    b:  passive waiting - no CPU usage when sleeping
    One task printing own usage of stak
*/

#include <krnl.h>

#define STKSIZE 100

#define TASKPRIO 10

char stakA[STKSIZE], stakB[STKSIZE],stakC[STKSIZE];
struct k_t * pA,*pB, *pC;

struct k_t * sem1;

void a() {
  while (1) {
    k_sleep(2000);
    k_wait(sem1,0);
    k_eat_msec_time(400);
    k_signal(sem1);
    while (1);
  }
}

void d()
{
}

void b() {
  d();

  while (1) {
    k_sleep(3000);
    k_eat_msec_time(10000);
  }
}


void c() {
  while (1) {
    k_sleep(1000);
    k_wait(sem1,0);
    k_eat_msec_time(4000);
    k_signal(sem1);
  }

}

 
void setup() {


  pinMode(13, OUTPUT);

  k_init(3, 1, 0); // 1 task, 1 semaphores, 0 messaegQueues */

  pA = k_crt_task(a, TASKPRIO-1, stakA, STKSIZE);
  pB = k_crt_task(b, TASKPRIO, stakB, STKSIZE);
  pC = k_crt_task(c, TASKPRIO+1, stakC, STKSIZE);
  sem1 = k_crt_sem(1, 2);

  k_start(1); /* start krnl timer speed 1 milliseconds*/

}

void loop() {}

extern "C" {

  //  http://kom.aau.dk/~jdn/edu/doc/arduino/uno.png
  // led 13 on UNO is bit no 45 in PoRTBB regs

  void k_breakout() // called every task shift from dispatcher
  {

    if (pRun->nr == 1) {
      PORTB |=  0x20;   // bit 5 on bq we are dummy

    }
    else {
      PORTB &=  0xdf;
    }

    // for a MEGA you have to find another port :-)
    // port K (adc8-15) seems feasible
  }
}
