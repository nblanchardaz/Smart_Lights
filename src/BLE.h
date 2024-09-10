#pragma once

#include <NimBLEDevice.h>
#include <NeoPixelBus.h>
#include "MasterLedController.h"
#include <Preferences.h>


class ServerCallbacks: public NimBLEServerCallbacks {
    public:
        void onConnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo);
        void onDisconnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, int reason);
        void onMTUChange(uint16_t MTU, NimBLEConnInfo& connInfo);
        uint32_t onPassKeyDisplay();
        void onConfirmPIN(const NimBLEConnInfo& connInfo, uint32_t pass_key);
        void onAuthenticationComplete(const NimBLEConnInfo& connInfo);
};


class PrimaryStartingColorCallbacks: public NimBLECharacteristicCallbacks {
    public:
        void onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo);
};

class PrimaryEndingColorCallbacks: public NimBLECharacteristicCallbacks {
    public:
        void onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo);
};

class PrimarySpeedCallbacks: public NimBLECharacteristicCallbacks {
    public:
        void onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo);
};

class SecondaryStartingColorCallbacks: public NimBLECharacteristicCallbacks {
    public:
        void onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo);
};

class SecondaryEndingColorCallbacks: public NimBLECharacteristicCallbacks {
    public:
        void onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo);
};

class SecondarySpeedCallbacks: public NimBLECharacteristicCallbacks {
    public:
        void onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo);
};

class ProtocolCallbacks: public NimBLECharacteristicCallbacks {
    public:
        void onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo);
};


class BLE {
    public:
        BLE();
        void updateParameters();
        void updateController(MasterLedController *in);
        void printParameters();
        int checkUpdateFlag();
        int saveParameters(Preferences* preferences);
        String protocol;
        String protocols_arr[2] = {"NeoEsp32Rmt0Ws2812xMethod", "NeoEsp32Rmt0Ws2811Method"};
        bool updateFlag;

    private:
        NimBLEServer *pServer;
        NimBLEService *pService;

        NimBLECharacteristic *primaryStartingColorCharacteristic;
        NimBLECharacteristic *primaryEndingColorCharacteristic;
        NimBLECharacteristic *primarySpeedCharacteristic;
        NimBLECharacteristic *primarySensitivityCharacteristic;
        NimBLECharacteristic *primaryNoiseFloorCharacteristic;
        NimBLECharacteristic *secondaryStartingColorCharacteristic;
        NimBLECharacteristic *secondaryEndingColorCharacteristic;
        NimBLECharacteristic *secondarySpeedCharacteristic;
        NimBLECharacteristic *protocolCharacteristic;
        NimBLECharacteristic *updateFlagCharacteristic;
        NimBLECharacteristic *firmwareCharacteristic;
        NimBLECharacteristic *modeCharacteristic;

        NimBLEAdvertising *pAdvertising;
        
        PrimaryStartingColorCallbacks pscc;
        PrimaryEndingColorCallbacks pecc;
        PrimarySpeedCallbacks psc;
        SecondaryStartingColorCallbacks sscc;
        SecondaryEndingColorCallbacks secc;
        SecondarySpeedCallbacks ssc;
        ProtocolCallbacks pc;

        MasterLedController *controller;
        RgbColor primaryStartingColor;
        RgbColor primaryEndingColor;
        uint16_t primarySpeed;
        RgbColor secondaryStartingColor;
        RgbColor secondaryEndingColor;
        uint16_t secondarySpeed;
        uint16_t primarySensitivity;
        uint16_t primaryNoiseFloor;
        uint8_t mode;

        void updateExternParameters();
};
