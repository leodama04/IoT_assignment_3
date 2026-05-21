#ifndef __MANUAL_VALVE_TASK__
#define __MANUAL_VALVE_TASK__

#include "model/global.h"
#include "kernel/Task.h"
#include "../kernel/MsgService.h"

class ManualValveTask : public Task {

public:
    void init(int period);
    void tick();
};

#endif