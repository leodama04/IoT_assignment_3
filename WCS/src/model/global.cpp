#include "global.h"
#include "../devices/ServoMotorImpl.h"

ServoMotorImpl* servoMotor = new ServoMotorImpl(9);
Valve* valve = new Valve(servoMotor);
