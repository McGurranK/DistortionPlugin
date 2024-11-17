/*
 * Atesh main GUI component
 */

#pragma once

class AteshAudioEditor : public juce::AudioProcessorEditor
{
public:
    explicit AteshAudioEditor (AteshAudioProcessor& ProcessorRef);
    ~AteshAudioEditor() override;

    void paint (juce::Graphics& GraphicsRef) override;
    void resized() override;
    
private:
    AtesLookAndFeel atesLookAndFeel;
    
    ComponentMover<RotaryDefault> gainSlider;
    RotaryDefault mixSlider;
    
    RotaryDefault outputGainSlider;
    
    RotaryDefault delayTimeSlider;
    RotaryDefault delayFeedbackSlider;
    
    juce::Slider filterCuttoff {juce::Slider::SliderStyle::LinearVertical, juce::Slider::NoTextBox};
    juce::Slider filterQ {juce::Slider::SliderStyle::LinearVertical, juce::Slider::NoTextBox};
    
    std::unique_ptr<juce::SliderParameterAttachment> gainSliderLink;
    std::unique_ptr<juce::SliderParameterAttachment> mixSliderLink;
    std::unique_ptr<juce::SliderParameterAttachment> driveSliderLink;
    
    std::unique_ptr<juce::SliderParameterAttachment> outputSliderLink;
    
    std::unique_ptr<juce::SliderParameterAttachment> delayTimeSliderLink;
    std::unique_ptr<juce::SliderParameterAttachment> delayFeedbackSliderLink;
    
    std::unique_ptr<juce::SliderParameterAttachment> filterCuttoffSliderLink;
    std::unique_ptr<juce::SliderParameterAttachment> filterQSliderLink;
    
    AteshHeader header;
    juce::Label pluginName {"Atesh", "Atesh"};

    AteshAudioProcessor& audioProcessor;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AteshAudioEditor);
};
