#include "Arduino_LED_Matrix.h"

ArduinoLEDMatrix matrix;

// Define an 8-row by 12-column array (1 = LED on, 0 = LED off)
byte frame[8][12] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 },
  { 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 },
  { 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

void setup() {
  Serial.begin(115200); // Initializes serial communication at 9600 baud

  matrix.begin();
  matrix.renderBitmap(frame, 8, 12);

  delay(100);
}

void loop() {
  for (int row = 0; row < 8; row++) {
    for (int column = 0; column < 12; column++) {
      frame[row][column] = random(2);
    }
  }

  delay(50);

  matrix.renderBitmap(frame, 8, 12);
}
