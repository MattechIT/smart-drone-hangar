#ifndef __TEMP_SENSOR__
#define __TEMP_SENSOR__

class TempSensor {
public:
    TempSensor(int pin);

    /* Update the misuration */
    bool update();

    /* @return the last measured temperature */
    float getTemperature();

private:
    int pin;
    float lastTemperature;
    unsigned long lastUpdateTime;
    float prevTemperature;
    int spikeCount = 0;
};

#endif