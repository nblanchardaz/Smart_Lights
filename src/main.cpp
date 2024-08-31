#include <Arduino.h>
#include "MasterLedController.h"
#include "BLE.h"


MasterLedController &master = MasterLedController::getInstance();
BLE* bluetooth;

void setup() {
  Serial.begin(9600);
  delay(1000);
  Serial.print("Entering setup...");

  bluetooth = new BLE();

  // Update pointer to controller
  bluetooth->updateController(&master);

}

void loop() {

  // Serial.println("Looping...");
  delay(1000);

  bluetooth->updateParameters();
  bluetooth->printParameters();
  // master.loop();

}
