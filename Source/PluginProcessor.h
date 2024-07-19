#pragma once

#include <JuceHeader.h>
#include "AteParameters.h"
#include "algorithm.h"

class DistortionPluginAudioProcessor final : public juce::AudioProcessor
                                           , public juce::AudioProcessorValueTreeState::Listener
{
public:
	DistortionPluginAudioProcessor();
    ~DistortionPluginAudioProcessor() override = default;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
	
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

	void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    void parameterChanged (const juce::String& parameterID, float newValue) final;

    juce::AudioProcessorValueTreeState::ParameterLayout createAPVTSLayout();

private:
    juce::dsp::DryWetMixer<float> mixControl;
    
    juce::dsp::Gain<float> inputDriveProcessor;
    juce::dsp::WaveShaper<float> waveShaper;
    
    juce::dsp::Gain<float> outputGain;
    
    juce::dsp::DelayLine<float> feedbackPath;
    
	juce::AudioProcessorValueTreeState parameters;
    
    juce::dsp::StateVariableTPTFilter<float> stateVariableFilter;
    juce::dsp::StateVariableTPTFilter<float> svfBandFilter;
    
    juce::dsp::Oscillator<float> modulation { [](float Frequency){ return std::sin (Frequency); } };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistortionPluginAudioProcessor)
};
