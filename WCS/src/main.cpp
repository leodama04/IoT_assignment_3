#include <Arduino.h>
#include "model/global.h"
#include "kernel/Scheduler.h"
#include "tasks/ValveTask.h"
#include "tasks/ButtonTask.h"
#include "tasks/ManualValveTask.h"
#include "tasks/LCDTask.h"

Scheduler* scheduler = new Scheduler();
ValveTask* valveTask = new ValveTask();
ButtonTask* buttonTask = new ButtonTask();
ManualValveTask* manualValveTask = new ManualValveTask();
LCDTask* lcdTask = new LCDTask();

void setup() {
  Serial.begin(9600);
  delay(2000);
  valve->init();
  scheduler->init(200);  
  valveTask->init(200);  
  buttonTask->init(20);
  manualValveTask->init(100);
  lcdTask->init(100);
  scheduler->addTask(valveTask);
  scheduler->addTask(buttonTask);
  scheduler->addTask(manualValveTask);
  scheduler->addTask(lcdTask);
}

void loop() {
  scheduler->schedule();
}