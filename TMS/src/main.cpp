#include <Arduino.h>
#include "connectionManager.h"

ConnectionManagerClass connectionManager;

void setup() {
  Serial.begin(9600);
  connectionManager.setup_wifi();
  connectionManager.setup_mqtt_connection();
  connectionManager.try_reconnect();
}

void loop() {
  connectionManager.try_reconnect();
}
