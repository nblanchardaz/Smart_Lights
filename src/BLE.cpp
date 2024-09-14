// Custom classes ServerCallbacks and CharacteristicCallbacks originally from the documentation
// for the NimBLE-Arduino library at: https://github.com/h2zero/NimBLE-Arduino/blob/master/examples/NimBLE_Server/NimBLE_Server.ino
// Additional code also taken from this documentation.


#include "BLE.h"

// Constructor
BLE::BLE() {
    
    // Make sure serial is available
    Serial.begin(9600);

    // Initialize variables
    primaryStartingColor = RgbColor(0, 0, 0);
    primaryEndingColor = RgbColor(0, 0, 0);
    primarySpeed = 0;
    primaryNoiseFloor = 0;
    primarySensitivity = 0;
    primaryWaveSpeed = 0;
    primaryWavePeriod = 0;
    secondaryStartingColor = RgbColor(0, 0, 0);
    secondaryEndingColor = RgbColor(0, 0, 0);
    protocol = "NeoEsp32Rmt0Ws2811Method";
    secondarySpeed = 0;
    primarySensitivity = 0;
    updateFlag = 0;

    // DEBUG
    Serial.println("Starting BLE Initialization...");

    // Setup BLE device, service, and characteristics
    NimBLEDevice::init(DEVICE_NAME);
    NimBLEDevice::setPower(ESP_PWR_LVL_P9);
    NimBLEDevice::setSecurityAuth(false, false, false);
    pServer = NimBLEDevice::createServer();
    pService = pServer->createService(SERVICE_UUID);
    primaryStartingColorCharacteristic = pService->createCharacteristic(primaryStartingUUID, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE| NIMBLE_PROPERTY::NOTIFY);
    primaryEndingColorCharacteristic = pService->createCharacteristic(primaryEndingUUID, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::NOTIFY);
    primarySpeedCharacteristic = pService->createCharacteristic(primarySpeedUUID, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::NOTIFY);
    primarySensitivityCharacteristic = pService->createCharacteristic(primarySensitivityUUID, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::NOTIFY);
    primaryNoiseFloorCharacteristic = pService->createCharacteristic(primaryNoiseFloorUUID, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::NOTIFY);
    primaryWaveSpeedCharacteristic = pService->createCharacteristic(primaryWaveSpeedUUID, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::NOTIFY);
    primaryWavePeriodCharacteristic = pService->createCharacteristic(primaryWavePeriodUUID, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::NOTIFY);
    secondaryStartingColorCharacteristic = pService->createCharacteristic(secondaryStartingUUID, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::NOTIFY);
    secondaryEndingColorCharacteristic = pService->createCharacteristic(secondaryEndingUUID, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::NOTIFY);
    secondarySpeedCharacteristic = pService->createCharacteristic(secondarySpeedUUID, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::NOTIFY);
    protocolCharacteristic = pService->createCharacteristic(protocolUUID, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::NOTIFY);
    updateFlagCharacteristic = pService->createCharacteristic(updateFlagUUID, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::NOTIFY);
    firmwareCharacteristic = pService->createCharacteristic(firmwareVersionUUID, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::NOTIFY);
    modeCharacteristic = pService->createCharacteristic(modeUUID, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::NOTIFY);
    stripLengthCharacteristic = pService->createCharacteristic(stripLengthUUID, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::NOTIFY);

    // Set callbacks (UNUSED)
    // pServer->setCallbacks(new ServerCallbacks());
    // primaryStartingColorCharacteristic->setCallbacks(&pscc);
    // primaryEndingColorCharacteristic->setCallbacks(&pecc);
    // primarySpeedCharacteristic->setCallbacks(&psc);
    // secondaryStartingColorCharacteristic->setCallbacks(&sscc);
    // secondaryEndingColorCharacteristic->setCallbacks(&secc);
    // secondarySpeedCharacteristic->setCallbacks(&ssc);
    // protocolCharacteristic->setCallbacks(&pc);

    // DEBUG
    Serial.println("BLE Characteristics created...");

    // Start BLE service
    pService->start();

    // Set protocol characteristic
    protocolCharacteristic->setValue(1);

    // Set firmware version
    firmwareCharacteristic->setValue(FIRMWARE_VERSION);

    // Start BLE advertising
    pAdvertising = NimBLEDevice::getAdvertising();
    if (pAdvertising == NULL) {
        Serial.println("pAdvertising is null.");
    }
    else {
        Serial.println("pAdvertising OK. Adding service UUID...");
        pAdvertising->addServiceUUID(pService->getUUID());
        Serial.println("Setting scan response...");
        pAdvertising->setScanResponse(true);
        Serial.println("Starting advertising...");
        pAdvertising->start();
        Serial.println("Advertising started.");
    }
}

