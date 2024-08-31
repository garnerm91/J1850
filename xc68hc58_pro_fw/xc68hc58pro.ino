#include <SPI.h>

// Define pin numbers for Chip Select and Reset
#define csPIN  10
#define rstPIN  8

bool sent = false;

enum Command {
  STANDBY,
  RX,
  TX,
  IDN
};

Command currentCommand = STANDBY;

void setup() {
  byte statusByte, receivedData;
  Serial.begin(115200);
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

// Function to receive data via SPI and print if the status byte is not 0x10
void rxdata() {
  byte statusByte, receivedData;
  SPIcycle(0x00, 0x02, statusByte, receivedData);
  if (statusByte != 0x10) {
    // Print the received status byte and data
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
      rxdata(); // Continuously call rxdata() in RX mode
      break;
    case STANDBY:
      checkForCommand();
      // Do nothing, just wait for new commands
      break;
    case TX:
      sent = false; 
      Serial.println("RTS");
      while(sent == false) {
        if (Serial.available() > 0) {
          String tosend = Serial.readStringUntil('\n');
          tosend.trim();
          // Parse the received data
          if (tosend.length() == 22) { // 11 bytes, each represented as 2 hex digits
            bool valid = true;
            byte data[11];
            for (int i = 0; i < 11; ++i) {
              String byteStr = tosend.substring(2*i, 2*i+2);
              if (!isHexadecimalDigit(byteStr.charAt(0)) || !isHexadecimalDigit(byteStr.charAt(1))) {
                valid = false;
                break;
              }
               data[i] = strtol(byteStr.c_str(), NULL, 16);
            }
            if (valid) {
              // Execute the TX commands
              byte statusByte, receivedData;
              for (int i = 0; i < 11; i++) {
                byte commandByte = (i == 0) ? 0x14 : ((i == 10) ? 0x0C : 0x04);
                SPIcycle(data[i], commandByte, statusByte, receivedData);
              }
              sent = true;
              Serial.println("MSG: Message sent successfully.");
            } else {
              Serial.println("MSG: Invalid data format. Exiting TX Mode.");
              sent = true; // have to set it to true to exit
            }
          } else {
            Serial.println("MSG: Invalid data length. Exiting TX Mode.");
            sent = true; // have to set it to true to exit
          }
        }
      }
      currentCommand = STANDBY;
      break;
    case IDN:
      // Print identifier and switch to standby mode
      Serial.println("68hc58-PRO v0.2");
      currentCommand = STANDBY;
      break;
  }
  delay(25); 
}
