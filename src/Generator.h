#pragma once
#include "NeoPixelBus.h"

class Generator {

  public:
    void update();
    RgbColor calculatePixel(RgbColor startingColor, RgbColor endingColor, uint16_t velocity);
    Generator();

};
