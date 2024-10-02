#include "userInterface.h"

userInterface::userInterface()
{
    pinMode(SW_Select, INPUT);
    pinMode(SW_Enter, INPUT);
    pinMode(SW_Manual, INPUT);
    pinMode(SW_Auto, INPUT);
    pinMode(Encoder_Dial_A, INPUT);
    pinMode(Encoder_Dial_B, INPUT);
}

userInterface::~userInterface()
{

}

bool userInterface::readSelectButton()
{
    if (digitalRead(SW_Select)) {
        if (!selectPressed) {
            selectPressed = true;
            return true;
        }
    }
    else {
        selectPressed = false;
        return false;
    }
}

bool userInterface::readEnterButton()
{
    if (digitalRead(SW_Enter)) {
        if (!enterPressed) {
            enterPressed = true;
            return true;
        }
    }
    else {
        enterPressed = false;
        return false;
    }
}

bool userInterface::readLimitSwitch()
{
    if (digitalRead(SW_Limit)) {
        if (!limitPressed) {
            limitPressed = true;
            return true;
        }
    }
    else {
        limitPressed = false;
        return false;
    }
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