#ifndef __MSGSERVICE__
#define __MSGSERVICE__

#include "Arduino.h"
#include "model/global.h"

class Msg {
  String content;

public:
  Msg(String content){
    this->content = content;
  }
  
  String getContent(){
    return content;
  }
};

class Pattern {
public:
  virtual boolean match(const Msg& m) = 0;  
};

class MsgServiceClass {

private:
  Msg* receiveMsg();
  bool isMsgAvailable(Pattern& pattern);
  Msg* receiveMsg(Pattern& pattern);
  void sendMsg(const String& msg);
public:
  Msg* currentMsg;
  bool msgAvailable;
  bool isMsgAvailable();
  void init();  
  ParsedMsg handleMessage();
  void sendValveState(int valvePerc);
  void sendMode(String mode);
};

extern MsgServiceClass MsgService;

#endif