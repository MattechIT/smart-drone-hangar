#ifndef __LED__
#define __LED__

#include "Light.h"

class Led : public Light {
  private:
    bool blinking;
    unsigned long blinkPeriod;
    unsigned long lastToggleTime;
  protected:
    int pin;
    bool status;
  public:
    Led(int pin);

    void turnOn() override;
    void turnOff() override;
    bool isOn();
    void switchStatus();
    void startBlink(unsigned long period);
    void stopBlink();
    void updateBlink();  
};

#endif