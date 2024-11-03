/*
* AteRotary.h
*/

#pragma once

/*
* Ate Rotary will be used to add keyboard navigation with a text editor
*/

// Todo: Add internal label
// Todo: Add keyboard control
// Todo: Fix navigation issues

class RotaryDefault : public juce::Slider
{
public:
    RotaryDefault() : juce::Slider (juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::NoTextBox)
    {
    }
};
