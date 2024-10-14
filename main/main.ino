#include "pinMap.h"
#include "userInterface.h"
#include "control.h"
// #include "parameters.h"

userInterface UI;
control Control;

Parameter tens_speed(50.0f, "Tensioner Speed", "steps/s", 0);
Parameter spool_speed(50.0f, "Spooler Speed", "steps/s", 1);
Parameter feeder_speed(0.0f, "Feeder Speed", "steps/s", 2);
Parameter fans_on(5, "Fans On", "/5", 3);

Parameter autoParams[4] = {
    tens_speed,
    spool_speed,
    feeder_speed,
    fans_on,
};
Parameter manualParams[4] = {
    tens_speed,
    spool_speed,
    feeder_speed,
    fans_on,
};

int i = 0;   

void setup(){
    Serial.begin(9600);
    UI.displayInit();
    
}


void loop(){

    if (UI.selectPressed){
        UI.selectPressed = false;
        // perform select actions
        Serial.println("select pressed");
        i++;
        if (i>4){i=0;}
        // UI.updateDisplay("select pressed");
    }

    if (UI.enterPressed){
        UI.enterPressed = false;
        // perform enter actions
        Serial.println("enter pressed");
        // UI.updateDisplay("enter pressed");
    }

    UI.mode = UI.readMode();

    switch (UI.mode)
    {
    case OFF:
        // turn off
        // dont send move commands
        break;
    
    case AUTO:
        // control system to set autoParams
        // send move commands at autoParams values
        break;

    case MANUAL:
        // send move commands at manualParamas values
        manualParams[i] = UI.updateParameter(manualParams[i]);
        Control.move(manualParams);
        
        break;

    default:
        // do nothing
        break;
    }
    
}