#include "measurement.h"

    volatile int measurement::caliperValue = 0;
    volatile int measurement::sign = 1;
    volatile int measurement::value = 0;
    volatile unsigned long measurement::interruptTime = 0;
    volatile int measurement::i = 0;
    volatile float measurement::zeroOffset = 0;

    const int signTable[2] = {1, -1};  // Lookup table for sign values

measurement::measurement()
{
    pinMode(Meas_Clock, INPUT);
    pinMode(Meas_Data, INPUT);    
}

measurement::~measurement()
{

}

void measurement::caliperInit()
{
    unsigned long highTime = millis();

    while (true) {
        // Wait for caliper to be finished current data packet to begin interrupt

        if (!digitalRead(Meas_Clock)) {
            highTime = millis(); // reset high time
        }
        
        if ((millis() - highTime) > 10) {
            // If high time > 10ms, caliper is inbetween data packets
            break;
        }
    }
    // Initiliase values
    i = 0;
    value = 0;
    sign = 1;

    attachInterrupt(digitalPinToInterrupt(Meas_Clock), caliperInterrupt, FALLING);

    delay(200); // Wait for caliper to read value
    zeroCaliper();
}

void measurement::caliperInterrupt()
{
    // Interuppt on low clock value

    // value |= (digitalRead(Meas_Data) << i) * (i < 20);  // Accumulate bits only for i < 20
    if (digitalRead(Meas_Data)) {
        value |= (1 << (i-1)) * (i < 20) * (i != 0);
    }

    if (i == 0) {
        // First bit should always be low, i think
        if (!digitalRead(Meas_Data)) {
            i = 1;  // Assume it missed a bit and iterate to next
        }
    }
    else if (i == 20) {
        sign = signTable[digitalRead(Meas_Data)];  // Assign sign value
        caliperValue = (value * sign) - zeroOffset; 
    } 
    else if (i == 23) {
        i = -1;       // Reset index for next packet
        value = 0;    // Reset accumulated value
        // sign = 1;     // Reset sign
    }

    i++;
}

float measurement::readCaliper()
{   
    // Blocking method to read caliper value
    // Time taken between 6-70ms, depending on when called

    sign = 1;
    value = 0;

    // Read 24 bits from the caliper
    for (int i = 0; i < 24; i++)
    {
        while (digitalRead(Meas_Clock) == LOW)
        {
            // Wait for the clock pin to go high
        }
        while (digitalRead(Meas_Clock) == HIGH)
        {
            // Wait for the clock pin to go low
        }

        // Read the data bit
        if (digitalRead(Meas_Data) == HIGH)
        {
            if ((i < 20) && (i > 0))
                value |= (1 << (i-1)); // Build the value from bits 0-19
            if (i == 21)
                // value <= 1;  // Right shift by 1
                sign = -1; // Set the sign bit
        }
    }

    // Convert the raw value to a measurement in millimeters *10^-2
    caliperValue = (value * sign) - zeroOffset;
    return caliperValue;
}

void measurement::zeroCaliper()
{
    readCaliper();
    zeroOffset = caliperValue; // Set the offset to the current value to zero out future measurements
}
