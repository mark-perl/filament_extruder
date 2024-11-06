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
    void autoControl(Parameter param[], Parameter meas_diam);


private:
    int feederControl(int spooler_speed);
    int feeder_speed = 0;
    int feeder_dir = 1;
    long feeder_pos = 0;

    int spoolerControl(int spooler_speed);
    unsigned long spooler_control_time;

};