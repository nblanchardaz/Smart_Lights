#include "MasterLedController.h"

MasterLedController::MasterLedController() {

    // segment_12 = new LedStripSegment<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod>(sigPinDef, 0, NUM_LEDS, 1);
    segment_11 = new LedStripSegment<NeoGrbFeature, NeoEsp32Rmt0Ws2811Method>(sigPinDef, 0, NUM_LEDS, 1);

    return;
}

MasterLedController &MasterLedController::getInstance()
{   
    static MasterLedController instance;
    return instance;
}

void MasterLedController::calculateStrip() {

    // To enable music reactivity:
    this->chip->getResults();

    // WS2812x
    if (this->protocol == "NeoEsp32Rmt0Ws2812xMethod") {

        // Calculate result of music reactivity
        this->chip->calculateLen(this->segment_12->getPrimarySensitivity());

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

        // Calculate result of music reactivity
        this->chip->calculateLen(this->segment_11->getPrimarySensitivity());

        // Primary part of the segment (music reactivity)
        for (int j = this->segment_11->start; j < this->chip->len; j++)
        {
            this->segment_11->SetPixelColor(j, this->generator->calculatePixel(this->segment_11->getPrimaryStartingColor(), this->segment_11->getPrimaryEndingColor(), this->segment_11->getPrimarySpeed()));
        }

        // Secondary part of the segment (post music reactivity)
        for (int j = this->chip->len; j < this->segment_11->end; j++)
        {
            this->segment_11->SetPixelColor(j, this->generator->calculatePixel(this->segment_11->getSecondaryStartingColor(), this->segment_11->getSecondaryEndingColor(), this->segment_11->getSecondarySpeed()));
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