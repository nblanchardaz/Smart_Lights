#include "MasterLedController.h"

MasterLedController::MasterLedController() {
    return;
}

MasterLedController &MasterLedController::getInstance()
{   
    static MasterLedController instance;
    return instance;
}

void MasterLedController::calculateStrip()
{
    if (this->protocol == "NeoEsp32Rmt0Ws2812xMethod") {
        // Existing functionality to set pixel color using particle generator
        for (int j = this->segment_12->start; j < this->segment_12->end; j++)
        {
            this->segment_12->SetPixelColor(j, this->generator->calculatePixel());
        }

        // To enable music reactivity:
        this->chip->getResults();
        this->chip->calculateLen();
        for (int j = this->segment_12->start; j < this->segment_12->end; j++)
        {
            this->segment_12->SetPixelColor(j, this->chip->c);
        }
    }
    else if (this->protocol == "NeoEsp32Rmt0Ws2811Method") {
        // Existing functionality to set pixel color using particle generator
        for (int j = this->segment_11->start; j < this->segment_11->end; j++)
        {
            this->segment_11->SetPixelColor(j, this->generator->calculatePixel());
        }

        // To enable music reactivity:
        this->chip->getResults();
        this->chip->calculateLen();
        for (int j = this->segment_11->start; j < this->segment_11->end; j++)
        {
            this->segment_11->SetPixelColor(j, this->chip->c);
        }
    }
}

void MasterLedController::showStrip() {
    if (this->protocol == "NeoEsp32Rmt0Ws2812xMethod") {
        this->segment_12->Show();
    }
    else if (this->protocol == "NeoEsp32Rmt0Ws2811Method") {
        this->segment_11->Show();
    }
}

void MasterLedController::loop()
{
    // DB.updatePalette();
    calculateStrip();
    showStrip();        //this->segment->Show();
}