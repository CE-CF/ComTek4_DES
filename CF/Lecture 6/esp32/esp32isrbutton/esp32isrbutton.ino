

// https://lastminuteengineers.com

// https://lastminuteengineers.com/handling-esp32-gpio-interrupts-tutorial/

// https://lastminuteengineers.com/wp-content/uploads/arduino/Wiring-Push-Buttons-to-ESP32-For-GPIO-Interrupt.png
struct Button {
  const uint8_t PIN;
  uint32_t numberKeyPresses;
  bool pressed;
};

Button button1 = {18, 0, false}; // pin D18 /JDN

uint32_t lastIsr = 0;
void IRAM_ATTR isr() {  // put me in internal RAM bq it is faster
  if (millis() - lastIsr > 500) {
    button1.numberKeyPresses += 1;
    button1.pressed = true;
    lastIsr = millis();
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(button1.PIN, INPUT_PULLUP);
  attachInterrupt(button1.PIN, isr, FALLING);
}

void loop() {
  if (button1.pressed) {
    Serial.printf("Button 1 has been pressed %u times\n", button1.numberKeyPresses);
    button1.pressed = false;
  }

  //Detach Interrupt after 1 Minute
  static uint32_t lastMillis = 0;
  if (millis() - lastMillis > 60000) {
    lastMillis = millis();
    detachInterrupt(button1.PIN);
    Serial.println("Interrupt Detached!");
  }
}
