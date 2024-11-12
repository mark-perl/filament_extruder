#include "control.h"
#include "parameters.h"

AccelStepper tensioner (1, M1_Step, M1_Dir);
AccelStepper feeder    (1, M2_Step, M2_Dir);
AccelStepper spooler   (1, M3_Step, M3_Dir);

#define TENS_MICROSTEPS     -8
#define SPOOL_MICROSTEPS    8
#define FEEDER_MICROSTEPS   8

#define FANS_CONTROLLED     3
#define FANS_ALWAYS_ON      2

#define FEEDER_PITCH_MM         8.0
#define FEEDER_HOME_POS_MM      5
#define FEEDER_HOME_POS_STEPS   FEEDER_HOME_POS_MM / FEEDER_PITCH_MM * 200.0
#define SPOOL_WIDTH_MM          60
#define FEEDER_END_POS_STEPS    FEEDER_HOME_POS_STEPS + SPOOL_WIDTH_MM / FEEDER_PITCH_MM * 200.0
#define FEEDER_SPOOLER_RATIO    16.0

#define SPOOLER_CONTROL_DELAY_MS    500
#define TENSIONER_CONTROL_DELAY_MS  500


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
    pinMode(Spooling_Pot, INPUT);
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

    // Timer1.initialize(100);
    // motorsOn();
    // ITimer1.init();
    
}

void moveMotorsInterrupt()
{
    tensioner.runSpeed();
    feeder.runSpeed();
    spooler.runSpeed();
}

void control::motorsOn()
{
    // Timer1.attachInterrupt(moveMotorsInterrupt);
}

void control::motorsOff()
{
    // Timer1.detachInterrupt();
}

void control::moveMotors()
{
    tensioner.runSpeed();
    feeder.runSpeed();
    spooler.runSpeed();
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

void control::autoControl(Parameter params[], Parameter meas_diam, Parameter goal_diam)
{
    params[0].value = tensionerControl(params[0].value, (goal_diam.value - meas_diam.value));
    params[1].value = spoolerControl(params[1].value);
}

void control::setParams(Parameter params[])
{
    tensioner.setSpeed(params[0].value * TENS_MICROSTEPS);
    
    spooler.setSpeed(params[1].value * SPOOL_MICROSTEPS);
    
    params[2].value = powerFans(params[2].value);

    params[3].value = feederControl(params[1].value);
    feeder.setSpeed(params[3].value * FEEDER_MICROSTEPS);
}

int control::feederControl(int spooler_speed)
{
    feeder_pos = feeder.currentPosition() / FEEDER_MICROSTEPS;
    if (feeder_pos > FEEDER_END_POS_STEPS) {feeder_dir = -1;}
    if (feeder_pos < FEEDER_HOME_POS_STEPS) {feeder_dir = 1;}
    
    feeder_speed = feeder_dir * (spooler_speed / FEEDER_SPOOLER_RATIO);

    return feeder_speed;
}

int control::spoolerControl(int spooler_speed)
{
    if ((millis() - spooler_control_time) > SPOOLER_CONTROL_DELAY_MS) 
    {
        // Only update after control delay since last speed update.
        if (analogRead(Spooling_Pot) > 225) {
            // Slow down
            spooler_speed -= 1;
            spooler_control_time = millis();
        }
        else if (analogRead(Spooling_Pot) < 175) {
            // Speed up
            spooler_speed += 1;
            spooler_control_time = millis();
        }
    }

    return spooler_speed;
}

int control::tensionerControl(int tensioner_speed, int diam_error)
{
    if ((millis() - tensioner_control_time) > TENSIONER_CONTROL_DELAY_MS) 
    {
        // Only update after control delay since last speed update.
        if (diam_error < -2) {
            // Slow down
            tensioner_speed -= 1;
            tensioner_control_time = millis();
        }
        else if (diam_error > 2) {
            // Speed up
            tensioner_speed += 1;
            tensioner_control_time = millis();
        }
    }

    return tensioner_speed;
}