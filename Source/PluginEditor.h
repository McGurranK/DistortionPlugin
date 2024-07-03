/*
 * GUI implementation
 */

#pragma once

#include <JuceHeader.h>

class DistortionPluginAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    DistortionPluginAudioProcessorEditor (DistortionPluginAudioProcessor&, juce::AudioProcessorValueTreeState& vts);
    ~DistortionPluginAudioProcessorEditor() override = default;

    void resized() override;

	typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
private:
	juce::Slider GainSlider		{juce::Slider::Rotary, juce::Slider::NoTextBox};
	std::unique_ptr<SliderAttachment> gainAttachment;
	
	juce::Slider DriveSlider	{juce::Slider::Rotary,juce::Slider::NoTextBox};
	std::unique_ptr<SliderAttachment> driveAttachment;
	
	juce::Slider MixSlider		{juce::Slider::Rotary,juce::Slider::NoTextBox};
	std::unique_ptr<SliderAttachment> mixAttachment;			

    DistortionPluginAudioProcessor& audioProcessor;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistortionPluginAudioProcessorEditor);
};
