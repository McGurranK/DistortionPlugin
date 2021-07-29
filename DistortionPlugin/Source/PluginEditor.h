/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class DistortionPluginAudioProcessorEditor  : public juce::AudioProcessorEditor, juce::ComboBox::Listener, juce::Slider::Listener
{
public:
    DistortionPluginAudioProcessorEditor (DistortionPluginAudioProcessor&, juce::AudioProcessorValueTreeState& vts);
    ~DistortionPluginAudioProcessorEditor() override;

    //==============================================================================
    void resized() override;
	void OptionChangeEditor();
	const juce::StringArray distortionTypes{"Analogue","Hardclipper","Sinefold" };
	typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;


private:
	juce::AudioProcessorValueTreeState& valueTreeState;

	// Initialise Gain Slider and Slider Attachment
	juce::Slider GainSlider		{juce::Slider::Rotary,juce::Slider::NoTextBox};			// Volume Slider
	std::unique_ptr<SliderAttachment> gainAttachment;
	
	// Initialise Drive Slider and Slider Attachment
	juce::Slider DriveSlider	{juce::Slider::Rotary,juce::Slider::NoTextBox};			// Drive Control
	std::unique_ptr<SliderAttachment> driveAttachment;
	
	// Mix 
	juce::Slider MixSlider		{juce::Slider::Rotary,juce::Slider::NoTextBox};			// Wet / Dry Mix
	std::unique_ptr<SliderAttachment> mixAttachment;			
	
	void comboBoxChanged(juce::ComboBox*) override;
	void sliderValueChanged(juce::Slider*) override;

	juce::ComboBox OptionsCombobox;	// Option box with Types of distortion
	

	// This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DistortionPluginAudioProcessor& audioProcessor;

	//JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DistortionPluginAudioProcessorEditor);
};
