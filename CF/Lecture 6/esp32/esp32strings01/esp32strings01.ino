//esp32strings01
#include <stdio.h>
void setup()
{
  char x[200];
  char str[] = "hey";
  Serial.begin(115200);

  Serial.println( strlen(str));

  for (int i = 0; i <  strlen(str); i++) {
    Serial.print( str[i]);
    Serial.print(" ");
    Serial.print("0x"); Serial.println( str[i], HEX);
  }
  Serial.print("last(0): 0x"); 
  Serial.println(str[strlen(str)],HEX);
  sprintf(x,"heyyyy%s %3.1f",str,3.123);
  Serial.println(x);
}

void loop() {}
