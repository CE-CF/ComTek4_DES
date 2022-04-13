#include <esp_intr_alloc.h>

volatile int interruptCounter;
int totalInterruptCounter = 0;

hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;


unsigned int minn = 0 - 1;
unsigned int maxx = 0, last, act;
volatile int sharedVar = 0;


int freq = 1200;
int channel = 0;
int resolution = 8;

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


void IRAM_ATTR isr() {  // put me in internal RAM bq it is faster
  // not needed ! portENTER_CRITICAL_ISR(&isrMux);
  sharedVar++;
}


void setup() {
  Serial.begin(115200);

  pinMode(18, INPUT_PULLUP);  // pin 18 :-)
  attachInterrupt(18, isr, FALLING);

  ledcSetup(channel, freq, resolution);
  ledcAttachPin(12, channel);

  ledcWrite(channel, 50);
}

void loop() {
  unsigned int mx, mn, sh;

  delay(1000);

  portENTER_CRITICAL(&timerMux);     // entry in same critical region ... beware.
  mn = minn;
  mx = maxx;
  sh = sharedVar;
  portEXIT_CRITICAL(&timerMux);      // leave again

  Serial.print(sh);
  Serial.print(" : ");
  Serial.print(mn);
  Serial.print(" ");
  Serial.println(mx);

}
