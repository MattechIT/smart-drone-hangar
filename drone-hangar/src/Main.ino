#include <Arduino.h>
#include "Config.h"
#include "Scheduler.h"
#include "MsgService.h"

#include "components/ButtonImpl.h"
#include "components/Led.h"
#include "components/Sonar.h"
#include "components/PirImpl.h"
#include "components/TempSensor.h"
#include "components/HangarDoor.h"
#include "components/Lcd.h"

#include "tasks/HangarControllerTask.h"
#include "tasks/TemperatureMonitorTask.h"
#include "tasks/MsgTask.h"
#include "tasks/HangarDoorTask.h"

Scheduler scheduler;

Led l1(L1_PIN);
Led l2(L2_PIN);
Led l3(L3_PIN);
ButtonImpl resetButton(RESET_BUTTON_PIN);
Sonar sonar(SONAR_TRIG_PIN, SONAR_ECHO_PIN);
PirImpl pir(PIR_PIN);
TempSensor tempSensor(TEMP_PIN);
HangarDoor door(SERVO_PIN);
Lcd display(LCD_ADDR, LCD_COLS, LCD_ROWS);

MsgTask msgTask;
HangarDoorTask doorTask(&door);
TemperatureMonitorTask tempTask(&tempSensor, &l3, &display, &doorTask, &resetButton, &MsgService);
HangarControllerTask hangarTask(&l1, &l2, &display, &doorTask, &sonar, &pir, &MsgService, &tempTask);

void setup(){
  MsgService.init();
  display.init();

  scheduler.init(SCHEDULER_PERIOD);

  msgTask.init(SCHEDULER_PERIOD);
  scheduler.addTask(&msgTask);

  doorTask.init(SCHEDULER_PERIOD);
  scheduler.addTask(&doorTask);

  tempTask.init(TEMP_SAMPLING_PERIOD);
  scheduler.addTask(&tempTask);

  hangarTask.init(SONAR_SAMPLING_PERIOD);
  scheduler.addTask(&hangarTask);
}

void loop(){
  scheduler.schedule();
}