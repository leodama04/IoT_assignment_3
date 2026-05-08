#include <Arduino.h>
#include "tasks/tmsManagerTask.h"
#include "devices/Sonar.h"
#include "devices/ProximitySensor.h"
#include "kernel/connectionManager.h"

#define ECHO_PIN 5
#define TRIG_PIN 18
#define MAX_TIME 30000

ProximitySensor* sonar = new Sonar(ECHO_PIN, TRIG_PIN, MAX_TIME);
ConnectionManagerClass* connectionManager = new ConnectionManagerClass();

void setup() {
  Serial.begin(9600);
  TmsManagerTask* tmsManagerTask = new TmsManagerTask(sonar);
  tmsManagerTask->init(200);
  tmsManagerTask->start("TmsTask", 10000, 1, 0);
  delay(2000);
}

void loop() {
  /*while(!connectionManager->isConnected()) {
    connectionManager->setupWifi();
    connectionManager->setupMqttConnection();
    connectionManager->tryReconnect();
  }

  connectionManager->sendWaterLevel(0.0);
  delay(2000);

  connectionManager->sendWaterLevel(25.5);
  delay(1000);
  connectionManager->sendWaterLevel(25.5);
  delay(1000);
  connectionManager->sendWaterLevel(25.5);
  delay(1000);
  connectionManager->sendWaterLevel(25.5);
  delay(1000);
  connectionManager->sendWaterLevel(25.5);
  delay(1000);
  connectionManager->sendWaterLevel(25.5);
  delay(1000);
  connectionManager->sendWaterLevel(25.5);
  delay(1000);
  connectionManager->sendWaterLevel(25.5);
  delay(1000);
  connectionManager->sendWaterLevel(70.5);
  delay(2000);
  */
}
