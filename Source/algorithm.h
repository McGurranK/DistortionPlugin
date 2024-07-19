/*
 * Plugins DSP
 */

#pragma once

class AteAlgorithm final : public juce::AudioProcessorValueTreeState::Listener
{
public:
    AteAlgorithm (Parameters& Parameters)
    {
        // Add listener function
    }
    
    void parameterChanged (const juce::String& ParameterID, float NewValue) override
    {
        
    }
    
    void prepare (double SampleRate, unsigned int BlockSize, unsigned int NumberOfChannels)
    {
        
    }
    
    void process (juce::AudioBuffer<float> AudioBuffer, juce::MidiMessage& MidiMessages)
    {
        
    }
    
};
