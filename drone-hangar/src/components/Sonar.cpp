#include "Sonar.h"
#include <Arduino.h>

Sonar::Sonar(int triggerPin, int echoPin) {
	this->triggerPin = triggerPin;
	this->echoPin = echoPin;
    this->temperature = 20.0f;
    this->state = IDLE;
    this->lastDistance = -1;
	pinMode(triggerPin, OUTPUT);
	pinMode(echoPin, INPUT);
}

void Sonar::setTemperature(float temp){
    this->temperature=temp;
}

void Sonar::trigger(){
    if(state == IDLE){
        digitalWrite(this->triggerPin,HIGH);
        delayMicroseconds(10);
        digitalWrite(this->triggerPin,LOW);
        state = WAITING_FOR_ECHO;
        lastTriggerTime = micros();
    }
}

void Sonar::update(){
    if(state == WAITING_FOR_ECHO){
        unsigned long duration = pulseIn(echoPin, HIGH, 30000);
        if (duration > 0) {
            float vs_local = 331.45f + 0.62f * this->temperature;
            float t_s = (float)duration * 1e-6f / 2.0f;
            lastDistance = (t_s * vs_local) * 100.0f;
            state = IDLE;
        } else if (micros() - lastTriggerTime > 30000) {
            lastDistance = NAN;
            state = IDLE;
        }
    }
}

float Sonar::getDistance(){
    float dist = this->lastDistance;
    this->lastDistance = -1;
    return dist;
}

bool Sonar::isAvailable(){
    return (state == IDLE && lastDistance != -1);
}
