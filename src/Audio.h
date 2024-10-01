// Author: Nick nblanchardaz@gmail.com
// This header file contains the MSGEQ7 class definition.

#pragma once

#include <Arduino.h>
#include <NeopixelBus.h>

class Processor {

    public:

        // Int to store length of affected pixels
        uint16_t len;
        
        // Constructor
        Processor(int reset, int strobe, int dcOut, bool MSGEQ7flag);

        // Read results
        int getResults();
        int readMSGEQ7();
        int readGPIO();

        // Calculate how many pixels to affect
        int calculateLen(uint16_t sensitivity, uint16_t noiseFloor, uint16_t numLeds);

    private:

        // ESP32 pin numbers being used by MSGEQ7
        int resetPin;
        int strobePin;

        // Pin data is read from
        int dataInPin;

        // Array to store results of 7-bin FFT
        uint16_t fft[7];

        // Flag for whether MSGEQ7 is used or not
        bool MSGEQ7flag;

};

inline int square(int x);