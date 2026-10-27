#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Initialize the driver using the default I2C address (0x40)
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Calibration values: Adjust these values so your servo doesn't strain at the ends!
#define SERVOMIN  150  // Minimum pulse length count (out of 4096) for 0 degrees
#define SERVOMAX  600  // Maximum pulse length count (out of 4096) for 180 degrees

// Specify the port your servo is plugged into (0 through 15)
const int servoPort = 1; 

void setup() {
  Serial.begin(115200);
  delay(250);
  Serial.println("16-Channel Servo Shield Test");

  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  delay(10);
}

// Helper function to write angles (0-180) to a specific channel
void setServoAngle(uint8_t channel, int angle) {
  // Map degrees (0-180) to pulse lengths (SERVOMIN to SERVOMAX)
  int pulse = map(angle, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(channel, 0, pulse);
}

void loop() {
  // Sweep from 0 to 180 degrees
  //for (int angle = 0; angle <= 180; angle += 1) {
  //  setServoAngle(servoPort, angle);
  //  delay(15);
  //}
  
  //delay(500);

  // Sweep from 180 to 0 degrees
  //for (int angle = 180; angle >= 0; angle -= 1) {
  //  setServoAngle(servoPort, angle);
  //  delay(15);
  //}

  for (int rattle = 0; rattle <= 4; rattle += 1) {
    int rattle_delay = 1000;

    setServoAngle(servoPort, 70);
    delay(rattle_delay);
    setServoAngle(servoPort, 110);
    delay(rattle_delay);
    Serial.println(String(rattle));
  }

  delay(500);
}
