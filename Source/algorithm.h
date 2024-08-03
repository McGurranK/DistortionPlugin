/*
 * Plugins DSP
 */

#pragma once

class AteAlgorithm final : public juce::AudioProcessorValueTreeState::Listener
{
public:
    AteAlgorithm (Parameters& Parameters) : dspParameters (Parameters)
    {
        Parameters.LinkParametersToListener (this);
        
        delayLineProcessor.setMaximumDelayInSamples (48000);
        delayLineProcessor.setDelay (1000);
    }
    
    ~AteAlgorithm ()
    {
        jassert (&dspParameters);
        dspParameters.RemoveParameterListener (this);
    }
        
    void parameterChanged (const juce::String& ParameterID, float NewValue) override
    {
        if (ParameterID == dspParameters.parameterIDS.inputGainID)
            inputDriveProcessor.setGainDecibels (NewValue);
        if (ParameterID == dspParameters.parameterIDS.mixID)
            mixProcessor.setWetMixProportion (NewValue / 100.f); // Parameter uses the displayed value of 0-100
        if (ParameterID == dspParameters.parameterIDS.delayTimeID)
            delayLineProcessor.setDelay (NewValue);
        if (ParameterID == dspParameters.parameterIDS.delayfeedBackAmountID)
            delayAmountGainProcessor.setGainDecibels (NewValue);
        if (ParameterID == dspParameters.parameterIDS.filterCuttOffID)
            stateVariableFilter.setCutoffFrequency (NewValue);
        if (ParameterID == dspParameters.parameterIDS.filterQID)
            stateVariableFilter.setResonance (NewValue);
        if (ParameterID == dspParameters.parameterIDS.outputGainID)
            outputGainProcessor.setGainDecibels (NewValue);
    }
    
    void prepare (double SampleRate, unsigned int BlockSize, unsigned int NumberOfChannels)
    {
        juce::dsp::ProcessSpec spec { SampleRate, BlockSize, NumberOfChannels };
        
        mixProcessor.prepare (spec);
        inputDriveProcessor.prepare (spec);
        waveShaperProcessor.prepare (spec);
        delayAmountGainProcessor.prepare (spec);
        delayLineProcessor.prepare (spec);
        stateVariableFilter.prepare (spec);
        outputGainProcessor.prepare (spec);
    }
    
    void process (juce::AudioBuffer<float> AudioBuffer, juce::MidiBuffer& MidiMessages)
    {
//        juce::ignoreUnused (MidiMessages);
//        
//        const auto numberOfChannels = AudioBuffer.getNumChannels();
//        const auto numberOfSamples = AudioBuffer.getNumSamples();
//        
//        const auto currentDelayTime = dspParameters.timePtr->getValue();
//        
//        juce::dsp::AudioBlock<float> audioBlock;
//        juce::dsp::ProcessContextReplacing<float> processorContext (audioBlock);
//        
//        mixProcessor.pushDrySamples (audioBlock);
//        inputDriveProcessor.process (processorContext);
//        
//        // Delay pop, gain and filter
//        for (int channel = 0; channel < numberOfChannels; channel++)
//        {
//            const auto channelPointer = AudioBuffer.getWritePointer (channel);
//            
//            for (int sampleIndex = 0; sampleIndex < numberOfSamples; sampleIndex++)
//            {
//                auto tempSampleCopy =  channelPointer[sampleIndex];
//                auto delaySample = delayLineProcessor.popSample (channel, currentDelayTime);
//                delaySample = stateVariableFilter.processSample (channel, delaySample);
//                
//                delaySample = delayAmountGainProcessor.processSample (delaySample);
//                
//                channelPointer[sampleIndex] = tempSampleCopy + delaySample;
//            }
//        }
//        
//        waveShaperProcessor.process (processorContext); // Implement filters around this
//        
//        // Delay Push
//        for (int channel = 0; channel < numberOfChannels; channel++)
//        {
//            const auto channelPointer = AudioBuffer.getWritePointer (channel);
//            
//            for (int sampleIndex = 0; sampleIndex < numberOfSamples; sampleIndex++)
//                delayLineProcessor.pushSample (channel, channelPointer [sampleIndex]);
//        }
//        
//        juce::dsp::AudioBlock<float> wetSampleBlock (AudioBuffer);
//        mixProcessor.mixWetSamples (wetSampleBlock);
//        
//        juce::dsp::AudioBlock<float> outputBlock;
//        juce::dsp::ProcessContextReplacing<float> outputContext (outputBlock);
//        outputGainProcessor.process (outputContext);
    }

    Parameters& dspParameters;
    
    juce::dsp::Gain<float> outputGainProcessor;
    juce::dsp::DryWetMixer<float> mixProcessor;
    
    juce::dsp::Gain<float> inputDriveProcessor;
    juce::dsp::WaveShaper<float> waveShaperProcessor;
    
    juce::dsp::Gain<float> delayAmountGainProcessor;
    
    juce::dsp::DelayLine<float> delayLineProcessor;

    juce::dsp::StateVariableTPTFilter<float> stateVariableFilter;
};
