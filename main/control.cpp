#include "control.h"
#include "parameters.h"

AccelStepper tensioner (1, M1_Step, M1_Dir);
AccelStepper feeder    (1, M2_Step, M2_Dir);
AccelStepper spooler   (1, M3_Step, M3_Dir);

#define TENS_MICROSTEPS     1
#define SPOOL_MICROSTEPS    1
#define FEEDER_MICROSTEPS   1

#define FANS_CONTROLLED     3
#define FANS_ALWAYS_ON      2

#define FEEDER_PITCH_MM         8.0
// #define FEEDER_HOME_POS_MM      -108.0  // Offset from button to home pos
#define FEEDER_HOME_POS_MM      5
#define FEEDER_HOME_POS_STEPS   FEEDER_HOME_POS_MM / FEEDER_PITCH_MM * 200.0
#define SPOOL_WIDTH_MM          60
#define FEEDER_END_POS_STEPS    FEEDER_HOME_POS_STEPS + SPOOL_WIDTH_MM / FEEDER_PITCH_MM * 200.0
#define FEEDER_SPOOLER_RATIO    16.0


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
    tensioner.setMaxSpeed(400 * TENS_MICROSTEPS);
    feeder.setMaxSpeed(400 * FEEDER_MICROSTEPS);
    spooler.setMaxSpeed(400 * SPOOL_MICROSTEPS);
    feeder.setAcceleration(100000); // large for no accel
}

void control::feederHome()
{
    feeder.setSpeed(-400*FEEDER_MICROSTEPS);

    while (!digitalRead(SW_Limit)){
        feeder.runSpeed();
    }

    // Limit switch hit
    feeder.setCurrentPosition(0);
    
    feeder.runToNewPosition(FEEDER_HOME_POS_STEPS * FEEDER_MICROSTEPS);
    
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

void control::setParams(Parameter params[])
{
    tensioner.setSpeed(params[0].value * TENS_MICROSTEPS);
    spooler.setSpeed( -1 * params[1].value * SPOOL_MICROSTEPS);
    params[2].value = powerFans(params[2].value);
    params[3].value = feederControl(params[1].value);
    feeder.setSpeed(params[3].value * FEEDER_MICROSTEPS);
}

void control::moveMotors()
{
    tensioner.runSpeed();
    feeder.runSpeed();
    spooler.runSpeed();
}

int control::feederControl(int spooler_speed)
{
    feeder_pos = feeder.currentPosition() / FEEDER_MICROSTEPS;
    if (feeder_pos > FEEDER_END_POS_STEPS) {feeder_dir = -1;}
    if (feeder_pos < FEEDER_HOME_POS_STEPS) {feeder_dir = 1;}
    
    feeder_speed = feeder_dir * (spooler_speed / FEEDER_SPOOLER_RATIO);

    return feeder_speed;
}