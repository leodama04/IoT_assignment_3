#include "ButtonTask.h"

void ButtonTask::init(int period) {
    Task::init(period);
}

void ButtonTask::tick() {
    switch (state) {
        case AUTOMATIC:
            if(button->isPressed()) {
                MsgService.sendMode("MANUAL");
                state = MANUAL;
            }
            break;
        case MANUAL:
            if(button->isPressed()) {
                MsgService.sendMode("AUTOMATIC");
                state = AUTOMATIC;
            }
            break;
        }
}