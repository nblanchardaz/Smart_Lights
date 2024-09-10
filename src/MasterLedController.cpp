#include "MasterLedController.h"

MasterLedController::MasterLedController() {

    // No work to do
    // Idea -> pass in 'protocol' as an argument to constructor,
    // that way we don't need to have a separate call to the
    // setProtocol() function.

    return;
}

MasterLedController &MasterLedController::getInstance() {   
    static MasterLedController instance;
    return instance;
}

void MasterLedController::setProtocol(String* _protocol) {
    this->protocol = *_protocol;
}

void MasterLedController::calculateStrip() {

    // To enable music reactivity:
    this->chip->getResults();

    // WS2812x
    if (this->protocol == "NeoEsp32Rmt0Ws2812xMethod") {

        // Calculate result of music reactivity
        this->chip->calculateLen(this->segment_12->getPrimarySensitivity(), this->segment_12->getPrimaryNoiseFloor());

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
        this->chip->calculateLen(this->segment_11->getPrimarySensitivity(), this->segment_11->getPrimaryNoiseFloor());

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

void MasterLedController::doStuff() {

    // Figure out what each pixel color should be, and set the pixels to their colors
    calculateStrip();

    // Update the physical pixels on the strip with the new colors
    showStrip();

}


void MasterLedControllerWs2811::loadParams(RgbColor* _primaryStarting, RgbColor* _primaryEnding, uint16_t* _primarySpeed, uint16_t* _primarySensitivity, uint16_t* _primaryNoiseFloor, RgbColor* _secondaryStarting, RgbColor* _secondaryEnding, uint16_t* _secondarySpeed, uint8_t* _mode) {
    this->segment_11->setParameters(*_primaryStarting, *_primaryEnding, *_primarySensitivity, *_secondaryStarting, *_secondaryEnding, *_primarySpeed, *_secondarySpeed, this->protocol, *_primaryNoiseFloor, *_mode);
}


void MasterLedControllerWs2812x::loadParams(RgbColor* _primaryStarting, RgbColor* _primaryEnding, uint16_t* _primarySpeed, uint16_t* _primarySensitivity, uint16_t* _primaryNoiseFloor, RgbColor* _secondaryStarting, RgbColor* _secondaryEnding, uint16_t* _secondarySpeed, uint8_t* _mode) {
    this->segment_11->setParameters(*_primaryStarting, *_primaryEnding, *_primarySensitivity, *_secondaryStarting, *_secondaryEnding, *_primarySpeed, *_secondarySpeed, this->protocol, *_primaryNoiseFloor, *_mode);
}