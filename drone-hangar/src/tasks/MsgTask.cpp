#include "MsgTask.h"
#include "Arduino.h"

MsgTask::MsgTask() {}

void MsgTask::init(int period) {
  Task::init(period);
}

void MsgTask::tick() {
  MsgService.processSerialBuffer();
  
  if (MsgService.isMsgAvailable()) {
    Msg* msg = MsgService.receiveMsg();
    if (msg != NULL){
      MsgService.lastMsg = msg->getContent();
      delete msg;
    }
  }

  while (MsgService.hasMsgToSend()) {
    Msg* msg = MsgService.dequeueToSend();
    MsgService.sendMsg(*msg);
    delete msg;
  }
}