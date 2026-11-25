#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Bounce2.h>

// debug settings
const bool DEBUGGING = false;

// pin assignments
const uint8_t MOTION_DETECTOR_PIN = 2;   // hc-sr502 pir output
const uint8_t BUTTON_PIN          = 3;   // toggle switch (active low)
const uint8_t BUTTON_LED_PIN      = 4;   // button status led (active low)
const uint8_t LED_PIN             = 13;  // built-in status led

// servo calibration & geometry
const int SERVOMIN            = 102;     // pulse count for 0 degrees (50 hz)
const int SERVOMAX            = 512;     // pulse count for 180 degrees (50 hz)
const int SERVO_COUNT         = 16;      // number of active pca9685 channels
const int SERVO_IDLE_ANGLE    = 90;      // resting center angle
const int SERVO_SWING_DEGREES = 20;      // +/- degrees from idle during swing
const int SERVO_SWING_COUNT   = 6;       // number of back-and-forth cycles

// timing constants (milliseconds)
const unsigned long MOTION_WARMUP                  = 30000;
const unsigned long SERVO_NOISE_DELAY_AFTER_MOTION = 3000;
const unsigned long SERVO_SWING_DELAY              = 125;
const unsigned long SERVO_DISABLE_DELAY            = 250;
const unsigned long SERVO_MOTION_QUIET             = 10000;

// hardware & state objects
Adafruit_PWMServoDriver servos = Adafruit_PWMServoDriver();
Bounce2::Button button = Bounce2::Button();

bool catMachineActive = false;
unsigned long stayQuietUntil = 0;

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
  servos.setPWMFreq(50);
  delay(10);

  disableServos();

  printStatus("Initializing sensor");
  delay(MOTION_WARMUP);
  printStatus("Sensor ready");

  digitalWrite(LED_PIN, LOW);

  clearMotionDetector();
}

void loop() {
  checkButtonState();

  if (catMachineActive) {
    if ((stayQuietUntil != 0) && (stayQuietUntil > millis())) {
      printStatus("Cat Machine Is Quiet " + String(millis()) + " " + String(stayQuietUntil));
    } else {
      stayQuietUntil = 0;
      checkForMotion();
    }

    delay(100);
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

  if (button.fell()) {
    catMachineActive = !catMachineActive;
  }

  if (catMachineActive) {
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
  disableServos();

  stayQuietUntil = millis() + SERVO_MOTION_QUIET;
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

void disableServos() {
  delay(SERVO_DISABLE_DELAY);

  for (int servo = 0; servo < SERVO_COUNT; servo++) {
    servos.setPWM(servo, 0, 0);
  }
}

void printStatus(String s) {
  if (DEBUGGING) {
    Serial.println(s + " " + String(millis()));
  }
}
