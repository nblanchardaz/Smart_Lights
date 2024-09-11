#include "MasterLedController.h"


// Constructor
MasterLedController::MasterLedController() {

    // No work to do
    // Idea -> pass in 'protocol' as an argument to constructor,
    // that way we don't need to have a separate call to the
    // setProtocol() function.

    return;
}

// Singleton class
MasterLedController &MasterLedController::getInstance() {   
    static MasterLedController instance;
    return instance;
}

// Update protocol saved in member variable
void MasterLedController::setProtocol(String* _protocol) {
    this->protocol = *_protocol;
}

// Initialize stored parameters based on what is dug up out of flash memory.
// Function can ONLY BE CALLED ONCE. Because there's no protection to prevent segment_11 from creating a new LedStripSegment if it already exists.
void MasterLedController::loadParams(RgbColor* _primaryStarting, RgbColor* _primaryEnding, uint16_t* _primarySpeed, uint16_t* _primarySensitivity, uint16_t* _primaryNoiseFloor, RgbColor* _secondaryStarting, RgbColor* _secondaryEnding, uint16_t* _secondarySpeed, uint8_t* _mode, uint16_t* _numLeds) {
    
    // First, quantify the protocol
    uint8_t quant = (this->protocol == "NeoEsp32Rmt0Ws2812xMethod");
    
        switch (quant) {
            case 0:
                this->segment_11 = new LedStripSegment<NeoGrbFeature, NeoEsp32Rmt0Ws2811Method>(sigPinDef, 0, MAX_LEDS, 1);
                this->segment_11->setParameters(*_primaryStarting, *_primaryEnding, *_primarySensitivity, *_secondaryStarting, *_secondaryEnding, *_primarySpeed, *_secondarySpeed, this->protocol, *_primaryNoiseFloor, *_mode, *_numLeds);
                Serial.println("Initialized as Ws2811.");
                break;
            case 1:
                this->segment_12 = new LedStripSegment<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod>(sigPinDef, 0, MAX_LEDS, 1);
                this->segment_12->setParameters(*_primaryStarting, *_primaryEnding, *_primarySensitivity, *_secondaryStarting, *_secondaryEnding, *_primarySpeed, *_secondarySpeed, this->protocol, *_primaryNoiseFloor, *_mode, *_numLeds);
                Serial.println("Initialized as Ws2812x.");
                break;
            default:
                Serial.println("Protocol uninitialized.");
        }
    }

// Change protocol to WS2811.
// Note: Must delete all other instances of LedStripSegment before creating a new one.
// Haveing multiple instantiated at once will instantly crash the ESP32 with an RMT driver_install() error.
void MasterLedController::addWs2811() {
    // Delete other segment if it exists
    if (this->segment_12 != nullptr) {
        delete this->segment_12;
        this->segment_12 = nullptr;
    }
    // Instantiate our new segment if it doesn't exist
    if (this->segment_11 == nullptr) {
        this->segment_11 = new LedStripSegment<NeoGrbFeature, NeoEsp32Rmt0Ws2811Method>(sigPinDef, 0, MAX_LEDS, 1);
    }
}

// Change protocol to WS2812x.
// Note: Must delete all other instances of LedStripSegment before creating a new one.
// Haveing multiple instantiated at once will instantly crash the ESP32 with an RMT driver_install() error.
void MasterLedController::addWs2812x() {
    // Delete other segment if it exists
    if (this->segment_11 != nullptr) {
        delete this->segment_11;
        this->segment_11 = nullptr;
    }
    // Instantiate our new segment if it doesn't exist
    if (this->segment_12 == nullptr) {
        this->segment_12 = new LedStripSegment<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod>(sigPinDef, 0, MAX_LEDS, 1);
    }
}

