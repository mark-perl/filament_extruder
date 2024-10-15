#include "control.h"
#include "parameters.h"

AccelStepper tensioner (1, M1_Step, M1_Dir);
AccelStepper feeder    (1, M2_Step, M2_Dir);
AccelStepper spooler   (1, M3_Step, M3_Dir);


control::control()
{
    pinMode(M1_Dir, OUTPUT);
    pinMode(M2_Dir, OUTPUT);
    pinMode(M3_Dir, OUTPUT);
    pinMode(M1_Step, OUTPUT);
    pinMode(M2_Step, OUTPUT);
    pinMode(M3_Step, OUTPUT);

    pinMode(Fan_1, OUTPUT);
    pinMode(Fan_2, OUTPUT);
    pinMode(Fan_3, OUTPUT);

    pinMode(SW_Limit, INPUT);
    

}

control::~control()
{

}

void control::motorsInit()
{
    tensioner.setMaxSpeed(1200);
    feeder.setMaxSpeed(1200);
    spooler.setMaxSpeed(1200);
}

int control::powerFans(int fans_on)
{
    if (fans_on > FANS_CONTROLLED){
        fans_on = FANS_CONTROLLED;
    }
    if (fans_on < 0){
        fans_on = 0;
    }

    switch (fans_on)
    {
    case 0:
        digitalWrite(Fan_1, LOW);
        digitalWrite(Fan_2, LOW);
        digitalWrite(Fan_3, LOW);
        break;
    case 1:
        digitalWrite(Fan_1, HIGH);
        digitalWrite(Fan_2, LOW);
        digitalWrite(Fan_3, LOW);
        break;
    case 2:
        digitalWrite(Fan_1, HIGH);
        digitalWrite(Fan_2, HIGH);
        digitalWrite(Fan_3, LOW);
        break;
    case 3:
        digitalWrite(Fan_1, HIGH);
        digitalWrite(Fan_2, HIGH);
        digitalWrite(Fan_3, HIGH);
        break;
    default:
        digitalWrite(Fan_1, LOW);
        digitalWrite(Fan_2, LOW);
        digitalWrite(Fan_3, LOW);
        break;
    }

    return fans_on;
}

void control::setParams(Parameter params[]){
    tensioner.setSpeed(params[0].value * TENS_MICROSTEPS);
    feeder.setSpeed(params[1].value * FEEDER_MICROSTEPS);
    spooler.setSpeed(params[2].value * SPOOL_MICROSTEPS);

    params[3].value = powerFans(params[3].value);
}

void control::moveMotors(){
    tensioner.runSpeed();
    feeder.runSpeed();
    spooler.runSpeed();
}

void control::feederControl()
{
    
}