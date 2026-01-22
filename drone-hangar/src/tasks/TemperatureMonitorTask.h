#ifndef __TEMPERATURE_MONITOR_TASK__
#define __TEMPERATURE_MONITOR_TASK__

#include "../Task.h"
#include "../components/TempSensor.h"
#include "../components/Light.h"
#include "../components/Lcd.h"
#include "HangarDoorTask.h"
#include "../components/Button.h"
#include "../MsgService.h"

enum TempState {
  NORMAL,
  PRE_ALARM,
  ALARM
};

class TemperatureMonitorTask : public Task, public ButtonListener {

public:
  TemperatureMonitorTask(TempSensor* tempSensor, Light* l3, Lcd* lcd, HangarDoorTask* doorTask, AbstractButton* resetButton, MsgServiceClass* msgService);
  void init(int period);
  void tick();
  TempState getState();
  void notifyButtonPressed();
  void notifyButtonReleased();

private:
  TempSensor* tempSensor;
  Light* l3;
  Lcd* lcd;
  HangarDoorTask* doorTask;
  AbstractButton* resetButton;
  MsgServiceClass* msgService;
  volatile TempState currentState;
  
  unsigned long temp1StartTime;
  unsigned long temp2StartTime;
};

#endif
