#pragma once
#include <Arduino.h>

#define FANS_CONTROLLED     3
#define FANS_ALWAYS_ON      2

#define TENS_MICROSTEPS     8
#define SPOOL_MICROSTEPS    4
#define FEEDER_MICROSTEPS   4


struct Parameter {
    int value;
    String name;
    String units;
    int divisor;

    Parameter(float v, const String& n, const String& u, int i)
        : value(v), name(n), units(u), divisor(i) {}
};


