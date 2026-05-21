#include "ManualValveTask.h"

void ManualValveTask::init(int period) {
    Task::init(period);
}

void ManualValveTask::tick() {
    switch (state) {
        case MANUAL: {
            pot->sync();
            int percent = pot->getValue();
            if (valve != NULL) {
                if (valve->getOpenPercentage() != percent) {
                    valve->open(percent);
                    MsgService.sendValveState(valve->getOpenPercentage());
                }
            }
            break;
        }
        case AUTOMATIC:
            break;
        case REMOTE_MANUAL:
            break;
    }
}