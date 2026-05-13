#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "Valve.h"
#include <Arduino.h>

extern Valve* valve;

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
