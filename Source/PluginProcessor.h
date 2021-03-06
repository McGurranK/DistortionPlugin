/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class DistortionPluginAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
	DistortionPluginAudioProcessor();				// Processor Constructor
    ~DistortionPluginAudioProcessor() override;		// Processor Destructor

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
	
    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
	void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
	float OptionChange(float *channelData);
	int switchOptions;


private:
	
	// Initialise AudioProcessorValueTreeState as parameters
	juce::AudioProcessorValueTreeState parameters;
	
	// Atomic float varibles used to store value tree varibles.
	// Gain Variable
	std::atomic<float>* mGainParameter;	
	
	// Drive Parameter
	std::atomic<float>* mDriveParameter;
	
	// Mix Variable
	std::atomic<float>* mMixParameter;
	
	// Parameter for switching between algorithms
	juce::AudioParameterInt* mSwitchParameter;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistortionPluginAudioProcessor)
};