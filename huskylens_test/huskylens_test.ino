#include <Wire.h>
#include "DFRobot_HuskylensV2.h"
#include <Servo.h>

// Instantiate the updated V2 object class
HuskylensV2 huskylens;
Servo servo1;

void setup() {
    Serial.begin(115200);
    delay(1000);

    Wire1.begin();

    // Initialize communication via I2C
    while (!huskylens.begin(Wire1)) {
        Serial.println("HuskyLens 2 initialization failed! Verify I2C wiring and screen protocol setting.");
        delay(1000);
    }

    Serial.println("HuskyLens 2 initialized successfully.");

    huskylens.switchAlgorithm(ALGORITHM_OBJECT_RECOGNITION);

    servo1.attach(9);
    servo1.write(90);
}

void loop() {
    huskylens.getResult(ALGORITHM_OBJECT_RECOGNITION);

    if (huskylens.available(ALGORITHM_OBJECT_RECOGNITION)) {
        int totalObjects = huskylens.getCachedResultNum(ALGORITHM_OBJECT_RECOGNITION);

        for (int i = 0; i < totalObjects; i++) {
            Result *result = (Result *)huskylens.getCachedResultByIndex(ALGORITHM_OBJECT_RECOGNITION, i);
      
            if (result != NULL) {
                int objectID = RET_ITEM_NUM(result, Result, ID);
                //int xCenter  = RET_ITEM_NUM(result, Result, xCenter);
                //int yCenter  = RET_ITEM_NUM(result, Result, yCenter);
                //int width    = RET_ITEM_NUM(result, Result, width);
                //int height   = RET_ITEM_NUM(result, Result, height);
        
                if(objectID == 1) {
                    Serial.print("NEW CAT -> [Item "); Serial.print(i + 1); Serial.print("] ");
                    Serial.println("Object ID: "); Serial.print(objectID);
                }
            }
        }
    }

    delay(200);
}
