#ifndef __TMS_MANAGER_TASK__
#define __TMS_MANAGER_TASK__

#include "kernel/task.h"
#include "devices/ProximitySensor.h"
#include "kernel/connectionManager.h"
#include "devices/Led.h"

class TmsManagerTask: public Task {

public:
    TmsManagerTask(ProximitySensor* sonar, Led* greenLed, Led* redLed);

protected:
    void tick() override;

private:
    enum State {UNCONNECTED, CONNECTED};
    State state;
    ConnectionManagerClass* connectionManager;
    ProximitySensor* sonar;
    Led* greenLed;
    Led* redLed;
};

#endif