#include "HangarDoor.h"
#include "Arduino.h"

HangarDoor::HangarDoor(int pin) {
    this->pin = pin;
    this->currentAngle = POS_CLOSE; 
}

void HangarDoor::setPosition(int angle) {
    if (!servo.attached()) {
        servo.attach(pin);
    }
    servo.write(angle);
    currentAngle = angle;
}

bool HangarDoor::isOpen() {
    return currentAngle >= POS_OPEN;
}