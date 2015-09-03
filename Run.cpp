/////////////////////////////////////////////////////////
// Run
// Arduino cooperative multitask library
// (c)2015, Alexander Emelianov (a.m.emelainov@gmail.com)

#include <Run.h>

byte Run::addTask(task thread) {
 tasks[this->count]    = thread;
 nextRun[this->count]  = 0;
 count++;
 return count - 1;
}
bool Run::delTask(byte id) {    //Not implemented
 return false;
}
//* Main thread usage:
//* void loop() {
//*  tasks->execute();
//* }
void Run::execute() {
    for (byte i = 0; i < this->count; i++) {
      if (nextRun[i] < millis()) {
        unsigned long nextDelay = tasks[i]();
        if (nextDelay >=0) {
          nextRun[i] = millis() + nextDelay;
        } else {
          if (delTask(i)) i--;
        }
      }
    }
}