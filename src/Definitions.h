#pragma once
#define NUM_LEDS 20
#define sigPinDef D0
#define strobePinDef D2
#define resetPinDef D3
#define dcOutPinDef D1
#define primaryStartingUUID uint16_t(11)        // MUST BE 16, 32, or 128 BITS
#define primaryEndingUUID uint16_t(12)          // MUST BE 16, 32, or 128 BITS
#define primarySpeedUUID uint16_t(13)           // MUST BE 16, 32, or 128 BITS
#define secondaryStartingUUID uint16_t(14)      // MUST BE 16, 32, or 128 BITS
#define secondaryEndingUUID uint16_t(15)        // MUST BE 16, 32, or 128 BITS
#define secondarySpeedUUID uint16_t(16)         // MUST BE 16, 32, or 128 BITS
#define protocolUUID uint16_t(17)               // MUST BE 16, 32, OR 128 BITS
#define SERVICE_UUID uint16_t(1234)             // MUST BE 16, 32, or 128 BITS
#define DEVICE_NAME "LED CONTROLLER"
