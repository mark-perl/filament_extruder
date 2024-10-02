#include "pinMap.h"

class userInterface
{
private:
    bool selectPressed = false;
    bool enterPressed = false;
    bool limitPressed = false;


public:
    userInterface();
    ~userInterface();

    bool readSelectButton();
    bool readEnterButton();
    bool readLimitSwitch();
    int readMode();

    
};

#define OFF 0
#define AUTO 1
#define MANUAL 2