#ifndef __VALVE_TASK__
#define __VALVE_TASK__

#include "../kernel/Task.h"
#include "../kernel/MsgService.h"
#include "../model/global.h"

class ValveTask : public Task {

public:
  void init(int period);
  void tick();

private:
  State parseMode(String msgMode);
};



#endif
