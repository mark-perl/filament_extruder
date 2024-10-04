#pragma once
#include "pinMap.h"

#define OFF 0
#define AUTO 1
#define MANUAL 2

class userInterface
{
public:
    userInterface();
    ~userInterface();

    // User Inputs
    static void buttonPressedInterrupt();
    int readMode();

    // LCD Display
    void editParameter(int variable);
    void displayOverview(int mode);

    static volatile bool selectPressed;
    static volatile bool enterPressed;
    static volatile bool limitPressed;
    
    int mode = OFF;

private:


};