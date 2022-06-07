#include <krnl.h>

//low precision floating pt type
typedef struct myfloat {
  signed char mantissa;
  signed char exponent;
} myfloat_type;

//convert from double to low precision type
void doub2mydouble(double arg, myfloat_type *res) {
   int exponent;
   double temp;
   exponent = ceil(log(abs(arg))/log(2)); //base 2 logarithm
   temp=arg*pow(2,7-exponent);
   res->mantissa = (signed char)temp;
   res->exponent = exponent-7;
}

//convert from low precision type to double
double myfloat2double(myfloat_type *arg1) {
  double res = (double)(arg1->mantissa) * pow(2,arg1->exponent);
  return res;
}

//multiply to low precision types
void mult_float(myfloat_type *arg1,myfloat_type *arg2,myfloat_type *result) {
  int temp;
  unsigned char sign;
  
  sign=0x80 & ((unsigned char)arg1-> mantissa ^ (unsigned char)arg2-> mantissa); //find sign of result 
  char i=0;
  
  temp = (int)(arg1-> mantissa) * (int)(arg2-> mantissa);
  temp = temp & 0x7f00; //take away sign from product
  
  while(abs(temp)>128){
    i++;
    temp=temp>>1;
  }
   
  result->mantissa = (unsigned char) temp;
  result->mantissa = result->mantissa | sign; //add recorded sign  
  result->exponent = arg1->exponent + arg2->exponent + i;
}

/****** My Code ******/

const int arrLen = 10;
double a=1.001;
double da[arrLen];            double da2[arrLen];
myfloat_type mda[arrLen];     float lowPrecisionArray[arrLen];
myfloat_type mda2[arrLen];    float lowPrecisionArray2[arrLen];
myfloat_type f1;              myfloat_type f;

void setup() {
  Serial.begin(115200);
  
  distributeDoubles();                                                      // exercise 1
  
  convertArray();                                                           // exercise 2
  
  double meanRelativeError = calcRelativeError();                           // exercise 3
  p("Exercise 3: Mean Relative Error between da and mda: ");
  pd(meanRelativeError, 30); pn("");
  
  arrayEntriesSquared();                                                    // exercise 4+5
  
  double meanRelativeError2 = calcRelativeError2();                         // exercise 6
  p("Exercise 6: Mean Relative Error between da2 and mda2: ");
  pd(meanRelativeError2, 30); pn("");
  
  double executionTimeEx7 = multiplyToA();                                     // exercise 7
  p("Exercise 7: Execution time of iterating a *= da[i]: ");
  pd(executionTimeEx7, 3); pn(" microseconds");

  convertToMyfloat();                                                       // exercise 8

  double executionTimeEx9 = ex9();

  double relativeDifference = ex10(executionTimeEx7,executionTimeEx9);
  Serial.println((String)"Exercise 10: relative difference between results from ex 7 and 9:   " + relativeDifference);
  
  //ex11(1);
}

/* shorter print functions for nicer code */
void p(String arr) {
  Serial.print(arr);
}

void pn(String arr) {
  Serial.println(arr);
}


void pd(double d, int decimals) {
  Serial.print(d, decimals);
}

void pi(int i){
  Serial.println(i);
}

/* Ex. 1 */
void distributeDoubles() {
  long randomLong = 0;
  
  for(int i = 0; i < arrLen; i++){
    randomLong = random(-500000000, 500000000);                                     // long -2,147 til 2,147 milliarder
    da[i] = (randomLong / (double)100000000);                                       // !!! can a double be this big???
  }
}

// Ex. 2
void convertArray(){
  for(int i = 0; i < arrLen; i++){
    doub2mydouble(da[i], &mda[i]);
  }
}

// Ex. 3
double calcRelativeError(){
  double totalRelativeError = 0;                                                                  // <-- this divided by arrLen gives the meanRelativeError
  
  for(int i = 0; i < arrLen; i++){
    lowPrecisionArray[i] = myfloat2double(&mda[i]);                                               // calculation of float into double
    totalRelativeError += abs((da[i]-lowPrecisionArray[i])/lowPrecisionArray[i] * 100);           // da[arrLen] contains "exact double values" | abs((da-lowPrec)/lowPrec)*100=relErr
  }

  return totalRelativeError / arrLen;
}

// Ex. 4 + 5 
void arrayEntriesSquared(){
  for(int i = 0; i < arrLen; i++){
    da2[i] = pow(da[i],2);
    mult_float(&mda[i], &mda[i], &mda2[i]);               // mult_float find mda^2 and inserts values into mda2 w. pointers
    lowPrecisionArray2[i] = myfloat2double(&mda2[i]);     // mda2 is still myfloat_type so this converts it into double
    /*Serial.print(da2[i], 5); p("   ");
    Serial.println(lowPrecisionArray2[i], 5);*/
  }
}

// Ex. 6 
double calcRelativeError2(){
  double totalRelativeError = 0;

  for(int i = 0; i < arrLen; i++){
    totalRelativeError += abs((da2[i]-lowPrecisionArray2[i])/lowPrecisionArray2[i] * 100);
  }

  return totalRelativeError / arrLen;
}

// Ex. 7 
double multiplyToA(){
  double startT, endT, execT;
  startT = micros();
  
  for(int i = 0; i < arrLen; i++){
    a *= da[i];
  }

  endT = micros();
  execT = endT - startT;
  return execT;
}

// Ex. 8
void convertToMyfloat(){
  double fake_a = 1.001;                                              // setting "a" to 1.001 to make ex9 like ex7
  doub2mydouble(fake_a, &f1);                                        // converts "a" which is a double into myfloat_type
  Serial.println((String)"Exercise 8: a's conversion into myfloat_type: " + f1.mantissa + "*2^" + f1.exponent + " = " + a);
}

/* Ex. 9 */
double ex9(){
  double startT, endT, execT;
  startT = micros();
  
  for(int i = 0; i < arrLen; i++){
    mult_float(&f1, &mda[i], &f);             // "f1" and "mda[i]" is multiplied into "f" for all entries in "mda"
    memcpy(&f1, &f, 2);                       // 2 bytes from "f" is copied into "f1"
  }

  endT = micros();
  execT = endT - startT;
  
  Serial.println((String)"Exercise 9: Executed in " + execT + " micro seconds");
  return execT;
}

/* Ex. 10 */
double ex10(double ex7time, double ex9time){
  double totalRelativeError = 0;
  totalRelativeError = abs((ex7time-ex9time)/ex9time*100);  
  return totalRelativeError;
}

/* Ex. 11 */
void ex11(int x){
  // Taylor series of sin x
  // https://socratic.org/questions/how-do-you-find-the-taylor-series-of-f-x-sin-x

  double sum = 0;
  double factorialVal = 0;
  int factorialRange = 0;
  int iterations = 2;
  
  for (int n = 0; n < iterations; n++){
    factorialVal = 0;
    factorialRange = (2*n+1);
    for (int m = 0; m < factorialRange; m++){     // factorial loop
      factorialVal *= ((2*n+1) - m);
      Serial.println(factorialVal);
    }
    sum += pow(-1, n) * (pow(x,2*n+1)/factorialVal);
  }
}


void loop(){
  
}
