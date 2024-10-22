#pragma once
#include "pinMap.h"
#include "parameters.h"
#include "AccelStepper.h"


class control
{
public:
    control();
    ~control();
    
    void motorsInit();
    void feederHome();
    void setParams(Parameter params[]);
    void moveMotors();
    int powerFans(int fans_on);


private:
    void feederControl();
    int feeder_end_pos_mm = 60; 

};