unsigned long tStart, tStop;

void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  while (!Serial);
  Serial.println("going...");
  delay(400);
  tStart = millis();
  for (int i = 0; i < 10000; i++) {
    digitalWrite(13, HIGH);
    digitalWrite(13, LOW);
    digitalWrite(13, HIGH);
    digitalWrite(13, LOW);
    digitalWrite(13, HIGH);
    digitalWrite(13, LOW);
    digitalWrite(13, HIGH);
    digitalWrite(13, LOW);
    digitalWrite(13, HIGH);
    digitalWrite(13, LOW);
  }
  tStop = millis();
  Serial.println(tStop-tStart);
  Serial.println("fast now");
  tStart = millis();
  for (int i = 0; i < 10000; i++) {
    PORTB |= 0x20;
    PORTB &= 0xDF;
    PORTB |= 0x20;
    PORTB &= 0xDF;
    PORTB |= 0x20;
    PORTB &= 0xDF;
    PORTB |= 0x20;
    PORTB &= 0xDF;
    PORTB |= 0x20;
    PORTB &= 0xDF;
  }
  tStop = millis();
  Serial.println(tStop-tStart);

}


void loop() {
  // put your main code here, to run repeatedly:

}
