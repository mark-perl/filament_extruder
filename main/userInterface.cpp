#include "userInterface.h"

volatile bool userInterface::selectPressed = false;
volatile bool userInterface::enterPressed = false;
// volatile bool userInterface::limitPressed = false;

userInterface::userInterface()
{
    pinMode(SW_Select, INPUT);
    pinMode(SW_Enter, INPUT);
    pinMode(SW_Manual, INPUT);
    pinMode(SW_Auto, INPUT);
    pinMode(Encoder_Dial_A, INPUT);
    pinMode(Encoder_Dial_B, INPUT);

    attachInterrupt(digitalPinToInterrupt(SW_Select), buttonPressedInterrupt, RISING);
    attachInterrupt(digitalPinToInterrupt(SW_Enter), buttonPressedInterrupt, RISING);
    // attachInterrupt(digitalPinToInterrupt(SW_Limit), buttonPressed, RISING);
}

userInterface::~userInterface()
{

}

void userInterface::buttonPressedInterrupt()
{
    if (digitalRead(SW_Select)) {selectPressed = true;}
    if (digitalRead(SW_Enter))  {enterPressed = true;}
    // if (digitalRead(SW_Limit))  {limitPressed = true;}
}

int userInterface::readMode()
{
    if (digitalRead(SW_Auto)) {
        return AUTO;
    }
    else if (digitalRead(SW_Manual)) {
        return MANUAL;
    }
    else {
        return OFF;
    }
}