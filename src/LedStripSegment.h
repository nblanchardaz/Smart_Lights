#pragma once

#include <NeopixelBus.h>

/**
 * @class LedStripSegment
 * @brief This class represents a segment of an LED strip.
 *
 * @tparam Feature The feature set for the LED strip.
 * @tparam Method The method used to drive the LED strip.
 */
template <class Feature, class Method>
class LedStripSegment
{
public:
    /**
     * @brief Construct a new Led Strip Segment object.
     *
     * @param pin The pin number.
     * @param start The start position of the segment.
     * @param end The end position of the segment.
     * @param direction The direction of the segment.
     */
    LedStripSegment(int _pin, int _start, int _end, int _direction)
        : strip(_end - _start, _pin)
    {
        pin = _pin;
        start = _start;
        end = _end;
        direction = _direction;

        pinMode(pin, OUTPUT);
        strip.Begin();
        strip.Show();
    }

    /**
     * @brief Set the color of a pixel in the segment.
     *
     * @param position The position of the pixel.
     * @param color The color to set.
     * @return int Returns 0 on success, non-zero on failure.
     */
    void SetPixelColor(int position, RgbColor color)
    {
        if (direction == 1)
        {
            strip.SetPixelColor(position - start, color);
        }
        else
        {
            strip.SetPixelColor(end - position, color);
        }
    }

    void Show()
    {
        strip.Show();
    }

    void setParameters(RgbColor _primaryStarting, RgbColor _primaryEnding, RgbColor _secondaryStarting, RgbColor _secondaryEnding, uint16_t _primarySpeed, uint16_t _secondarySpeed, String _protocol) {
        
        this->primaryStarting = _primaryStarting;
        this->primaryEnding = _primaryEnding;
        this->secondaryStarting = _secondaryStarting;
        this->secondaryEnding = _secondaryEnding;
        this->primarySpeed = _primarySpeed;
        this->secondarySpeed = _secondarySpeed;
        this->protocol = _protocol;

    }

    RgbColor getPrimaryStartingColor() {
        return this->primaryStarting;
    }

    RgbColor getPrimaryEndingColor() {
        return this->primaryEnding;
    }

    uint16_t getPrimarySpeed() {
        return this->primarySpeed;
    }

    RgbColor getSecondaryStartingColor() {
        return this->secondaryStarting;
    }

    RgbColor getSecondaryEndingColor() {
        return this->secondaryEnding;
    }

    uint16_t getSecondarySpeed() {
        return this->secondarySpeed;
    }

    int start; ///< The start position of the segment.
    int end;   ///< The end position of the segment.

private:
    int pin;                            ///< The pin number.
    int direction;                      ///< The direction of the segment.
    NeoPixelBus<Feature, Method> strip; ///< The LED strip object.
    RgbColor primaryStarting;
    RgbColor primaryEnding;
    RgbColor secondaryStarting;
    RgbColor secondaryEnding;
    uint16_t primarySpeed;
    uint16_t secondarySpeed;
    String protocol;
};