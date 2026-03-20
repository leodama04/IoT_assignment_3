#include <Arduino.h>
#include "connectionManager.h"

ConnectionManagerClass connectionManager;

void setup() {
  Serial.begin(9600);
  connectionManager.setupWifi();
  connectionManager.setupMqttConnection();
  connectionManager.tryReconnect();
}

void loop() {
  connectionManager.tryReconnect();
  delay(5000);
  connectionManager.sendWaterLevel(75.00);
  delay(5000);
  connectionManager.sendWaterLevel(15.00);
}
