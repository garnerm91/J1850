#include <SPI.h>

#define csPIN  10
#define rstPIN  8

void setup() {
  Serial.begin(9600);
  pinMode(rstPIN, OUTPUT);
  pinMode(csPIN, OUTPUT);
  SPI.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE3));
  SPI.begin();
  digitalWrite(rstPIN, LOW);
  delay(1000);
  digitalWrite(rstPIN, HIGH);
  SPIcycle(0x12, 0x18);
}

void SPIcycle(int firstbyte, int secondbyte) {
  digitalWrite(csPIN, LOW);
  delay(30);
  SPI.transfer(firstbyte);
  SPI.transfer(secondbyte);
  delay(30);
  digitalWrite(csPIN, HIGH);
}

void rxdata() {
  digitalWrite(csPIN, LOW);
  delay(30);
  //sends dummy byte to get status and fifo
  byte statusByte = SPI.transfer(0x00);
  byte receivedData = SPI.transfer(0x02);
  // Print the received status byte and data
  Serial.print("Status Byte: 0x");
  Serial.println(statusByte, HEX);
  Serial.print("Received Data: 0x");
  Serial.println(receivedData, HEX);
  Serial.println("-----");
  delay(30);

  // sends RFC flush byte
  //statusByte = SPI.transfer(0x00);
  //receivedData = SPI.transfer(0x02);

  digitalWrite(csPIN, HIGH);

  // Print the received status byte and data
  //Serial.print("Status Byte: 0x");
 // Serial.println(statusByte, HEX);
  //Serial.print("Received Data: 0x");
 // Serial.println(receivedData, HEX);
 // Serial.println("-----");
 // delay(30);
}

void loop() {
  rxdata();
  delay(400);
}