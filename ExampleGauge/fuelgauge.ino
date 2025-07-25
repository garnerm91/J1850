#include <SPI.h>
#include <Adafruit_GFX.h>   
#include <Adafruit_ST7735.h> 
#include <Adafruit_ST7789.h> 

// Pin config
#define TFT_CS        17
#define TFT_RST       20
#define TFT_DC        21
#define csPIN  22
#define rstPIN 15


byte frame[20]; //Frame Buff
int framePos = 0;
bool frameInProgress = false;
float prec;
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
int lastNeedleX = 120;
int lastNeedleY = 140;
int indicatorX = 200;
int indicatorY = 40;
int indicatorRadius = 25;
int WarnVal = 15; // set this to set when the warning light is on

void setup() {
   byte status, data;
  Serial.begin(115200);

  pinMode(rstPIN, OUTPUT);
  pinMode(csPIN, OUTPUT);
  
  SPI.begin();
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE3));
  
  // Reset the XC68HC58
  digitalWrite(rstPIN, LOW);
  delay(500);
  digitalWrite(rstPIN, HIGH);
  SPIcycle(0x12, 0x1A, status, data);
  tft.init(240, 280);           // Init ST7789 
  tft.setSPISpeed(1000000);
  tft.fillScreen(ST77XX_BLACK);
  tft.drawCircle(120, 140, 100, ST77XX_WHITE); // outer circle
  tft.drawCircle(120, 140, 101, ST77XX_WHITE); // outer circle
  tft.drawCircle(120, 140, 102, ST77XX_WHITE); // outer circle
  tft.drawCircle(indicatorX, indicatorY, indicatorRadius, ST77XX_WHITE);
  for (int i = -135; i <= 135; i += 45) {
    float tickAngle = i * 0.0174533;
    int x0 = 120 + cos(tickAngle) * 93;
    int y0 = 140 + sin(tickAngle) * 93;
    int x1 = 120 + cos(tickAngle) * 100;
    int y1 = 140 + sin(tickAngle) * 100;
    tft.drawLine(x0, y0, x1, y1, ST77XX_MAGENTA);
    tft.drawLine(x0+1, y0, x1+1, y1, ST77XX_MAGENTA);
  }
}

void SPIcycle(byte command1, byte command2, byte &statusByte, byte &dataByte) {
  digitalWrite(csPIN, LOW);
  delayMicroseconds(100);
  statusByte = SPI.transfer(command1);
  dataByte   = SPI.transfer(command2);
  digitalWrite(csPIN, HIGH);
  delayMicroseconds(100);
}

void processframe(){

     if (frame[0] == 0xA8 && frame[1] == 0x83 && frame[2] == 0x10 && frame[3] == 18) {
        prec = (frame[4]/2.55);
        drawGauge(prec);
      }
}
void loop() {
  byte status, data;
  SPIcycle(0x00, 0x02, status, data);
  // Status 0x10 or 0x00: frame in progress
  if (status != 0x10 && status != 0x00) {
    if (status != 0xF0 && status != 0xD0) {
      frame[framePos] = data;
      framePos++;
    }else if (framePos>3){
 processframe();
  framePos=0;
    } else{framePos=0;}
  }
}

void drawGauge(int value) {
  // Erase previous needle (draw over with black - 3px wide)
  for (int i = -1; i <= 1; i++) {
    tft.drawLine(120 + i, 140, lastNeedleX + i, lastNeedleY, ST77XX_BLACK);
  }
  float angle = ((value / 100.0) * 270.0 - 135.0) * 0.0174533; 
  // New needle endpoint
  int x = 120 + cos(angle) * 90;
  int y = 140 + sin(angle) * 90;
  for (int i = -1; i <= 1; i++) {
    tft.drawLine(120 + i, 140, x + i, y, ST77XX_RED);
  }

  // Update stored needle position
  lastNeedleX = x;
  lastNeedleY = y;
  tft.fillCircle(120, 140, 5, ST77XX_WHITE);
 
  if (value < WarnVal) { // Change to > if its a higher than warning
    tft.fillCircle(indicatorX, indicatorY, indicatorRadius-1, ST77XX_RED);
  }else{
    tft.fillCircle(indicatorX, indicatorY, indicatorRadius-1, ST77XX_BLACK);
  }
}