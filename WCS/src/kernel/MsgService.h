#ifndef __MSGSERVICE__
#define __MSGSERVICE__

#include "Arduino.h"

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
  bool isMsgAvailable();
  Msg* receiveMsg();
  bool isMsgAvailable(Pattern& pattern);
  Msg* receiveMsg(Pattern& pattern);
  void sendMsg(const String& msg);
public:
  Msg* currentMsg;
  bool msgAvailable;
  
  void init();  
  void handleMessage();
  void sendValveState();
  void sendMode();
};

extern MsgServiceClass MsgService;

#endif