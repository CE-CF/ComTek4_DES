void setup() {
  Serial.begin(115200);

  Serial.println("ttgo1");
}

#define FP double
FP f0, f1, f2, f3, f4;
int t1;
FP f(FP d)
{

  f1 = f1 / f2;
  f2 = f2 * f3;
  f4 = f1 + f2;
  f4 *= f1;
  f1 = f1 / f2;
  f2 = f2 * f3;
  f4 = f1 + f2;
  f4 *= f1;
  return f4;
}

void loop() {
  Serial.print("time ");
  t1 = millis();
  f0 = 0.0;
  f1 = 1.1, f2 = 1.2; f3 = 1.3; f4 = 1.4;

  for (int i = 0; i < 125000; i++) {
    f0 += f(f1);
  }


  Serial.println(millis() - t1);
}

#if 0
nodmcu  esp32 - wroom - 32d float
time 76
time 77
time 76
time 77
time 77
time 76
time 77
time 77
time 76
time 77
time 76
time 76
time 77
time 76
time 77
time 77
time 76
time 77
time 77

 

double
time 150
time 149
time 150
time 150
time 150
time 149
time 150
time 150
time 149
time 150
time 150
time 149
time 149
time 150
time 150
time 149
time 150
time 150
time 149
time 150
time 150
time 150
time 149
time 150
time 150
time 149

#endif
