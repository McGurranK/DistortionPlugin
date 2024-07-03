/*
* Parameter Struct that contains raw pointers to add to the audio processor
*/

#pragma once

struct AteshParameters
{
    AteshParameters (juce::AudioProcessor& ProcessorRef)
    {
        inputGainParameter = new juce::AudioParameterFloat ("gain", "Gain", 0.0f, 1.0f, 1.0f);
        driveAmountParameter = new juce::AudioParameterFloat ("drive","Drive", 1.0f, 100.0f, 1.0f);
        mixParameter = new juce::AudioParameterFloat ("mix","mix",0.0f,1.0f,1.0f);
        
        for (auto* param : { inputGainParameter, driveAmountParameter, mixParameter })
            ProcessorRef.addParameter (param);
    }
    
    juce::AudioParameterFloat* mixParameter { nullptr };
    juce::AudioParameterFloat* driveAmountParameter { nullptr };
    juce::AudioParameterFloat* inputGainParameter { nullptr };
};
