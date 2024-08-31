#include <SPI.h>

// Define pin numbers for Chip Select and Reset
#define csPIN  10
#define rstPIN  8


void setup() {
  byte statusByte, receivedData;
  Serial.begin(9600);
  pinMode(rstPIN, OUTPUT);
  pinMode(csPIN, OUTPUT);
  SPI.begin();
  SPI.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE3));
  digitalWrite(rstPIN, LOW);
  delay(1000);
  digitalWrite(rstPIN, HIGH);
  SPIcycle(0x12, 0x18, statusByte, receivedData);
}

void SPIcycle(int firstbyte, int secondbyte, byte &statusByte, byte &receivedData) {
  digitalWrite(csPIN, LOW);
  delay(26);
  statusByte = SPI.transfer(firstbyte);
  receivedData = SPI.transfer(secondbyte);
  delay(26);
  digitalWrite(csPIN, HIGH);
}

void rxdata() {
  byte statusByte, receivedData;
  SPIcycle(0x00, 0x02, statusByte, receivedData);
    // Print the received status byte and data
    Serial.print("RX: ");
    Serial.print(statusByte, HEX);
    Serial.print(" ");
    Serial.println(receivedData, HEX);
}
void loop() {
 delay(200);
 rxdata(); 
}