// Update BLE parameters to what the current characteristic values are.
void BLE::updateParameters() {

    uint32_t temp;
    uint8_t red;
    uint8_t green;
    uint8_t blue;

    // Primary starting color
    temp = this->primaryStartingColorCharacteristic->getValue<uint32_t>(nullptr, true);
    red = temp & 0xFF;
    green = (temp >> 8) & 0xFF;
    blue = (temp >> 16) & 0xFF;
    this->primaryStartingColor = RgbColor(red, green, blue);

    // Primary ending color
    temp = this->primaryEndingColorCharacteristic->getValue<uint32_t>(nullptr, true);
    red = temp & 0xFF;
    green = (temp >> 8) & 0xFF;
    blue = (temp >> 16) & 0xFF;
    this->primaryEndingColor = RgbColor(red, green, blue);

    // Primary velocity
    this->primarySpeed = this->primarySpeedCharacteristic->getValue<uint16_t>(nullptr, true);

    // Primary sensitivity
    this->primarySensitivity = this->primarySensitivityCharacteristic->getValue<uint16_t>(nullptr, true);

    // Primary noise floor
    this->primaryNoiseFloor = this->primaryNoiseFloorCharacteristic->getValue<uint16_t>(nullptr, true);

    // Primary wave speed
    this->primaryWaveSpeed = this->primaryWaveSpeedCharacteristic->getValue<uint16_t>(nullptr, true);

    // Primary wave period
    this->primaryWavePeriod = this->primaryWavePeriodCharacteristic->getValue<uint16_t>(nullptr, true);

    // Secondary starting color
    temp = this->secondaryStartingColorCharacteristic->getValue<uint32_t>(nullptr, true);
    red = temp & 0xFF;
    green = (temp >> 8) & 0xFF;
    blue = (temp >> 16) & 0xFF;
    this->secondaryStartingColor = RgbColor(red, green, blue);

    // Secondary ending color
    temp = this->secondaryEndingColorCharacteristic->getValue<uint32_t>(nullptr, true);
    red = temp & 0xFF;
    green = (temp >> 8) & 0xFF;
    blue = (temp >> 16) & 0xFF;
    this->secondaryEndingColor = RgbColor(red, green, blue);

    // Secondary velocity
    this->secondarySpeed = this->secondarySpeedCharacteristic->getValue<uint16_t>(nullptr, true);

    // Protocol
    this->protocol = protocols_arr[this->protocolCharacteristic->getValue<uint8_t>(nullptr, true)];

    // Strip length
    this->numLeds = this->stripLengthCharacteristic->getValue<uint16_t>(nullptr, true);

    // Mode
    this->mode = this->modeCharacteristic->getValue<uint8_t>(nullptr, true);

    // Update parameters stored in the appropriate LED segment variable
    this->updateExternParameters();

}

// Check to see if the BLE update flag is set.
int BLE::checkUpdateFlag() {

    this->updateFlag = this->updateFlagCharacteristic->getValue<bool>(nullptr, true);
    this->updateFlagCharacteristic->setValue(0);

    return this->updateFlag;
}

// Update parameters stored in the MasterLedController (and subsequent LedStripSegment objects) to match what is stored in the BLE object.
void BLE::updateExternParameters() {

    // First, update protocol
    if (this->controller) {

        // DEBUG
        // Serial.println("Updated protocol to: " + String(this->protocol));
        this->controller->protocol = this->protocol;

        // Then, update the parameters of the strip object corresponding to the correct protocol
        if (this->controller->protocol == "NeoEsp32Rmt0Ws2812xMethod") {
            this->controller->addWs2812x();
            this->controller->segment_12->setParameters(this->primaryStartingColor, this->primaryEndingColor, this->primarySensitivity, this->secondaryStartingColor, this->secondaryEndingColor, this->primarySpeed, this->secondarySpeed, this->protocol, this->primaryNoiseFloor, this->mode, this->numLeds, this->primaryWaveSpeed, this->primaryWavePeriod);
        }
        else if (this->controller->protocol == "NeoEsp32Rmt0Ws2811Method") {
            this->controller->addWs2811();
            this->controller->segment_11->setParameters(this->primaryStartingColor, this->primaryEndingColor, this->primarySensitivity, this->secondaryStartingColor, this->secondaryEndingColor, this->primarySpeed, this->secondarySpeed, this->protocol, this->primaryNoiseFloor, this->mode, this->numLeds, this->primaryWaveSpeed, this->primaryWavePeriod);
        }
    }
    return;
}

