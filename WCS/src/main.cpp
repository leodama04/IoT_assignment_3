#include <Arduino.h>
#include "model/global.h"
#include "kernel/Scheduler.h"
#include "tasks/ValveTask.h"

Scheduler* scheduler = new Scheduler();
ValveTask* valveTask = new ValveTask();
void setup() {
  Serial.begin(9600);
  delay(2000);
  valve->init();
  scheduler->init(200);  
  valveTask->init(200);  
  scheduler->addTask(valveTask);
}

void loop() {
  scheduler->schedule();
}