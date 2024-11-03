/*
 * AteAlgorithm.h
 */

#pragma once

/*
* Contained algorithm with listener callback mechanism
*/

class AteAlgorithm final : public juce::AudioProcessorValueTreeState::Listener
{
public:
    AteAlgorithm (Parameters& Parameters) : dspParameters (Parameters)
    {
        Parameters.LinkParametersToListener (this);
        
        delayLineProcessor.setMaximumDelayInSamples (48000);
        delayLineProcessor.setDelay (1000);
        
        waveShaperProcessor.functionToUse = [](float value) {return std::tanh (value); };
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
            mixProcessor.setWetMixProportion (NewValue / 100.f);
        if (ParameterID == dspParameters.parameterIDS.delayTimeID)
            delayTimeSmoothing.setTargetValue (NewValue);
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
        
        delayTimeSmoothing.reset (SampleRate, 0.05f);
        
        mixProcessor.prepare (spec);
        inputDriveProcessor.prepare (spec);
        waveShaperProcessor.prepare (spec);
        delayAmountGainProcessor.prepare (spec);
        delayLineProcessor.prepare (spec);
        stateVariableFilter.prepare (spec);
        outputGainProcessor.prepare (spec);
    }
    
    void process (juce::AudioBuffer<float>& AudioBuffer, juce::MidiBuffer& MidiMessages)
    {
        const auto numberOfChannels = AudioBuffer.getNumChannels();
        const auto numberOfSamples = AudioBuffer.getNumSamples();
        
        juce::dsp::AudioBlock<float> audioBlock (AudioBuffer);
        juce::dsp::ProcessContextReplacing<float> processorContext (audioBlock);
        
        mixProcessor.pushDrySamples (audioBlock);
        inputDriveProcessor.process (processorContext);
        
        // Delay pop, gain and filter
        for (int channel = 0; channel < numberOfChannels; channel++)
        {
            const auto channelPointer = AudioBuffer.getWritePointer (channel);
            
            for (int sampleIndex = 0; sampleIndex < numberOfSamples; sampleIndex++)
            {
                auto tempSampleCopy =  channelPointer[sampleIndex];
                auto delaySample = delayLineProcessor.popSample (channel,  delayTimeSmoothing.getNextValue());
                delaySample = stateVariableFilter.processSample (channel, delaySample);
                
                delaySample = delayAmountGainProcessor.processSample (delaySample);
                
                channelPointer[sampleIndex] = tempSampleCopy + delaySample;
            }
        }
        
        juce::dsp::AudioBlock<float> waveBlock (AudioBuffer);
        juce::dsp::ProcessContextReplacing<float> waveShapingContext (waveBlock);
        waveShaperProcessor.process (waveShapingContext); // Implement filters around this
        
        // Delay Push
        for (int channel = 0; channel < numberOfChannels; channel++)
        {
            const auto channelPointer = AudioBuffer.getWritePointer (channel);
            
            for (int sampleIndex = 0; sampleIndex < numberOfSamples; sampleIndex++)
                delayLineProcessor.pushSample (channel, channelPointer [sampleIndex]);
        }

        juce::dsp::AudioBlock<float> outputBlock (AudioBuffer);
        juce::dsp::ProcessContextReplacing<float> outputContext (outputBlock);
        
        mixProcessor.mixWetSamples (audioBlock);
        outputGainProcessor.process (outputContext);
    }

    juce::SmoothedValue<float> delayTimeSmoothing;
    
    Parameters& dspParameters;
    
    juce::dsp::Gain<float> inputDriveProcessor;
    juce::dsp::Gain<float> outputGainProcessor;
    juce::dsp::DryWetMixer<float> mixProcessor;

    juce::dsp::WaveShaper<float> waveShaperProcessor;
    
    juce::dsp::Gain<float> delayAmountGainProcessor;
    
    juce::dsp::DelayLine<float> delayLineProcessor;

    juce::dsp::StateVariableTPTFilter<float> stateVariableFilter;
};
