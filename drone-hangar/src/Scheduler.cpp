#include "Scheduler.h"
#include <Arduino.h>

void Scheduler::init(int basePeriod){
  this->basePeriod = basePeriod;
  nTasks = 0;
  lastScheduleTime = 0;
}

bool Scheduler::addTask(Task* task){
  if (nTasks < MAX_TASKS-1){
    taskList[nTasks] = task;
    nTasks++;
    return true;
  } else {
    return false; 
  }
}

void Scheduler::schedule(){
  unsigned long now = millis();
  if (now - lastScheduleTime >= (unsigned long)basePeriod) {
    lastScheduleTime = now;

    for (int i = 0; i < nTasks; i++){
      if (taskList[i]->isActive()){
        if (taskList[i]->updateAndCheckTime(basePeriod)){
          taskList[i]->tick();
        }
      }
    }
  }
}
