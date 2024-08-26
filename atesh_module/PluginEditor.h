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

class RotaryDefault : public juce::Slider
{
public:
    RotaryDefault() : juce::Slider (Rotary, juce::Slider::NoTextBox)
    {
        
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
    RotaryDefault gainSlider;
    RotaryDefault mixSlider;
    
    RotaryDefault outputGainSlider;
    
    RotaryDefault delayTimeSlider;
    RotaryDefault delayFeedbackSlider;
    
    RotaryDefault filterCuttoff;
    RotaryDefault filterQ;
    
    AtesLookAndFeel atesLookAndFeel;
    
    std::unique_ptr<juce::SliderParameterAttachment> gainSliderLink;
    std::unique_ptr<juce::SliderParameterAttachment> mixSliderLink;
    std::unique_ptr<juce::SliderParameterAttachment> driveSliderLink;
    
    std::unique_ptr<juce::SliderParameterAttachment> outputSliderLink;
    
    std::unique_ptr<juce::SliderParameterAttachment> delayTimeSliderLink;
    std::unique_ptr<juce::SliderParameterAttachment> delayFeedbackSliderLink;
    
    std::unique_ptr<juce::SliderParameterAttachment> filterCuttoffSliderLink;
    std::unique_ptr<juce::SliderParameterAttachment> filterQSliderLink;
    
    WaveformVisualiser graphVisuliser;

    AteshAudioProcessor& audioProcessor;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AteshAudioEditor);
};
