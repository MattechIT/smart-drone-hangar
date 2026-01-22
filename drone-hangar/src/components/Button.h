#ifndef __BUTTON__
#define __BUTTON__

#include "Event.h"

#define MAX_BUTTON_LISTENERS 5

/*
 * Button interface
 */
class Button {
  public: 
    virtual bool isPressed() = 0;
    virtual void enable() {};
    virtual void disable() {};
};

/*
 * Interface of listeners that are notified 
 * when a button generate an event
 */
class ButtonListener {
  public:
    virtual void notifyButtonPressed() = 0;
    virtual void notifyButtonReleased() = 0;
};

/*
 * The base class for buttons generating events
 */
class AbstractButton : public EventSource, public Button {
public:

  /* to add a new listener */
  bool addListener(ButtonListener* listener) {
    if (nListeners < MAX_BUTTON_LISTENERS){
      listeners[nListeners++] = listener; 
      return true;   
    } else {
      return false; 
    }
  }

protected:
  AbstractButton(){
    nListeners = 0;   
  }
  
  int nListeners;  
  ButtonListener* listeners[MAX_BUTTON_LISTENERS];
};
#endif
