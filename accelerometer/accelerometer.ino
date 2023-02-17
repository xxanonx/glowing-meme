#include <Adafruit_GFX.h>
#include <ArducamSSD1306.h>
#include <SPI.h>
#include <Wire.h>
#include "lcd_util.h"


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
//#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
//ArducamSSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
ArducamSSD1306 display(-1);

int X = A6;
int Y = A7;
int i;
int Xaverage;
int Yaverage;
const PROGMEM int sample = 2;
int xma[sample];
int yma[sample];
/*unsigned long startMillis;  //some global variables available anywhere in the program
  unsigned long currentMillis;
  const PROGMEM unsigned long period = 10;  //the value is a number of milliseconds*/


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  //ArducamSSD1306 display(OLED_RESET);
  display.begin();
  //startMillis = millis();  //initial start time

  /* // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
    }*/
  display.clearDisplay();
  display.drawBitmap(0, 0, skull, 6, 16, WHITE);
  display.display();
  delay(3000);
  display.clearDisplay();
}

void loop() {
  /*Serial.print(analogRead(X));
    Serial.print(",");
    Serial.println(analogRead(Y));
    delay(10);*/

  /*if (currentMillis - startMillis >= period) {
    Serial.print(Xaverage);
    Serial.print(",");
    Serial.print(yma[i]);
    Serial.print(",");
    Serial.println(Yaverage);
    display.clearDisplay();
    display.fillCircle(((display.width() / 2) + Yaverage), ((display.height() / 2) - Xaverage), 3, WHITE);
    display.display();
    startMillis = currentMillis;*/
  //}
  //else {
  //currentMillis = millis();
  //delayMicroseconds(50);
  xma[i] = map(analogRead(X), 472, 506, -30, 30);
  yma[i] = map(analogRead(Y), 472, 504, -30, 30);
  i += 1;
  if (i == sample) i = 0;
  Xaverage = 0;
  Yaverage = 0;
  for (int s = 0; s <= sample; s++) {
    Xaverage += xma[s];
    Yaverage += yma[s];
  }
  Xaverage /= sample;
  Yaverage /= sample;
  display.clearDisplay();
  display.fillCircle(((display.width() / 2) + Yaverage), ((display.height() / 2) - Xaverage), 3, WHITE);
  display.display();
  //}

}
