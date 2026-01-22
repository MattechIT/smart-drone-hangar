#include "PirImpl.h"
#include "Arduino.h"

PirImpl::PirImpl(int pin){
  this->pin = pin;
  pinMode(pin, INPUT);
} 

bool PirImpl::isDetected() {
  return digitalRead(this->pin) == HIGH;
}

/*
 * Function called when status change
 */
void PirImpl::notifyInterrupt(int pin){
  bool motionDetected = digitalRead(this->pin);

  // Notifiy
  if (motionDetected) {
    for (int i = 0; i < nListeners; i++){
      listeners[i]->notifyMotionDetected();
    }
  } else {
    for (int i = 0; i < nListeners; i++){
      listeners[i]->notifyMotionStopped();
    }
  }
}