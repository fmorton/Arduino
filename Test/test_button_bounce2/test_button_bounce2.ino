#include <Bounce2.h>

const int BUTTON_PIN = 3;
const int LED_PIN = LED_BUILTIN;

// Use Bounce2::Button instead of the legacy Bounce class
Bounce2::Button button = Bounce2::Button(); 

void setup() {
  Serial.begin(115200);
  
  // Attach pin with internal pull-up and a 25ms debounce interval
  button.attach(BUTTON_PIN, INPUT_PULLUP); 
  button.interval(25);
  
  // CRITICAL: Tell the library that because we use INPUT_PULLUP, 
  // a physical press pulls the signal LOW.
  button.setPressedState(LOW); 
  
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  button.update(); // Keeps the debounce state machine running

  // 1. Level check: Is the button currently held down?
  if (button.isPressed()) {
    digitalWrite(LED_PIN, HIGH); // Keep LED on while holding
    
    // 2. Combined check: Is it held down AND has it been held for over 2000ms?
    if (button.currentDuration() > 2000) {
      Serial.println("You have been holding the button for over 2 seconds!");
    }
  } else {
    digitalWrite(LED_PIN, LOW); // Turn LED off when released
  }
}
