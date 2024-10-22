#pragma once
#include <Arduino.h>

struct Parameter {
    int value;
    String name;
    String units;
    int divisor;

    Parameter(float v, const String& n, const String& u, int i)
        : value(v), name(n), units(u), divisor(i) {}
};


