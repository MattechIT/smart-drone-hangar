#ifndef __HANGAR_CONTROLLER_TASK__
#define __HANGAR_CONTROLLER_TASK__

#include "../Task.h"
#include "../components/Led.h"
#include "../components/Lcd.h"
#include "HangarDoorTask.h"
#include "../components/Sonar.h"
#include "../components/Pir.h"
#include "../MsgService.h"

// Avoid circular import
class TemperatureMonitorTask; 

class HangarControllerTask : public Task {

enum State {
  IDLE, 
  TAKE_OFF, 
  DRONE_OUT,
  LANDING,
  ALARMED
};

public:
  HangarControllerTask(Led* l1, Led* l2, Lcd* lcd, HangarDoorTask* doorTask, Sonar* sonar, AbstractPir* pir, MsgServiceClass* msgService, TemperatureMonitorTask* tempTask);
  void init(int period);
  void tick();

private:
  Led* l1;
  Led* l2;
  Lcd* lcd;
  HangarDoorTask* doorTask;
  Sonar* sonar;
  AbstractPir* pir;
  MsgServiceClass* msgService;
  TemperatureMonitorTask* tempTask;
  State currentState;
  State previousState;
  unsigned long detectionStartTime;
  unsigned long blinkLedTime;
  bool motionDetected = false;
  
  void enterState(State state);
};

#endif