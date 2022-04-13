
/*
  e02 - std critical region
  task2 will only enter region if it's free

  About using a semaphore for conditional wait

  k_wait(sem,0)
      0: no timeout - we will wait until we get an signal
      returns:
        0: we got a signal, and we has been sleeping at the semaphore waiting for a signal
        1: we got a signal that was already waiting at the semaphore. We has not been sleeping

  k_wait(sem,10)  (2nd parm can be 1,2,3,... etc. Using 1 is not recommended bq timeotu will be ]0-1] tick
                                                  because you might issue wait call just bef sys timer is ticking
      10: 10 tick maximal waiting time at semaphore
      returns:
        0: we got a signal, and we has been sleeping at the semaphore waiting for a signal
        1: we got a signal that was already waiting at the semaphore. We has not been sleeping
        -1: we got a timeout after ... 10 ticks. So wait call has not been a success

  k_wait(sem,-1)
      -1: we will not wait for a signal. We will just eat one if already available
      returns:
         1: we got a signal that was already waiting at the semaphore. We has not been sleeping
        -1: no signal available - so wait call has not succeded



        // res == 1 means there was a signal pending on the semaphore - so we was not waiting
      // res == 0
      //     k_eat_time(5);

*/

#include <krnl.h>

#define STK 150

#define TASKPRIO 10

char stak1[STK], stak2[STK], stak3[STK];
struct k_t * pTask1, *pTask2, *pTask3, *sem1, *sem2, *sem3, *sem4;

#define CEIL

void task1()
{
  k_set_sem_timer(sem3, 20);
  // priorities: task1 50, task2 : 20 ceil on sem1 10
  while (1) {
    k_wait(sem3, 0);
#ifdef CEIL
    k_mut_ceil_enter(sem1, 0);
#else
    k_wait(sem1, 0);
#endif
   bugBitSet(4);
 
    k_eat_time(3);

    bugBitReset(4);
 
#ifdef CEIL
    k_mut_ceil_leave(sem1);
#else
    k_signal(sem1);
#endif

  }
}


void task2()
{
  int res;
  k_set_sem_timer(sem2, 15);
  while (1) {
    k_wait(sem2, 0);

#ifdef CEIL
    k_mut_ceil_enter(sem1, 0);
#else
    k_wait(sem1, 0);
#endif

    bugBitSet(4);

    k_eat_time(3);

    bugBitReset(4);

#ifdef CEIL
    k_mut_ceil_leave(sem1);
#else
    k_signal(sem1);
#endif

  }
}

void task3()
{
  k_set_sem_timer(sem4, 100);
  while (1) {
    k_wait(sem4, 0);
    k_eat_time(40);
  }
}

void setup() {
  // for debugging - only on uno eq- If Mega etc use PORTA(which is pn 22,23,...)
  // see http://kom.aau.dk/~jdn/edu/doc/arduino/mega.png

  init_IO_for_debug();
  Serial.begin(9600);

  k_init(3, 4, 0); // 2 task, 1 semaphores, 0 messaegQueues */
  pTask1 = k_crt_task(task1, 9, stak1, STK);
  pTask2 = k_crt_task(task2, 15 , stak2, STK);
  pTask3 = k_crt_task(task3, 13 , stak3, STK);

  sem1 = k_crt_sem(1, 10); // 1: start value, 10: max value (clipping)
  k_mut_ceil_set(sem1, 8);

  sem2 = k_crt_sem(0, 10); // 1: start value, 10: max value (clipping)
  sem3 = k_crt_sem(0, 10); // 1: start value, 10: max value (clipping)
  sem4 = k_crt_sem(0, 10); // 1: start value, 10: max value (clipping)
  k_start(1); /* start krnl timer speed 1 milliseconds*/

  Serial.println("If you see this then krnl didnt start :-( ");
}

void loop() {}

/*****************************************/


void init_IO_for_debug()
{
#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega328__)
  for (int i = 8; i < 8 + 6; i++)
    pinMode(i, OUTPUT);

#elif defined (__AVR_ATmega2560__) || defined (__AVR_ATmega1280__) || defined(__AVR_ATmega2561__)
  for (int i = 22; i < 22 + 6; i++)
    pinMode(i, OUTPUT);
#endif

}

/**********************************************************************/
/***** DEBUGGING PART - LED ON 8-12**********/
/************************ DEBUG CALLBACK BREAKOUT PART ****************/
// must be extern C ! its not mandatory to supply with these  functions - only if you need
volatile unsigned char reg = 0;

  void bugBitSet(int bitNr)
  {
    DI();
    reg = 1 << bitNr;

#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega328__)

    PORTB  = (1 << pRun->nr) | reg;  // arduino uno !! specific usage of PORTB

#elif defined (__AVR_ATmega2560__) || defined (__AVR_ATmega1280__) || defined(__AVR_ATmega2561__)

    PORTA  = (1 << pRun->nr) | reg; //  // arduino uno !! specific usage of PORTB

#endif
    EI();
  }

  void bugBitReset(int bitNr)
  {
  char r;  
    DI();
    r = ~(1 << bitNr);
    reg = 0;
#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega328__)

    PORTB  = (1 << pRun->nr) & r;  // arduino uno !! specific usage of PORTB

#elif defined (__AVR_ATmega2560__) || defined (__AVR_ATmega1280__) || defined(__AVR_ATmega2561__)

    PORTA  = (1 << pRun->nr) & r; //  // arduino uno !! specific usage of PORTB

#endif
    EI();
  }



extern "C" {
  // called when a semphore is clipping - nr is id of semaphore and i os nr of times clip has occured
  unsigned char led13;

  void k_sem_clip(unsigned char nr, int i)
  {
    return; // !!!!
    if (nr == 2)
      led13 |= 0x20;
  }

  void k_sem_unclip(unsigned char nr)
  {
    return; /// !!!
    if (nr == 2)
      led13 = 0x00;
  }


  /* void k_send_Q_clip(unsigned char nr, int i) {} */

  // task numbering is in creation order: dummy: 0,  first of yours 1, next 2,...
  void k_breakout() // called every task shift from dispatcher
  {
    unsigned char c;

    // if we do have a uno,nano etc use PORTB directly
#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega328__)

    PORTB  = (1 << pRun->nr) | reg;  // arduino uno !! specific usage of PORTB

#elif defined (__AVR_ATmega2560__) || defined (__AVR_ATmega1280__) || defined(__AVR_ATmega2561__)

    PORTA  = (1 << pRun->nr) | reg; //  // arduino uno !! specific usage of PORTB

#endif


  }
  // for a MEGA you have to find another port :-)
  // port K (adc8-15) seems feasible
  // get inspired at http://kom.aau.dk/~jdn/edu/doc/arduino/ards.html
}





