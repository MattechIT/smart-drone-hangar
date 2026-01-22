#include "TemperatureMonitorTask.h"
#include "../Config.h"
#include "Arduino.h"

TemperatureMonitorTask::TemperatureMonitorTask(TempSensor* tempSensor, Light* l3, Lcd* lcd, HangarDoorTask* doorTask, AbstractButton* resetButton, MsgServiceClass* msgService) {
  this->tempSensor = tempSensor;
  this->l3 = l3;
  this->lcd = lcd;
  this->doorTask = doorTask;
  this->resetButton = resetButton;
  this->msgService = msgService;
}

void TemperatureMonitorTask::init(int period) {
  Task::init(period);
  currentState = NORMAL;
  l3->turnOff();
  temp1StartTime = 0;
  temp2StartTime = 0;
  resetButton->addListener(this);
}

void TemperatureMonitorTask::notifyButtonPressed() {
  if (currentState == ALARM) {
    currentState = NORMAL;
    l3->turnOff();
    temp1StartTime = 0;
    temp2StartTime = 0;
    resetButton->disable();
  }
}

void TemperatureMonitorTask::notifyButtonReleased() {
}

TempState TemperatureMonitorTask::getState() {
  return currentState;
}

void TemperatureMonitorTask::tick() {
  tempSensor->update();
  double currentTemp = tempSensor->getTemperature();
  msgService->enqueueToSend("TEMP:"+String(currentTemp));

  // Manage Timers for Thresholds
  if (currentTemp >= TEMP1) {
    if (temp1StartTime == 0) temp1StartTime = millis();
  } else {
    temp1StartTime = 0;
  }

  if (currentTemp >= TEMP2) {
    if (temp2StartTime == 0) temp2StartTime = millis();
  } else {
    temp2StartTime = 0;
  }

  switch (currentState) {
    case NORMAL:
      // Priority Check: ALARM (Temp2 for T4)
      if (temp2StartTime > 0 && (millis() - temp2StartTime >= T4)) {
        currentState = ALARM;
        l3->turnOn();
        doorTask->close();
        lcd->print(ALARM_MSG);
        msgService->enqueueToSend(ALARM_MSG);
        resetButton->enable();
      }
      // Check: PRE_ALARM (Temp1 for T3)
      else if (temp1StartTime > 0 && (millis() - temp1StartTime >= T3)) {
        currentState = PRE_ALARM;
        msgService->enqueueToSend(PRE_ALARM_MSG);
      }
      break;

    case PRE_ALARM:
      // Check: Upgrade to ALARM (Temp2 for T4)
      if (temp2StartTime > 0 && (millis() - temp2StartTime >= T4)) {
        currentState = ALARM;
        l3->turnOn();
        doorTask->close();
        lcd->print(ALARM_MSG);
        msgService->enqueueToSend(ALARM_MSG);
        resetButton->enable();
      }
      // Check: Return to NORMAL
      else if (currentTemp < TEMP1) {
        currentState = NORMAL;
        msgService->enqueueToSend(NORMAL_MSG);
      }
      break;
      
    case ALARM:
      break;
  }
}