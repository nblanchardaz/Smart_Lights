#include "MasterLedController.h"

MasterLedController::MasterLedController() {
    return;
}

MasterLedController &MasterLedController::getInstance()
{   
    static MasterLedController instance;
    return instance;
}

void MasterLedController::calculateStrip() {

    Serial.println("Now calculating: ");

    // To enable music reactivity:
    this->chip->getResults();
    this->chip->calculateLen();

    // WS2812x
    if (this->protocol == "NeoEsp32Rmt0Ws2812xMethod") {

        // Primary part of the segment (music reactivity)
        for (int j = this->segment_12->start; j < this->chip->len; j++)
        {
            this->segment_12->SetPixelColor(j, this->generator->calculatePixel(this->segment_12->getPrimaryStartingColor(), this->segment_12->getPrimaryEndingColor(), this->segment_12->getPrimarySpeed()));
        }

        // Secondary part of the segment (post music reactivity)
        for (int j = this->chip->len; j < this->segment_12->end; j++)
        {
            this->segment_12->SetPixelColor(j, this->generator->calculatePixel(this->segment_12->getSecondaryStartingColor(), this->segment_12->getSecondaryEndingColor(), this->segment_12->getSecondarySpeed()));
        }
    }

    // WS2811
    else if (this->protocol == "NeoEsp32Rmt0Ws2811Method") {
        // Primary part of the segment (music reactivity)
        for (int j = this->segment_11->start; j < this->chip->len; j++)
        {
            // this->segment_11->SetPixelColor(j, this->generator->calculatePixel(this->segment_11->getPrimaryStartingColor(), this->segment_11->getPrimaryEndingColor(), this->segment_11->getPrimarySpeed()));
            this->segment_11->SetPixelColor(j, RgbColor(255, 0, 0));
        }

        // Secondary part of the segment (post music reactivity)
        for (int j = this->chip->len; j < this->segment_11->end; j++)
        {
            // this->segment_11->SetPixelColor(j, this->generator->calculatePixel(this->segment_11->getSecondaryStartingColor(), this->segment_11->getSecondaryEndingColor(), this->segment_11->getSecondarySpeed()));
            this->segment_11->SetPixelColor(j, RgbColor(0, 0, 255));
        }
    }
    else {
        Serial.println("Error calculating strip - protocol not recognized.");
    }
}

void MasterLedController::showStrip() {
    Serial.println("Now showing: ");
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