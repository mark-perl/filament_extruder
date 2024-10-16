#include "pinMap.h"
#include "userInterface.h"
#include "control.h"
#include "parameters.h"

userInterface UI;
control Control;

Parameter tens_speed(50, "Tensioner Speed", "steps/s", 0);
Parameter feeder_speed(0, "Feeder Speed", "steps/s", 0);
Parameter spool_speed(50, "Spooler Speed", "steps/s", 0);
Parameter fans_on(0, "Fans On", "/3", 0);

Parameter goal_diam(175, "Goal Diameter", "mm", 2);

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
        UI.updateDisplay = true;
        // Update variable for parameter selection
        i++;
        if (i > 3) {i = 0;}
        // Reset dial
        UI.resetDial();
    }

    if (UI.enterPressed){
        UI.enterPressed = false;
        UI.displayMode = OVERVIEW;
        UI.updateDisplay = true;
    }

    if (UI.mode != UI.readMode()) {
        UI.mode = UI.readMode();
        UI.updateDisplay = true;
        UI.displayMode = OVERVIEW;
    }

    switch (UI.mode)
    {
    case OFF:
        Control.powerFans(0);   // Turn fans off
        UI.offDisplay();
        // Don't move motors
        break;
    
    case AUTO:
        if (UI.displayMode == EDIT){
            goal_diam = UI.updateParameter(goal_diam);
        }
        if (UI.displayMode == OVERVIEW){
            UI.overviewDisplay(autoParams);
        }

        // TODO: Update parameter from control system

        Control.setParams(autoParams);
        Control.moveMotors();
        break;

    case MANUAL:
        if (UI.displayMode == EDIT){
            manualParams[i] = UI.updateParameter(manualParams[i]);
        }
        if (UI.displayMode == OVERVIEW){
            UI.overviewDisplay(manualParams);
        }

        Control.setParams(manualParams);
        Control.moveMotors();
        break;

    default:
        // do nothing
        break;
    }
    
}