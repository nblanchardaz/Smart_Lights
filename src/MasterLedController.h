#pragma once

#include <NeoPixelBus.h>
#include "LedStripSegment.h"
#include "Audio.h"
#include "Definitions.h"
#include "Generator.h"

class MasterLedController {

    private:
        MasterLedController();
        LedStripSegment<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod> *segment_12;
        LedStripSegment<NeoGrbFeature, NeoEsp32Rmt0Ws2811Method> *segment_11;
        MSGEQ7 *chip = new MSGEQ7(resetPinDef, strobePinDef, dcOutPinDef);
        Generator *generator = new Generator();
        void showStrip();

    public:
        static MasterLedController &getInstance();
        void calculateStrip();
        void loop();
        String protocol;
};