#include <Arduino.h>
#include "MasterLedController.h"
#include "BLE.h"


MasterLedController &master = MasterLedController::getInstance();
BLE* bluetooth;

void setup() {

  Serial.begin(9600);

  bluetooth = new BLE();

  // Update pointer to controller
  bluetooth->updateController(&master);

}

void loop() {

  bluetooth->checkUpdateFlag();
  if (bluetooth->updateFlag) {
      bluetooth->updateParameters();
      bluetooth->printParameters();
  }

  master.doStuff();

}
