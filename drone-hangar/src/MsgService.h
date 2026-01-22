#ifndef __MSGSERVICE__
#define __MSGSERVICE__

#include "Arduino.h"

class Msg {
  String content;

public:
  Msg(String content){
    this->content = content;
  }
  
  String getContent() const{
    return content;
  }
};

class Pattern {
public:
  virtual boolean match(const Msg& m) = 0;  
};

class MsgServiceClass {
    
public:
  Msg* currentMsg;
  bool msgAvailable;
  void init();
  void checkForMessages();
  bool isMsgAvailable();
  Msg* receiveMsg();
  bool isMsgAvailable(Pattern& pattern);
  Msg* receiveMsg(Pattern& pattern);
  void sendMsg(const Msg& msg);
  void sendMsg(const String& msg);
  void sendMsg(const char* msg);
  void enqueueToSend(const String& msg);
  void enqueueToSend(const Msg& msg);
  bool hasMsgToSend();
  Msg* dequeueToSend();
  void appendSerialChar(char ch);
  void processSerialBuffer();
  
  // Shared storage for the last valid command
  String lastMsg; 
  
private:
  String serialBuffer;
  static const int MAX_OUT_QUEUE = 4;
  Msg* outQueue[MAX_OUT_QUEUE];
  int outHead = 0;
  int outTail = 0;
  int outCount = 0;
};

extern MsgServiceClass MsgService;

#endif
