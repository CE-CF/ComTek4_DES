//esp32strings01

 void setup()
 {
  Serial.begin(115200);
 }

const int lgt = 200;


void doStuff(char a[])
{
char str[lgt];
  
  strcpy(str, a);
  Serial.println(str);
  Serial.print("lengt: "); 
  Serial.println( strlen(str) );
  
}

 void loop()

 {
  doStuff("ABC");
  doStuff("ÆØÅ");
  Serial.println("vi holder os lidt væk fra nationale tegn idag (3 bogstaver fylder 6(hint: UTF tegnsæt)";
  delay(10000);
 }
