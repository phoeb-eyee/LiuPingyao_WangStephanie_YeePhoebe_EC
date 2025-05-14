#include <Wire.h>
#include "SparkFun_SGP40_Arduino_Library.h"
#include "VOCGasIndexAlgorithm.h"

SGP40 mySensor;
VOCGasIndexAlgorithm vocAlgorithm;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  if (!mySensor.begin()) {
    Serial.println("SGP40 not found.");
    while (1);
  }
  Serial.println("SGP40 ready.");
}

void loop() {
  // Replace with real humidity and temperature if needed
  uint16_t srawVoc = mySensor.getVOCindex();
  Serial.println(srawVoc);  // Send to Java
  delay(1000);
}