// Print out strip parameters.
void MasterLedController::printParameters() {

    if (this->protocol == "NeoEsp32Rmt0Ws2811Method") {
        Serial.println("Primary Start Color: " + String((this->segment_11->getPrimaryStartingColor())[0]) + " " + String((this->segment_11->getPrimaryStartingColor())[1]) + " " + String((this->segment_11->getPrimaryStartingColor())[2]) + " ");
        Serial.println("Primary End Color: " + String((this->segment_11->getPrimaryEndingColor())[0]) + " " + String((this->segment_11->getPrimaryEndingColor())[1]) + " " + String((this->segment_11->getPrimaryEndingColor())[2]) + " ");
        Serial.println("Primary Vel: " + String(this->segment_11->getPrimarySpeed()) + " ");
        Serial.println("Primary Sens: " + String(this->segment_11->getPrimarySensitivity()) + " ");
        Serial.println("Primary Noise Floor: " + String(this->segment_11->getPrimaryNoiseFloor()) + " ");
        Serial.println("Secondary Start Color: " + String((this->segment_11->getSecondaryStartingColor())[0]) + " " + String((this->segment_11->getSecondaryStartingColor())[1]) + " " + String((this->segment_11->getSecondaryStartingColor())[2]) + " ");
        Serial.println("Starting End Color: " + String((this->segment_11->getSecondaryEndingColor())[0]) + " " + String((this->segment_11->getSecondaryEndingColor())[1]) + " " + String((this->segment_11->getSecondaryEndingColor())[2]) + " ");
        Serial.println("Secondary Vel: " + String(this->segment_11->getSecondarySpeed()) + " ");
        Serial.println("Protocol: " + this->protocol);
        Serial.println("Mode: " + String(this->segment_11->getMode()));
        Serial.println("Num LEDS: " + String(this->segment_11->getNumLeds()));
        Serial.println("");
    }
    else if (this->protocol == "NeoEsp32Rmt0Ws2812xMethod") {
        Serial.println("Primary Start Color: " + String((this->segment_12->getPrimaryStartingColor())[0]) + " " + String((this->segment_12->getPrimaryStartingColor())[1]) + " " + String((this->segment_12->getPrimaryStartingColor())[2]) + " ");
        Serial.println("Primary End Color: " + String((this->segment_12->getPrimaryEndingColor())[0]) + " " + String((this->segment_12->getPrimaryEndingColor())[1]) + " " + String((this->segment_12->getPrimaryEndingColor())[2]) + " ");
        Serial.println("Primary Vel: " + String(this->segment_12->getPrimarySpeed()) + " ");
        Serial.println("Primary Sens: " + String(this->segment_12->getPrimarySensitivity()) + " ");
        Serial.println("Primary Noise Floor: " + String(this->segment_12->getPrimaryNoiseFloor()) + " ");
        Serial.println("Secondary Start Color: " + String((this->segment_12->getSecondaryStartingColor())[0]) + " " + String((this->segment_12->getSecondaryStartingColor())[1]) + " " + String((this->segment_12->getSecondaryStartingColor())[2]) + " ");
        Serial.println("Starting End Color: " + String((this->segment_12->getSecondaryEndingColor())[0]) + " " + String((this->segment_12->getSecondaryEndingColor())[1]) + " " + String((this->segment_12->getSecondaryEndingColor())[2]) + " ");
        Serial.println("Secondary Vel: " + String(this->segment_12->getSecondarySpeed()) + " ");
        Serial.println("Protocol: " + this->protocol);
        Serial.println("Mode: " + String(this->segment_12->getMode()));
        Serial.println("Num LEDS: " + String(this->segment_12->getNumLeds()));
        Serial.println("");
    }
    return;
}

// Figure out what each pixel color needs to be.
void MasterLedController::calculateStrip() {

    // To enable music reactivity:
    this->chip->getResults();

    // WS2812x
    if (this->protocol == "NeoEsp32Rmt0Ws2812xMethod") {

        // Calculate result of music reactivity
        this->chip->calculateLen(this->segment_12->getPrimarySensitivity(), this->segment_12->getPrimaryNoiseFloor(), this->segment_12->getNumLeds());

        // Primary part of the segment (music reactivity)
        for (int j = this->segment_12->start; j < this->chip->len; j++)
        {
            this->segment_12->SetPixelColor(j, this->generator->calculatePixel(this->segment_12->getMode(), this->segment_12->getPrimaryStartingColor(), this->segment_12->getPrimaryEndingColor(), this->segment_12->getPrimarySpeed()));
        }

        // Secondary part of the segment (post music reactivity)
        for (int j = this->chip->len; j < this->segment_12->end; j++)
        {
            this->segment_12->SetPixelColor(j, this->generator->calculatePixel(this->segment_12->getMode(), this->segment_12->getSecondaryStartingColor(), this->segment_12->getSecondaryEndingColor(), this->segment_12->getSecondarySpeed()));
        }
    }

    // WS2811
    else if (this->protocol == "NeoEsp32Rmt0Ws2811Method") {

        // Calculate result of music reactivity
        this->chip->calculateLen(this->segment_11->getPrimarySensitivity(), this->segment_11->getPrimaryNoiseFloor(), this->segment_11->getNumLeds());

        // Primary part of the segment (music reactivity)
        for (int j = this->segment_11->start; j < this->chip->len; j++)
        {
            this->segment_11->SetPixelColor(j, this->generator->calculatePixel(this->segment_11->getMode(), this->segment_11->getPrimaryStartingColor(), this->segment_11->getPrimaryEndingColor(), this->segment_11->getPrimarySpeed()));
        }

        // Secondary part of the segment (post music reactivity)
        for (int j = this->chip->len; j < this->segment_11->end; j++)
        {
            this->segment_11->SetPixelColor(j, this->generator->calculatePixel(this->segment_11->getMode(), this->segment_11->getSecondaryStartingColor(), this->segment_11->getSecondaryEndingColor(), this->segment_11->getSecondarySpeed()));
        }
    }
    else {
        Serial.println("Error calculating strip - protocol not recognized.");
    }
}

// Update the strip to show the calculated pixel values.
void MasterLedController::showStrip() {

    if (this->protocol == "NeoEsp32Rmt0Ws2812xMethod") {
        this->segment_12->Show();
    }
    else if (this->protocol == "NeoEsp32Rmt0Ws2811Method") {
        this->segment_11->Show();
    }
    else {
        Serial.println("Error showing strip - protocol not recognized.");
    }
}

// Loop function: figure out pixel colors, then show those pixel colors on the strip.
void MasterLedController::doStuff() {

    // Figure out what each pixel color should be, and set the pixels to their colors
    calculateStrip();

    // Update the physical pixels on the strip with the new colors
    showStrip();

}