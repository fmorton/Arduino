#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

const int DEBUGGING = true;
const int PIR_PIN = 2;   // HC-SR502 Output connected to Digital Pin 2
const int LED_PIN = 13;  // Built-in Arduino LED
const int SERVOMIN = 150;  // Minimum pulse length count (out of 4096) for 0 degrees
const int SERVOMAX = 600;  // Maximum pulse length count (out of 4096) for 180 degrees
const int SERVO_COUNT = 4;
const int SERVO_SWING_DEGREES = 20;
const int SERVO_SWING_COUNT = 6;
const int SERVO_SWING_DELAY = 125;
const int SERVO_NOISE_DELAY_AFTER_MOTION = 3000;
const int SERVO_IDLE_ANGLE = 90;
const int SERVO_MOTION_QUIET = 10000;

Adafruit_PWMServoDriver servos = Adafruit_PWMServoDriver();

void setup() {
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  
  Serial.begin(115200);
  delay(1000);

  digitalWrite(LED_PIN, HIGH);

  servos.begin();
  servos.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  delay(10);

  servoPosition(SERVO_IDLE_ANGLE);

  if(DEBUGGING) {
    servoNoise();
  }

  printStatus("Initializing sensor");
  delay(30000); 
  printStatus("Sensor ready");

  clearMotionDetector();
}

void loop() {
  int motionDetected = digitalRead(PIR_PIN);

  if (motionDetected == HIGH) {
    digitalWrite(LED_PIN, HIGH);

    printStatus("Motion Detected");

    servoNoise();

    digitalWrite(LED_PIN, LOW);

    clearMotionDetector();
  } else {
    digitalWrite(LED_PIN, LOW);

    printStatus("Motion NOT detected");
  }
  
  delay(100);
}

void servoNoise() {
  delay(SERVO_NOISE_DELAY_AFTER_MOTION);

  for (int count = 0; count < SERVO_SWING_COUNT; count++) {
    servoPosition(SERVO_IDLE_ANGLE + SERVO_SWING_DEGREES);
    delay(SERVO_SWING_DELAY);
    servoPosition(SERVO_IDLE_ANGLE - SERVO_SWING_DEGREES);
    delay(SERVO_SWING_DELAY);
  }

  servoPosition(SERVO_IDLE_ANGLE);

  printStatus("Motion Disabled");
  delay(SERVO_MOTION_QUIET);
  printStatus("Motion Enabled");
}

void servoPosition(int angle) {
  for (int servo = 0; servo < SERVO_COUNT; servo++) {
    setServoAngle(servo, angle);
  }
}

void setServoAngle(uint8_t channel, int angle) {
  int pulse = map(angle, 0, 180, SERVOMIN, SERVOMAX);

  servos.setPWM(channel, 0, pulse);
}

void clearMotionDetector() {
  while (digitalRead(PIR_PIN) == HIGH) {
    printStatus("Waiting");

    delay(1000);
  }
}

void printStatus(String s) {
  if(DEBUGGING) Serial.println(s + " " + String(millis()));
}
