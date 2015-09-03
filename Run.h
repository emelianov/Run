/////////////////////////////////////////////////////////
// Run
// Arduino cooperative multitask library
// (c)2015, Alexander Emelianov (a.m.emelainov@gmail.com)

#ifndef Run_h
 #define Run_h

 #include <inttypes.h>
 #if ARDUINO >= 100
  #include "Arduino.h"       // for delayMicroseconds, digitalPinToBitMask, etc
  #include "WString.h"
 #else
  #include "WProgram.h"      // for delayMicroseconds
  #include "WString.h"
 #endif

 #ifndef MAX_TASKS
 #define MAX_TASKS      32
 #endif

 typedef unsigned long (*task)();

 class Run {
  int count = 0;
  task tasks[MAX_TASKS];
  unsigned long nextRun[MAX_TASKS];
  public:
  byte addTask(task thread);
  bool delTask(byte id);
  void execute();
 };

#endif