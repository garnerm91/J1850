#include <SPI.h>
#define csPIN  10
#define rstPIN  8
 
void setup() {  
  pinMode(rstPIN, OUTPUT);
  pinMode(csPIN, OUTPUT);
  SPI.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE3));
  SPI.begin();
  digitalWrite(rstPIN, LOW);
  delay(1000);
  digitalWrite(rstPIN, HIGH);
  SPIcycle(0x12,0x18);
}

void SPIcycle(int firstbyte, int secondbyte){
  digitalWrite(csPIN, LOW);
  delay(30); 
  SPI.transfer(firstbyte);
  SPI.transfer(secondbyte);
  delay(30);
  digitalWrite(csPIN, HIGH);
}

void loop() {
//  SPIcycle(J1850 byte,XC68HC58 command byte); //priority  
  delay(3500);
  SPIcycle(0x6C,0x14); //priority
  SPIcycle(0x60,0x04); //destination
  SPIcycle(0xF1,0x04); //source
  SPIcycle(0xAE,0x04); //command
  SPIcycle(0x20,0x04); //sub-command
  SPIcycle(0xFF,0x04); //data
  SPIcycle(0xFF,0x04); //data
  SPIcycle(0x00,0x04); //data
  SPIcycle(0x00,0x04); //data
  SPIcycle(0x00,0x04); //data
  SPIcycle(0x00,0x0C); //data
//Sweeps all gauges

  delay(1000);
  SPIcycle(0x6C,0x14); //priority
  SPIcycle(0x60,0x04); //destination
  SPIcycle(0xF1,0x04); //source
  SPIcycle(0xAE,0x04); //command
  SPIcycle(0x11,0x04); //sub-command
  SPIcycle(0x01,0x04); //data
  SPIcycle(0x01,0x04); //data
  SPIcycle(0x00,0x04); //data
  SPIcycle(0x00,0x04); //data
  SPIcycle(0x00,0x04); //data
  SPIcycle(0x00,0x0C); //data
//Turns on all lights controlled by the bus
  
  delay(1000);
  SPIcycle(0x6C,0x14); //priority
  SPIcycle(0x60,0x04); //destination
  SPIcycle(0xF1,0x04); //source
  SPIcycle(0xAE,0x04); //command
  SPIcycle(0x21,0x04); //sub-command
  SPIcycle(0x88,0x04); //data
  SPIcycle(0x88,0x04); //data
  SPIcycle(0x00,0x04); //data
  SPIcycle(0x00,0x04); //data
  SPIcycle(0x00,0x04); //data
  SPIcycle(0x00,0x0C); //data
//Turns on all lights controlled by the bus
  
}
