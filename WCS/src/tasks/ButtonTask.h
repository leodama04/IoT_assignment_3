#ifndef __BUTTON_TASK__
#define __BUTTON_TASK__

#include "model/global.h"
#include "kernel/Task.h"
#include "../kernel/MsgService.h"


class ButtonTask : public Task {

public:
    void init(int period);
    void tick();
};

#endif