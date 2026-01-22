#include "TempSensor.h"
#include "Arduino.h"

// Read frequence
const unsigned long TEMP_READ_PERIOD = 2000;
const int NUM_READINGS = 5;

TempSensor::TempSensor(int pin){
    this->pin = pin;
    this->lastTemperature = 0.0;
    this->prevTemperature = 0.0;
    this->lastUpdateTime = 0;
}

bool TempSensor::update(){
    if (millis() - this->lastUpdateTime >= TEMP_READ_PERIOD) {
        this->lastUpdateTime = millis();

        long total = 0;
        for (int i = 0; i < NUM_READINGS; i++) {
            total += analogRead(this->pin);
            delay(2); 
        }
    
        float averageReading = total / (float)NUM_READINGS;
        float voltage = (averageReading / 1023.0) * 5000.0;
        float temperatureC = voltage / 10.0;

        float maxDelta = 5.0;
         if (this->prevTemperature != 0.0 && abs(temperatureC - this->prevTemperature) > maxDelta) {
            spikeCount++;
            if (spikeCount < 5) {
                temperatureC = this->prevTemperature;
            } else {
                spikeCount = 0;
            }
        } else {
            spikeCount = 0;
        }

        this->prevTemperature = temperatureC;
        this->lastTemperature = temperatureC;
        return true;
    }
    return false;
}

float TempSensor::getTemperature(){
    return this->lastTemperature;
}