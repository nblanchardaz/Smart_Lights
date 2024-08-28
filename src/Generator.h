#pragma once
#include "NeoPixelBus.h"

class Generator {

  public:
    void update();
    RgbColor calculatePixel();
    Generator();

};
