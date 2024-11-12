#include "pinMap.h"
#include "userInterface.h"
#include "control.h"
#include "parameters.h"
#include "measurement.h"

#define LOG_DATA true

userInterface UI;
control Control;
measurement Meas;

Parameter tens_speed(200, "Tensioner Speed", "steps/s", 0);
Parameter feeder_speed(0, "Feeder Speed", "steps/s", 0);
Parameter spool_speed(200, "Spooler Speed", "steps/s", 0);
Parameter fans_on(5, "Fans On", "/5", 0);

Parameter meas_diam(0, "Meas. Diam.", "mm", -2);
Parameter goal_diam(175, "Goal Diameter", "mm", -2);

Parameter autoParams[4] = {
    tens_speed,      // 0
    spool_speed,     // 1
    fans_on,         // 2
    feeder_speed,    // 3
};

Parameter manualParams[4] = {
    tens_speed,      // 0
    spool_speed,     // 1
    fans_on,         // 2
    feeder_speed,    // 3
};

int i = 0;
unsigned long lastMillis = 0;
unsigned long lastMillisLong = 0;


void setup(){
    Serial.begin(9600);

    UI.displayInit();
    Control.motorsInit();

    Meas.zeroCaliper();
    // Meas.caliperInit();
    
    Control.powerFans(0);
    Control.feederHome();

    UI.updateDisplay = true;
}


void loop(){

    if ((millis() - lastMillis) > 200)
    {
        // Only update every 200ms
        lastMillis = millis();

        if (UI.selectPressed){
            UI.selectPressed = false;
            UI.displayMode = EDIT;
            UI.updateDisplay = true;
            // Update variable for parameter selection
            i++;
            if (i > 2) {i = 0;}
            // Reset dial
            UI.resetDial();
        }

        if (UI.enterPressed){
            UI.enterPressed = false;
            UI.displayMode = OVERVIEW;
            UI.updateDisplay = true;
            i--;
        }

        if (UI.mode != UI.readMode()) {
            UI.mode = UI.readMode();
            UI.updateDisplay = true;
            UI.displayMode = OVERVIEW;
            // if (UI.mode != OFF){
            //     Control.motorsOn();
            // }
        }   
    }

    
    if ((millis() - lastMillisLong) > 1000) {
        lastMillisLong = millis();

        if (UI.mode != OFF) { 
            Meas.readCaliper();
            if (meas_diam.value != Meas.caliperValue) {
                meas_diam.value = Meas.caliperValue;
                UI.updateMeasDiameter(meas_diam);
                // Serial.println(meas_diam.value);
            }
        }

        if (LOG_DATA) {
            Serial.print(autoParams[0].value);
            Serial.print(",");
            Serial.print(autoParams[1].value);
            Serial.print(",");
            Serial.print(meas_diam.value);
            // Serial.print(",\t");
            // Serial.println(String(meas_diam.value, BIN));
            Serial.println();
        }
    }

    

    switch (UI.mode)
    {
    case OFF:
        Control.powerFans(0);   // Turn fans off
        // Control.motorsOff();
        UI.offDisplay();
        // Don't move motors
        break;
    
    case AUTO:
        if (UI.lastControlMode != AUTO) {
            UI.lastControlMode = AUTO;
            // If mode changed, reset params to manual params
            for (int i = 0; i < 4; i++) {
                autoParams[i].value = manualParams[i].value;
            }
        }

        if (UI.displayMode == EDIT) {
            goal_diam = UI.updateParameter(goal_diam);
        }
        if (UI.displayMode == OVERVIEW) {
            UI.overviewDisplay(autoParams, meas_diam);
        }

        Control.autoControl(autoParams, meas_diam, goal_diam);
        Control.setParams(autoParams);
        Control.moveMotors();

        break;

    case MANUAL:
        if (UI.lastControlMode != MANUAL) {
            UI.lastControlMode = MANUAL;
            // If mode changed, reset params to auto params
            for (int i = 0; i < 4; i++) {
                manualParams[i].value = autoParams[i].value;
            }
        }

        if (UI.displayMode == EDIT){
            manualParams[i] = UI.updateParameter(manualParams[i]);
        }
        if (UI.displayMode == OVERVIEW){
            UI.overviewDisplay(manualParams, meas_diam);
        }

        Control.setParams(manualParams);
        Control.moveMotors();

        break;

    default:
        // do nothing
        break;
    }
    
}