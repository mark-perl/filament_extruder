#include "measurement.h"


measurement::measurement()
{

}

measurement::~measurement()
{

}

float measurement::readCaliper() 
{
  sign = 1;
  value = 0;

  // Read 24 bits from the caliper
  for (int i = 0; i < 24; i++) {
    // Wait for the clock pin to go high
    while (digitalRead(I2C_Clock) == LOW) {}
    // Wait for the clock pin to go low
    while (digitalRead(I2C_Clock) == HIGH) {}

    // Read the data bit
    if (digitalRead(I2C_Data) == HIGH) {
      if (i < 20) value |= (1 << i);  // Build the value from bits 0-19
      if (i == 20) sign = -1;         // Set the sign bit
    }
  }

  // Convert the raw value to a measurement in millimeters
  return (value * sign) / 100.0 - offset;
}

void measurement::zeroCaliper() 
{
  offset = readCaliper();  // Set the offset to the current value to zero out future measurements
  // Serial.println("Caliper zeroed.");
}


