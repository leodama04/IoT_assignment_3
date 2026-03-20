#ifndef __CONNECTION_MANAGER_H__
#define __CONNECTION_MANAGER_H__

#include "Arduino.h"

class ConnectionManagerClass {
    
public:
    void setupWifi();
    void setupMqttConnection();
    void tryReconnect();
    void sendWaterLevel(float waterLevel);
};

#endif