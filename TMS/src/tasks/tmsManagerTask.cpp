#include "tmsManagerTask.h"

TmsManagerTask::TmsManagerTask(ProximitySensor* sonar, Led* greenLed, Led* redLed) {
    this->sonar = sonar;
    this->greenLed = greenLed;
    this->redLed = redLed;
    this->connectionManager = new ConnectionManagerClass();
    this->state = UNCONNECTED;
    this->redLed->switchOn();
    this->greenLed->switchOff();
}

void TmsManagerTask::tick() {
    switch (this->state) {
        case UNCONNECTED:
            connectionManager->setupWifi();
            connectionManager->setupMqttConnection();
            connectionManager->tryReconnect();
            if(connectionManager->isConnected()) {
                this->redLed->switchOff();
                this->greenLed->switchOn();
                this->state = CONNECTED;
            }
            break;
        case CONNECTED:
            if(!connectionManager->isConnected()) {
                this->redLed->switchOn();
                this->greenLed->switchOff();
                this->state = UNCONNECTED;
            }
            connectionManager->sendWaterLevel(sonar->getDistance());
            break;
    }
}
