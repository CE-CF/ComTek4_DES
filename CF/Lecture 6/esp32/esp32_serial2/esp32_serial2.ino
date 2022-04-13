void setup() {
// put your setup code here, to run once:
//void begin(unsigned long baud, uint32_t config=SERIAL_8N1, 
//             int8_t rxPin=-1, int8_t txPin=-1, 
//             bool invert=false, 
//             unsigned long timeout_ms = 20000UL);

Serial2.begin(115200,SERIAL_8N1,16,17);
Serial1.begin(115200,SERIAL_8N1,18,19);

}

void loop() {
    // put your main code here, to run repeatedly:

}
