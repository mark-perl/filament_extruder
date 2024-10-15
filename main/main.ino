#include "pinMap.h"
#include "userInterface.h"
#include "control.h"
#include "parameters.h"

userInterface UI;
control Control;

Parameter tens_speed(50.0f, "Tensioner Speed", "steps/s", 0);
Parameter feeder_speed(0.0f, "Feeder Speed", "steps/s", 1);
Parameter spool_speed(50.0f, "Spooler Speed", "steps/s", 2);
Parameter fans_on(0, "Fans On", "/5", 3);

Parameter autoParams[4] = {
    tens_speed,      // 0
    feeder_speed,    // 1
    spool_speed,     // 2
    fans_on,         // 3
};

Parameter manualParams[4] = {
    tens_speed,      // 0
    feeder_speed,    // 1
    spool_speed,     // 2
    fans_on,         // 3
};

int i = 0;


void setup(){
    Serial.begin(9600);
    UI.displayInit();
    Control.powerFans(0);
    Control.motorsInit();
}


void loop(){

    if (UI.selectPressed){
        UI.selectPressed = false;
        UI.displayMode = EDIT;
        // Update variable for parameter selection
        i++;
        if (i>4){i=0;}
    }

    if (UI.enterPressed){
        UI.enterPressed = false;
        UI.displayMode = OVERVIEW;


    }

    UI.mode = UI.readMode();    

    switch (UI.mode)
    {
    case OFF:
        // OFF: Don't move motors
        Control.powerFans(0);   // Turn fans off
        break;
    
    case AUTO:
    if (UI.displayMode == EDIT){
        autoParams[i] = UI.updateParameter(autoParams[i]);
    }
        Control.setParams(autoParams);

        Control.moveMotors();
        break;

    case MANUAL:
    if (UI.displayMode == EDIT){
        manualParams[i] = UI.updateParameter(manualParams[i]);
    }
        Control.setParams(manualParams);

        Control.moveMotors();
        break;

    default:
        // do nothing
        break;
    }
    
}