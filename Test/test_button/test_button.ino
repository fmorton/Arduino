const int buttonPin = 3; // The number of the pushbutton pin

void setup() {
  // Initialize serial communication for debugging output
  Serial.begin(115200);
  
  // Initialize pin 3 as an input with an internal pull-up resistor
  // When the button is unpressed, it reads HIGH
  // When the button is pressed, it connects to ground and reads LOW
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  // Read the state of the button
  int buttonState = digitalRead(buttonPin);

  // Check if the button is pressed (LOW due to INPUT_PULLUP)
  if (buttonState == LOW) {
    Serial.println("Button is pressed!");
  } else {
    Serial.println("Button is released.");
  }

  // Small delay to prevent flooding the serial monitor
  delay(100);
}
