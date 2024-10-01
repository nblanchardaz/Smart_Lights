#include "Generator.h"


// Constructor
Generator::Generator() {
    return;
}

// Currently unused
void Generator::update() {
    return;
}

// Calculate color of a given pixel
RgbColor Generator::calculatePixel(uint8_t mode, RgbColor primaryStartingColor, RgbColor primaryEndingColor, uint16_t primaryVelocity, uint16_t primaryWaveSpeed, uint16_t primaryWavePeriod, RgbColor secondaryStartingColor, RgbColor secondaryEndingColor, uint16_t secondaryVelocity, uint16_t index, uint16_t numLeds, uint16_t musicLen) {

    switch (mode) {

        // Classic (0) mode
        // Primary segment length is proportional to detected music amplitude. The rest of the strip is the secondary segment. Both segments oscillate between their respective
        // starting and ending colors.
        case 0: {
            // Let's use a sine function to create a smooth transition from startinColor, to endingColor, and back in an infinite loop.
            // component = a * sin( time ) + b

            // First, determine if we are in the primary (music reactive) or secondary (non music reactive) portion of the strip
            RgbColor startingColor;
            RgbColor endingColor;
            uint16_t velocity;
            if (index < musicLen) {
                startingColor = primaryStartingColor;
                endingColor = primaryEndingColor;
                velocity = primaryVelocity;
            }
            else {
                startingColor = secondaryStartingColor;
                endingColor = secondaryEndingColor;
                velocity = secondaryVelocity;
            }

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
        }

        // Waves (1) mode
        // 'Waves' propogate down the strip; half of the wave is the primary segment, and the other equally long half is the secondary segment. The secondary segment oscillates between
        // it's starting and ending colors (just like in the previous mode), but now the detected music amplitude controls how close the color of the primary segments gets to the ending color.
        case 1: {
            // Let's use a sine function to create 'waves' that propogate down the strip.
            // The 'waves' will change intensity based on observed frequency amplitudes.
            // Note: This is inherently more complicated than the routine above because
            // the strip is split into many segments, not just two discrete segments.

            // Stores our new color
            uint16_t new_color[3];

            // Stores out starting params
            RgbColor startingColor;
            RgbColor endingColor;
            uint16_t velocity;

            // First, figure out effective wave speed.
            // wave speed = 50 -> no transformation
            // wave speed < 50 -> slower
            // wave speed > 50 -> faster
            uint16_t effectiveWaveSpeed = primaryWaveSpeed - 50;

            // We need a time factor, which will be transformed by our effective wave speed.
            // Default: wave will shift by 1 pixel every 0.5 seconds
            float timeFactor = float(millis()) / 1000.0 / 0.5;

            // Multiply the effective wave speed by the time factor to get the effective time factor.
            // The default (0.5) can now be sped up or slowed down by a factor of 5.
            float effectiveTimeFactor = timeFactor * pow(2, float(effectiveWaveSpeed / 12.5));

            // This sin function determines the width and speed of the waves. At a wave speed of 1 (lowest possible),
            // the wave will shift by 1 pixel every 5 seonds. At a wave period of 1 (lowest possible), the waves will
            // span 1 pixel.
            float period = 2 * PI * 10 / float(primaryWavePeriod);
            float sinVal = sin( period  * (effectiveTimeFactor + float(index)) );

            // First, determine if we are in the primary (music reactive) or secondary (non music reactive) portion of the strip.
            // To create music reactivity, let's make it so that the primary segment doesn't oscillate between the starting and ending colors, but instead
            // gets closer to the secondary color the louder the music is.
            if (sinVal >= 0) {

                // Primary -> music reactive
                startingColor = primaryStartingColor;
                endingColor = primaryEndingColor;
                velocity = primaryVelocity;

                // a represents how much we need to add to startingColor to reach endingColor. Let's scale this value by the music length, and then add it to
                // startingColor to get our new color. A maximum length of numLeds means we are exactly at SecondaryColor, and a minimum length of 0 means
                // we are exactly at StartingColor;
                float a[3];
                a[0] = float(startingColor[0] - endingColor[0]) * float(musicLen) / float(numLeds) * -1;
                a[1] = float(startingColor[1] - endingColor[1]) * float(musicLen) / float(numLeds) * -1;
                a[2] = float(startingColor[2] - endingColor[2]) * float(musicLen) / float(numLeds) * -1;

                // Now, we just need to add our calculated offsets to the starting color.
                new_color[0] = startingColor[0] + a[0];
                new_color[1] = startingColor[1] + a[1];
                new_color[2] = startingColor[2] + a[2];
            }
            else {

                // Secondary -> not music reactive
                startingColor = secondaryStartingColor;
                endingColor = secondaryEndingColor;
                velocity = secondaryVelocity;

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
                new_color[0] = a[0] * sin( 2 * PI * velocity * float(millis()) / 1000.0 / 60.0 ) + b[0];
                new_color[1] = a[1] * sin( 2 * PI * velocity * float(millis()) / 1000.0 / 60.0 ) + b[1];
                new_color[2] = a[2] * sin( 2 * PI * velocity * float(millis()) / 1000.0 / 60.0 ) + b[2];
            }

            Serial.println("Waves retuning: " + String(new_color[1]) + " " + String(new_color[0]) + " " + String(new_color[2]));
            return RgbColor(new_color[1], new_color[0], new_color[2]);
            break;
        }

        // Classic centered (2) mode
        // Primary segment length is proportional to detected music amplitude. The rest of the strip is the secondary segment. Both segments oscillate between their respective
        // starting and ending colors. Primary segment spawns from the center.
        case 2: {
            // Let's use a sine function to create a smooth transition from startinColor, to endingColor, and back in an infinite loop.
            // component = a * sin( time ) + b

            // First, determine if we are in the primary (music reactive) or secondary (non music reactive) portion of the strip
            RgbColor startingColor;
            RgbColor endingColor;
            uint16_t velocity;
            if (abs(index - numLeds/2) < (musicLen / 2)) {
                startingColor = primaryStartingColor;
                endingColor = primaryEndingColor;
                velocity = primaryVelocity;
            }
            else {
                startingColor = secondaryStartingColor;
                endingColor = secondaryEndingColor;
                velocity = secondaryVelocity;
            }

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
        }

        // Classic mirrored (3) mode
        // Primary segment length is proportional to detected music amplitude. The rest of the strip is the secondary segment. Both segments oscillate between their respective
        // starting and ending colors. Primary segments spawn on both ends of the strip.
        case 3: {
            // Let's use a sine function to create a smooth transition from startinColor, to endingColor, and back in an infinite loop.
            // component = a * sin( time ) + b

            // First, determine if we are in the primary (music reactive) or secondary (non music reactive) portion of the strip
            RgbColor startingColor;
            RgbColor endingColor;
            uint16_t velocity;
            if ( (index) < (musicLen / 2) || (numLeds - index) < (musicLen / 2) ) {
                startingColor = primaryStartingColor;
                endingColor = primaryEndingColor;
                velocity = primaryVelocity;
            }
            else {
                startingColor = secondaryStartingColor;
                endingColor = secondaryEndingColor;
                velocity = secondaryVelocity;
            }

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
        }

        // Strobe (4) mode
        // There is only a primary segment, no secondayr segment. Lights are on the starting color by default and approach
        // the ending color as music reactivity increases.
        case 4: {
            
            // Stores our new color
            uint16_t new_color[3];

            // Stores out starting params
            RgbColor startingColor;
            RgbColor endingColor;
            uint16_t velocity;

            // Primary -> music reactive
            startingColor = primaryStartingColor;
            endingColor = primaryEndingColor;
            velocity = primaryVelocity;

            // a represents how much we need to add to startingColor to reach endingColor. Let's scale this value by the music length, and then add it to
            // startingColor to get our new color. A maximum length of numLeds means we are exactly at SecondaryColor, and a minimum length of 0 means
            // we are exactly at StartingColor;
            float a[3];
            a[0] = float(startingColor[0] - endingColor[0]) * float(musicLen) / float(numLeds) * -1;
            a[1] = float(startingColor[1] - endingColor[1]) * float(musicLen) / float(numLeds) * -1;
            a[2] = float(startingColor[2] - endingColor[2]) * float(musicLen) / float(numLeds) * -1;

            // Now, we just need to add our calculated offsets to the starting color.
            new_color[0] = startingColor[0] + a[0];
            new_color[1] = startingColor[1] + a[1];
            new_color[2] = startingColor[2] + a[2];

            return RgbColor(new_color[1], new_color[0], new_color[2]);
            break;
        }

        // Default: Return white if invalid mode is supplied
        default: {
            return RgbColor(255, 255, 255);
            break;
        }
    }
}