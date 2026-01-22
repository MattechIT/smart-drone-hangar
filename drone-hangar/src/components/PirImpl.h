#ifndef __PIRIMPL__
#define __PIRIMPL__

#include "Pir.h"

class PirImpl: public AbstractPir { 
public: 
  PirImpl(int pin);
  bool isDetected();
  
  void notifyInterrupt(int pin) override;

private:
  int pin;
};

#endif