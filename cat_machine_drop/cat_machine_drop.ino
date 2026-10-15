#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

const int PIR_PIN = 2;   // HC-SR502 Output connected to Digital Pin 2
const int LED_PIN = 13;  // Built-in Arduino LED
const int SERVOMIN = 150;  // Minimum pulse length count (out of 4096) for 0 degrees
const int SERVOMAX = 600;  // Maximum pulse length count (out of 4096) for 180 degrees
const int SERVO_COUNT = 5;
const int SERVO_DROP_INTERVAL = 10000;   // minimum delay seconds between drops
const int SERVO_IDLE_ANGLE = 0;
const int SERVO_DROP_ANGLE = 45;

Adafruit_PWMServoDriver servos = Adafruit_PWMServoDriver();

int lastDrop = 0;
int nextServo = 0;

void setServoAngle(uint8_t channel, int angle) {
  int pulse = map(angle, 0, 180, SERVOMIN, SERVOMAX);

  servos.setPWM(channel, 0, pulse);
}

void setup() {
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  
  Serial.begin(115200);
  delay(1000);

  digitalWrite(LED_PIN, HIGH);

  servos.begin();
  servos.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  delay(10);

  for (int channel = 0; channel < 16; channel++) {
    setServoAngle(channel, SERVO_IDLE_ANGLE);
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
    //printStatus("Motion Detected");
    servoDrop();
    digitalWrite(LED_PIN, LOW);

    clearMotionDetector();
  } else {
    digitalWrite(LED_PIN, LOW);
    //printStatus("Motion NOT detected");
  }
  
  delay(100);
}

void servoDrop() {
  if(nextServo < SERVO_COUNT) {
    if(millis() < lastDrop + SERVO_DROP_INTERVAL) {
      printStatus("Too Recent");
    } else  {
      printStatus("Noise");

      setServoAngle(nextServo, SERVO_DROP_ANGLE);
      delay(1000);
      setServoAngle(nextServo, SERVO_IDLE_ANGLE);

      ++nextServo;

      delay(SERVO_DROP_INTERVAL);

      printStatus("Noise Done");

      lastDrop = millis();
    }
  }
}

void clearMotionDetector() {
  while (digitalRead(PIR_PIN) == HIGH) {
    printStatus("Waiting");

    delay(1000);
  }
}

void printStatus(String s) {
  Serial.println(s + " " + String(millis()) + " " + String(millis() - lastDrop) + " Current Servo: " + String(nextServo));
}

