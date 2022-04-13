#include <Arduino.h>

#ifndef LED_BUILTIN
// Set LED_BUILTIN if it is not defined by Arduino framework
#define LED_BUILTIN 4
#endif
void setup()
{
  // initialize LED digital pin as an output.
  for (int i = 0; i < 6; i += 2)
    pinMode(i, OUTPUT);
}

void loop()
{
  // turn the LED on (HIGH is the voltage level)
  digitalWrite(0, HIGH);
  // wait for a second
  delay(1000);
  digitalWrite(2, HIGH);
  // wait for a second
  delay(1000);
  digitalWrite(4, HIGH);
  // wait for a second
  delay(1000);
  // turn the LED off by making the voltage LOW
  digitalWrite(4, LOW);
  // wait for a second
  delay(1000);
  digitalWrite(2, LOW);
  // wait for a second
  delay(1000);digitalWrite(0, LOW);
  // wait for a second
  delay(1000);
}
