#include "pinMap.h"
#include "userInterface.h"
#include "parameters.h"

userInterface UI;
parameters manualParams;
parameters autoParams;

void setup(){
    Serial.begin(9600);

    // set default manual params
    manualParams.spool_speed = 100;
    manualParams.feeder_speed = 100;
    manualParams.tensioner_speed = 100;
    manualParams.fans_on = 5;

    // set default auto params
    autoParams.spool_speed = 100;
    autoParams.feeder_speed = 100;
    autoParams.tensioner_speed = 100;
    autoParams.fans_on = 5;
    
}


void loop(){

    if (UI.selectPressed){
        UI.selectPressed = false;
        // perform select actions
        Serial.println("select pressed");
        // UI.updateDisplay("select pressed");
    }

    if (UI.enterPressed){
        UI.enterPressed = false;
        // perform enter actions
        Serial.println("enter pressed");
        // UI.updateDisplay("enter pressed");
    }

    UI.mode = UI.readMode();
    delay(1000);

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
        break;

    default:
        // do nothing
        break;
    }
    
}