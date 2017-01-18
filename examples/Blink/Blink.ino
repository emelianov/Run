/////////////////////////////////////////////////////////
// Run 2016.2
// Arduino simple cooperative multitask library
// (c)2017, Alexander Emelianov (a.m.emelianov@gmail.com)
//
// Simple blink example
//

#define LED1_GPIO   4          // GPIO pin to which LED is connected
#define LED1_ON_OFF 1000       // Blinking period

#define MAX_TASKS 4            // Decrease maximum task count from default value
#include <Run.h>

uint32_t blink() {             // Task function that changes LED state on/off
 if (digitalRead(LED1_GPIO) == HIGH) {
   digitalWrite(LED1_GPIO, LOW);
 } else {
   digitalWrite(LED1_GPIO, HIGH);
 }
 return LED1_ON_OFF;           // Return time to next blink() function execution back to scheduller
}

void setup() {
  pinMode(LED1_GPIO, OUTPUT);  // Set GPIO mode for output
  addTask(blink);              // Append task to run immediatly
}

void loop() {
  taskExec();                  // Sheduller function
}