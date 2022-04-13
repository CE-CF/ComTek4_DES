
#include <Wire.h>


void setup() {
 Serial.begin(115200);
 delay(1000);
 Wire.begin();
}

void loop() {
  Wire.beginTransmission(8);
  Wire.write('1');
  Wire.endTransmission();
 
}
