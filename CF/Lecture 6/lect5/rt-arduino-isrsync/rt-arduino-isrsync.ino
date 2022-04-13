/* critical region - by semaphores - aka mutex
    JDN
 *  */

#include <krnl.h>

#define STK 150

char a1[STK];

#define TASKPRIO 10


struct k_t * pTask1, *pTask2;

struct k_t *mutSem, *syncSem;

struct k_t *timedSem1, *timedSem2;

volatile int t2Missed = 0, t2Hit = 0;


volatile int ISRoverflow=0; 

ISR(INT0_vect, ISR_NAKED) { // digital pin 2 på uno

  PUSHREGS();
  if (!k_running)
    goto exitt ;
  // maybe room for sw debounce  - use millis
  if (-1 == ki_signal(syncSem))
  ISRoverflow++;
  K_CHG_STAK();

exitt:
  POPREGS();
  RETI();
}


void task1()
{
  int i = 0;
  int miss = 0;
  while (1) {

    if (-1 == k_wait(syncSem, 1000)) {
      miss++;
      Serial.print("timeout "); Serial.print(miss); Serial.print(" "); Serial.println(ISRoverflow);
    }
    else {
      i++;
      Serial.print("get kick no "); Serial.print(i); Serial.print(" "); Serial.println(ISRoverflow);
    }
  }
}




void setup() {
  // for debugging - only on uno eq- If Mega etc use PORTA(which is pn 22,23,...)
  // see http://kom.aau.dk/~jdn/edu/doc/arduino/mega.png

  Serial.begin(115200);
  delay(2000);

  Serial.println("just bef init part");

  k_init(1, 1, 0); // 2 task, 1 semaphores, 0 messaegQueues */

  pTask1 = k_crt_task(task1, 15, a1, STK);

  syncSem = k_crt_sem(0, 10); // 1: start value, 10: max value (clipping)

  Serial.println("just bef k_start");


  pinMode(2, INPUT_PULLUP);

  noInterrupts();

  // eller fra krnl  DI();
  EICRA |= (1 << ISC00); // trigger INT0 on falling edge
  EICRA = 0x00;
  EIMSK |= (1 << INT0);  // enable external intr

  interrupts();

  k_start(1); /* start krnl timer speed 1 milliseconds*/

  Serial.println("If you see this then krnl didnt start :-( ");
}

void loop() {}
