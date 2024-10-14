#pragma once
#include "pinMap.h"
#include "parameters.h"
#include "AccelStepper.h"


class control
{
public:
    control();
    ~control();
    
    void move(Parameter params[]);

private:
    void feederControl();
    int powerFans(int fans_on);


};