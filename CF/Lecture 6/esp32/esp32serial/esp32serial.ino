//https://embetronicx.com/tutorials/wireless/esp32/idf/esp32-idf-serial-communication-tutorial/
//https://www.esp32.com/viewtopic.php?t=4719

// !! https://circuits4you.com/2018/12/31/esp32-hardware-serial2-example/

#include <HardwareSerial.h>

HardwareSerial MySerial(1);

MySerial.begin(19200, SERIAL_8N1, x, y);


void setup() {
    // put your setup code here, to run once:

}

void loop() {
    // put your main code here, to run repeatedly:

}
