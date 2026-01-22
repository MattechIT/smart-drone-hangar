#include "HangarControllerTask.h"
#include "../Config.h"
#include "TemperatureMonitorTask.h"
#include "Arduino.h"

HangarControllerTask::HangarControllerTask(Led* l1, Led* l2, Lcd* lcd, HangarDoorTask* doorTask, Sonar* sonar, AbstractPir* pir, MsgServiceClass* msgService, TemperatureMonitorTask* tempTask) {
  this->l1 = l1;
  this->l2 = l2;
  this->lcd = lcd;
  this->doorTask = doorTask;
  this->sonar = sonar;
  this->pir = pir;
  this->msgService = msgService;
  this->tempTask = tempTask;
}

void HangarControllerTask::init(int period) {
  Task::init(period);
  previousState = IDLE;
  enterState(IDLE);
}

void HangarControllerTask::tick() {
  sonar->trigger();
  sonar->update();
  float currentDist = sonar->getDistance();
  
  TempState tempState = tempTask->getState();
  if (tempState == ALARM && currentState != ALARMED) {
    if (currentState == DRONE_OUT) {
      msgService->enqueueToSend(ALARM_MSG);
    }
    previousState = currentState;
    currentState = ALARMED;
    msgService->lastMsg = "";
    return;
  }

  switch (currentState) {
        case IDLE:
          if (tempState != PRE_ALARM && msgService->lastMsg == TAKE_OFF_MSG) {
            enterState(TAKE_OFF);
            msgService->lastMsg = ""; 
          }
          break;
        
        case TAKE_OFF:
          if (millis() - blinkLedTime >= L2_BLINK_PERIOD) {
            blinkLedTime = millis();
            l2->switchStatus();
          }

          if (!isnan(currentDist) && currentDist >= 0) {
            msgService->enqueueToSend("DIST:"+String(currentDist));
          }
    
          if (!isnan(currentDist) && currentDist >= 0 && currentDist > D1) {
            if (detectionStartTime == 0) {
              detectionStartTime = millis();
            } else if (millis() - detectionStartTime >= T1) {
              enterState(DRONE_OUT);
              msgService->enqueueToSend(DRONE_OUT_MSG);
            }
          } else {
            detectionStartTime = 0;
          }
          break;
    
        case DRONE_OUT:
          if (tempState != PRE_ALARM && msgService->lastMsg == LANDING_MSG) {
            if (pir->isDetected()) { 
              enterState(LANDING);
              msgService->lastMsg = ""; 
            }
          }
          break;

    case LANDING:
      if (millis() - blinkLedTime >= L2_BLINK_PERIOD) {
        blinkLedTime = millis();
        l2->switchStatus();
      }

      if (!isnan(currentDist) && currentDist >= 0) {
            msgService->enqueueToSend("DIST:"+String(currentDist));
          }
      if (!isnan(currentDist) && currentDist >= 0 && currentDist < D2) {
        if (detectionStartTime == 0) {
          detectionStartTime = millis();
        } else if (millis() - detectionStartTime >= T2) {
          enterState(IDLE);
          msgService->enqueueToSend(DRONE_INSIDE_MSG);
        }
      }
      else {
        detectionStartTime = 0; 
      }
      break;

    case ALARMED:
      if (tempState != ALARM) {
        msgService->enqueueToSend(NORMAL_MSG);
        enterState(previousState);
      }
      break;
  }
}

void HangarControllerTask::enterState(State state) {
  currentState = state;
  detectionStartTime = 0;
  blinkLedTime = millis();

  switch (state) {
    case IDLE:
      l1->turnOn();
      l2->turnOff();
      lcd->print(DRONE_INSIDE_MSG);
      doorTask->close();
      break;

    case TAKE_OFF:
      l1->turnOff();
      l2->turnOn();
      lcd->print(TAKE_OFF_MSG);
      doorTask->open();
      break;

    case DRONE_OUT:
      l1->turnOff();
      l2->turnOff();
      lcd->print(DRONE_OUT_MSG);
      doorTask->close();
      break;

    case LANDING:
      l1->turnOff();
      l2->turnOn();
      lcd->print(LANDING_MSG);
      doorTask->open();
      break;

    case ALARMED:
      // State handled by temperature task
      break;
  }
}