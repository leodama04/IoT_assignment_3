#include "Arduino.h"
#include "MsgService.h"
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

void MsgServiceClass::sendValveState(int valvePerc) {
    float valveState = valvePerc;  
    StaticJsonDocument<128> doc;
    doc["valve_state"] = valveState;
    String output;
    serializeJson(doc, output);
    MsgService.sendMsg(output);
}

void MsgServiceClass::sendMode(String mode) {
    StaticJsonDocument<64> doc;
    doc["mode"] = mode;
    String output;
    serializeJson(doc, output);
    MsgService.sendMsg(output);
}

static ParsedMsg parseCommand(String content){
    ParsedMsg result;
    result.cmd = CMD_UNKNOWN;
    result.value = "";
    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, content);
    if (error) {
        return result;
    }
    const char* type = doc["type"];
    if (strcmp(type, "mode") == 0) {
        result.cmd = CMD_MODE;
        result.type = "mode";
        result.value = doc["value"].as<String>();   
    } else if (strcmp(type, "valve_state") == 0) {
        result.cmd = CMD_VALVE_STATE;
        result.type = "valve_state";
        result.value = String(doc["value"].as<float>());
    }
    return result;
}

ParsedMsg MsgServiceClass::handleMessage() {
    Msg* m = MsgService.receiveMsg();
    if (m == NULL) return;
    String content = m->getContent();
    ParsedMsg parsedMsg = parseCommand(content);
    delete m;
    return parsedMsg;
}
