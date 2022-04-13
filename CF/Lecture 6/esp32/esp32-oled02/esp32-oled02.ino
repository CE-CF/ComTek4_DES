
#include <U8g2lib.h>
#include <Wire.h>
#define PIN_OLED_SCL A5
#define PIN_OLED_SDA A4
//Check others from U8g2 documentation, if doesn't work
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, PIN_OLED_SCL, PIN_OLED_SDA);

#define ONN()  digitalWrite(LED_BUILTIN, HIGH)
#define OFF()  digitalWrite(LED_BUILTIN, LOW)
void test1()
{
  pinMode(LED_BUILTIN, OUTPUT);
  ONN();
  u8g2.setFont(u8g_font_unifont);
  u8g2.clearBuffer();
  u8g2.drawStr( 0, 12 , "Hello");
  u8g2.sendBuffer();
  u8g2.setCursor(0, 27);
  u8g2.print("world");
  u8g2.sendBuffer();
  OFF();
  delay(1000);
  ONN();
  delay(1000);
  OFF();

}

void setup() {
  u8g2.begin();
  test1();
}

void loop() {

}
