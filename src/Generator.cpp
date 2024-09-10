#include "Generator.h"

Generator::Generator() {

    return;

}

void Generator::update() {

    return;

}

RgbColor Generator::calculatePixel(uint8_t mode, RgbColor startingColor, RgbColor endingColor, uint16_t velocity) {

    switch (mode) {

        case 0:
            // Let's use a sine function to create a smooth transition from startinColor, to endingColor, and back in an infinite loop.
            // component = a * sin( time ) + b

            // a represents how much we need to add to startingColor to reach endingColor when our sine wave is at +1, divided by two.
            float a[3];
            a[0] = float(startingColor[0] - endingColor[0]) / -2.0;
            a[1] = float(startingColor[1] - endingColor[1]) / -2.0;
            a[2] = float(startingColor[2] - endingColor[2]) / -2.0;

            // b represents our starting point (midpoint between startingColor and endingColor).
            uint16_t b[3];
            b[0] = startingColor[0] + a[0];
            b[1] = startingColor[1] + a[1];
            b[2] = startingColor[2] + a[2];

            // Now, let's generate our new color component values using the equation above.
            // At a velocity value of 1 (lowest possible), it will take 60 seconds to complete the cycle.
            uint16_t new_color[3];
            new_color[0] = a[0] * sin( 2 * PI * velocity * float(millis()) / 1000.0 / 60.0 ) + b[0];
            new_color[1] = a[1] * sin( 2 * PI * velocity * float(millis()) / 1000.0 / 60.0 ) + b[1];
            new_color[2] = a[2] * sin( 2 * PI * velocity * float(millis()) / 1000.0 / 60.0 ) + b[2];

            return RgbColor(new_color[1], new_color[0], new_color[2]);
            break;

            // LEGACY
            // // Create a 'time factor', which oscillates between 0 and 1. Oscillation speed (frequency) is affected by the user-defined velocity.
            // float timeFactor = sin(millis() * velocity / 100000);

            // // Return a linear blend of the two colors
            // return RgbColor::LinearBlend(startingColor, endingColor, timeFactor);

            // return RgbColor()
        case 1:
            // Let's use a sine function to create 'waves' that propogate down the strip.
            // The 'waves' will change intensity based on observed frequency amplitudes.
            return RgbColor(255, 0, 255);
            break;

        default:
        // Return white
            return RgbColor(255, 255, 255);
            break;
    }

}