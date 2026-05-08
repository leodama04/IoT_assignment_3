#include "Arduino.h"
#include "MsgService.h"
#include "model/global.h"
#include <ArduinoJson.h>

String content;
MsgServiceClass MsgService;


bool MsgServiceClass::isMsgAvailable(){
    return msgAvailable;
}

Msg* MsgServiceClass::receiveMsg(){
    if (msgAvailable){
        Msg* msg = currentMsg;
        msgAvailable = false;
        currentMsg = NULL;
        content = "";
        return msg;  
    } else {
        return NULL; 
    }
}

void MsgServiceClass::init(){
    Serial.begin(9600);
    content.reserve(256);
    content = "";
    currentMsg = NULL;
    msgAvailable = false;  

    delay(100);
    MsgService.sendMsg("READY");
}

void MsgServiceClass::sendMsg(const String& msg){
    Serial.println(msg);  
}

void serialEvent() {
    /* reading the content */
    while (Serial.available()) {
        char ch = (char) Serial.read();
        if (ch == '\n'){
            MsgService.currentMsg = new Msg(content);
            MsgService.msgAvailable = true;      
        } else {
            content += ch;      
        }
    }
}

bool MsgServiceClass::isMsgAvailable(Pattern& pattern){
    return (msgAvailable && pattern.match(*currentMsg));
}

Msg* MsgServiceClass::receiveMsg(Pattern& pattern){
    if (msgAvailable && pattern.match(*currentMsg)){
        Msg* msg = currentMsg;
        msgAvailable = false;
        currentMsg = NULL;
        content = "";
        return msg;  
    } else {
        return NULL; 
    } 
}

void MsgServiceClass::sendValveState() {
    if (valve != NULL) {
        int percentage = valve->getOpenPercentage();
        float valveState = percentage;  
        StaticJsonDocument<128> doc;
        doc["valve_state"] = valveState;
        String output;
        serializeJson(doc, output);
        MsgService.sendMsg(output);
    }
}

void MsgServiceClass::sendMode() {
    Serial.println("");
}

enum Command {
    CMD_MODE,
    CMD_VALVE_STATE,
    CMD_UNKNOWN
};

struct ParsedMsg {
    Command cmd;
    String type;
    float value;
};

static ParsedMsg parseCommand(String content){
    ParsedMsg result;
    result.cmd = CMD_UNKNOWN;
    result.value = 0;
    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, content);
    if (error) {
        return result;
    }
    const char* type = doc["type"];
    if (strcmp(type, "mode") == 0) {
        result.cmd = CMD_MODE;
        result.type = "mode";
        result.value = doc["value"];
    } else if (strcmp(type, "valve_state") == 0) {
        result.cmd = CMD_VALVE_STATE;
        result.type = "valve_state";
        result.value = doc["value"];
    }
    return result;
}

void MsgServiceClass::handleMessage() {
    if (MsgService.isMsgAvailable()){
        Msg* m = MsgService.receiveMsg();
        if (m == NULL) return;
        String content = m->getContent();
        ParsedMsg parsedMsg = parseCommand(content);
        switch(parsedMsg.cmd){
            case CMD_MODE:
                MsgService.sendMode();
                break;
            case CMD_VALVE_STATE:
                if (valve != NULL) {
                    int percentage = (int)parsedMsg.value;
                    if(percentage != valve->getOpenPercentage()) {
                        valve->open(percentage);
                        this->sendValveState();
                    }
                }
                break;
            case CMD_UNKNOWN:
            default:
                break;
        }
        delete m;
    }
}