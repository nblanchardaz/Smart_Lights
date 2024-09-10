// Author: Nick nblanchardaz@gmail.com
// This cpp file contains functions to read data from an MSGEQ7
// 7-band equalizer IC. Reference the MSGEQ7 datashet for exact
// timing constraints.

#include "Audio.h"
#include "Definitions.h"

MSGEQ7::MSGEQ7(int reset, int strobe, int dcOut) {

    // Set ESP32 pin modes, and store these pins (we will need them to read data later)
    pinMode(reset, OUTPUT);
    this->resetPin = reset;
    pinMode(strobe, OUTPUT);
    this->strobePin = strobe;
    pinMode(dcOut, INPUT);
    this->dcOutPin = dcOut;

    // Begin with output pins in idle states
    digitalWrite(reset, LOW);
    digitalWrite(strobe, HIGH);

    // Set color of affected pixels
    this->c = RgbColor(255, 255, 255);

}

int MSGEQ7::getResults(void) {

    char text[5];

    // Begin by resettting the MSGEQ7 counter
    digitalWrite(this->resetPin, HIGH);
    delay(5);
    digitalWrite(this->resetPin, LOW);

    // Now, let's iterate through out data array and populate the freq bins
    // (length of array is hardcoded to 7 because the MSGEQ7 always has 7 FFT bins)
    for (uint8_t i = 0; i < 7; i++) {
        digitalWrite(this->strobePin, LOW);
        delayMicroseconds(35);
        this->fft[i] = analogRead(this->dcOutPin); // Analog 12-bit value
        digitalWrite(this->strobePin, HIGH);

        // Debug
        // sprintf(text, "%04d", this->fft[i]);
        // Serial.print(text);
        // Serial.print(this->fft[i]);
        // Serial.print(" ");
    }

    // Debug
    // Serial.println();

    return 0;
}

int MSGEQ7::calculateLen(uint16_t sensitivity, uint16_t noiseFloor, uint16_t numLeds) {

    // Calculate the sum of the lowest two frequency bands.
    int temp = (this->fft[0] + this->fft[1]);

    // Calculate noise constant
    int noiseConstant = noiseFloor * 20;

    // Noise suppression: If the sum is less than 1500, do nothing.
    if (temp < noiseConstant) {
        temp = noiseConstant;
    }
    uint16_t res = ((temp - noiseConstant) * (numLeds / 2) / (4096));

    // If we get a length greater than NUM_LEDS, then set it equal to NUM_LEDS
    if (res > numLeds) {
        res = numLeds;
    }

    // Now, factor in sensitivity.
    // sensitivty = 50 -> no transformation
    // sensitivty < 50 -> reduce length of res
    // sensitivity > 50 -> increase length of res
    uint16_t effectiveSensitivty = sensitivity - 50;

    // Can be increased/decreased by up to a factor of 5.
    this->len = res + res * sensitivity / 12.5;

    return this->len;
}

