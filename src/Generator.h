#pragma once
#include "NeoPixelBus.h"

class Generator {

  public:
    void update();
    RgbColor calculatePixel(uint8_t mode, RgbColor startingColor, RgbColor endingColor, uint16_t velocity);
    Generator();

};
