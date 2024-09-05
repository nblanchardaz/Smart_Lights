#pragma once
#define NUM_LEDS 50
#define sigPinDef D0
#define strobePinDef D2
#define resetPinDef D3
#define dcOutPinDef D1
#define primaryStartingUUID uint16_t(1)        // MUST BE 16, 32, or 128 BITS
#define primaryEndingUUID uint16_t(2)          // MUST BE 16, 32, or 128 BITS
#define primarySpeedUUID uint16_t(3)           // MUST BE 16, 32, or 128 BITS
#define secondaryStartingUUID uint16_t(4)      // MUST BE 16, 32, or 128 BITS
#define secondaryEndingUUID uint16_t(5)        // MUST BE 16, 32, or 128 BITS
#define secondarySpeedUUID uint16_t(6)         // MUST BE 16, 32, or 128 BITS
#define protocolUUID uint16_t(7)               // MUST BE 16, 32, OR 128 BITS
#define updateFlagUUID uint16_t(8)
#define SERVICE_UUID uint16_t(1234)             // MUST BE 16, 32, or 128 BITS
#define DEVICE_NAME "LED CONTROLLER"
