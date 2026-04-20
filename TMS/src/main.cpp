#include <Arduino.h>
#include "tasks/tmsManagerTask.h"
#include "devices/Sonar.h"
#include "devices/ProximitySensor.h"

#define ECHO_PIN 5
#define TRIG_PIN 18
#define MAX_TIME 30000

ProximitySensor* sonar = new Sonar(ECHO_PIN, TRIG_PIN, MAX_TIME);

void setup() {
  Serial.begin(9600);
  TmsManagerTask* tmsManagerTask = new TmsManagerTask(sonar);
  tmsManagerTask->init(200);
  tmsManagerTask->start("TmsTask", 10000, 1, 0);
}

void loop() { }
