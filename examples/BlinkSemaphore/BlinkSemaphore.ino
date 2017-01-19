/////////////////////////////////////////////////////////
// Run 2017.1
// Arduino simple cooperative multitask library
// (c)2017, Alexander Emelianov (a.m.emelianov@gmail.com)
//
// Semaphore blink example
// Demonstrates different type of tasks:
// 1. Generic periodical task that blinking LED. Task starts only after initialization task
//    set semaphore.
// 2. Two tasks that swaps eachother to blinking LED with different ON and OFF timings.
//    Task starts only after initialization task set semaphore.
// 3. Initialization task that runs only once and set semaphore to start blinking tasks
//


// LED 1 control PIN. Default is ESP-12 building LED PIN
#define LED1_PIN    2
#define LED1_ON_OFF 500

// LED 1 control PIN. Default is NodeMCU LED PIN
#define LED2_PIN    16
#define LED2_ON     2000
#define LED2_OFF    1000

#include <Run.h>

// Semaphore to control main tasks do not run until initialization completes
bool initialized = false;

//LED 1 blinking task
uint32_t led1Blink() {
 if (digitalRead(LED1_PIN) == HIGH) {
   digitalWrite(LED1_PIN, LOW);
 } else {
   digitalWrite(LED1_PIN, HIGH);
 }
// Return time to next run of current task (mSec)
// So as result task will be executed every LED1_ON_OFF mSeconds
 return LED1_ON_OFF;
}

// LED 2 turn ON task
uint32_t led2On() {
 digitalWrite(LED2_PIN, LOW);
// Add task to LED 2 off in LED2_ON mSec
 taskAddWithDelay(led2Off, LED2_ON);
// Return RUN_DELETE caused task to be removed from sheduller queue
 return RUN_DELETE;
}

// LED 2 turn OFF task
uint32_t led2Off() {
 digitalWrite(LED2_PIN, HIGH);
// Add task to LED 2 off in LED2_ON mSec
 taskAddWithDelay(led2On, LED2_OFF);
// Return RUN_DELETE caused task to be removed from sheduller queue
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
  taskAddWithSemaphore(led1Blink, &initialized);
// Add task that will wait until semaphore become true
  taskAddWithSemaphore(led2On, &initialized);
// Perform initialization as task
  taskAdd(ledInit);
}

void loop() {
// Execute scheduller forever
  taskExec();
}