/////////////////////////////////////////////////////////
// Run 2016.1
// Arduino simple cooperative multitask library
// (c)2016, Alexander Emelianov (a.m.emelainov@gmail.com)
//
// Usage:
// uint32_t taskToRun() {
//   //DO SOMETHING
//  return delayToNextRun_OR_0_ToRemoveTask
// }
// void setup() {
//  addTask(taskToRun);
// }
// void loop() {
//  TASKEXEC //Macro to run tasks
// }

#pragma once

#include <inttypes.h>
#include "Arduino.h"

#ifndef MAX_TASKS
 #define MAX_TASKS      16
#endif
#define TASKEXEC for(uint8_t i = 0; i < taskCount; i++) {if (millis() - taskTasks[i].lastRun > taskTasks[i].delay) {taskTasks[i].lastRun = millis(); taskTasks[i].delay = taskTasks[i].thread();if (taskTasks[i].delay == 0) if (taskDel(i)) break;}}

typedef uint32_t (*task)();
struct taskThread {
 task thread;
 uint32_t lastRun;
 uint32_t delay;
};

uint8_t taskCount = 0;
taskThread taskTasks[MAX_TASKS];

uint8_t taskAddWithDelay(task thread, uint32_t delay) {
 taskTasks[taskCount].thread    = thread;
 taskTasks[taskCount].lastRun  = millis();
 taskTasks[taskCount].delay = delay;
 taskCount++;
 return taskCount - 1;
}

uint8_t taskAdd(task thread) {
 return taskAddWithDelay(thread, 1);
}

bool taskDel(byte id) {
 if (id < taskCount) {
  memcpy(&taskTasks[id], &taskTasks[id + 1], (taskCount - id - 1) * sizeof(taskThread));
  taskCount--;
  return true;
 } else {
  return false;
 }
}
/*
void taskExec() {
 for(uint8_t i = 0; i < taskCount; i++) {
  if (millis() - taskLastRun[i] > taskDelay[i]) {
   taskLastRun[i] = millis(); taskDelay[i] = taskTasks[i]();
   if (taskDelay[i] == 0) {
    if (taskDel(i)) {
     break;
    }
   }
  }
 }
}
*/