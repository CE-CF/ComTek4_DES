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

char stakA[STKSIZE], stakB[STKSIZE];
struct k_t * pA, *pB;

struct k_t * sem1;

long sharedVar = 1;
void a()
{
  while (1) {
    // critical region
    k_wait(sem1, 0); // wait forever
    sharedVar++;
    k_signal(sem1);
    k_eat_msec_time(200); // eat 20 msec

  }
}

void b()
{
  while (1) {
    // critical region
    k_wait(sem1, 0); // wait forever
    sharedVar++;
    k_signal(sem1);
    
    k_eat_msec_time(200); // eat 20 msec

  }
}

void setup() {


  pinMode(13, OUTPUT);

  k_init(2, 1, 0); // 1 task, 1 semaphores, 0 messaegQueues */

  pA = k_crt_task(a, TASKPRIO , stakA, STKSIZE);
  pB = k_crt_task(b, TASKPRIO, stakB, STKSIZE);
  sem1 = k_crt_sem(1, 2);

  k_start(30); /* start krnl timer speed 1 milliseconds*/

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
