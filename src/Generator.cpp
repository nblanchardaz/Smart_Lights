#include "Generator.h"

Generator::Generator() {

    return;

}

void Generator::update() {

    return;

}

RgbColor Generator::calculatePixel(RgbColor startingColor, RgbColor endingColor, uint16_t velocity) {

    // Create a 'time factor', which oscillates between 0 and 1. Oscillation speed (frequency) is affected by the user-defined velocity.
    float timeFactor = sin(millis() * velocity);

    // Return a linear blend of the two colors
    return RgbColor::LinearBlend(startingColor, endingColor, timeFactor);

}