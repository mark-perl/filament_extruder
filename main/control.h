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
    void setParams(Parameter params[]);
    void moveMotors();
    int powerFans(int fans_on);


private:
    void feederControl();


};