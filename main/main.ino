#include "pinMap.h"
#include "userInterface.h"
#include "parameters.h"

userInterface UI;
parameters manualParams;
parameters autoParams;

void setup(){
    Serial.begin(9600);

}


void loop(){

    if (UI.selectPressed){
        UI.selectPressed = 0;
        // perform select actions
    }

    if (UI.enterPressed){
        UI.enterPressed = 0;
        // perform enter actions
    }

    UI.mode = UI.readMode();

    switch (UI.mode)
    {
    case OFF:
        
        break;
    
    case AUTO:

        break;

    case MANUAL:

        break;

    default:
        // do nothing
        break;
    }
    







}