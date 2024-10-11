#include "userInterface.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RotaryEncoder.h>

RotaryEncoder dial(Encoder_Dial_A, Encoder_Dial_B, RotaryEncoder::LatchMode::FOUR3);
LiquidCrystal_I2C display(0x27, 4, 20);

volatile bool userInterface::selectPressed = false;
volatile bool userInterface::enterPressed = false;
volatile int userInterface::dialValue = 0;
// volatile bool userInterface::limitPressed = false;

userInterface::userInterface()
{
    pinMode(SW_Select, INPUT);
    pinMode(SW_Enter, INPUT);
    pinMode(SW_Manual, INPUT);
    pinMode(SW_Auto, INPUT);

    attachInterrupt(digitalPinToInterrupt(Encoder_Dial_A), encoderInterrupt, CHANGE);
    attachInterrupt(digitalPinToInterrupt(Encoder_Dial_B), encoderInterrupt, CHANGE);
    
    attachInterrupt(digitalPinToInterrupt(SW_Select), buttonPressedInterrupt, RISING);
    attachInterrupt(digitalPinToInterrupt(SW_Enter), buttonPressedInterrupt, RISING);
}

userInterface::~userInterface()
{

}

void userInterface::displayInit()
{
    display.init();
    display.backlight(); 
    display.clear();
    display.setCursor(1,1);
    display.print("Hello");
}

void userInterface::updateDisplay(String text)
{
    Serial.print(text);
    display.clear();
    display.setCursor(0,0);
    display.print(text);
}

void userInterface::encoderInterrupt()
{
    dial.tick();
    dialValue = dial.getPosition();
}

void userInterface::buttonPressedInterrupt()
{
    if (digitalRead(SW_Select)) {selectPressed = true;}
    if (digitalRead(SW_Enter))  {enterPressed = true;}
}

int userInterface::readMode()
{
    if (digitalRead(SW_Auto)) {
        // Serial.println("Mode: Auto");
        return AUTO;
    }
    else if (digitalRead(SW_Manual)) {
        // Serial.println("Mode: Manual");
        return MANUAL;
    }
    else {
        // Serial.println("Mode: Off");
        return OFF;
    }
}

