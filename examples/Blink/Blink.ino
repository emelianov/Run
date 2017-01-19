/////////////////////////////////////////////////////////
// Run 2017.1
// Arduino simple cooperative multitask library
// (c)2017, Alexander Emelianov (a.m.emelianov@gmail.com)
//
// Simple blink example
//

// GPIO PIN to which LED is connected
#define LED1_PIN    2
// Blinking period
#define LED1_ON_OFF 1000

#include <Run.h>

// Task function that changes LED state on/off
uint32_t blink() {
 if (digitalRead(LED1_PIN) == HIGH) {
   digitalWrite(LED1_PIN, LOW);
 } else {
   digitalWrite(LED1_PIN, HIGH);
 }
// Return time to next blink() function execution back to scheduller
 return LED1_ON_OFF;
}

void setup() {
// Set GPIO mode for output
  pinMode(LED1_PIN, OUTPUT);
// Append task to run immediatly
  addTask(blink);
}

void loop() {
// Sheduller function
  taskExec();
}