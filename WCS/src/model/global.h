#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "Valve.h"
#include <Arduino.h>
#include "devices/Button.h"
#include "devices/Pot.h"
#include "LiquidCrystal_I2C.h"

#define BUT_PIN 2
#define MOT_PIN 9
#define POT_PIN A0

extern Valve* valve;
extern Button* button;
extern Potentiometer* pot;
extern LiquidCrystal_I2C lcd;   
enum State { MANUAL, AUTOMATIC, REMOTE_MANUAL, UNCONNECTED };
extern State state;

enum Command {
    CMD_MODE,
    CMD_VALVE_STATE,
    CMD_UNKNOWN
};

struct ParsedMsg {
    Command cmd;
    String type;
    String value;
};

#endif
