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
class DistortionPluginAudioProcessorEditor  : public juce::AudioProcessorEditor,
	public juce::Slider::Listener, public juce::ComboBox::Listener
{
public:
    DistortionPluginAudioProcessorEditor (DistortionPluginAudioProcessor&);
    ~DistortionPluginAudioProcessorEditor() override;

    //==============================================================================
    void resized() override;
	void OptionChangeEditor();
	const juce::StringArray distortionTypes{"Analogue","Hardclipper","Sinefold" };



private:

	void sliderValueChanged(juce::Slider* slider) override;
	void comboBoxChanged(juce::ComboBox*) override;

	juce::Slider GainSlider		{juce::Slider::Rotary,juce::Slider::NoTextBox};			// Volume Slider
	juce::Slider DriveSlider	{juce::Slider::Rotary,juce::Slider::NoTextBox};			// Drive Control
	juce::Slider MixSlider		{juce::Slider::Rotary,juce::Slider::NoTextBox};			// Wet / Dry Mix
	juce::ComboBox OptionsCombobox;	// Option box with Types of distortion
	

	// This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DistortionPluginAudioProcessor& audioProcessor;

	//JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DistortionPluginAudioProcessorEditor);
};
