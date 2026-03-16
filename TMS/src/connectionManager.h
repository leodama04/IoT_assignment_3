#ifndef __CONNECTION_MANAGER_H__
#define __CONNECTION_MANAGER_H__

#include "Arduino.h"

class ConnectionManagerClass {
    
public:
    void setup_wifi();
    void setup_mqtt_connection();
    void try_reconnect();
    void send_msg(char *msg);
};

#endif