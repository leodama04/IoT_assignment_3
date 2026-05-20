#include <Arduino.h>
#include "model/global.h"
#include "kernel/Scheduler.h"
#include "tasks/ValveTask.h"
#include "tasks/ButtonTask.h"

Scheduler* scheduler = new Scheduler();
ValveTask* valveTask = new ValveTask();
ButtonTask* buttonTask = new ButtonTask();

void setup() {
  Serial.begin(9600);
  delay(2000);
  valve->init();
  scheduler->init(200);  
  valveTask->init(200);  
  buttonTask->init(20);
  scheduler->addTask(valveTask);
  scheduler->addTask(buttonTask);
}

void loop() {
  scheduler->schedule();
}