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
  Serial.println("Updating controller: ");
  bluetooth->updateController(&master);

}

void loop() {

  Serial.println("Looping...");
  delay(500);

  Serial.println("Updating parameters...");
  bluetooth->updateParameters();
  if (bluetooth->updateFlag) {
      Serial.println("Printing parameters...");
      bluetooth->printParameters();
  }

  Serial.println("About to do stuff: ");
  master.doStuff();

  // Serial.print("Protocol (loop): ");
  // Serial.println(master.protocol);

}
