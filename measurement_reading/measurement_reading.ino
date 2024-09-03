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
float displayValue  = 0;
float selectedTarget = 0;

unsigned long previousMillis = 0;  // Stores the last time the LED was updated
const long interval = 300;  // Interval at which to blink (milliseconds)

bool blinkled1 = false;
bool blinkled2 = false;

int button1_state;
int button2_state;

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

  // Print the result in millimeters
  Serial.print(displayValue);
  Serial.print(" ");
  Serial.print(result);
  Serial.println(" mm");

  checkTargetValue();
  
  if(blinkled1 == true && blinkled2 == false)
  {
    blinkled_1();
    blinkled2 = false;
    digitalWrite(LED2_PIN,LOW);
  }
  if(blinkled2 == true && blinkled1 == false )
  {
    blinkled_2();
    blinkled1 = false;
    digitalWrite(LED1_PIN,LOW);
  }

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

void blinkled_1()
{
  unsigned long currentMillis = millis();  // Get the current time

  // Check if 300 milliseconds have passed
  if (currentMillis - previousMillis >= interval) 
  {
    previousMillis = currentMillis;  // Save the last time the LED blinked

    // Toggle the LED state
    if (digitalRead(LED1_PIN) == LOW) {
      digitalWrite(LED1_PIN, HIGH);  // Turn the LED on
    } else {
      digitalWrite(LED1_PIN, LOW);   // Turn the LED off
    }
  }
}

void blinkled_2()
{
  unsigned long currentMillis = millis();  // Get the current time

  // Check if 300 milliseconds have passed
  if (currentMillis - previousMillis >= interval) 
  {
    previousMillis = currentMillis;  // Save the last time the LED blinked

    // Toggle the LED state
    if (digitalRead(LED2_PIN) == LOW) 
    {
      digitalWrite(LED2_PIN, HIGH);  // Turn the LED on
    } else {
      digitalWrite(LED2_PIN, LOW);   // Turn the LED off
    }
  }
}

void checkTargetValue() 
{
  // Check for button presses to select the target range
  button1_state = digitalRead(BUTTON1_PIN);
  button2_state = digitalRead(BUTTON2_PIN);

  // If button 1 is pressed, select target 1 (1.75mm with ±0.05mm tolerance)
  if (button1_state == LOW && selectedTarget != 1) 
  {
    selectedTarget = 1;
    digitalWrite(LED2_PIN, LOW);  // Turn off LED 2 when switching to target 1
    blinkled2 = false;
    Serial.println("Button 1 is pressed, Target 1 selected");
  }

  // If button 2 is pressed, select target 2 (2.50mm with ±0.05mm tolerance)
  if (button2_state == LOW && selectedTarget != 2) 
  {
    selectedTarget = 2;
    digitalWrite(LED1_PIN, LOW);  // Turn off LED 1 when switching to target 2
    blinkled1 = false;
    Serial.println("Button 2 is pressed, Target 2 selected");
  }

  // Continuously check the result value against the selected target range
  if (selectedTarget == 1) 
  {
    if (result >= 1.70 && result <= 1.80) 
    {
      digitalWrite(LED1_PIN, HIGH);  // Turn LED 1 on solid
      blinkled1 = false;
      Serial.println("Target 1 Achieved");
    } 
    else 
    {
      blinkled1 = true;  // Blink LED 1
      Serial.println("Target 1 in Pursuit");
    }
  }
  else if (selectedTarget == 2) 
  {
    if (result >= 2.45 && result <= 2.55) 
    {
      digitalWrite(LED2_PIN, HIGH);  // Turn LED 2 on solid
      blinkled2 = false;
      Serial.println("Target 2 Achieved");
    } 
    else 
    {
      blinkled2 = true;  // Blink LED 2
      Serial.println("Target 2 in Pursuit");
    }
  }
}
