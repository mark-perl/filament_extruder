#include "userInterface.h"

RotaryEncoder dial(Encoder_Dial_A, Encoder_Dial_B, RotaryEncoder::LatchMode::FOUR3);
LiquidCrystal_I2C display(0x27, 4, 20);

volatile bool userInterface::selectPressed = false;
volatile bool userInterface::enterPressed = false;
volatile int userInterface::dialValue = 0;
unsigned long userInterface::buttonPressedMillis = 0;

byte diamSymbol[8] = {
  0b00001,
  0b01110,
  0b10011,
  0b10101,
  0b11001,
  0b01110,
  0b10000,
  0b00000
};

userInterface::userInterface()
{ 
    pinMode(SW_Select, INPUT);
    pinMode(SW_Enter, INPUT);
    pinMode(SW_Mode, INPUT);

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
    display.setCursor(0,0);
    display.print("Filament Extruder");
    // display.setCursor(0,1);
    // display.print("Setup:");
    display.setCursor(0,2);
    display.print("Feeder Homing");
    display.setCursor(3,3);
    display.print("Please Wait...");

    display.createChar(0, diamSymbol);

}

void userInterface::overviewDisplay(Parameter params[], Parameter meas_diam)
{
    if (updateDisplay) {
        updateValues = true;
        updateDisplay = false;

        display.clear();
        display.home();

        if (mode==AUTO){display.print("AUTO");}
        if (mode==MANUAL){display.print("MANUAL");}

        display.setCursor(11,0);
        // display.print(meas_diam.name + ": ");
        display.write(0);   // diameter symbol
        display.print(": ");
        updateMeasDiameter(meas_diam);
        display.print(meas_diam.units);

        // for (int i = 0; i < 3; i++) {
        //     display.setCursor(0,i+1);
        //     display.print(params[i].name + ": ");
        //     display.print(intToString(params[i].value, params[i].scaler));
        // }

        display.setCursor(0,1);
        display.print("Tensio. Speed: ");
        display.setCursor(0,2);
        display.print("Spooler Speed: ");
        display.setCursor(0,3);
        display.print("Fans on: ");
    }

    if (updateValues) {
        updateValues = false;

        display.setCursor(15,1);
        display.print(intToString(params[0].value, params[0].scaler));
        display.print(" ");     // Print blank character after to override possible digit
        display.setCursor(15,2);
        display.print(intToString(params[1].value, params[1].scaler));
        display.print(" ");     // Print blank character after to override possible digit
        display.setCursor(9,3);
        display.print(intToString(params[2].value, params[2].scaler));
    }
}

void userInterface::offDisplay()
{
    if (updateDisplay) {
        updateDisplay = false;

        display.clear();
        display.setCursor(0,1);
        display.print("OFF");
    }
}

void userInterface::updateMeasDiameter(Parameter meas_diam)
{
    display.setCursor(14,0);    // Always location of diam.

    valueString = intToString(meas_diam.value, meas_diam.scaler);
    // Only allow 'x.xx' length results
    if (valueString.length() != 4) {
        display.print("ERR ");
    }
    else {
        display.print(intToString(meas_diam.value, meas_diam.scaler));
    }
}


Parameter userInterface::updateParameter(Parameter param)
{ 
    if (updateDisplay) {
        // Update whole display
        updateDisplay = false;
        
        display.clear();
        display.setCursor(0,0);
        display.print("EDIT");

        display.setCursor(0,2);
        display.print(param.name);

        display.setCursor(0,3);
        display.print(intToString(param.value, param.scaler));
        display.print(" " + param.units);
    }

    if (updateDisplayValue) {
        // Update only display value
        updateDisplayValue = false;

        display.setCursor(0,3);
        display.print(intToString(param.value, param.scaler));
        display.print(" " + param.units + "  ");
    }

    if (dialValue != 0){
        // Update value from dial input
        param.value += dialValue;
        // Reset dial to 0
        resetDial();
        // Prompt to update display value only
        updateDisplayValue = true;   
    }

    return param;
}

void userInterface::encoderInterrupt()
{
    dial.tick();
    dialValue = dial.getPosition();
}

void userInterface::buttonPressedInterrupt()
{
    if ((millis() - buttonPressedMillis) > 300){
        if (digitalRead(SW_Select)) {selectPressed = true;}
        if (digitalRead(SW_Enter))  {enterPressed = true;}
        buttonPressedMillis = millis();
    }
}

int userInterface::readMode()
{   
    // SW_Mode uses one pin with voltage divider
    // Manual = 5V (1024), Auto = 2.5V (512), Off = 0V (0)

    if (analogRead(SW_Mode) > 768) {
        // Serial.println("Mode: Auto");
        return MANUAL;
    }
    else if (analogRead(SW_Mode) > 256) {
        // Serial.println("Mode: Manual");
        return AUTO;
    }
    else {
        // Serial.println("Mode: Off");
        return OFF;
    }
}

String userInterface::intToString(int value, int scaler)
{
    valueString = String(value);
    stringLen = valueString.length();

    if (scaler == 0) {
        return valueString;
    }
    else if (scaler < 0) {
        // Add decimal place
        if (stringLen <= -scaler) {
            // Add leading 0s
            for (int i = 0; i < (-scaler + 1 - stringLen); i++) {
                valueString = "0" + valueString;
            }
            stringLen = valueString.length();
        }
        decimalLoc = valueString.length() + scaler;
        return valueString.substring(0,decimalLoc) + '.' + valueString.substring(decimalLoc, stringLen);
    }
    else {
        // Add trailing 0s
        for (int i = 0; i < (scaler + 1 - stringLen); i++) {
            valueString.concat("0");
        }
        return valueString;
    }
}

void userInterface::resetDial()
{
    dial.setPosition(0);
    dialValue = 0;
}