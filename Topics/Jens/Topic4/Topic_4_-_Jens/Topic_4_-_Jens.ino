#include <krnl.h>

const unsigned int blinkInterval = 50; 
const unsigned int writeInterval = 400;
unsigned long blinkSchedule = blinkInterval;
unsigned long writeSchedule = writeInterval;
unsigned long counter = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(13, OUTPUT);
}

void blinker(){
  digitalWrite(13, !digitalRead(13)); // toggle el Led
}

void writer(){
  counter += 1;
  Serial.print("Count: ");
  Serial.println(counter);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (blinkSchedule <= millis()) {
    blinker();
    blinkSchedule += blinkInterval;
  }
  if (writeSchedule <= millis()){
    writer();
    writeSchedule += writeInterval;
  }
}
