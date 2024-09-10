#pragma once

#include <NeoPixelBus.h>
#include "LedStripSegment.h"
#include "Audio.h"
#include "Definitions.h"
#include "Generator.h"

class MasterLedController {

    private:
        MasterLedController();
        MSGEQ7 *chip = new MSGEQ7(resetPinDef, strobePinDef, dcOutPinDef);
        Generator *generator = new Generator();
        void showStrip();

    public:
        LedStripSegment<NeoGrbFeature, NeoEsp32Rmt0Ws2811Method>* segment_11 = nullptr;
        LedStripSegment<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod>* segment_12 = nullptr;
        void loadParams(RgbColor* _primaryStarting, RgbColor* _primaryEnding, uint16_t* _primarySpeed, uint16_t* _primarySensitivity, uint16_t* _primaryNoiseFloor, RgbColor* _secondaryStarting, RgbColor* _secondaryEnding, uint16_t* _secondarySpeed, uint8_t* _mode, uint16_t* _numLeds);
        void printParameters();
        void addWs2811();
        void addWs2812x();
        static MasterLedController &getInstance();
        void calculateStrip();
        void doStuff();
        void setProtocol(String* _protocol);
        String protocol;
};


// class MasterLedControllerWs2811 : public MasterLedController {
//     public:
//         MasterLedControllerWs2811() {
//             this->segment = new LedStripSegment<NeoGrbFeature, NeoEsp32Rmt0Ws2811Method>(sigPinDef, 0, NUM_LEDS, 1);
//         }
//         void loadParams(RgbColor* _primaryStarting, RgbColor* _primaryEnding, uint16_t* _primarySpeed, uint16_t* _primarySensitivity, uint16_t* _primaryNoiseFloor, RgbColor* _secondaryStarting, RgbColor* _secondaryEnding, uint16_t* _secondarySpeed, uint8_t* _mode);
//         LedStripSegment<NeoGrbFeature, NeoEsp32Rmt0Ws2811Method> *segment;
// };


// class MasterLedControllerWs2812x : public MasterLedController {
//     public:
//         MasterLedControllerWs2812x() {
//             this->segment = new LedStripSegment<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod>(sigPinDef, 0, NUM_LEDS, 1);
//         }
//         void loadParams(RgbColor* _primaryStarting, RgbColor* _primaryEnding, uint16_t* _primarySpeed, uint16_t* _primarySensitivity, uint16_t* _primaryNoiseFloor, RgbColor* _secondaryStarting, RgbColor* _secondaryEnding, uint16_t* _secondarySpeed, uint8_t* _mode);
//         LedStripSegment<NeoGrbFeature, NeoEsp32Rmt0Ws2811Method> *segment;
// };