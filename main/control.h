#pragma once
#include "pinMap.h"
#include "parameters.h"
#include "AccelStepper.h"
// #include "megaAVR_TimerInterrupt.h"


class control
{
public:
    control();
    ~control();
    
    void motorsInit();
    void feederHome();

    void motorsOn();
    void motorsOff();
    
    void setParams(Parameter params[]);
    void moveMotors();
    int powerFans(int fans_on);
    void autoControl(Parameter param[], Parameter meas_diam, Parameter goal_diam);


private:
    int feederControl(int spooler_speed);
    int feeder_speed = 0;
    int feeder_dir = 1;
    long feeder_pos = 0;

    int spoolerControl(int spooler_speed);
    unsigned long spooler_control_time;

    int tensionerControl(int tensioner_speed, int diam_error);
    unsigned long tensioner_control_time;

};

// void moveMotorsInterrupt();