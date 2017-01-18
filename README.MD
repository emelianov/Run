#Run 2017.1

Arduino simple cooperative multitask library

*&copy;2017, Alexander Emelianov (a.m.emelianov@gmail.com)*

###1. Usage

Simple task:

```c
#define RUN_TASKS 8
#include <Run.h>
uint32_t taskToRun() {
  //DO SOMETHING
 return delayToNextRun_OR_0_ToRemoveTask
}
void setup() {
  addTask(taskToRun);
//or
  addTaskWithDelay(taskToRun, firstRunIn_value_miliseconds);
}
void loop() {
  taskExec();
}
```

Semaphore task:

```c
#define RUN_TASKS 8
#include <Run.h>
bool semaphore = false;
uint32_t taskToWait() {
  //DO SOMETHING
 return RUN_NEVER;	//Delay To NextRun or 0 To Remove Task also supported
}
void setup() {
  addTaskWithSemaphore(taskToWait, &semaphore);	//first runs as semaphore become true
}
void loop() {
  taskExec();
}
```

###2. Reference
`#define RUN_TASKS 16`

Maximum count of tasks to schedulle.

`typedef uint32_t (*task)()`

Task callback functuin.

Returns time to next run in milliseconds.

Returns 0 (zero) remove task from schedule.

`int16_t taskAddWithDelay(task thread, uint32_t delay, bool *signal = NULL)`

Add task with callback function thread and first run in delay milliseconds or when signal become true.

Returns current position id on success.

Returns -1 on error.

`int16_t taskAdd(task thread)`

Add task with callback function thread to run ASAP.

Returns current position id on success.

Returns -1 on error.

`int16_t taskAddWithSemaphore(task thread, bool *signal)`

Add task with callback function thread to run when signal become true.

Returns current position id on success.

Returns -1 on error.

`bool taskDel(uint_8t i)`

Internal use only. Remove task at position id from schedule.

Returns false if task not found.

`bool taskDel(task thread)`

Remove task from schedule by callback function address.

Returns false if task not found.

`void taskExec()`

Execute all tasks assigned to schedule. Shiuld be run in loop.

###3. Changes history
* 2017.1

 * Add semaphore support

 * Interrupt-safe code

* 2016.1.2

 * Add check to not exceed MAX_TASKS on addin task.

 * taskAdd and taskAddWithDelay now returns -1 on error.

* 2016.1.1

 * Added function to remove task from schedule by function address (taskDel(task thread)).

* 2016.1

 * Initial release.
