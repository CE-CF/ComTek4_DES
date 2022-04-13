
// https://blog.llandsmeer.com/tech/2021/04/08/esp32-s2-fpu.html

inline float recipsf2(float a) __attribute__((always_inline));

float recipsf2(float a)
{
  float result;
  asm volatile (
    "wfr f1, %1\n"

    "recip0.s f0, f1\n"
    "const.s f2, 1\n"
    "msub.s f2, f1, f0\n"
    "maddn.s f0, f0, f2\n"
    "const.s f2, 1\n"
    "msub.s f2, f1, f0\n"
    "maddn.s f0, f0, f2\n"

    "rfr %0, f0\n"
    :"=r"(result):"r"(a):"f0", "f1", "f2"
  );
  return result;
}

#define DIV(a, b) (a)*recipsf2(b)

#define MUL(a,b) (a)*(b)

void setup() {
float v1,v2,v3;
  Serial.begin(115200);
  delay(1000);
  Serial.println("ttgo1");
  Serial.println(DIV(10000.0,250.0));

   v1 = 1.23;
   v2 = 3.34;
   v3 = v1*v2;
   Serial.print(v3);
  
}

float f0, f1, f2=1.1, f3, f4;
int t1;
void loop() {
  Serial.print("time ");
  t1 = millis();

  f1 = 1.1;

  for (int i = 0; i < 1000000; i++) {
    f1 = f1 / 3.2;
  }


  Serial.print(millis() - t1);
  // -------------------
  Serial.print(" ");
  t1 = millis();
  f1 = 1.1;

  for (int i = 0; i < 1000000; i++) {
    f1 = f1 * (1.0 / 3.2);
  }


  Serial.print(millis() - t1);
  // -------------------
  Serial.print(" ");
  t1 = millis();
  f1 = 1.1;


  for (int i = 0; i < 1000000; i++) {
    f1 = DIV(f1, f2);
    f2 = f1+1.0;

  }
  Serial.print(millis() - t1);


  Serial.print(" ");
  t1 = millis();
  f1 = 1.1;

  for (int i = 0; i < 1000000; i++) {
    f1 = f1 * 1.1;
  }
  Serial.println(millis() - t1);

}
 
