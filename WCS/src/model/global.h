#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "Valve.h"
#include <Arduino.h>
#include "devices/Button.h"

#define BUT_PIN 2
#define MOT_PIN 9

extern Valve* valve;
extern Button* button;
enum State { MANUAL, AUTOMATIC };
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
