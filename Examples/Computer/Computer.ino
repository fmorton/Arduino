#include "Arduino_LED_Matrix.h"

ArduinoLEDMatrix matrix;

byte led_matrix[8][12];

void setup() {
  Serial.begin(115200);

  matrix.begin();
}

void loop() {
  for (int row = 0; row < 8; row++) {
    for (int column = 0; column < 12; column++) {
      led_matrix[row][column] = random(2);
    }
  }

  delay(150);

  matrix.renderBitmap(led_matrix, 8, 12);
}
