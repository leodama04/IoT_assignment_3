#include "tmsManagerTask.h"

TmsManagerTask::TmsManagerTask() {
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
            break;
    }
}
