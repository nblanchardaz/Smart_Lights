// Author: Nick nblanchardaz@gmail.com
// This cpp file contains functions to read data from an MSGEQ7
// 7-band equalizer IC. Reference the MSGEQ7 datashet for exact
// timing constraints.

#include "Audio.h"

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
        // Serial.print(this->fft[i]);
        // Serial.print(" ");
    }

    // Debug
    // Serial.println();

    return 0;
}

int MSGEQ7::calculateLen() {

    this->len = ((this->fft[0] + this->fft[1]) * 10 / 4096);

    return 0;
}

