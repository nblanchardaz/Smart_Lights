#pragma once
#include "NeoPixelBus.h"


class Generator {

  public:
    void update();
    RgbColor calculatePixel(uint8_t mode, RgbColor primaryStartingColor, RgbColor primaryEndingColor, uint16_t primaryVelocity, uint16_t primaryWaveSpeed, uint16_t primaryWavePeriod, RgbColor secondaryStartingColor, RgbColor secondaryEndingColor, uint16_t secondaryVelocity, uint16_t index, uint16_t numLeds, uint16_t musicLen);
    Generator();

};
