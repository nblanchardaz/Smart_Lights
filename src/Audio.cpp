// Author: Nick nblanchardaz@gmail.com
// This cpp file contains functions to read data from an MSGEQ7
// 7-band equalizer IC. Reference the MSGEQ7 datashet for exact
// timing constraints.


#include "Audio.h"
#include "Definitions.h"


// Constructor
Processor::Processor(int reset, int strobe, int dcOut, bool MSGEQ7flag) {

    // Set ESP32 pin modes, and store these pins (we will need them to read data later)
    pinMode(reset, OUTPUT);
    this->resetPin = reset;
    pinMode(strobe, OUTPUT);
    this->strobePin = strobe;
    pinMode(dcOut, INPUT);
    this->dataInPin = dcOut;

    // Begin with output pins in idle states
    digitalWrite(reset, LOW);
    digitalWrite(strobe, HIGH);

    // Set MSGEQ7 flag
    this->MSGEQ7flag = MSGEQ7flag;

}

int Processor::getResults(void) {
    if (this->MSGEQ7flag == true) {
        return this->readMSGEQ7();
    }
    else {
        return this->readGPIO();
    }
}

// Retrieve results without MSGEQ7
int Processor::readGPIO(void) {

    // NOTE: Powers of 2 (namely 128) are used here to create easy division via right shift operator and avoid
    // costly floating point calculations.

    // Calculate the power of the incoming signal by taking 128 measurments over a 5 ms period
    // (5 ms is the period of a 20Hz signal, the lowest frequency we are interested in measuring)
    uint16_t measurements[128];  // 16 bits is enough to store the ADC results which are 12 bit resolution
    uint32_t avg = 0;            // Running average of measurements; used for power calculation.
    uint32_t power = 0;
    float result;
    uint16_t resultInt;

    // Measurement stage
    for (uint8_t i  = 0; i < 128; i++) {
        // Take measurement
        measurements[i] = analogRead(this->dataInPin);

        // Add to average
        avg = avg + measurements[i];

        // Wait 39 us
        delayMicroseconds(39);
    }

    // Energy calculation stage
    avg = avg >> 7;          // Finish average calculation (divide by 128)
    for (uint8_t i = 0; i < 100; i++) {
        power = power + square((measurements[i] - avg));
    }
    power = power >> 7;

    // Normalizing power
    result = power / 8388608.0; // Maximum power achievable (3.3V peak-to-peak)

    // Translating power back into a scale of 0 to 4096
    resultInt = result * 4096;
    this->fft[0] = resultInt;
    this->fft[1] = resultInt;

    return resultInt;
}

// Retrieve results from MSGEQ7
int Processor::readMSGEQ7(void) {

    // Debug
    // char text[5];

    // Begin by resettting the MSGEQ7 counter
    digitalWrite(this->resetPin, HIGH);
    delay(5);
    digitalWrite(this->resetPin, LOW);

    // Now, let's iterate through out data array and populate the freq bins
    // (length of array is hardcoded to 7 because the MSGEQ7 always has 7 FFT bins)
    for (uint8_t i = 0; i < 7; i++) {
        digitalWrite(this->strobePin, LOW);
        delayMicroseconds(35);
        this->fft[i] = analogRead(this->dataInPin); // Analog 12-bit value
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

// Calculate number of affected LEDs based on most recent MSGEQ7 results.
int Processor::calculateLen(uint16_t sensitivity, uint16_t noiseFloor, uint16_t numLeds) {

    // Calculate the sum of the lowest two frequency bands.
    int temp = (this->fft[0] + this->fft[1]);

    // Calculate noise constant
    int noiseConstant = noiseFloor * 20;

    // Noise suppression: If the sum is less than 1500, do nothing.
    if (temp < noiseConstant) {
        temp = noiseConstant;
    }
    uint16_t res = ((temp - noiseConstant) * (numLeds / 2) / (4096*2 - noiseConstant));

    // Now, factor in sensitivity.
    // sensitivty = 50 -> no transformation
    // sensitivty < 50 -> reduce length of res
    // sensitivity > 50 -> increase length of res
    int16_t effectiveSensitivity = (sensitivity - 50); 

    // Can be increased by up to a factor of 5, or down to zero.
    this->len = uint16_t(float(res) * pow(2, float(effectiveSensitivity) / 12.5));

    // If we get a length greater than NUM_LEDS, then set it equal to NUM_LEDS
    if (this->len > numLeds) {
        this->len = numLeds;
    }
    else if (this->len < 0) {
        this->len = 0;
    }

    return this->len;
}

// Extra: simple square function
inline int square(int x) {
    return x*x;
}

