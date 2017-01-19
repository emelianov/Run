/////////////////////////////////////////////////////////
// Run 2017.1
// Arduino simple cooperative multitask library
// (c)2017, Alexander Emelianov (a.m.emelianov@gmail.com)
//

#pragma once

#define RUN_DELETE 0
#define RUN_NEVER 0xFFFFFFFF
#ifndef RUN_TASKS
 #define RUN_TASKS      16
#endif

typedef uint32_t (*task)();
struct taskThread {
 task thread;
 uint32_t lastRun;
 uint32_t delay;
 bool *signal;
};

uint8_t taskCount = 0;
taskThread taskTasks[RUN_TASKS];

int16_t taskAddWithDelay(task thread, uint32_t delay, bool *signal = NULL) {
 if (taskCount < RUN_TASKS) {
   taskTasks[taskCount].thread   = thread;
   taskTasks[taskCount].lastRun  = millis();
   taskTasks[taskCount].delay    = delay;
   taskTasks[taskCount].signal   = signal;
   taskCount++;
   return taskCount - 1;
 }
 return -1;
}

int16_t taskAddWithSemaphore(task thread, bool * signal) {
 return taskAddWithDelay(thread, RUN_NEVER, signal);
}

int16_t taskAdd(task thread) {
 return taskAddWithDelay(thread, 1);
}

bool taskDel(uint8_t i) {
 if (i < taskCount) {
  taskTasks[i].delay = RUN_DELETE;
  return true;
 }
 return false;
}

bool taskDel(task thread) {
  for (uint8_t i = 0; i < taskCount; i++) {
    if (taskTasks[i].thread == thread) {
      taskTasks[i].delay = RUN_DELETE;
      return true;
    }
  }
  return false;
}

void taskExec() {
  uint8_t i, j;
  for(i = 0; i < taskCount; i++) {
    if (taskTasks[i].delay != 0) {
      if (taskTasks[i].signal != NULL && *taskTasks[i].signal) {
        for (j = 0; j < taskCount; j++) {
          if (taskTasks[j].signal == taskTasks[i].signal) {
            taskTasks[j].lastRun = millis();
            taskTasks[j].delay = taskTasks[j].thread();
          }
        }
        *taskTasks[i].signal = false;
      }
      if (millis() - taskTasks[i].lastRun > taskTasks[i].delay) {
        taskTasks[i].lastRun = millis();
        taskTasks[i].delay = taskTasks[i].thread();
      }
    }
  }
  for(i = 0; i < taskCount; i++) {
   if (taskTasks[i].delay == RUN_DELETE) { 
     memcpy(&taskTasks[i], &taskTasks[i + 1], (taskCount - i - 1) * sizeof(taskThread));
     taskCount--;
     i--; 
   }
  }
}