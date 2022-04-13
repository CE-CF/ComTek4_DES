portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED; 
int sharedVar = 23; 

void IRAM_ATTR isr() {  // put me in internal RAM bq it is faster
// your code
  portENTER_CRITICAL_ISR(&timerMux);   
  sharedVar++;
  portEXIT_CRITICAL_ISR(&timerMux);
}

void setup() {
  Serial.begin(115200);
  pinMode(18, INPUT_PULLUP);  // pin 18 :-)
  attachInterrupt(18, isr, FALLING);
}

int totCount = 0;
void loop() {
                                                            //  if you need to detachInterrupt(18);
  portENTER_CRITICAL_ISR(&timerMux);   
  totCount +=sharedVar;                      // just to show conflice
  portEXIT_CRITICAL_ISR(&timerMux);

}
