#ifndef __PIR__
#define __PIR__

#include "Event.h"

#define MAX_PIR_LISTENERS 2

/*
 * Interface
 */
class PirListener {
public:
  virtual void notifyMotionDetected() = 0;
  virtual void notifyMotionStopped() = 0;
};


/*
 * Abstract Pir Class
 * Inherit from EventSource in order to be able of register interrupt
 */
class AbstractPir : public EventSource {
public:
  virtual bool isDetected() = 0;

  bool addListener(PirListener* listener) {
    if (nListeners < MAX_PIR_LISTENERS){
      listeners[nListeners++] = listener;
      return true;
    } else {
      return false;
    }
  }

protected:
  AbstractPir(){
    nListeners = 0;   
  }
  
  int nListeners;  
  PirListener* listeners[MAX_PIR_LISTENERS];
};

#endif