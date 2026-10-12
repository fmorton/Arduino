#include <Servo.h>

Servo servo1;  // Create a servo object to control your motor
int servoPin = 9; // Define the digital pin connected to the signal wire

void setup() {
  servo1.attach(servoPin);
}

void loop() {
  for (int pos = 0; pos <= 180; pos += 1) { 
    servo1.write(pos);
    delay(20);
  }
}
