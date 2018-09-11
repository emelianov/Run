# Run 2018.1

Arduino simple cooperative multitask library

*&copy;2017-2018, Alexander Emelianov (a.m.emelianov@gmail.com)*

### 1. Usage

Simple task:

```c
#define RUN_TASKS 8
#include <Run.h>
uint32_t taskToRun() {
  //DO SOMETHING
 return delayToNextRun_or_RUN_DELETE_ToRemoveTask
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
uint16_t semaphore = 0;
uint32_t taskToWait() {
  //DO SOMETHING
 return RUN_NEVER;	//Delay To NextRun or RUN_DELETE To Remove Task also supported
}
void setup() {
  addTaskWithSemaphore(taskToWait, &semaphore);	//first runs as semaphore become non-zerro
}
void loop() {
  taskExec();
}
```

### 2. Reference
```c
#define RUN_TASKS 16
`

Maximum count of tasks to schedulle.

```c
typedef uint32_t (*task)();
`

Task callback functuin.

Returns time to next run in milliseconds.

Returns RUN_DELETE remove task from schedule.

Returns RUN_NEVER to not perform timed run.

```c
uint16_t taskAddWithDelay(task thread, uint32_t delay, uint16_t* signal = NULL);
`

Add task with callback function thread and first run in delay milliseconds or when signal become true.

Returns task id on success.

Returns 0 on error.

```c
uint16_t taskAdd(task thread);
`

Add task with callback function thread to run ASAP.

Returns task id on success.

Returns 0 on error.

```c
uint16_t taskAddWithSemaphore(task thread, uint16_t* signal);
`

Add task with callback function thread to run when signal become true.

Returns task id on success.

Returns 0 on error.

```c
bool taskDel(task thread);
bool taskDel(uint_16t id);
`

Remove task from schedule by callback function address or task id.

Returns false if task not found.

```c
bool taskExists(task thread);
bool taskExists(uint16_t);
`

Return true if task with specific callback function address or task id exists in schedule.

Returns false if task not found.

```c
uint32_t taskRemaider(task thread);
uint32_t taskRemaider(uint16_t id);
`

Return time to next run of task.

Returns RUN_NOW  if timeout is expired.

Returns RUN_DELETE if task not exists.

Returns RUN_NEVER for task that will never run by schedule run. But can run by semaphore.

```c
void taskExec();
`

Execute all tasks assigned to schedule. Shiuld be run in loop.

### 3. Changes history
* 2018.1
  * task id implemented
* 2017.4
  * taskReminder() added
* 2017.3
  * taskExists() added
* 2017.2
  * Change semaphore API
* 2017.1
  * Semaphore support added
  * Interrupt-safe code
  * Examples added
  * MAX_TASKS renamed to RUN_TASKS
* 2016.1.2
  * Add check to not exceed MAX_TASKS on addin task.
  * taskAdd() and taskAddWithDelay() now returns -1 on error.
* 2016.1.1
  * Added function to remove task from schedule by function address (taskDel(task thread)).
* 2016.1
  * Initial release.
