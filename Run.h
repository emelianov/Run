/////////////////////////////////////////////////////////
// Run 2018.1
// Arduino simple cooperative multitask library
// (c)2017-2018, Alexander Emelianov (a.m.emelianov@gmail.com)
//

#pragma once

#define RUN_DELETE	0
#define RUN_NEVER	0xFFFFFFFF
#define RUN_NOW		1
#ifndef RUN_TASKS
 #define RUN_TASKS  16
#endif

typedef uint32_t (*task)();
struct taskThread {
 task thread;
 uint32_t lastRun;
 uint32_t delay;
 uint16_t* signal;
 uint16_t id;
};

uint8_t taskCount = 0;
taskThread taskTasks[RUN_TASKS];
uint16_t taskLastId = 0;
uint16_t taskRunningId = 0;

uint16_t taskAddWithDelay(task thread, uint32_t delay, uint16_t* signal = NULL) {
 if (taskCount >= RUN_TASKS) return false; 
 taskLastId++;
 if (!taskLastId) taskLastId = 1;
 taskTasks[taskCount].thread   = thread;
 taskTasks[taskCount].lastRun  = millis();
 taskTasks[taskCount].delay    = delay;
 taskTasks[taskCount].signal   = signal;
 taskTasks[taskCount].id       = taskLastId;
 taskCount++;
 return taskLastId;
}

uint16_t taskAddWithSemaphore(task thread, uint16_t* signal) {
 return taskAddWithDelay(thread, RUN_NEVER, signal);
}

uint16_t taskAdd(task thread) {
 return taskAddWithDelay(thread, RUN_NOW);
}

bool taskDel(uint8_t id) {
 for (uint8_t i = 0; i < taskCount; i++) {
  if (taskTasks[i].id == id) {
   taskTasks[i].delay = RUN_DELETE;
   return true;
  }
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

bool taskExists(task thread) {
  for (uint8_t i = 0; i < taskCount; i++) {
    if (taskTasks[i].thread == thread && taskTasks[i].delay != RUN_DELETE) {
      return true;
    }
  }
  return false;
}

bool taskExists(uint16_t id) {
  for (uint8_t i = 0; i < taskCount; i++) {
    if (taskTasks[i].id == id && taskTasks[i].delay != RUN_DELETE) {
      return true;
    }
  }
  return false;
}

uint32_t taskRemainder(task thread) {
  for (uint8_t i = 0; i < taskCount; i++) {
    if (taskTasks[i].thread == thread) {
    	if (taskTasks[i].delay == RUN_DELETE) return RUN_DELETE;
    	if (taskTasks[i].delay == RUN_NEVER) return RUN_NEVER;
    	uint32_t t = millis() - taskTasks[i].lastRun;
    	if (t > taskTasks[i].delay) return 1;
    	return taskTasks[i].delay - t;
    }
  }
  return RUN_DELETE;
}

uint32_t taskRemainder(uint16_t id) {
  for (uint8_t i = 0; i < taskCount; i++) {
    if (taskTasks[i].id == id) {
    	if (taskTasks[i].delay == RUN_DELETE) return RUN_DELETE;
    	if (taskTasks[i].delay == RUN_NEVER) return RUN_NEVER;
    	uint32_t t = millis() - taskTasks[i].lastRun;
    	if (t > taskTasks[i].delay) return 1;
    	return taskTasks[i].delay - t;
    }
  }
  return RUN_DELETE;
}

uint16_t taskId() {
 return taskRunningId;
}

void taskExec() {
  uint8_t i, j;
  uint32_t delay;
  for(i = 0; i < taskCount; i++) {
    if (taskTasks[i].delay != 0) {
      if (taskTasks[i].signal != NULL && *taskTasks[i].signal > 0) {
	uint16_t* sig = taskTasks[i].signal;
        for (j = 0; j < taskCount; j++) {
          if (taskTasks[j].signal == sig) {
            taskTasks[j].lastRun = millis();
            taskRunningId = taskTasks[j].id;
            delay = taskTasks[j].thread();
            if (taskTasks[j].delay != RUN_DELETE) taskTasks[j].delay = delay;
          }
        }
        if (*sig > 0) *sig = *sig - 1;
      }
      if (taskTasks[i].delay == RUN_NOW || millis() - taskTasks[i].lastRun > taskTasks[i].delay) {
        taskTasks[i].lastRun = millis();
        taskRunningId = taskTasks[i].id;
        delay = taskTasks[i].thread();
	if (taskTasks[i].delay != RUN_DELETE) taskTasks[i].delay = delay;
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