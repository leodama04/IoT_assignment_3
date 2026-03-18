#include <WiFi.h>
#include <PubSubClient.h>
#include "connectionManager.h"
#include "config.h"

#define MSG_BUFFER_SIZE  50

WiFiClient espClient;
PubSubClient client(espClient);

void ConnectionManagerClass::setup_wifi() {
    Serial.println(String("Connecting to ") + WIFI_NAME);
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_NAME, WIFI_PW);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("Connecting to WiFi..");
    }
    Serial.println("WiFi connected");
}

void ConnectionManagerClass::setup_mqtt_connection() {
    client.setServer(MQTT_SERVER_NAME, MQTT_PORT);
    if(client.connected()){
        Serial.println("connected to server");
    }
    else{
        Serial.println("not connected to server");
    }
}

void ConnectionManagerClass::try_reconnect() {
    if (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        String clientId = String("iot-client-")+String(random(0xffff), HEX);
        if (client.connect(clientId.c_str())) {
            Serial.println("connected");
            client.subscribe(MQTT_TOPIC);
            Serial.println(String("subscribed to ") + MQTT_TOPIC);
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void ConnectionManagerClass::send_msg(char *msg){
    char message[MSG_BUFFER_SIZE];
    Serial.println(String("Sending msg:") + message);
    snprintf (message, MSG_BUFFER_SIZE, msg);
    client.publish(MQTT_TOPIC, message); 
}