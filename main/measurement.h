#pragma once
#include "pinMap.h"
#include <Wire.h>

class measurement
{
public:
    measurement();
    ~measurement();
    
    static void caliperInterrupt();

    void caliperInit();
    float readCaliper();
    void zeroCaliper();

    static volatile int caliperValue;

private:

    static volatile int sign;
    static volatile int value;
    static volatile unsigned long interruptTime;
    static volatile int i;
    static volatile float zeroOffset; 
    
};