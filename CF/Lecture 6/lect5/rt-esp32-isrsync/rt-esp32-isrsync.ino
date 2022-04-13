// sync form isr 
// inspired by  https://www.dfrobot.com/blog-1045.html
SemaphoreHandle_t syncSem;

volatile int nrI=0;
void IRAM_ATTR handleInterrupt() {
    nrI++;
  xSemaphoreGiveFromISR(syncSem, NULL);
}


void t1( void * parameter )
{
    int nrIrcv=0;
  for (;;)
  {
    if ( xSemaphoreTake(syncSem, (TickType_t)1000)  == pdTRUE )
    {
      // yep got it
      Serial.print("isr detected ");
      nrIrcv++;
      Serial.print(nrIrcv); Serial.print(" "); Serial.println(nrI);
    }
    else {
      Serial.println("time out");
    }
  }
}

int  interruptPin = 23;

void setup() {
  Serial.begin(115200);

  syncSem = xSemaphoreCreateBinary();

  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, FALLING);
  xTaskCreate(t1, "t1", 10000, NULL,  3, NULL);

}

void loop() {
}
