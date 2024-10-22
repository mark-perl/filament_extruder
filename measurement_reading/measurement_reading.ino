#include <Wire.h>

#define CLOCK_PIN 12
#define DATA_PIN 11
#define ZERO_BUTTON_PIN 2 // Pin connected to the zero button
#define BUTTON1_PIN 3     // Pin connected to button 1 for 1.75mm target
#define BUTTON2_PIN 4     // Pin connected to button 2 for 2.50mm target
#define LED1_PIN 5        // Pin connected to LED 1
#define LED2_PIN 6        // Pin connected to LED 2

char buf[20];
unsigned long tmpTime;
int sign;
long value;
float result = 0;
float offset = 0; // Offset value for zeroing
float displayValue = 0;
float selectedTarget = 0;

void setup() 
{
  Serial.begin(9600);
  
  pinMode(CLOCK_PIN, INPUT);
  pinMode(DATA_PIN, INPUT);
  pinMode(ZERO_BUTTON_PIN, INPUT_PULLUP); // Use internal pull-up resistor
  pinMode(BUTTON1_PIN, INPUT_PULLUP);     // Use internal pull-up resistor
  pinMode(BUTTON2_PIN, INPUT_PULLUP);     // Use internal pull-up resistor
  pinMode(LED1_PIN, OUTPUT);              // Set LED 1 as output
  pinMode(LED2_PIN, OUTPUT);              // Set LED 2 as output
}

void loop() 
{
  // Check if zero button is pressed
  if (digitalRead(ZERO_BUTTON_PIN) == LOW) {
    zeroCaliper();  // Call function to zero the caliper
    delay(200);     // Simple debounce delay for button press
  }

  // Wait for the clock pin to go high
  while (digitalRead(CLOCK_PIN) == LOW) {}

  tmpTime = micros();  // Record the time when the clock goes high

  // Wait for the clock pin to go low
  while (digitalRead(CLOCK_PIN) == HIGH) {}

  // Check if the signal is valid (debounce)
  if ((micros() - tmpTime) < 500) return;

  // Read the data from the caliper
  readCaliper();

  // Apply the offset to zero the reading
  displayValue = result - offset;

  // Print the result in millimeters to the serial monitor
  Serial.print(displayValue);
  Serial.print(" ");
  Serial.println(" mm");

  delay(100); // Delay for 100ms before the next read
}

void readCaliper() 
{
  sign = 1;
  value = 0;

  // Read 24 bits from the caliper
  for (int i = 0; i < 24; i++) {
    // Wait for the clock pin to go high
    while (digitalRead(CLOCK_PIN) == LOW) {}
    // Wait for the clock pin to go low
    while (digitalRead(CLOCK_PIN) == HIGH) {}

    // Read the data bit
    if (digitalRead(DATA_PIN) == HIGH) {
      if (i < 20) value |= (1 << i);  // Build the value from bits 0-19
      if (i == 20) sign = -1;         // Set the sign bit
    }
  }

  // Convert the raw value to a measurement in millimeters
  result = (value * sign) / 100.0;
}

void zeroCaliper() 
{
  offset = result;  // Set the offset to the current value to zero out future measurements
  Serial.println("Caliper zeroed.");
}
