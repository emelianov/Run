/////////////////////////////////////////////////////////
// Run 2017.1
// Arduino simple cooperative multitask library
// (c)2017, Alexander Emelianov (a.m.emelianov@gmail.com)
//
// Semaphore blink example
//

#define LED1_PIN    4
#define LED1_ON_OFF 500

#define LED2_PIN    0
#define LED2_ON     2000
#define LED2_OFF    1000

#define RUN_TASKS   4
#include <Run.h>

// Semaphore to control main tasks do not run until initialization completes
bool initialized = false;

//LED 1 blinking task
uint32_t led1Blink() {
 if (digitalRead(LED1_GPIO) == HIGH) {
   digitalWrite(LED1_GPIO, LOW);
 } else {
   digitalWrite(LED1_GPIO, HIGH);
 }
// Return time to next run of current task (mSec)
 return LED1_ON_OFF;
}

// LED 2 turn ON task
uint32_t led2On() {
 digitalWrite(LED2_GPIO, LOW);
// Add task to LED 2 off in LED2_ON mSec
 taskAdd(ledOff, LED2_ON);
// Return RUN_DELETE caused task to be removed from sheduller queue
 return RUN_DELETE;
}

// LED 2 turn OFF task
uint32_t led2Off() {
 digitalWrite(LED2_GPIO, HIGH);
 taskAdd(ledOff, LED2_OFF);
 return RUN_DELETE;
}

// Initialize digital PINs
uint32_t ledInit() {
 pinMode(LED1_PIN, OUTPUT);
 pinMode(LED2_PIN, OUTPUT);
// Set semaphore indicates initialization complete
// Semaphore will be reset to false by scheduller after all waiting tasks completes
 initialized = true;
 return RUN_DELETE;
}

void setup() {
// Add task that will wait until semaphore become true
  addTaskWithSemaphore(led1Blink, initialized);
// Add task that will wait until semaphore become true
  addTaskWithSemaphore(led2On, initialized);
// Perform initialization as task
  addTask(ledInit);
}

void loop() {
// Execute scheduller forever
  taskExec();
}