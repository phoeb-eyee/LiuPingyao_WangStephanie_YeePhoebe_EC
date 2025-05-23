#include <Wire.h>  
// Include Wire library for I2C communication

#include "SparkFun_SGP40_Arduino_Library.h"  
// Include library for SGP40 sensor

#include "VOCGasIndexAlgorithm.h"  
// Include algorithm for VOC gas index calculation

SGP40 mySensor;  // Create an instance of the SGP40 sensor class
VOCGasIndexAlgorithm vocAlgorithm;  // Instance for VOC algorithm (not used directly in sketch)

void setup() {
  Serial.begin(9600);  // Start serial communication at 9600 baud (matches Java code)
  Wire.begin();  // Initialize I2C communication

  // Initialize the SGP40 sensor, halt if not found
  if (!mySensor.begin()) {
    Serial.println("SGP40 not found.");
    while (1);  // Infinite loop to halt further execution
  }
  Serial.println("SGP40 ready.");  // Confirm sensor is ready
}

void loop() {
  // Get the raw VOC index value from the sensor (16-bit unsigned integer)
  uint16_t srawVoc = mySensor.getVOCindex();

  // Send VOC index value as a line to serial port (Java reads this line)
  Serial.println(srawVoc);

  delay(1000);  // Wait 1 second before taking next reading
}
