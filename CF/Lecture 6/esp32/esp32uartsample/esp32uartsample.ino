/*
    Sample_UART_demo.ino

    1. Place a wire between RX0 (GPIO3) and ADC0 (GPIO36)
    2. Open the Serial Plotter with a baudrate of 4800
    3. Send a single charecter and watch the plot
*/

int counter = 0;

int Analog_Pin = A0;           // Select the input pin for the analog input
int Analog_Array[500];         // Array with the same length as the plotter

hw_timer_t * timer = NULL;

void IRAM_ATTR onTimer() {
  Analog_Array[(counter++) % 500] = analogRead(Analog_Pin); // Sample the analog result into a circular buffer
}

void setup() {

  Serial.begin(4800);
  delay(3000);

  adcAttachPin(Analog_Pin);      // Attach a pin to the ADC
  analogSetClockDiv(1);          // Set the divider for the ADC clock (necessary for fast ADC rate)

  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, onTimer, true);  // true = edge (not level) triggered
  timerAlarmWrite(timer, 20, true);             // 20 us -> fs = 50000 Hz, true = autoreload
  timerAlarmEnable(timer);      // enable
}

void loop() {
  serialEvent();
}

void serialEvent() {
  if (Serial.available()) {
    timerAlarmDisable(timer);           // disable interrupt
    char inChar = Serial.read();        // get the new byte
    for (int i = 0; i < 500; i++) {
      Serial.println((float)Analog_Array[(counter + i) % 500] * 3.3 / 4095.0); // send the circular buffer to the plotter
      // Serial.println(9.0);
    }
    timerAlarmEnable(timer);            // enable interrupt
  }
}
