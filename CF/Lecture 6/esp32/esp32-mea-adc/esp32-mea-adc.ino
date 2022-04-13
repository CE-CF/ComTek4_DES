
void setup() {
  Serial.begin(115200);
}

#define L 20

int doAverADC()
{
  int a[L];
  int sum = 0;
  int i1, i2;
  i1 = micros();
  for (int i = 0; i < L ; i++) {
    sum += analogRead(0);
  }
  i2 = micros();
  Serial.println( (i2 - i1));
  return sum / L;

}
void loop() {
  int  mean = doAverADC();
  delay(10);
}
