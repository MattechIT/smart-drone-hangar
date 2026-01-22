#include "HangarDoorTask.h"
#include "Arduino.h"

HangarDoorTask::HangarDoorTask(HangarDoor* door) {
  this->door = door;
}

void HangarDoorTask::init(int period) {
  Task::init(period);
  currentAngle = HangarDoor::POS_CLOSE;
  targetAngle = HangarDoor::POS_CLOSE;
  door->setPosition(currentAngle);
}

void HangarDoorTask::tick() {
  if (currentAngle < targetAngle) {
    currentAngle += step;
    if (currentAngle > targetAngle) currentAngle = targetAngle;
    door->setPosition(currentAngle);
  } else if (currentAngle > targetAngle) {
    currentAngle -= step;
    if (currentAngle < targetAngle) currentAngle = targetAngle;
    door->setPosition(currentAngle);
  }
}

void HangarDoorTask::open() {
  targetAngle = HangarDoor::POS_OPEN;
}

void HangarDoorTask::close() {
  targetAngle = HangarDoor::POS_CLOSE;
}