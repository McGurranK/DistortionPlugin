/*
* AteLookAndFeel.h
*/

#pragma once

/*
*  Custom Look and feel for the Ate Plugin
*/

class AtesLookAndFeel final : public juce::LookAndFeel_V4
{
public:
    AtesLookAndFeel();
    ~AtesLookAndFeel() override = default;
    
    void drawRotarySlider (juce::Graphics& Graphics, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& SliderRef) override;
};
