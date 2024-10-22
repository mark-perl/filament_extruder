#include "control.h"
#include "parameters.h"

AccelStepper tensioner (1, M1_Step, M1_Dir);
AccelStepper feeder    (1, M2_Step, M2_Dir);
AccelStepper spooler   (1, M3_Step, M3_Dir);

#define TENS_MICROSTEPS     8
#define SPOOL_MICROSTEPS    8
#define FEEDER_MICROSTEPS   4

#define FANS_CONTROLLED     3
#define FANS_ALWAYS_ON      2

#define FEEDER_PITCH_MM         8.0
#define FEEDER_HOME_POS_MM      -108.0  // Offset from button to home pos
#define FEEDER_HOME_POS_STEPS   FEEDER_HOME_POS_MM / FEEDER_PITCH_MM * 200.0


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
    feeder.setMaxSpeed(3200);
    spooler.setMaxSpeed(1200);
    feeder.setAcceleration(100000);
}

void control::feederHome()
{
    feeder.setSpeed(400*FEEDER_MICROSTEPS);

    while (!digitalRead(SW_Limit)){
        feeder.runSpeed();
    }

    // Limit switch hit
    feeder.setCurrentPosition(0);
    // feeder.moveTo(FEEDER_HOME_POS_STEPS * FEEDER_MICROSTEPS);
    // feeder.setSpeed(600 * FEEDER_MICROSTEPS);

    // while (feeder.isRunning()){
    //     feeder.runSpeedToPosition();
    // }
    feeder.runToNewPosition(FEEDER_HOME_POS_STEPS * FEEDER_MICROSTEPS);
    
    Serial.println("Done");
    feeder.setCurrentPosition(0);
}

int control::powerFans(int fans_on)
{
    if (fans_on > (FANS_CONTROLLED+FANS_ALWAYS_ON) ){
        fans_on = (FANS_CONTROLLED+FANS_ALWAYS_ON);
    }
    if (fans_on < FANS_ALWAYS_ON){
        fans_on = FANS_ALWAYS_ON;
    }

    switch (fans_on)
    {
    case FANS_ALWAYS_ON:
        digitalWrite(Fan_1, LOW);
        digitalWrite(Fan_2, LOW);
        digitalWrite(Fan_3, LOW);
        break;
    case (FANS_ALWAYS_ON + 1):
        digitalWrite(Fan_1, HIGH);
        digitalWrite(Fan_2, LOW);
        digitalWrite(Fan_3, LOW);
        break;
    case (FANS_ALWAYS_ON + 2):
        digitalWrite(Fan_1, HIGH);
        digitalWrite(Fan_2, HIGH);
        digitalWrite(Fan_3, LOW);
        break;
    case (FANS_ALWAYS_ON + 3):
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
    spooler.setSpeed(params[1].value * SPOOL_MICROSTEPS);
    params[2].value = powerFans(params[2].value);
    feeder.setSpeed(params[3].value * FEEDER_MICROSTEPS);
}

void control::moveMotors(){
    tensioner.runSpeed();
    feeder.runSpeed();
    spooler.runSpeed();
}

void control::feederControl()
{
    
}