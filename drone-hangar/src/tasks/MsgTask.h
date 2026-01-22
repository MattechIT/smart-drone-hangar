#ifndef __MSGTASK__
#define __MSGTASK__

#include "../Task.h"
#include "../MsgService.h"

class MsgTask : public Task {
public:
  MsgTask();
  void init(int period) override;
  void tick() override;
};

#endif