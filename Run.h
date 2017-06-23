/////////////////////////////////////////////////////////
// Run 2017.4
// Arduino simple cooperative multitask library
// (c)2017, Alexander Emelianov (a.m.emelianov@gmail.com)
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
};

uint8_t taskCount = 0;
taskThread taskTasks[RUN_TASKS];

int16_t taskAddWithDelay(task thread, uint32_t delay, uint16_t* signal = NULL) {
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

int16_t taskAddWithSemaphore(task thread, uint16_t* signal) {
 return taskAddWithDelay(thread, RUN_NEVER, signal);
}

int16_t taskAdd(task thread) {
 return taskAddWithDelay(thread, RUN_NOW);
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

bool taskExists(task thread) {
  for (uint8_t i = 0; i < taskCount; i++) {
    if (taskTasks[i].thread == thread && taskTasks[i].delay != RUN_DELETE) {
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


void taskExec() {
  uint8_t i, j;
  for(i = 0; i < taskCount; i++) {
    if (taskTasks[i].delay != 0) {
      if (taskTasks[i].signal != NULL && *taskTasks[i].signal > 0) {
        for (j = 0; j < taskCount; j++) {
          if (taskTasks[j].signal == taskTasks[i].signal) {
            taskTasks[j].lastRun = millis();
            taskTasks[j].delay = taskTasks[j].thread();
          }
        }
        if (*taskTasks[i].signal > 0) {
        	*taskTasks[i].signal = *taskTasks[i].signal - 1;
        }
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