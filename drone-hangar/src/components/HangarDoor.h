#ifndef __HANGAR_DOOR__
#define __HANGAR_DOOR__

#include <Servo.h>

class HangarDoor {
public:
    HangarDoor(int pin);
    void setPosition(int angle);
    bool isOpen();
    
    static const int POS_OPEN = 95;
    static const int POS_CLOSE = 5;

private:
    Servo servo;
    int pin;
    int currentAngle;
};

#endif