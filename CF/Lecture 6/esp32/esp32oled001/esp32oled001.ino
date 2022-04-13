#include "U8g2lib.h"            // U8glib library for the OLED you download below
#include <Wire.h>              // Set Wire library for I2C communication
 
U8GLIB_SH1106_128X64 u8g(13, 11, 10, 9, 8); // D0=13, D1=11, CS=10, DC=9, Reset=8
 
void draw(void) 
{
  u8g.setFont(u8g_font_profont15r);        // Set display font 1
  u8g.drawStr(1, 12, "14CORE |i2C TEST CODE");// 
  u8g.setFont(u8g_font_profont29r);        // Set display font 2
  u8g.println("Line 1  i2C OLED");         
  u8g.setPrintPos(35, 45);                 // set print position
  u8g.println("Line 2 i2C OLED");  
  u8g.drawRFrame(15, 20, 100, 30, 10);     // Set to draws frame with rounded edges
}
 
void setup(void) 
{
    Serial.begin(9600);
    Serial.print("14CORE | Test for Waveshare 4 Wire | 3 Wire | I2C OLED Display Module");
    Serial.print("---------------------------------------------------------------------");
    delay(1000); //Set delay 1 Second
}
 
void loop(void) 
{
  u8g.firstPage();  
  do 
    {
     draw();      
    }
  while( u8g.nextPage() );
  
  delay(1000);  //Set delay 1 second
}
