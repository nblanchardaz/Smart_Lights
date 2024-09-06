// Author: Nick nblanchardaz@gmail.com
// This header file contains the MSGEQ7 class definition.

#pragma once

#include <Arduino.h>
#include <NeopixelBus.h>

class MSGEQ7 {

    public:

        // Int to store length of affected pixels
        uint16_t len;

        // Color that affected pixels are changed to
        RgbColor c;
        
        // Constructor
        MSGEQ7(int reset, int strobe, int dcOut);

        // Read results
        int getResults();

        // Calculate how many pixels to affect
        int calculateLen(uint16_t sensitivity);

    private:

        // ESP32 pin numbers being used by MSGEQ7
        int resetPin;
        int strobePin;
        int dcOutPin;

        // Array to store results of 7-bin FFT
        uint16_t fft[7];

};