#include <Arduino.h>
#include "MasterLedController.h"
#include "BLE.h"
#include <NeoPixelBus.h>
#include <Preferences.h>


MasterLedController &master = MasterLedController::getInstance();
BLE* bluetooth;
Preferences preferences;


void setup() {

  // Begin serial
  Serial.begin(9600);

  // Begin preferences
  preferences.begin("my-app", false);

  // Retrieve saved settings
  uint8_t primaryStartingRed = preferences.getUChar("priStarRed", 0);
  uint8_t primaryStartingGreen = preferences.getUChar("priStarGreen", 0);
  uint8_t primaryStartingBlue = preferences.getUChar("priStarBlue", 0);
  RgbColor primaryStarting = RgbColor(primaryStartingRed, primaryStartingGreen, primaryStartingBlue);

  uint8_t primaryEndingRed = preferences.getUChar("priEndRed", 0);
  uint8_t primaryEndingGreen = preferences.getUChar("priEndGreen", 0);
  uint8_t primaryEndingBlue = preferences.getUChar("priEndBlue", 0);
  RgbColor primaryEnding = RgbColor(primaryEndingRed, primaryEndingGreen, primaryEndingBlue);

  uint8_t secondaryStartingRed = preferences.getUChar("secStarRed", 0);
  uint8_t secondaryStartingGreen = preferences.getUChar("secStarGreen", 0);
  uint8_t secondaryStartingBlue = preferences.getUChar("secStarBlue", 0);
  RgbColor secondaryStarting = RgbColor(secondaryStartingRed, secondaryStartingGreen, secondaryStartingBlue);

  uint8_t secondaryEndingRed = preferences.getUChar("secondaryEndRed", 0);
  uint8_t secondaryEndingGreen = preferences.getUChar("secEndGreen", 0);
  uint8_t secondaryEndingBlue = preferences.getUChar("secEndBlue", 0);
  RgbColor secondaryEnding = RgbColor(secondaryEndingRed, secondaryEndingGreen, secondaryEndingBlue);

  uint16_t primarySpeed = preferences.getUShort("priSpeed", 50);
  uint16_t secondarySpeed = preferences.getUShort("secSpeed", 50);
  uint16_t primarySensitivity = preferences.getUShort("priSens", 50);
  uint16_t primaryNoiseFloor = preferences.getUShort("priNoiseFloor", 50);
  String protocol = preferences.getString("protocol", "NeoEsp32Rmt0Ws2811Method");
  uint8_t mode = preferences.getUChar("mode", 0);
  uint16_t numLeds = preferences.getUShort("numLeds", 150);


  // Initialize MasterLedController
  master.setProtocol(&protocol);
  master.loadParams(&primaryStarting, &primaryEnding, &primarySpeed, &primarySensitivity, &primaryNoiseFloor, &secondaryStarting, &secondaryEnding, &secondarySpeed, &mode, &numLeds);

  // Print initialized MasterLedController parameters
  master.printParameters();

  // Instantiate BLE object
  bluetooth = new BLE();

  // Update pointer to controller
  bluetooth->updateController(&master);

  // Set BLE values for our initialized parameters
  bluetooth->sendParameters();

}


void loop() {

  // If we have new BLE parameters, then update the parameters
  bluetooth->checkUpdateFlag();
  if (bluetooth->updateFlag) {
      bluetooth->updateParameters();
      master.printParameters();                   // Can also use bluetooth.printParameters();
      bluetooth->saveParameters(&preferences);
  }

  // Calculate pixel values and show the results on the strip
  master.doStuff();

}