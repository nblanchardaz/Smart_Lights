#pragma once

#include <NeoPixelBus.h>
#include "LedStripSegment.h"
#include "Audio.h"
#include "Definitions.h"
#include "Generator.h"

class MasterLedController {

    private:
        MSGEQ7 *chip = new MSGEQ7(resetPinDef, strobePinDef, dcOutPinDef);
        Generator *generator = new Generator();
        void showStrip();

    public:
        MasterLedController();
        LedStripSegment<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod> *segment_12;
        LedStripSegment<NeoGrbFeature, NeoEsp32Rmt0Ws2811Method> *segment_11;
        static MasterLedController &getInstance();
        void calculateStrip();
        void doStuff();
        void setProtocol(String* _protocol);
        void loadParams(RgbColor* _primaryStarting, RgbColor* _primaryEnding, uint16_t* _primarySpeed, uint16_t* _primarySensitivity, uint16_t* _primaryNoiseFloor, RgbColor* _secondaryStarting, RgbColor* _secondaryEnding, uint16_t* _secondarySpeed, uint8_t* _mode);
        String protocol = "NeoEsp32Rmt0Ws2811Method";
};


class MasterLedControllerWs2811 : public MasterLedController {
    public:
        MasterLedControllerWs2811() {
            this->segment_11 = new LedStripSegment<NeoGrbFeature, NeoEsp32Rmt0Ws2811Method>(sigPinDef, 0, NUM_LEDS, 1);
        }
        void loadParams(RgbColor* _primaryStarting, RgbColor* _primaryEnding, uint16_t* _primarySpeed, uint16_t* _primarySensitivity, uint16_t* _primaryNoiseFloor, RgbColor* _secondaryStarting, RgbColor* _secondaryEnding, uint16_t* _secondarySpeed, uint8_t* _mode);
};


class MasterLedControllerWs2812x : public MasterLedController {
    public:
        MasterLedControllerWs2812x() {
            this->segment_11 = new LedStripSegment<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod>(sigPinDef, 0, NUM_LEDS, 1);
        }
        void loadParams(RgbColor* _primaryStarting, RgbColor* _primaryEnding, uint16_t* _primarySpeed, uint16_t* _primarySensitivity, uint16_t* _primaryNoiseFloor, RgbColor* _secondaryStarting, RgbColor* _secondaryEnding, uint16_t* _secondarySpeed, uint8_t* _mode);
};