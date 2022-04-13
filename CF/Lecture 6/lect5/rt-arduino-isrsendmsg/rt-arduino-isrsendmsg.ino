/* critical region - by semaphores - aka mutex
    JDN
 *  */

#include <krnl.h>

#define STK 150
char a[STK];

#define TASKPRIO 10


struct k_t * pTask1 ;
struct k_msg_t * msgQ;
const int BUFSZ = 10;

int bufForQ[BUFSZ];




volatile int ISRoverflow = 0;

volatile int nrISR = 0;

volatile unsigned long last = 0;

ISR(INT0_vect, ISR_NAKED) { // digital pin 2 på uno
  PUSHREGS();
  if (!k_running)
    goto exitt ;
   nrISR++;
   ki_send(msgQ, &nrISR);
  
  K_CHG_STAK();

exitt:
  POPREGS();
  RETI();
}


void task1()
{
  int res = 0;
  int t = 0;
  int lost;

  while (1) {

    if (-1 == k_receive(msgQ, &res, 1000, &lost)) {   // next time nr lost is received inside msg Q system
      t++;
      Serial.print("timeout "); Serial.println(t); 
    }
    else {
      Serial.print("get msg "); Serial.print(res); Serial.print(" lost since last "); Serial.println(lost);
    }
  }
}




void setup() {
  // for debugging - only on uno eq- If Mega etc use PORTA(which is pn 22,23,...)
  // see http://kom.aau.dk/~jdn/edu/doc/arduino/mega.png

  Serial.begin(115200);
  delay(2000);

  Serial.println("just bef init part");

  k_init(1, 0, 1); // 2 task, 1 semaphores, 0 messaegQueues */

  pTask1 = k_crt_task(task1, 15,  a,STK);



  Serial.println("just bef k_start");


  pinMode(2, INPUT_PULLUP);

  noInterrupts();

  msgQ =  k_crt_send_Q(BUFSZ, sizeof(int), bufForQ);

  // eller fra krnl  DI();
 
  EICRA |= (1 << ISC01); // trigger INT0 on falling edge
  //https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf  p 54
   
  EIMSK |= (1 << INT0);  // enable external intr

  interrupts();

  k_start(1); /* start krnl timer speed 1 milliseconds*/

  Serial.println("If you see this then krnl didnt start :-( ");
}

void loop() {}
