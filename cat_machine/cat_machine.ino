#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Bounce2.h>

const int DEBUGGING = false;
const int MOTION_DETECTOR_PIN = 2;   // HC-SR502 Output connected to Digital Pin 2
const int BUTTON_PIN = 3;
const int BUTTON_LED_PIN = 4;
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
Bounce2::Button button = Bounce2::Button();

int cat_machine_active = false;
bool turned_on = false;

void setup() {
  pinMode(MOTION_DETECTOR_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUTTON_LED_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  button.attach(BUTTON_PIN, INPUT_PULLUP);
  button.interval(50);
  button.setPressedState(LOW);

  Serial.begin(115200);
  delay(1000);

  digitalWrite(LED_PIN, HIGH);

  servos.begin();
  servos.setPWMFreq(60);
  delay(10);

  servoPosition(SERVO_IDLE_ANGLE);

  printStatus("Initializing sensor");
  delay(30000); 
  printStatus("Sensor ready");

  clearMotionDetector();
}

void loop() {
  checkButtonState();

  if(cat_machine_active) {
    checkForMotion();
  }
}

void checkForMotion() {
  int motionDetected = digitalRead(MOTION_DETECTOR_PIN);

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

void checkButtonState() {
  button.update();
  
  if (button.fell()) cat_machine_active = !cat_machine_active;

  if(cat_machine_active) {
    digitalWrite(BUTTON_LED_PIN, LOW);
  } else {
    digitalWrite(BUTTON_LED_PIN, HIGH);
  }
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
  while (digitalRead(MOTION_DETECTOR_PIN) == HIGH) {
    printStatus("Waiting");

    delay(1000);
  }
}

void printStatus(String s) {
  if(DEBUGGING) Serial.println(s + " " + String(millis()));
}
