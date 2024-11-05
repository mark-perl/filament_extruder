#include <Arduino.h>
#include <AccelStepper.h>

// Motor pin configuration
const int dirPin = 2;
const int stepPin = 3;
AccelStepper motor_x(1, stepPin, dirPin); // 1 = DRIVER (step and direction pins)

// Motor parameters
const int baseSpeed = 1500; // Initial speed in steps per second
const int maxSpeed = 5000;  // Set max speed of motor
int adjustedSpeed = baseSpeed;

// Measurement parameters
float goalDiameter = 1.75;  // Default goal diameter
const float tolerance = 0.05; // Tolerance for measurement
float measurement1 = 0.0;    // First measurement device reading
float measurement2 = 0.0;    // Second measurement device reading

// Control variables
bool isGoalAchieved = false;
unsigned long lastUpdateTime = 0;
const unsigned long updateInterval = 100; // Check every 0.1 second

// Caliper pin configuration
#define CLOCK_PIN1 12
#define DATA_PIN1 11
#define ZERO_BUTTON_PIN1 4 // Pin connected to the zero button for the first caliper
#define CLOCK_PIN2 13
#define DATA_PIN2 10
#define ZERO_BUTTON_PIN2 5 // Pin connected to the zero button for the second caliper

int sign;
long value;
float result = 0;
float offset1 = 0; // Offset value for zeroing first caliper
float offset2 = 0; // Offset value for zeroing second caliper
float displayValue = 0;

void setup() {
    Serial.begin(9600);
    Serial.println("Enter goal diameter: Select '1' = 1.75mm or '2' = 2.85mm");

    // Motor setup
    motor_x.setMaxSpeed(maxSpeed);
    motor_x.setSpeed(baseSpeed);

    // Caliper pin setup
    pinMode(CLOCK_PIN1, INPUT);
    pinMode(DATA_PIN1, INPUT);
    pinMode(ZERO_BUTTON_PIN1, INPUT_PULLUP);
    pinMode(CLOCK_PIN2, INPUT);
    pinMode(DATA_PIN2, INPUT);
    pinMode(ZERO_BUTTON_PIN2, INPUT_PULLUP);
}

void loop() {
    if (Serial.available() > 0) {
        String input = Serial.readStringUntil('\n');
        input.trim();

        if (input == "1") {
            goalDiameter = 1.75;
            Serial.println("Goal diameter set to 1.75mm.");
        } else if (input == "2") {
            goalDiameter = 2.85;
            Serial.println("Goal diameter set to 2.85mm.");
        } else {
            Serial.println("Invalid input. Please enter '1' or '2'.");
        }
    }

    // Zero the first caliper if button pressed
    if (digitalRead(ZERO_BUTTON_PIN1) == LOW) {
        zeroCaliper1();
        delay(200); // Simple debounce delay for button press
    }
    
    // Zero the second caliper if button pressed
    if (digitalRead(ZERO_BUTTON_PIN2) == LOW) {
        zeroCaliper2();
        delay(200);
    }

    // Run the motor at the set speed
    motor_x.runSpeed();

    // Read measurements from both calipers
    measurement1 = readMeasurementDevice1();
    measurement2 = readMeasurementDevice2();

    // Calculate deviation and adjust motor speed based on measurement1
    float deviation = measurement1 - goalDiameter;
    int percentageChange = deviation / 0.05 * 10;
    adjustedSpeed = baseSpeed * (1 + (percentageChange / 100.0));
    motor_x.setSpeed(constrain(adjustedSpeed, 0, maxSpeed)); // Update motor speed

    // Check if the second measurement meets the goal consistently
    if (isWithinTolerance(measurement2, goalDiameter, tolerance)) {
        isGoalAchieved = true;
    } else {
        isGoalAchieved = false;
    }

    // Output message if goal is achieved
    if (isGoalAchieved) {
        Serial.print("Consistent goal diameter achieved. Tensioner speed: ");
        Serial.print(adjustedSpeed);
        Serial.println(" steps/sec");
        delay(5000); // Wait before re-checking
    }

    delay(updateInterval); // Wait before next measurement
}

// Functions to read measurements from the calipers
float readMeasurementDevice1() {
    // Wait for the clock pin to go high
    while (digitalRead(CLOCK_PIN1) == LOW) {}

    unsigned long tmpTime = micros();

    // Wait for the clock pin to go low
    while (digitalRead(CLOCK_PIN1) == HIGH) {}

    if ((micros() - tmpTime) < 500) return -1;

    sign = 1;
    value = 0;

    for (int i = 0; i < 24; i++) {
        while (digitalRead(CLOCK_PIN1) == LOW) {}
        while (digitalRead(CLOCK_PIN1) == HIGH) {}

        if (digitalRead(DATA_PIN1) == HIGH) {
            if (i < 20) value |= (1 << i);
            if (i == 20) sign = -1;
        }
    }

    result = (value * sign) / 100.0;
    return result - offset1;
}

float readMeasurementDevice2() {
    while (digitalRead(CLOCK_PIN2) == LOW) {}
    unsigned long tmpTime = micros();
    while (digitalRead(CLOCK_PIN2) == HIGH) {}

    if ((micros() - tmpTime) < 500) return -1;

    sign = 1;
    value = 0;

    for (int i = 0; i < 24; i++) {
        while (digitalRead(CLOCK_PIN2) == LOW) {}
        while (digitalRead(CLOCK_PIN2) == HIGH) {}

        if (digitalRead(DATA_PIN2) == HIGH) {
            if (i < 20) value |= (1 << i);
            if (i == 20) sign = -1;
        }
    }

    result = (value * sign) / 100.0;
    return result - offset2;
}

bool isWithinTolerance(float measurement, float target, float tolerance) {
    return measurement >= (target - tolerance) && measurement <= (target + tolerance);
}

void zeroCaliper1() {
    offset1 = result;
    Serial.println("Caliper 1 zeroed.");
}

void zeroCaliper2() {
    offset2 = result;
    Serial.println("Caliper 2 zeroed.");
}

