
#include <driver/dac.h>
hw_timer_t * timer = NULL;

const float weight = 0.1; // close to 0 no filter, close to 1 very smooth
float adcVal = 0, adcValOld, dacVal; // NB float

void IRAM_ATTR onTimer() {
  adcValOld = adcVal;
  adcVal = analogRead(0);
  // simple recursive fil
  dacVal = adcVal * weight + adcValOld * (1 - weight);
  // You need to scale from adc/dac values to/from SI units
  dac_output_voltage(DAC_CHANNEL_1, (int)dacVal);
}


void setup() {

  dac_output_enable(DAC_CHANNEL_1);

  timer = timerBegin(0, 80, true);              // start timer based on 80MHz clock frequency
  timerAttachInterrupt(timer, &onTimer, true);  // let function onTimer be called when timeout
  timerAlarmWrite(timer, 1000000, true);        // 1e6 microseconds  == 1 sec
  timerAlarmEnable(timer);                      // gogo
}

void loop() {}
//https://www.megunolink.com/articles/coding/3-methods-filter-noisy-arduino-measurements/
//https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/dac.html
