#include <Arduino.h>
#include "tasks/tmsManagerTask.h"

void setup() {
  Serial.begin(9600);
  TmsManagerTask* tmsManagerTask = new TmsManagerTask();
  tmsManagerTask->init(200);
  tmsManagerTask->start("TmsTask", 10000, 1, 0);
}

void loop() { }
