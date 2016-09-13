/////////////////////////////////////////////////////////
// Run 2016.1.2
// Arduino simple cooperative multitask library
// (c)2016, Alexander Emelianov (a.m.emelainov@gmail.com)
//

#pragma once

#include <inttypes.h>
#include "Arduino.h"

#ifndef MAX_TASKS
 #define MAX_TASKS      16
#endif
//#define TASKEXEC for(uint8_t i = 0; i < taskCount; i++) {if (millis() - taskTasks[i].lastRun > taskTasks[i].delay) {taskTasks[i].lastRun = millis(); taskTasks[i].delay = taskTasks[i].thread();if (taskTasks[i].delay == 0) if (taskDel(i)) break;}}
#define TASKEXEC taskExec();

typedef uint32_t (*task)();
struct taskThread {
 task thread;
 uint32_t lastRun;
 uint32_t delay;
};

uint8_t taskCount = 0;
taskThread taskTasks[MAX_TASKS];

int16_t taskAddWithDelay(task thread, uint32_t delay) {
 if (taskCount < MAX_TASKS) {
   taskTasks[taskCount].thread    = thread;
   taskTasks[taskCount].lastRun  = millis();
   taskTasks[taskCount].delay = delay;
   taskCount++;
   return taskCount - 1;
 } else {
   return -1;
 }
}

int16_t taskAdd(task thread) {
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

bool taskDel(task thread) {
  for (uint8_t i = 0; i < taskCount; i++) {
    if (taskTasks[i].thread = thread) {
      taskDel(i);
      break;
    }
  }
}

void taskExec() {
  for(uint8_t i = 0; i < taskCount; i++) {
    if (millis() - taskTasks[i].lastRun > taskTasks[i].delay) {
      taskTasks[i].lastRun = millis();
      taskTasks[i].delay = taskTasks[i].thread();
      if (taskTasks[i].delay == 0) {
        if (taskDel(i)) {
          break;
        }
      }
    }
  }
}