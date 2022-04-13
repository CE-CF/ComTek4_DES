#include <Wire.h>

void rx(int nbytes)
{
  while (Wire.available()) {
    Wire.read();
  }
}

void txback(int nBytes)
{
  Wire.write("1");
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Wire.begin(8);
  Wire.onReceive(rx);
  //Wire.onRequest(txback);
}

void loop() {

}
