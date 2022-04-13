#include <limits.h>

const int nrMaal = 100;

void setup() {
  Serial.begin(115200);
}


float BM(float *p, int antal) 
{
    return *p /antal;
}
void x()
{
    float ff[12],m;
    m = BM(ff,12);
}
int  maalAr[nrMaal], minV, maxV, minPos, maxPos, mean;
void loop() {

  mean = 0;
  for (int i = 0; i < nrMaal; i++) {
    maalAr[i] = analogRead(32);
    
  }

  mean /= nrMaal;  // hint mean som int eller float

  maxV = INT_MIN;
  minV = INT_MAX;
  Serial.println(maxV);

  // print out  

}
