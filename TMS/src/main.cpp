#include <Arduino.h>
#include "tasks/tmsManagerTask.h"
#include "devices/Sonar.h"
#include "devices/ProximitySensor.h"
#include "kernel/connectionManager.h"
#include "devices/Led.h"

#define ECHO_PIN 5
#define TRIG_PIN 18
#define MAX_TIME 30000
#define RED_PIN 15
#define GREEN_PIN 2

ProximitySensor* sonar = new Sonar(ECHO_PIN, TRIG_PIN, MAX_TIME);
ConnectionManagerClass* connectionManager = new ConnectionManagerClass();
Led* redLed = new Led(RED_PIN);
Led* greenLed = new Led(GREEN_PIN);

void setup() {
  Serial.begin(9600);
  TmsManagerTask* tmsManagerTask = new TmsManagerTask(sonar, greenLed, redLed);
  tmsManagerTask->init(200);
  tmsManagerTask->start("TmsTask", 10000, 1, 0);
  delay(2000);
}

void loop() { }
