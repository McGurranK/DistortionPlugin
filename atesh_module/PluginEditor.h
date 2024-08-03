/*
 * Atesh main GUI component
 */

#pragma once

class AtesLookAndFeel final : public juce::LookAndFeel_V4
{
public:
    AtesLookAndFeel() = default;
    
    void drawRotarySlider (juce::Graphics& Graphics, int x, int y, int width, int height,
                           float sliderPosProportional, float rotaryStartAngle,
                           float rotaryEndAngle, juce::Slider& SliderRef) override
    {
        Graphics.setColour (juce::Colours::red);
        Graphics.drawText (SliderRef.getName(), x, y, width, height, juce::Justification::centredTop);
        Graphics.drawText (SliderRef.getTextFromValue (SliderRef.getValue()), x, y, width, height, juce::Justification::centredBottom);
        juce::LookAndFeel_V4::drawRotarySlider (Graphics, x, y, width, height, sliderPosProportional, rotaryStartAngle,
                                                rotaryEndAngle, SliderRef);
    }
};

class AteshAudioEditor : public juce::AudioProcessorEditor
{
public:
    explicit AteshAudioEditor (AteshAudioProcessor& ProcessorRef);
    ~AteshAudioEditor() override = default;

    void paint (juce::Graphics& GraphicsRef) override;
    void resized() override;
    
private:
	juce::Slider gainSlider		{ juce::Slider::Rotary, juce::Slider::NoTextBox };
    juce::Slider driveSlider    { juce::Slider::Rotary, juce::Slider::NoTextBox };
    juce::Slider mixSlider      { juce::Slider::Rotary, juce::Slider::NoTextBox };
    
    juce::Slider outputGainSlider { juce::Slider::Rotary, juce::Slider::NoTextBox };
    juce::Slider delayTimeSlider { juce::Slider::Rotary, juce::Slider::NoTextBox };
    juce::Slider delayFeedbackSlider { juce::Slider::Rotary, juce::Slider::NoTextBox };
    
    juce::Slider filterCuttoff { juce::Slider::Rotary, juce::Slider::NoTextBox };
    juce::Slider filterQ { juce::Slider::Rotary, juce::Slider::NoTextBox };

    AteshAudioProcessor& audioProcessor;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AteshAudioEditor);
};
