// Finding the "goldilocks" speed for the desired filament diameter

#include <Arduino.h>
#include <AccelStepper.h>

// Pin configuration
const int dirPin = 2;
const int stepPin = 3;

// Create motor object using AccelStepper library
AccelStepper motor_x(1, stepPin, dirPin); // 1 = DRIVER (step and direction pins)

// Motor parameters
const int baseSpeed = 1500; // initial speed in steps per second
const int maxSpeed = 5000; // set max speed of motor
int adjustedSpeed = baseSpeed;

// Measurement parameters
const float goalDiameter = 1.75;
const float tolerance = 0.05;
float measurement1 = 0.0; // First measurement device reading
float measurement2 = 0.0; // Second measurement device reading

// Control variables
bool isGoalAchieved = false;
unsigned long lastUpdateTime = 0;
const unsigned long updateInterval = 100; // Check every 0.1 second

void setup() {
    // Initialize serial communication
    Serial.begin(9600);
    Serial.println("Enter goal diameter: Select '1' = 1.75mm or '2' = 2.85mm");

    // Set max speed and initial speed for the motor
    motor_x.setMaxSpeed(maxSpeed); // Max speed in steps per second
    motor_x.setSpeed(baseSpeed); // Set initial speed
}

void loop() {
    if (Serial.available() > 0) {
        String input = Serial.readStringUntil('\n');
        input.trim(); // Remove any newline or whitespace

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
    
    // Run the motor at the set speed
    motor_x.runSpeed();

    // Read from the first measurement device
    measurement1 = readMeasurementDevice1();

    // Calculate the deviation from the goal diameter
    float deviation = measurement1 - goalDiameter;

    // Adjust motor speed based on the deviation
    // Each 0.05mm deviation changes speed by 10%
    int percentageChange = deviation / 0.05 * 10;
    adjustedSpeed = baseSpeed * (1 + (percentageChange / 100.0));

    // Apply adjusted motor speed with AccelStepper
    motor_x.setSpeed(adjustedSpeed);

    // Read from the second measurement device
    measurement2 = readMeasurementDevice2();

    // Check if the second measurement consistently meets the goal
    if (isWithinTolerance(measurement2, goalDiameter, tolerance)) {
        isGoalAchieved = true;
    } else {
        isGoalAchieved = false;
    }

    // If goal is achieved consistently, output the speed
    if (isGoalAchieved) {
        Serial.print("Consistent goal diameter achieved. Tensioner speed: ");
        Serial.print(adjustedSpeed);
        Serial.println(" steps/sec");
        delay(5000); // Wait 5 seconds before re-checking
    }

    delay(updateInterval); // Wait before next measurement
}

// Functions to read from the measurement devices
float readMeasurementDevice1() {
    // Simulate reading a value from the first measurement device
    // Replace with actual code to read sensor value
    return 1.76; // Example value, replace with sensor data
}

float readMeasurementDevice2() {
    // Simulate reading a value from the second measurement device
    // Replace with actual code to read sensor value
    return 1.75; // Example value, replace with sensor data
}

// Function to check if measurement is within tolerance range
bool isWithinTolerance(float measurement, float target, float tolerance) {
    return measurement >= (target - tolerance) && measurement <= (target + tolerance);
}