// Print BLE parameters to the serial port.
void BLE::printParameters() {

    Serial.println("Primary Start Color: " + String(this->primaryStartingColor[0]) + " " + String(this->primaryStartingColor[1]) + " " + String(this->primaryStartingColor[2]) + " ");
    Serial.println("Primary End Color: " + String(this->primaryEndingColor[0]) + " " + String(this->primaryEndingColor[1]) + " " + String(this->primaryEndingColor[2]) + " ");
    Serial.println("Primary Vel: " + String(this->primarySpeed) + " ");
    Serial.println("Primary Sens: " + String(this->primarySensitivity) + " ");
    Serial.println("Primary Noise Floor: " + String(this->primaryNoiseFloor) + " ");
    Serial.println("Primary Wave Speed: " + String(this->primaryWaveSpeed) + " ");
    Serial.println("Primary Wave Period: " + String(this->primaryWavePeriod) + " ");
    Serial.println("Secondary Start Color: " + String(this->secondaryStartingColor[0]) + " " + String(this->secondaryStartingColor[1]) + " " + String(this->secondaryStartingColor[2]) + " ");
    Serial.println("Starting End Color: " + String(this->secondaryEndingColor[0]) + " " + String(this->secondaryEndingColor[1]) + " " + String(this->secondaryEndingColor[2]) + " ");
    Serial.println("Secondary Vel: " + String(this->secondarySpeed) + " ");
    Serial.println("Protocol: " + String(this->protocol));
    Serial.println("Mode: " + String(this->mode));
    Serial.println("Num LEDs: " + String(this->numLeds));
    Serial.println("");

}

// Save strip parameters to flash memory.
int BLE::saveParameters(Preferences *preferences) {

    preferences->putUChar("priStarRed", this->primaryStartingColor[0]);
    preferences->putUChar("priStarGreen", this->primaryStartingColor[1]);
    preferences->putUChar("priStarBlue", this->primaryStartingColor[2]);

    preferences->putUChar("priEndRed", this->primaryEndingColor[0]);
    preferences->putUChar("priEndGreen", this->primaryEndingColor[1]);
    preferences->putUChar("priEndBlue", this->primaryEndingColor[2]);

    preferences->putUChar("secStarRed", this->secondaryStartingColor[0]);
    preferences->putUChar("secStarGreen", this->secondaryStartingColor[1]);
    preferences->putUChar("secStarBlue", this->secondaryStartingColor[2]);

    preferences->putUChar("secEndRed", this->secondaryEndingColor[0]);
    preferences->putUChar("secEndGreen", this->secondaryEndingColor[1]);
    preferences->putUChar("secEndBlue", this->secondaryEndingColor[2]);

    preferences->putUShort("priSpeed", this->primarySpeed);
    preferences->putUShort("secSpeed", this->secondarySpeed);
    preferences->putUShort("priSens", this->primarySensitivity);
    preferences->putUShort("priNoiseFloor", this->primaryNoiseFloor);
    preferences->putUShort("priWaveSpeed", this->primaryWaveSpeed);
    preferences->putUShort("priWavePeriod", this->primaryWavePeriod);
    preferences->putString("protocol", this->protocol);
    preferences->putUChar("mode", this->mode);
    preferences->putUShort("numLeds", this->numLeds);

    return 0;

}

