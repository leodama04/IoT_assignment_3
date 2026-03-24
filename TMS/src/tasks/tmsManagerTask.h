#ifndef __TMS_MANAGER_TASK__
#define __TMS_MANAGER_TASK__

#include "kernel/task.h"
#include "devices/Sonar.h"
#include "kernel/connectionManager.h"

class TmsManagerTask: public Task {

public:
    TmsManagerTask();

protected:
    void tick() override;

private:
    enum State {UNCONNECTED, CONNECTED};
    State state;
    ConnectionManagerClass* connectionManager;
};

#endif