#include <Wire.h>

void setup() {
  Wire.begin();             // Initialize I2C bus
  Serial.begin(9600);       // Start serial communication at 9600 baud
  Serial.println("I2C Scanner");
}

void loop() {
  byte error, address;      // Variables to store error and address
  int nDevices = 0;         // Counter for number of devices found

  Serial.println("Scanning...");

  for (address = 1; address < 127; address++) { // I2C addresses range from 1 to 126
    Wire.beginTransmission(address);           // Begin I2C transmission to the device
    error = Wire.endTransmission();            // End the transmission and get the status

    if (error == 0) {                          // If no error, device found at this address
      Serial.print("Device found at 0x");
      if (address < 16) 
        Serial.print("0");                     // Add leading zero if address is below 0x10
      Serial.print(address, HEX);              // Print address in hexadecimal
      Serial.println(" !");
      nDevices++;
    } else if (error == 4) {                   // Error code 4 means a communication error
      Serial.print("Unknown error at address 0x");
      if (address < 16) 
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }

  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("Done scanning\n");

  delay(5000);  // Wait 5 seconds before scanning again
}