// Update BLE characteristics with the values BLE is initialized to.
void BLE::sendParameters() {

    std::vector<uint8_t> tempColor(4, 0x00);
    tempColor[3] = 0x00;

    if (this->controller->protocol == "NeoEsp32Rmt0Ws2812xMethod") {
        // Top selectors
        this->stripLengthCharacteristic->setValue(this->controller->segment_12->getNumLeds());
        this->modeCharacteristic->setValue(this->controller->segment_12->getMode());
        // Protocol value of 1 means WS2811, 0 means WS2812x
        this->protocolCharacteristic->setValue(this->controller->protocol == "NeoEsp32Rmt0Ws2811Method");

        // Primary colors
        tempColor[2] = (this->controller->segment_12->getPrimaryStartingColor()[2]);
        tempColor[1] = (this->controller->segment_12->getPrimaryStartingColor()[1]);
        tempColor[0] = (this->controller->segment_12->getPrimaryStartingColor()[0]);
        this->primaryStartingColorCharacteristic->setValue(tempColor);
        tempColor[2] = (this->controller->segment_12->getPrimaryEndingColor()[2]);
        tempColor[1] = (this->controller->segment_12->getPrimaryEndingColor()[1]);
        tempColor[0] = (this->controller->segment_12->getPrimaryEndingColor()[0]);
        this->primaryEndingColorCharacteristic->setValue(tempColor);

        // Other primary parameters
        this->primarySpeedCharacteristic->setValue(this->controller->segment_12->getPrimarySpeed());
        this->primarySensitivityCharacteristic->setValue(this->controller->segment_12->getPrimarySensitivity());
        this->primaryNoiseFloorCharacteristic->setValue(this->controller->segment_12->getPrimaryNoiseFloor());
        this->primaryWaveSpeedCharacteristic->setValue(this->controller->segment_12->getPrimaryWaveSpeed());
        this->primaryWavePeriodCharacteristic->setValue(this->controller->segment_12->getPrimaryWavePeriod());
   
        // Secondary colors
        tempColor[2] = (this->controller->segment_12->getSecondaryStartingColor()[2]);
        tempColor[1] = (this->controller->segment_12->getSecondaryStartingColor()[1]);
        tempColor[0] = (this->controller->segment_12->getSecondaryStartingColor()[0]);
        this->secondaryStartingColorCharacteristic->setValue(tempColor);
        tempColor[2] = (this->controller->segment_12->getSecondaryEndingColor()[2]);
        tempColor[1] = (this->controller->segment_12->getSecondaryEndingColor()[1]);
        tempColor[0] = (this->controller->segment_12->getSecondaryEndingColor()[0]);
        this->secondaryEndingColorCharacteristic->setValue(tempColor);

        // Other secondary parameters
        this->secondarySpeedCharacteristic->setValue(this->controller->segment_12->getSecondarySpeed());
    }
    else if (this->controller->protocol == "NeoEsp32Rmt0Ws2811Method") {
        // Top selectors
        this->stripLengthCharacteristic->setValue(this->controller->segment_11->getNumLeds());
        this->modeCharacteristic->setValue(this->controller->segment_11->getMode());
        this->protocolCharacteristic->setValue(this->controller->protocol == "NeoEsp32Rmt0Ws2811Method");

        // Primary colors
        tempColor[2] = (this->controller->segment_11->getPrimaryStartingColor()[2]);
        tempColor[1] = (this->controller->segment_11->getPrimaryStartingColor()[1]);
        tempColor[0] = (this->controller->segment_11->getPrimaryStartingColor()[0]);
        this->primaryStartingColorCharacteristic->setValue(tempColor);
        tempColor[2] = (this->controller->segment_11->getPrimaryEndingColor()[2]);
        tempColor[1] = (this->controller->segment_11->getPrimaryEndingColor()[1]);
        tempColor[0] = (this->controller->segment_11->getPrimaryEndingColor()[0]);
        this->primaryEndingColorCharacteristic->setValue(tempColor);

        // Other primary parameters
        this->primarySpeedCharacteristic->setValue(this->controller->segment_11->getPrimarySpeed());
        this->primarySensitivityCharacteristic->setValue(this->controller->segment_11->getPrimarySensitivity());
        this->primaryNoiseFloorCharacteristic->setValue(this->controller->segment_11->getPrimaryNoiseFloor());
        this->primaryWaveSpeedCharacteristic->setValue(this->controller->segment_11->getPrimaryWaveSpeed());
        this->primaryWavePeriodCharacteristic->setValue(this->controller->segment_11->getPrimaryWavePeriod());
   
        // Secondary colors
        tempColor[2] = (this->controller->segment_11->getSecondaryStartingColor()[2]);
        tempColor[1] = (this->controller->segment_11->getSecondaryStartingColor()[1]);
        tempColor[0] = (this->controller->segment_11->getSecondaryStartingColor()[0]);
        this->secondaryStartingColorCharacteristic->setValue(tempColor);
        tempColor[2] = (this->controller->segment_11->getSecondaryEndingColor()[2]);
        tempColor[1] = (this->controller->segment_11->getSecondaryEndingColor()[1]);
        tempColor[0] = (this->controller->segment_11->getSecondaryEndingColor()[0]);
        this->secondaryEndingColorCharacteristic->setValue(tempColor);

        // Other secondary parameters
        this->secondarySpeedCharacteristic->setValue(this->controller->segment_11->getSecondarySpeed());
    }
}

