#include <SPI.h>
#define csPIN 10
#define rstPIN 8

unsigned long previousMillisPCM = 0;
uint8_t I = 0x00; 
uint8_t I2 = 0x60; 
void setup() {
  pinMode(rstPIN, OUTPUT);
  pinMode(csPIN, OUTPUT);
  SPI.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE3));
  SPI.begin();
  digitalWrite(rstPIN, LOW);
  delay(1000);
  digitalWrite(rstPIN, HIGH);
  SPIcycle(0x12, 0x18);

      delay(250);            // Volt (about 14V)
  SPIcycle(0x88, 0x14);  //priority
  SPIcycle(0x73, 0x04);  //destination
  SPIcycle(0x40, 0x04);  //source
  SPIcycle(0x08, 0x04);  //Data
  SPIcycle(0x05, 0x04);  //Data
  SPIcycle(0xA0, 0x0C);  //data

}

void SPIcycle(int firstbyte, int secondbyte) {
  digitalWrite(csPIN, LOW);
  delay(1);
  SPI.transfer(firstbyte);
  SPI.transfer(secondbyte);
  digitalWrite(csPIN, HIGH);
  delay(1);
}

void loop() {
unsigned long currentMillis = millis();

if (currentMillis - previousMillisPCM >= 250) {
    // Send PCM Frame
    previousMillisPCM = currentMillis;
    SPIcycle(0xE8, 0x14);  // priority
    SPIcycle(0xFF, 0x04);  // destination
    SPIcycle(0x10, 0x04);  // source
    SPIcycle(0x03, 0x0C);  // data
    delay(10);
    SPIcycle(0xE8, 0x14);  // priority
    SPIcycle(0xFF, 0x04);  // destination
    SPIcycle(0x40, 0x04);  // source
    SPIcycle(0x03, 0x0C);  // data

  } 
           // "68 49 10 10 xx" Coolant
  SPIcycle(0x68, 0x14);  // priority
  SPIcycle(0x49, 0x04);  // destination
  SPIcycle(0x10, 0x04);  // source
  SPIcycle(0x10, 0x04);  // Data
  SPIcycle(I, 0x0C);     // Value (iterating value of I)
    delay(10); 
                    // oil A8 4B 10 11 XX
  SPIcycle(0xA8, 0x14);  //priority
  SPIcycle(0x4B, 0x04);  //destination
  SPIcycle(0x10, 0x04);  //source
  SPIcycle(0x11, 0x04);  //Data
  SPIcycle(I2, 0x0C);   // Value (iterating value of I)
      delay(10);
  SPIcycle(0xA8, 0x14);  //  A8 83 10 12 XXFUEL
  SPIcycle(0x83, 0x04);
  SPIcycle(0x10, 0x04);
  SPIcycle(0x12, 0x04);
  SPIcycle(I, 0x0C);  // Value (iterating value of I)
  delay(100);
I++;
  if (I > 0xFF) {
    I = 0x00; 
  }
  I2++;
  if (I2 > 0xFF) {
    I2 = 0x00; 
  }
}
