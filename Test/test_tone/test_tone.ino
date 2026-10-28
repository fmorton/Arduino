const int BUZZER_PIN = A3; 

void setup() {
  // Configure A3 as a digital output pin
  pinMode(BUZZER_PIN, OUTPUT); 
}

void loop() {
  // Play a 440 Hz tone (Musical note A4) on pin A3 for 500 milliseconds
  tone(A3, 440, 500); 
  
  // Wait 1 second before playing it again
  delay(1000);            
}