// Update pointer to the MasterLedController stored in each instance of BLE
void BLE::updateController(MasterLedController *in) {
    this->controller = in;
    return;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// UNUSED //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// // Server side callbacks for BLE.
// void ServerCallbacks::onConnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo) {
//     Serial.print("Client address: ");
//     Serial.println(connInfo.getAddress().toString().c_str());
//     /** We can use the connection handle here to ask for different connection parameters.
//      *  Args: connection handle, min connection interval, max connection interval
//      *  latency, supervision timeout.
//      *  Units; Min/Max Intervals: 1.25 millisecond increments.
//      *  Latency: number of intervals allowed to skip.
//      *  Timeout: 10 millisecond increments, try for 5x interval time for best results.
//      */
//     pServer->updateConnParams(connInfo.getConnHandle(), 24, 48, 0, 60);
// };
// void ServerCallbacks::onDisconnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, int reason) {
//     Serial.println("Client disconnected - start advertising");
//     NimBLEDevice::startAdvertising();
// };
// void ServerCallbacks::onMTUChange(uint16_t MTU, NimBLEConnInfo& connInfo) {
//     Serial.printf("MTU updated: %u for connection ID: %u\n", MTU, connInfo.getConnHandle());
// };

// /********************* Security handled here **********************
// ****** Note: these are the same return values as defaults ********/
// uint32_t ServerCallbacks::onPassKeyDisplay() {
//     Serial.println("Server Passkey Display");
//     /** This should return a random 6 digit number for security
//      *  or make your own static passkey as done here.
//      */
//     return 123456;
// };

// void ServerCallbacks::onConfirmPIN(const NimBLEConnInfo& connInfo, uint32_t pass_key) {
//     // TODO
//     return;
// };

// void ServerCallbacks::onAuthenticationComplete(const NimBLEConnInfo& connInfo) {
//     // TODO
//     return;
// };


// void PrimaryStartingColorCallbacks::onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) {
//     Serial.print(pCharacteristic->getUUID().toString().c_str());
//     Serial.print(": onWrite(), value: ");
//     Serial.println(pCharacteristic->getValue().c_str());
// };

// void PrimaryEndingColorCallbacks::onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) {
//     Serial.print(pCharacteristic->getUUID().toString().c_str());
//     Serial.print(": onWrite(), value: ");
//     Serial.println(pCharacteristic->getValue().c_str());
// };

// void PrimarySpeedCallbacks::onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) {
//     Serial.print(pCharacteristic->getUUID().toString().c_str());
//     Serial.print(": onWrite(), value: ");
//     Serial.println(pCharacteristic->getValue().c_str());
// };

// void SecondaryStartingColorCallbacks::onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) {
//     Serial.print(pCharacteristic->getUUID().toString().c_str());
//     Serial.print(": onWrite(), value: ");
//     Serial.println(pCharacteristic->getValue().c_str());
// };

// void SecondaryEndingColorCallbacks::onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) {
//     Serial.print(pCharacteristic->getUUID().toString().c_str());
//     Serial.print(": onWrite(), value: ");
//     Serial.println(pCharacteristic->getValue().c_str());
// };

// void SecondarySpeedCallbacks::onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) {
//     Serial.print(pCharacteristic->getUUID().toString().c_str());
//     Serial.print(": onWrite(), value: ");
//     Serial.println(pCharacteristic->getValue().c_str());
// };

// void ProtocolCallbacks::onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) {
//     Serial.print(pCharacteristic->getUUID().toString().c_str());
//     Serial.print(": onWrite(), value: ");
//     Serial.println(pCharacteristic->getValue().c_str());
// };