#include <SPI.h>

// Define pin numbers for Chip Select and Reset
#define csPIN 10
#define rstPIN 8
enum Command {
  STANDBY,
  RX,
  TX,
  IDN
};
bool sent = false;
Command currentCommand = STANDBY;
byte statusByte, receivedData;
word result;

void setup() {

  Serial.begin(115200);  //
  pinMode(rstPIN, OUTPUT);
  pinMode(csPIN, OUTPUT);
  SPI.begin();
  SPI.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE3));
  digitalWrite(rstPIN, LOW);
  delay(500);
  digitalWrite(rstPIN, HIGH);
  SPIcycle(0x12, 0x1A);  //Config first and then Command 4-11 datasheet for order
}

word SPIcycle(int firstbyte, int secondbyte) {
   digitalWrite(csPIN, LOW);
   delayMicroseconds(100);
   //PORTB &= ~(1 << 2); //cspin
  byte statusByte = SPI.transfer(firstbyte);
  byte receivedData = SPI.transfer(secondbyte);
   digitalWrite(csPIN, HIGH);
 // PORTB |= (1 << 2); //CSpin
   delayMicroseconds(100);
  return (statusByte << 8) | receivedData;
}
// Function to receive data via SPI and print if the status byte is not 0x10
void rxdata() {
  word result = SPIcycle(0x00, 0x02);
  byte statusByte = result >> 8;
  byte receivedData = result & 0xFF;
  if (statusByte != 0x10 && statusByte != 0x00) {
    Serial.print("RX: ");
    Serial.print(statusByte, HEX);
    Serial.print(" ");
    Serial.println(receivedData, HEX);
  }
}

void checkForCommand() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command.equalsIgnoreCase("rx")) {
      currentCommand = RX;
    } else if (command.equalsIgnoreCase("standby")) {
      currentCommand = STANDBY;
    } else if (command.equalsIgnoreCase("tx")) {
      currentCommand = TX;
    } else if (command.equalsIgnoreCase("idn")) {
      currentCommand = IDN;
    } else {
      Serial.println("MSG: Unknown command");
    }
  }
}

void loop() {
  switch (currentCommand) {
    case RX:
      checkForCommand();
      rxdata();  // Continuously call rxdata() in RX mode
      break;
    case STANDBY:
      checkForCommand();
      // Do nothing, just wait for new commands
      break;
    case TX:
      sent = false;
      Serial.println("RTS");
      while (!sent) {
        if (Serial.available() > 0) {
          String tosend = Serial.readStringUntil('\n');
          tosend.trim();

          // Calculate the number of bytes based on the message length
          int messageLength = tosend.length();
          if (messageLength % 2 == 0) {  // Ensure the length is even (pairs of hex digits)
            int numBytes = messageLength / 2;
            bool valid = true;
            byte data[numBytes];

            // Parse the received data
            for (int i = 0; i < numBytes; ++i) {
              String byteStr = tosend.substring(2 * i, 2 * i + 2);
              if (!isHexadecimalDigit(byteStr.charAt(0)) || !isHexadecimalDigit(byteStr.charAt(1))) {
                valid = false;
                break;
              }
              data[i] = strtol(byteStr.c_str(), NULL, 16);
            }

            if (valid) {
              // Execute the TX commands
              byte statusByte, receivedData;
              for (int i = 0; i < numBytes; ++i) {
                byte commandByte = (i == 0) ? 0x14 : ((i == numBytes - 1) ? 0x0C : 0x04);
                SPIcycle(data[i], commandByte);
              }
              sent = true;
              Serial.println("MSG: Message sent successfully.");
            } else {
              Serial.println("MSG: Invalid data format. Exiting TX Mode.");
              sent = true;  // Exit loop
            }
          } else {
            Serial.println("MSG: Invalid data length. Exiting TX Mode.");
            sent = true;  // Exit loop
          }
        }
      }
      currentCommand = STANDBY;
      break;
    case IDN:
      // Print identifier and switch to standby mode
      Serial.println("68hc58-PRO v0.3");
      currentCommand = STANDBY;
      break;
  }
}
