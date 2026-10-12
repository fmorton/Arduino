#include <Wire.h>

void setup() {
  Serial.begin(115200);
  while (!Serial); // Wait for Serial Monitor to open
  
  Serial.println("\n--- Scanning Wire1 (Qwiic Bus) ---");
  Wire1.begin();
}

void loop() {
  byte error, address;
  int devicesFound = 0;

  for (address = 1; address < 127; address++) {
    Wire1.beginTransmission(address);
    error = Wire1.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
      
      if (address == 0x50) {
        Serial.println(" (HuskyLens 2 Detected!)");
      } else {
        Serial.println();
      }
      devicesFound++;
    }
  }

  if (devicesFound == 0) {
    Serial.println("No I2C devices found on Wire1.");
  }
  
  delay(5000);
}
