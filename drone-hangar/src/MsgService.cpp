#include "Arduino.h"
#include "MsgService.h"



MsgServiceClass MsgService;

bool MsgServiceClass::isMsgAvailable(){
  return msgAvailable;
}

Msg* MsgServiceClass::receiveMsg(){
  if (msgAvailable){
    Msg* msg = currentMsg;
    msgAvailable = false;
    currentMsg = NULL;
    serialBuffer = "";
    return msg;  
  } else {
    return NULL; 
  }
}

void MsgServiceClass::init(){
  Serial.begin(115200);
  serialBuffer.reserve(64);
  serialBuffer = "";
  currentMsg = NULL;
  msgAvailable = false;  
}

void MsgServiceClass::sendMsg(const String& msg){
  Serial.println(msg);
}

void MsgServiceClass::sendMsg(const char* msg) {
  Serial.println(msg);
}

void MsgServiceClass::sendMsg(const Msg& msg){
  Serial.println(msg.getContent());
}


void MsgServiceClass::appendSerialChar(char ch) {
  if (ch == '\n') {
    currentMsg = new Msg(serialBuffer);
    msgAvailable = true;
    serialBuffer = "";
  } else {
    serialBuffer += ch;
  }
}

void MsgServiceClass::processSerialBuffer() {
  while (Serial.available()) {
    char ch = (char) Serial.read();
    appendSerialChar(ch);
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
    serialBuffer = "";
    return msg;  
  } else {
    return NULL; 
  }
  
}

// --- Gestione coda messaggi in uscita ---
void MsgServiceClass::enqueueToSend(const String& msg) {
  enqueueToSend(Msg(msg));
}

void MsgServiceClass::enqueueToSend(const Msg& msg) {
  if (outCount < MAX_OUT_QUEUE) {
    outQueue[outTail] = new Msg(msg.getContent());
    outTail = (outTail + 1) % MAX_OUT_QUEUE;
    outCount++;
  }
}

bool MsgServiceClass::hasMsgToSend() {
  return outCount > 0;
}

Msg* MsgServiceClass::dequeueToSend() {
  if (outCount > 0) {
    Msg* msg = outQueue[outHead];
    outHead = (outHead + 1) % MAX_OUT_QUEUE;
    outCount--;
    return msg;
  }
  return NULL;
}
