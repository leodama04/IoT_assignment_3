#include "tmsManagerTask.h"

TmsManagerTask::TmsManagerTask(ProximitySensor* sonar) {
    this->sonar = sonar;
    this->connectionManager = new ConnectionManagerClass();
    this->state = UNCONNECTED;
}

void TmsManagerTask::tick() {
    switch (this->state) {
        case UNCONNECTED:
            connectionManager->setupWifi();
            connectionManager->setupMqttConnection();
            connectionManager->tryReconnect();
            if(connectionManager->isConnected()) {
                this->state = CONNECTED;
            }
            break;
        case CONNECTED:
            if(!connectionManager->isConnected()) {
                this->state = UNCONNECTED;
            }
            delay(1000);
            connectionManager->sendWaterLevel(sonar->getDistance());
            //connectionManager->sendWaterLevel((rand() / (float)RAND_MAX) * 80.0f);
            break;
    }
}
