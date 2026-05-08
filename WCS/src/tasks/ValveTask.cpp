#include "ValveTask.h"

void ValveTask::init(int period) {
  Task::init(period);
}

void ValveTask::tick() {
  MsgService.handleMessage();
}
