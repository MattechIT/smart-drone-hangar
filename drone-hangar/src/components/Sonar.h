#ifndef __SONAR__
#define __SONAR__

class Sonar{
    private:
        int triggerPin;
        int echoPin;
        float temperature;

        volatile enum { IDLE, WAITING_FOR_ECHO } state;
        volatile unsigned long echoStartTime;
        volatile float lastDistance;
        unsigned long lastTriggerTime;
    public:
        Sonar(int triggerPin, int echoPin);

        /* Set environment temperature @param temp local temperature */
        void setTemperature(float temp);

        /* Start new misuration */
        void trigger(); 

        /* Update the misuration */
        void update();

        /* @return the last measured distance */
        float getDistance();

        /* @return if new misuration is available */
        bool isAvailable();
};

#endif