/*
* Atesh Plugin Processor
*/

#pragma once

#include <JuceHeader.h>
#include "AteParameters.h"
#include "algorithm.h"
#include "FifoGUIBuffer.h"
#include "WaveformVisualiser.h"

class AteshAudioProcessor final : public juce::AudioProcessor
{
public:
	AteshAudioProcessor();
    ~AteshAudioProcessor() override = default;

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
    
    AteshFiFo waveformVisualiserFifo;
    
private:
    Parameters algorithmParameters;
    AteAlgorithm dspAlgorithm;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AteshAudioProcessor)
};
