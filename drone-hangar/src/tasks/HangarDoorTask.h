#ifndef __HANGAR_DOOR_TASK__
#define __HANGAR_DOOR_TASK__

#include "../Task.h"
#include "../components/HangarDoor.h"

class HangarDoorTask : public Task {
public:
  HangarDoorTask(HangarDoor* door);
  void init(int period);
  void tick();
  
  void open();
  void close();

private:
  HangarDoor* door;
  int currentAngle;
  int targetAngle;
  int step = 5;
};

#endif