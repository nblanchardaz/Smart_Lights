#pragma once

#include <NeoPixelBus.h>
#include "LedStripSegment.h"
#include "Audio.h"
#include "Definitions.h"
#include "Generator.h"

class MasterLedController {

    private:
        MasterLedController();
        Processor *chip = new Processor(resetPinDef, strobePinDef, dcOutPinDef, MSGEQ7_FLAG);
        Generator *generator = new Generator();
        void showStrip();

    public:
        LedStripSegment<NeoGrbFeature, NeoEsp32Rmt0Ws2811Method>* segment_11 = nullptr;
        LedStripSegment<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod>* segment_12 = nullptr;
        void loadParams(RgbColor* _primaryStarting, RgbColor* _primaryEnding, uint16_t* _primarySpeed, uint16_t* _primarySensitivity, uint16_t* _primaryNoiseFloor, RgbColor* _secondaryStarting, RgbColor* _secondaryEnding, uint16_t* _secondarySpeed, uint8_t* _mode, uint16_t* _numLeds, uint16_t* _primaryWaveSpeed, uint16_t* _primaryWavePeriod);
        void printParameters();
        void addWs2811();
        void addWs2812x();
        static MasterLedController &getInstance();
        void calculateStrip();
        void doStuff();
        void setProtocol(String* _protocol);
        String protocol;
};