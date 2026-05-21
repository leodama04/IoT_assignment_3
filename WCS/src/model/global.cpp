#include "global.h"
#include "../devices/ServoMotorImpl.h"
#include "../devices/ButtonImpl.h"

ServoMotorImpl* servoMotor = new ServoMotorImpl(MOT_PIN);
Valve* valve = new Valve(servoMotor);
Button* button = new ButtonImpl(BUT_PIN);
Potentiometer* pot = new Potentiometer(POT_PIN);
LiquidCrystal_I2C lcd(0x27, 20, 4);  
State state;
