const byte interruptPin = 13;

volatile int interruptCounter = 0;

#define SWDEBOUNCE

unsigned long t;
portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR handleInterrupt() {
#ifdef SWDEBOUNCE
  if (300 < (millis() - t) ) {
    t = millis();
    interruptCounter++;
  }
#else
  interruptCounter++;  #endif
#endif
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);
  Serial.println( sizeof(double)) ;
  Serial.println( sizeof(long)) ;
  while (1);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, FALLING);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(interruptCounter);
  delay(500);
}
