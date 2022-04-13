// sync form isr
// inspired by  https://www.dfrobot.com/blog-1045.html

QueueHandle_t msgQ;



void IRAM_ATTR handleInterrupt() {
  static volatile int nrI = 0;
  nrI++;
  xQueueSendFromISR(msgQ,  (void *)&nrI , NULL );
}


void t1( void * parameter )
{
  int iii = 0;
  int res;
  int l = 0;
  for (;;)
  {
    iii = -1;
    if (pdTRUE == xQueueReceive(msgQ, &iii, 500)) {
      res = 0;
    }
    else {
      res = -1;
    }
    if (res == 0) {
      Serial.print("msg rcv  "); Serial.println(iii);
    }
    else {
      Serial.print("msg rcv - nope - loop nr "); Serial.println(l);
    }
    l++;
  }
}

int  interruptPin = 23;

void setup() {
  Serial.begin(115200);

  msgQ = xQueueCreate( 10, sizeof( int ) );

  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, FALLING);
  xTaskCreate(t1, "t1", 10000, NULL,  3, NULL);

}

void loop() {
}
