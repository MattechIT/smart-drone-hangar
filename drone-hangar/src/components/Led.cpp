#include "Led.h"
#include <Arduino.h>

Led::Led(int p){
  this->pin = p;
  pinMode(pin, OUTPUT);
  this->status = false;
  digitalWrite(pin, LOW);
}

bool Led::isOn(){
  return this->status; 
}

void Led::turnOn() {
  digitalWrite(pin, HIGH);
  this->status=true;
}

void Led::turnOff() {
  digitalWrite(pin, LOW);
  this->status=false;
}

void Led::switchStatus() {
  if (this->isOn())
    this->turnOff();
  else
    this->turnOn();
}

void Led::startBlink(unsigned long period) {
  this->blinking = true;
  this->blinkPeriod = period;
  this->lastToggleTime = millis();
}

void Led::stopBlink() {
  this->blinking = false;
  this->turnOff();
}

void Led::updateBlink() {
  if (this->blinking) {
    unsigned long currentTime = millis();
    if (currentTime - this->lastToggleTime >= this->blinkPeriod) {
      this->switchStatus();
      this->lastToggleTime = currentTime;
    }
  }
}
