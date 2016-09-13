Run 2016.1.2
Arduino simple cooperative multitask library
(c)2016, Alexander Emelianov (a.m.emelainov@gmail.com)

#1. Usage

--- Code ---
uint32_t taskToRun() {
  //DO SOMETHING
 return delayToNextRun_OR_0_ToRemoveTask
}
void setup() {
  addTask(taskToRun);
}
void loop() {
  TASKEXEC //Macro to run tasks
}
--- Code ---

#2. Reference

typedef uint32_t (*task)()
Task callback functuin.
Returns time to next run in milliseconds.
Returns 0 (zero) remove task from schedule.

int16_t taskAddWithDelay(task thread, uint32_t delay)
Add task with callback function thread and first run in delay milliseconds.
Returns current position id on success.
Returns -1 on error.

int16_t taskAdd(task thread)
Add task with callback function thread to run ASAP.
Returns current position id on success.
Returns -1 on error.

bool taskDel(uint_8t id)
Internal use only. Remove task at position id from schedule.
Returns false if task not found.

bool taskDel(task thread)
Remove task from schedule by callback function address.
Returns false if task not found.

void taskExec()
Execute all tasks assigned to schedule. Shiuld be run in loop.

#3. Changes history

2016.1.2
Add check to not exceed MAX_TASKS on addin task.
taskAdd* now returns -1 on error.

2016.1.1
Added function to remove task from schedule by function address (taskDel(task thread)).

2016.1
Initial release.