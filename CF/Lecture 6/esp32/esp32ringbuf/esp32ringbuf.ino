#define L 100
int buffer[L];
int rIndex = 0, wIndex = 0, nrInBuf = 0;
hw_timer_t * timer = NULL;
portMUX_TYPE adcMux = portMUX_INITIALIZER_UNLOCKED;
 
void IRAM_ATTR onTimer() {
  if (nrInBuf < L ) {
    buffer[wIndex] = analogRead(0);
    wIndex++;
    if (L <= wIndex)
      wIndex = 0;
    nrInBuf++;
  }
}

void setup() {
  timer = timerBegin(0, 80, true);              // start timer based on 80MHz clock frequency
  timerAttachInterrupt(timer, &onTimer, true);  // let function onTimer be called when timeout
  timerAlarmWrite(timer, 1000000, true);        // 1e6 microseconds  == 1 sec
  timerAlarmEnable(timer);                      // gogo
}

int newVal(int *pV)
{
  portENTER_CRITICAL(&adcMux);     // entry in same critical region ... beware.
  if ( !nrInBuf)
    return -1; // no elm
  *pV = buffer[rIndex];
  nrInBuf--;
  portEXIT_CRITICAL(&adcMux);      // leave again
  rIndex++;
  if (L <= rIndex)
  rIndex = 0;
  return 0; // ok
}

void loop()
{
  int val, res;
  res = newVal(&val);
  if (res == 0){  }
}
