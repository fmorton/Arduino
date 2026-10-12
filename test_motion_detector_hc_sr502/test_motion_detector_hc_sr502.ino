const int PIR_PIN = 2;   // HC-SR502 Output connected to Digital Pin 2
const int LED_PIN = 13;  // Built-in Arduino LED

void setup() {
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  
  Serial.begin(115200);
  
  // Note: PIR sensors require 30-60 seconds to calibrate when powered on.
  delay(1000); 
  digitalWrite(LED_PIN, HIGH);
  Serial.println("Initializing sensor... Please wait.");
  delay(30000); 
  Serial.println("Sensor ready!");
}

void loop() {
  int motionDetected = digitalRead(PIR_PIN);

  if (motionDetected == HIGH) {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("Motion Detected!");
  } else {
    digitalWrite(LED_PIN, LOW);
    Serial.println("Motion NOT Detected!");
  }
  
  delay(100);
}
