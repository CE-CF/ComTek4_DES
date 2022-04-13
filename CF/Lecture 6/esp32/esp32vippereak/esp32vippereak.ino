 
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
 


#define SPI_CS    5
#define SPI_DC    16
#define SPI_RESET 17

U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R0, SPI_CS, SPI_DC, SPI_RESET);


 

GY521 MPU6050sensor(0x68);



void barMeter(int x1, int y1, int w, int h, float vall)
{
  int m;

  // clip to be in interval [0-1] to be safe
  // its up to you to scale bef call

  if (1.0 < vall)
    vall = 1.0;
  else if (vall < 0.0)
    vall = 0.0;

  u8g2.setDrawColor(1);

  u8g2.drawFrame(x1, y1, w,  h);

  x1++;
  y1++;
  h -= 2;
  w -= 2;

  u8g2.setDrawColor(0);

  u8g2.drawBox(x1, y1, w, h); // clear interior

  u8g2.setDrawColor(1);

  m = round(h * vall);
  y1 += (h - m);
  h = m;

  u8g2.drawBox(x1, y1, w, h);
}


void barSMeter(int x1, int y1, int w, int h, float vall)
{
  int m;
  int hlp, yM;

  // clip to be in interval [0-1] to be safe
  // its up to you to scale bef call

  if (1.0 < vall)
    vall = 1.0;
  else if (vall < 0.0)
    vall = 0.0;

  u8g2.setDrawColor(1);

  u8g2.drawFrame(x1, y1, w,  h);

  x1++;
  y1++;
  h -= 2;
  w -= 2;

  u8g2.setDrawColor(0);

  u8g2.drawBox(x1, y1, w, h); // clear interior

  u8g2.setDrawColor(1);


  yM = y1 + h / 2;

  if (0.5 < vall) {
    h = round(h * (vall - 0.5));
    y1 = yM - h;
  }
  else {
    h = round(h * (0.5 - vall) );
    y1 = yM;
  }

  if (h < 1)
    h = 1;

  /* m = round(h * vall);
    y1 += (h - m);
    h = m;
  */
  u8g2.drawBox(x1, y1, w, h);
}






float ax, ay, az, gxe, gye, gze, temp;

void initMPU6050()
{
  delay(100);
  while (MPU6050sensor.wakeup() == false)
  {
    Serial.print(millis());
    Serial.println("\tCould not connect to GY521");
    delay(1000);
  }
  MPU6050sensor.setAccelSensitivity(0);  // 2g
  MPU6050sensor.setGyroSensitivity(0);   // 250 degrees/s

  MPU6050sensor.setThrottle();
  Serial.println("start...");

  /*MPU6050sensor.axe = 0;
    MPU6050sensor.aye = 0;
    MPU6050sensor.aze = 0;
    MPU6050sensor.gxe = 0;
    MPU6050sensor.gye = 0;
    MPU6050sensor.gze = 0;
  */
}

void  printMPU6050()
{
  static int counter = 0;

  if (counter % 10 == 0)
  {
    Serial.println("\n\tACCELEROMETER\t\tGYROSCOPE\t\tTEMPERATURE");
    Serial.println("\tax\tay\taz\tgx\tgy\tgz\tT");
    counter = 1;
  }

  Serial.print(counter);
  Serial.print('\t');
  Serial.print(ax);
  Serial.print('\t');
  Serial.print(ay);
  Serial.print('\t');
  Serial.print(az);
  Serial.print('\t');
  Serial.print(gxe);
  Serial.print('\t');
  Serial.print(gye);
  Serial.print('\t');
  Serial.print(gze);
  Serial.print('\t');
  Serial.print(temp);
  Serial.println();

  counter++;
}
void readMPU6050()
{
  MPU6050sensor.read(); // initiate samplig
  ax = MPU6050sensor.getAccelX();
  ay = MPU6050sensor.getAccelY();
  az = MPU6050sensor.getAccelZ();
  gxe = MPU6050sensor.getGyroX();
  gye = MPU6050sensor.getGyroY();
  gze = MPU6050sensor.getGyroZ();
  temp = MPU6050sensor.getTemperature();

}

void printOLED()
{
  char ar[100];

  barMeter(0, 0, 10, 60, 0.5 + 0.5 * ax);
  barSMeter(15, 0, 10, 60, 0.5 + 0.5 * ay);
  barMeter(30, 0, 10, 60, 0.5 + 0.5 * az);

  sprintf(ar, "%5.2f", ax);
  u8g2.drawStr(64, 10, ar);
  sprintf(ar, "%5.2f", ay);
  u8g2.drawStr(64, 30, ar);
  sprintf(ar, "%5.2f", az);
  u8g2.drawStr(64, 50, ar);
}

void setup()
{
  Serial.begin(115200);

  Wire.begin();

  initMPU6050();

  // set callibration values from calibration sketch.

  u8g2.begin();
  u8g2.setFont(u8g2_font_ncenB08_tr);  // choose a suitable font

}

void loop()
{

  u8g2.clearBuffer();

  readMPU6050();
  printMPU6050();
  printOLED();

  u8g2.sendBuffer();

  delay(100);

}
