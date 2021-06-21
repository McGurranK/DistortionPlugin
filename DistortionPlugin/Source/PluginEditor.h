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
public juce::Slider::Listener
{
public:
    DistortionPluginAudioProcessorEditor (DistortionPluginAudioProcessor&);
    ~DistortionPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
<<<<<<< Updated upstream

private:
=======
	void OptionChange();
	void sliderValueChanged(juce::Slider* slider) override;

private:
	juce::Slider Gain; // Volume Slider
	juce::Slider Mix;			// Wet/Dry Mix
	juce::Slider Drive;			// Drive Control
	juce::ComboBox Options;		// Option box with Types of distortion

>>>>>>> Stashed changes
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DistortionPluginAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistortionPluginAudioProcessorEditor)
};
