/////////////////////////////////////////////////////////
// Run 2016.2
// Arduino simple cooperative multitask library
// (c)2017, Alexander Emelianov (a.m.emelianov@gmail.com)
//
// Simple blink example
//

#define LED1_GPIO   4
#define LED1_ON_OFF 1000

#define LED2_GPIO 0
#define LED2_ON   1000
#define LED2_OFF  1000

#define MAX_TASKS 8
#include <Run.h>

uint32_t led1Blink() {
 if (digitalRead(LED1_GPIO) == HIGH) {
   digitalWrite(LED1_GPIO, LOW);
 } else {
   digitalWrite(LED1_GPIO, HIGH);
 }
 return LED1_ON_OFF;
}

uint32_t led2On() {
 return 0;
}

void setup() {
  addTask(taskToRun);
}
void loop() {
  taskExec();
}