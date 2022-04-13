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

char stakA[STKSIZE] ;
struct k_t * pA ;

void a()
{
  while (1) {

    delay(500);
    //k_sleep(500);
    k_eat_msec_time(500); // eat 20 msec

  }
}




void setup() {


  pinMode(13, OUTPUT);

  k_init(1, 0, 0); // 2 task, 0 semaphores, 0 messaegQueues */

  pA = k_crt_task(a, TASKPRIO, stakA, STKSIZE);

  k_start(1); /* start krnl timer speed 1 milliseconds*/

}

void loop() {}


// DEBUG
extern "C" {

  //  http://kom.aau.dk/~jdn/edu/doc/arduino/uno.png
  // led 13 on UNO is bit no 45 in PoRTBB regs

  void k_breakout() // called every task shift from dispatcher
  {

    if (pRun->nr == 0) {
      PORTB |=  0x20;   // bit 5(LED13) on bq we are dummy

    }
    else {
      PORTB &=  0xdf;
    }

    // for a MEGA you have to find another port :-)
    // port K (adc8-15) seems feasible
  }
}
