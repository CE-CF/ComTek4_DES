
#include <Arduino.h>
#include <U8g2lib.h>

#include <SPI.h>
#include "GY521.h"

// http://jensd.dk/edu/doc/esp32/mono-oleds.html
// https://github.com/olikraus/u8g2/wiki/u8g2reference
/*
  NodeMCU ESP32S        OLED
  ___________________________
  GND                   GND
  3.3V                  VDD
  VSPI MOSI             SDA
  VSPI SCK              SCK
  VSPI SS               CS
  GPIO 17               RES
  GPIO16                DC
*/

//#define BUG1


#define SPI_CS    5
#define SPI_DC    16
#define SPI_RESET 17

//U8G2_R0  rotation

U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R0, SPI_CS, SPI_DC, SPI_RESET);

#define REAKPIN 32

void setup()
{
  Serial.begin(115200);

  pinMode(REAKPIN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  u8g2.begin();
  u8g2.setFont(u8g2_font_ncenB08_tr);  // choose a suitable font
//  u8g2.setFont(u8g2_font_ncenB14_tr);

}

unsigned long meaReakTime()
{
  unsigned long startT, rTime;

  delay(random(1000, 3000));

  digitalWrite(LED_BUILTIN, HIGH);
  startT = millis();

  while (digitalRead(REAKPIN) ) ;
  rTime = millis() - startT;
  digitalWrite(LED_BUILTIN, LOW);

  Serial.println(rTime);

  return rTime;
}

void loop()
{
  unsigned long reakTime;


  reakTime = meaReakTime();
  
  u8g2.clearBuffer();
  
  u8g2.setCursor(0, 15);
  u8g2.print(reakTime);

  u8g2.sendBuffer();

  delay(2000);

}
