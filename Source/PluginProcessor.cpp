
#include "PluginProcessor.h"
#include "PluginEditor.h"
#define _USE_MATH_DEFINES
#include <math.h>


DistortionPluginAudioProcessor::DistortionPluginAudioProcessor()
     : AudioProcessor (BusesProperties()
                       .withInput ("Input",  juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true))
    , parameters(*this, nullptr, juce::Identifier("AVTS"),createAPVTSLayout())
{
    parameters.addParameterListener ("gain", this);
    parameters.addParameterListener ("drive", this);
    parameters.addParameterListener ("mix", this);
    parameters.addParameterListener ("q", this);
    parameters.addParameterListener ("cutt", this);
    parameters.addParameterListener ("time", this);
    parameters.addParameterListener ("type", this);
    
    waveShaper.functionToUse = [](float SampleValue)
    {
        return tanh (SampleValue);
    };
}

//==============================================================================
const juce::String DistortionPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DistortionPluginAudioProcessor::acceptsMidi() const
{
    return false;
}

bool DistortionPluginAudioProcessor::producesMidi() const
{
    return false;
}

bool DistortionPluginAudioProcessor::isMidiEffect() const
{
    return false;
}

double DistortionPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DistortionPluginAudioProcessor::getNumPrograms()
{
    return 1;
}

int DistortionPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DistortionPluginAudioProcessor::setCurrentProgram (int)
{
}

const juce::String DistortionPluginAudioProcessor::getProgramName (int)
{
	return {};
}

void DistortionPluginAudioProcessor::changeProgramName (int , const juce::String&)
{
}

//==============================================================================
void DistortionPluginAudioProcessor::prepareToPlay (double sampleRate, int maximumExpectedSamplesPerBlock)
{
    juce::dsp::ProcessSpec specification;
    specification.maximumBlockSize = maximumExpectedSamplesPerBlock;
    specification.sampleRate = sampleRate;
    specification.numChannels = getTotalNumInputChannels();
    
    mixControl.prepare (specification);
    inputDriveProcessor.prepare (specification);
    waveShaper.prepare (specification);
    outputGain.prepare (specification);
    stateVariableFilter.prepare (specification);
    feedbackPath.prepare (specification);    
    svfBandFilter.prepare (specification);
    
    feedbackPath.setDelay (10000); 
    feedbackPath.setMaximumDelayInSamples (10000);
    
    stateVariableFilter.setType (juce::dsp::StateVariableTPTFilterType::lowpass);
    stateVariableFilter.setCutoffFrequency (1000);
    
    svfBandFilter.setType (juce::dsp::StateVariableTPTFilterType::bandpass);
    svfBandFilter.setCutoffFrequency (1000);
}

void DistortionPluginAudioProcessor::releaseResources()
{
}

bool DistortionPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    return true;
}

void DistortionPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
	
	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    juce::dsp::AudioBlock<float> drySamplesBlock (buffer);
    mixControl.pushDrySamples (drySamplesBlock);
    
    for (int channel = 0; channel < buffer.getNumChannels(); channel++)
    {
        auto channelPointer = buffer.getWritePointer (channel);
        
        for (int sample = 0; sample < buffer.getNumSamples(); sample++)
        {
            auto poppedDelaySample = feedbackPath.popSample (channel, feedbackPath.getDelay());
            poppedDelaySample = stateVariableFilter.processSample (channel, poppedDelaySample) * 0.5 ;
            
            auto currentSample = channelPointer [sample];
            channelPointer[sample] = ((poppedDelaySample * 0.8) + (currentSample)) * 0.5;
        }
    }
    
    juce::dsp::AudioBlock<float> gainBlock (buffer);
    juce::dsp::ProcessContextReplacing<float> driveContext (gainBlock);
    inputDriveProcessor.process (driveContext);
    waveShaper.process (driveContext);
    
    for (int channel = 0; channel < buffer.getNumChannels(); channel++)
    {
        auto channelPointer = buffer.getWritePointer (channel);
        
        for (int sample = 0; sample < buffer.getNumSamples(); sample++)
        {
            auto currentSample = channelPointer [sample]; // Feedback Amount Control
            feedbackPath.pushSample (channel, currentSample);
        }
    }
    
    juce::dsp::AudioBlock<float> outputBlock (buffer);
    juce::dsp::ProcessContextReplacing<float> outputGainContext (outputBlock);
    outputGain.process (outputGainContext);
    
    juce::dsp::AudioBlock<float> wetSampleBlock (buffer);
    mixControl.mixWetSamples (wetSampleBlock);
}

bool DistortionPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorValueTreeState::ParameterLayout DistortionPluginAudioProcessor::createAPVTSLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout parameterLayout;
    
    parameterLayout.add (std::make_unique<juce::AudioParameterFloat> ("gain", "Gain", -80.f, 0.0f, 0.0f));
    parameterLayout.add (std::make_unique<juce::AudioParameterFloat> ("drive","Drive", -80.f, 24.0f, 0.0f));
    parameterLayout.add (std::make_unique<juce::AudioParameterFloat> ("mix", "mix", 0.0f, 1.0f, 1.0f));
    parameterLayout.add (std::make_unique<juce::AudioParameterFloat> ("cutt","cutt", 20.f, 20000.0f, 20000.0f));
    parameterLayout.add (std::make_unique<juce::AudioParameterFloat> ("q", "q", 0.0f, 10.0f, 1.0f));
    parameterLayout.add (std::make_unique<juce::AudioParameterFloat> ("time", "time", 1.f, 10000.0f, 1.0f));
    parameterLayout.add (std::make_unique<juce::AudioParameterChoice> ("type", "Filter Type", juce::StringArray {"Low Pass", "High Pass", "Band"}, 0));

    return parameterLayout;
}

void DistortionPluginAudioProcessor::parameterChanged (const juce::String& parameterID, float newValue)
{
    if (parameterID == "gain")
        outputGain.setGainDecibels (newValue);
    else if (parameterID == "drive")
        inputDriveProcessor.setGainDecibels (newValue);
    else if (parameterID == "mix")
        mixControl.setWetMixProportion (newValue);
    else if (parameterID == "cutt")
        stateVariableFilter.setCutoffFrequency (newValue);
    else if (parameterID == "q")
        stateVariableFilter.setResonance (newValue);
    else if (parameterID == "time")
        feedbackPath.setDelay (newValue);
    else if (parameterID == "type")
        stateVariableFilter.setType ((juce::dsp::StateVariableTPTFilterType) newValue);
}

juce::AudioProcessorEditor* DistortionPluginAudioProcessor::createEditor()
{
    return new juce::GenericAudioProcessorEditor (*this);
//    return new DistortionPluginAudioProcessorEditor (*this, parameters);
}

void DistortionPluginAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
	auto state = parameters.copyState();
	std::unique_ptr<juce::XmlElement> xml(state.createXml());
	copyXmlToBinary (*xml,destData);
}

void DistortionPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{	
	std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data,sizeInBytes));

	if (xmlState.get() != nullptr)
		if (xmlState->hasTagName (parameters.state.getType()))
			parameters.replaceState (juce::ValueTree::fromXml(*xmlState));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DistortionPluginAudioProcessor();
}
