#ifndef __BUTTONIMPL__
#define __BUTTONIMPL__

#include "Button.h"

class ButtonImpl: public AbstractButton { 
public: 
  ButtonImpl(int pin);
  bool isPressed();
  void enable() override;
  void disable() override;
  
protected:
  void notifyInterrupt(int pin) override;

private:
  int pin;
  int lastEventTime;
};

#endif
