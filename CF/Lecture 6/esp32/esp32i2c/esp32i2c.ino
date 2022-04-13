#include <Wire.h>

TwoWire I2Cone = TwoWire(0);
TwoWire I2Ctwo = TwoWire(1);

#define SDA_1 27
#define SCL_1 26

#define SDA_2 33
#define SCL_2 32

void setup() {
  TwoWire I2Cone = TwoWire(0);
  TwoWire I2Ctwo = TwoWire(1);
  
  I2Cone.begin(SDA_1, SCL_1, 100000); 
  I2Ctwo.begin(SDA_2, SCL_2, 100000);
}


void loop() {
 
}
