#pragma once
#include "pinMap.h"
#include <Wire.h>

class measurement
{
public:
    measurement();
    ~measurement();
    
    float readCaliper();
    void zeroCaliper();

private:

    char buf[20];
    unsigned long tmpTime;
    int sign;
    long value;
    // float result = 0;
    float offset = 0; 
   
};