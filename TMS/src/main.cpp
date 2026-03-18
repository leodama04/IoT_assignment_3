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
  delay(5000);
  connectionManager.send_msg("75");
}
