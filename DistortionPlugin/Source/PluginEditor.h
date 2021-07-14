/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

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

	enum OptionBoxValues
	{
		Analogue = 1,
		Hardclipper = 2,
		Sinefold = 3
		
	};


private:
	void sliderValueChanged(juce::Slider*) override;
	void comboBoxChanged(juce::ComboBox*);

	juce::Slider Gain;			// Volume Slider
	juce::Slider Mix;			// Wet/Dry Mix
	juce::Slider Drive;			// Drive Control
	juce::ComboBox Options;		// Option box with Types of distortion

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DistortionPluginAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistortionPluginAudioProcessorEditor)
};
