#include "ValveTask.h"

void ValveTask::init(int period) {
    Task::init(period);
    MsgService.init();
    valve->open(0);
    MsgService.sendValveState(valve->getOpenPercentage());
    state = AUTOMATIC;
}

State ValveTask::parseMode(String msgMode) {
    if (msgMode == "MANUAL") {
        return MANUAL;
    } else if (msgMode == "AUTOMATIC") {
        return AUTOMATIC;
    } else if (msgMode == "REMOTE_MANUAL") {
        return REMOTE_MANUAL;
    }
}

void ValveTask::tick() {
    switch (state) {
        case MANUAL:
            if(MsgService.isMsgAvailable()) {
                ParsedMsg msg = MsgService.handleMessage();
                switch (msg.cmd) {
                    case CMD_MODE: {
                        State msgState = parseMode(msg.value);
                        if (msgState != state) {
                            state = msgState;
                        }
                        break;
                    }
                    case CMD_VALVE_STATE:
                        break;
                    case CMD_UNKNOWN:
                        // ignora
                        break;
                    default:
                        // per sicurezza
                        break;
                }
            }
            break;

        case AUTOMATIC:
            if(MsgService.isMsgAvailable()) {
                ParsedMsg msg = MsgService.handleMessage();
                switch (msg.cmd) {
                    case CMD_MODE: {
                        State msgState = parseMode(msg.value);
                        if (msgState != state) {
                            state = msgState;
                        }
                        break;
                    }
                    case CMD_VALVE_STATE:
                        if (valve != NULL) {
                            int percent = msg.value.toInt();
                            if (valve->getOpenPercentage() != percent) {
                                valve->open(percent);
                                MsgService.sendValveState(valve->getOpenPercentage());
                            }
                        }
                        break;
                    case CMD_UNKNOWN:
                        break;
                    default:
                        break;
                }
            }
            break;

        case REMOTE_MANUAL:
            if(MsgService.isMsgAvailable()) {
                ParsedMsg msg = MsgService.handleMessage();
                switch (msg.cmd) {
                    case CMD_MODE: {
                        State msgState = parseMode(msg.value);
                        if (msgState != state) {
                            state = msgState;
                        }
                        break;
                    }
                    case CMD_VALVE_STATE:
                        if (valve != NULL) {
                            int percent = msg.value.toInt();
                            if (valve->getOpenPercentage() != percent) {
                                valve->open(percent);
                                MsgService.sendValveState(valve->getOpenPercentage());
                            }
                        }
                        break;
                    case CMD_UNKNOWN:
                        break;
                    default:
                        break;
                }
            }
    }
}

