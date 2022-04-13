volatile int interruptCounter;
int totalInterruptCounter = 0;

hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;



unsigned int minn = 0 - 1;
unsigned int maxx = 0, last, act;

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
  if (1 < interruptCounter) {  // for lige at få systejm stabiliseret

    setMax(&maxx, (act - last));
    setMin(&minn, (act - last));
  }
}

void setup() {
  Serial.begin(115200);

  timer = timerBegin(0, 80, true);              // start timer based on 80MHz clock frequency
  timerAttachInterrupt(timer, &onTimer, true);  // let function onTimer be called when timeout
  timerAlarmWrite(timer, 1000, true);        // 1000 microseconds  == 1 millisec
  timerAlarmEnable(timer);                      // gogo
}

void loop() {
  unsigned int mx, mn;

  delay(1000);

  portENTER_CRITICAL(&timerMux);     // entry in same critical region ... beware.
  mn = minn;
  mx = maxx;
  portEXIT_CRITICAL(&timerMux);      // leave again

  Serial.print(mn);
  Serial.print(" ");
  Serial.println(mx);

}
