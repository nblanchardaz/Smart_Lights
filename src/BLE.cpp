// Custom classes ServerCallbacks and CharacteristicCallbacks originally from the documentation
// for the NimBLE-Arduino library at: https://github.com/h2zero/NimBLE-Arduino/blob/master/examples/NimBLE_Server/NimBLE_Server.ino
// Additional code also taken from this documentation.


#include "BLE.h"


BLE::BLE() {
    
    // Make sure serial is available
    Serial.begin(9600);

    // Initialize protocol
    this->protocol = "NeoEsp32Rmt0Ws2811Method";

    // Initialize variables
    primaryStartingColor = RgbColor(0, 0, 0);
    primaryEndingColor = RgbColor(0, 0, 0);
    primarySpeed = 0;
    secondaryStartingColor = RgbColor(0, 0, 0);
    secondaryEndingColor = RgbColor(0, 0, 0);
    secondarySpeed = 0;

    // DEBUG
    Serial.println("Starting BLE Initialization...");

    // Setup BLE device, service, and characteristics
    NimBLEDevice::init(DEVICE_NAME);
    NimBLEDevice::setPower(ESP_PWR_LVL_P9);
    NimBLEDevice::setSecurityAuth(false, false, false);
    pServer = NimBLEDevice::createServer();
    pServer->setCallbacks(new ServerCallbacks());
    pService = pServer->createService(SERVICE_UUID);
    primaryStartingColorCharacteristic = pService->createCharacteristic(primaryStartingUUID, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE| NIMBLE_PROPERTY::NOTIFY);
    primaryEndingColorCharacteristic = pService->createCharacteristic(primaryEndingUUID, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::NOTIFY);
    primarySpeedCharacteristic = pService->createCharacteristic(primarySpeedUUID, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::NOTIFY);
    secondaryStartingColorCharacteristic = pService->createCharacteristic(secondaryStartingUUID, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::NOTIFY);
    secondaryEndingColorCharacteristic = pService->createCharacteristic(secondaryEndingUUID, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::NOTIFY);
    secondarySpeedCharacteristic = pService->createCharacteristic(secondarySpeedUUID, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::NOTIFY);
    protocolCharacteristic = pService->createCharacteristic(protocolUUID, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::NOTIFY);

    // Set callbacks
    primaryStartingColorCharacteristic->setCallbacks(&pscc);
    primaryEndingColorCharacteristic->setCallbacks(&pecc);
    primarySpeedCharacteristic->setCallbacks(&psc);
    secondaryStartingColorCharacteristic->setCallbacks(&sscc);
    secondaryEndingColorCharacteristic->setCallbacks(&secc);
    secondarySpeedCharacteristic->setCallbacks(&ssc);
    protocolCharacteristic->setCallbacks(&pc);

    // DEBUG
    Serial.println("BLE Characteristics created...");

    // Start BLE service
    pService->start();

    // DEBUG
    protocolCharacteristic->setValue(99);
    protocolCharacteristic->notify();

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
}

void BLE::printParameters() {
    Serial.print("Starting Red: " + String(this->primaryStartingColor[0]));
    Serial.print("Starting Green: " + String(this->primaryStartingColor[1]));
    Serial.print("Starting Blue: " + String(this->primaryStartingColor[2]));
    Serial.print("Primary speed: " + String(this->primarySpeed));
    Serial.print("Protocol: " + this->protocol);
}

void BLE::updateController(MasterLedController *in) {
    this->controller = in;
    return;
}


void ServerCallbacks::onConnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo) {
    Serial.print("Client address: ");
    Serial.println(connInfo.getAddress().toString().c_str());
    /** We can use the connection handle here to ask for different connection parameters.
     *  Args: connection handle, min connection interval, max connection interval
     *  latency, supervision timeout.
     *  Units; Min/Max Intervals: 1.25 millisecond increments.
     *  Latency: number of intervals allowed to skip.
     *  Timeout: 10 millisecond increments, try for 5x interval time for best results.
     */
    pServer->updateConnParams(connInfo.getConnHandle(), 24, 48, 0, 60);
};
void ServerCallbacks::onDisconnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, int reason) {
    Serial.println("Client disconnected - start advertising");
    NimBLEDevice::startAdvertising();
};
void ServerCallbacks::onMTUChange(uint16_t MTU, NimBLEConnInfo& connInfo) {
    Serial.printf("MTU updated: %u for connection ID: %u\n", MTU, connInfo.getConnHandle());
};

/********************* Security handled here **********************
****** Note: these are the same return values as defaults ********/
uint32_t ServerCallbacks::onPassKeyDisplay() {
    Serial.println("Server Passkey Display");
    /** This should return a random 6 digit number for security
     *  or make your own static passkey as done here.
     */
    return 123456;
};

void ServerCallbacks::onConfirmPIN(const NimBLEConnInfo& connInfo, uint32_t pass_key) {
    // TODO
    return;
};

void ServerCallbacks::onAuthenticationComplete(const NimBLEConnInfo& connInfo) {
    // TODO
    return;
};


void PrimaryStartingColorCallbacks::onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) {
    Serial.print(pCharacteristic->getUUID().toString().c_str());
    Serial.print(": onWrite(), value: ");
    Serial.println(pCharacteristic->getValue().c_str());
};

void PrimaryEndingColorCallbacks::onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) {
    Serial.print(pCharacteristic->getUUID().toString().c_str());
    Serial.print(": onWrite(), value: ");
    Serial.println(pCharacteristic->getValue().c_str());
};

void PrimarySpeedCallbacks::onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) {
    Serial.print(pCharacteristic->getUUID().toString().c_str());
    Serial.print(": onWrite(), value: ");
    Serial.println(pCharacteristic->getValue().c_str());
};

void SecondaryStartingColorCallbacks::onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) {
    Serial.print(pCharacteristic->getUUID().toString().c_str());
    Serial.print(": onWrite(), value: ");
    Serial.println(pCharacteristic->getValue().c_str());
};

void SecondaryEndingColorCallbacks::onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) {
    Serial.print(pCharacteristic->getUUID().toString().c_str());
    Serial.print(": onWrite(), value: ");
    Serial.println(pCharacteristic->getValue().c_str());
};

void SecondarySpeedCallbacks::onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) {
    Serial.print(pCharacteristic->getUUID().toString().c_str());
    Serial.print(": onWrite(), value: ");
    Serial.println(pCharacteristic->getValue().c_str());
};

void ProtocolCallbacks::onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) {
    Serial.print(pCharacteristic->getUUID().toString().c_str());
    Serial.print(": onWrite(), value: ");
    Serial.println(pCharacteristic->getValue().c_str());
};