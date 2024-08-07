/*
* Ate Parameter struct for easy access to parametes
*/

#pragma once

struct AteParameterIDs final
{
    juce::String outputGainID { "OUTG" };
    juce::String mixID { "MIX" };
    juce::String inputGainID { "DRIV" };
    
    juce::String delayTimeID { "TIME" };
    juce::String delayfeedBackAmountID { "FGAIN" };
    
    juce::String filterCuttOffID { "CUTT" };
    juce::String filterQID { "Q" };
    juce::String filterTypeID { "TIME" };
    
    std::vector<juce::String*> allParameterIDs { &outputGainID, &mixID, &inputGainID, &delayTimeID, &delayfeedBackAmountID, &filterCuttOffID, &filterQID, &filterTypeID};
};

struct Parameters final
{
    Parameters (juce::AudioProcessor& ProcessorRef) : parameterValueTree (ProcessorRef, nullptr, "PARAMS", createLayout())
    {
    }
    
    void LinkParametersToListener (juce::AudioProcessorValueTreeState::Listener* ListenerRef)
    {
        for (auto* ParameterIDs : parameterIDS.allParameterIDs)
            parameterValueTree.addParameterListener (*ParameterIDs, ListenerRef);
    }
    
    void RemoveParameterListener (juce::AudioProcessorValueTreeState::Listener* ListenerRef)
    {
        for (auto* ParameterIDs : parameterIDS.allParameterIDs)
            parameterValueTree.removeParameterListener (*ParameterIDs, ListenerRef);
    }

    AteParameterIDs parameterIDS;
    juce::AudioProcessorValueTreeState parameterValueTree;
    
private:
    juce::AudioProcessorValueTreeState::ParameterLayout createLayout()
    {
        juce::AudioProcessorValueTreeState::ParameterLayout parameterLayout;
    
        parameterLayout.add (std::make_unique<juce::AudioParameterFloat> (parameterIDS.mixID, "Mix", 0.0f, 100.0f, 50.0f));
        parameterLayout.add (std::make_unique<juce::AudioParameterFloat> (parameterIDS.inputGainID, "Drive", -80.f, 40.0f, 0.0f));

        parameterLayout.add (std::make_unique<juce::AudioParameterFloat> (parameterIDS.delayTimeID, "Time", 1.f, 10000.0f, 1.0f));
        parameterLayout.add (std::make_unique<juce::AudioParameterFloat> (parameterIDS.delayfeedBackAmountID, "Feedback Amount", -40.f, 40.0f, 0.0f));
        
        parameterLayout.add (std::make_unique<juce::AudioParameterFloat> (parameterIDS.filterCuttOffID,"cutt", 20.f, 20000.0f, 20000.0f));
        parameterLayout.add (std::make_unique<juce::AudioParameterFloat> (parameterIDS.filterQID, "q", 0.0f, 10.0f, 1.0f));
//        parameterLayout.add (std::make_unique<juce::AudioParameterChoice> (parameterIDS.filterTypeID, "Filter Type", juce::StringArray {"Low Pass", "High Pass", "Band"}, 0));
        
        parameterLayout.add (std::make_unique<juce::AudioParameterFloat> (parameterIDS.outputGainID, "Output Gain",  -80.f, 10.f, 0.f));

        return parameterLayout;
    }
    
};
