volatile int interruptCounter;
volatile int interruptCounter2;
int totalInterruptCounter = 0;

hw_timer_t * timer = NULL, * timer2 = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

unsigned int minn = 0 - 1, maxx = 0;
unsigned int minn2 = 0 - 1, maxx2 = 0;

 unsigned int last,act, last2, act2;

void setMax(unsigned int *m, unsigned int val)
{
  if (*m < val)
    *m = val;
}

void setMin(unsigned int *m, unsigned int val)
{
  if (*m > val)
    *m = val;
}



void IRAM_ATTR onTimer() {
  interruptCounter++;
  last = act;
  act = micros();
  if (5 < interruptCounter) {

    setMax(&maxx, (act - last));
    setMin(&minn, (act - last));
  }
}


void IRAM_ATTR onTimer2() {
  interruptCounter2++;
  last2 = act2;
  act2 = micros();
  if (5 < interruptCounter2) {

    setMax(&maxx2, (act2 - last2));
    setMin(&minn2, (act2 - last2));
  }
}

void setup() {
  Serial.begin(115200);

  timer = timerBegin(0, 80, true);              // start timer based on 80MHz clock frequency

  timerAttachInterrupt(timer, &onTimer, true);  // let function onTimer be called when timeout
  //timerAttachInterrupt(timer, &onTimer2, true);  // let function onTimer be called when timeout

  timerAlarmWrite(timer, 1000, true);        // 1000 microseconds  == 1 millisec

  timerAlarmEnable(timer);                      // gogo
}

void loop() {
  unsigned int mx, mn,mx2,mn2;

  delay(1000);

  portENTER_CRITICAL(&timerMux);     // entry in same critical region ... beware.
  mn = minn;
  mx = maxx;
  mn2 = minn2;
  mx2 = maxx2;
  portEXIT_CRITICAL(&timerMux);      // leave again

  Serial.print(mn);
  Serial.print(" ");
  Serial.print(mx);
  Serial.print(" nr 2");
  Serial.print(mn2);
  Serial.print(" ");
  Serial.println(mx2);

}
